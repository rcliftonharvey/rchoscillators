#pragma once
#ifndef RCH_OSCILLATORS_HELPERS_SKELETON_BANDLIMITED_H_INCLUDED
#define RCH_OSCILLATORS_HELPERS_SKELETON_BANDLIMITED_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// BAND-LIMITED OSCILLATOR SKELETON TEMPLATE
//
// Instead of generating a single wave that is shaped as it should be,
// this will attempt to re-create a complex waveform by adding together
// several simple sinusoid waveforms, kind of like artificial overtones.
//
// The reason for this is quite simply: no aliasing. If you don't know what
// aliasing is, or why we don't want it in audible sound waves, read this:
// http://www.danbullard.com/dan/aliasing.html
//
// The highest most frequency to be generated is always Nyquist, which is
// basically just a fancy word for "half the current sample rate" frequency.
// If your project runs at 44.1 kHz, then Nyquist (the highest possible to
// handle frequency) will be at 44.1 kHz / 2 = 22.05 kHz. If your project is
// running at 192 kHz, then Nyquist will be at 96 kHz.
//
// For a square wave at 1 kHz in a project running at 44.1 kHz, there are only
// about 10 multiples/harmonics/overtones until we reach Nyquist, so there's
// not a lot to generate. But if the project runs at 96 kHz, putting Nyquist
// at 48 kHz, and you're trying to generate a 24 Hz square wave... that would
// mean there are now Nyquist / Frequency = 96.000 / 48 = 2000 sine waves to
// add together until the overtones reach the Nyquist barrier.
//
// That's why I built in the possibility to pick from 10 "accuracy" modes.
// They're essentially a bunch of "maximum number of harmonics to calculate"
// limits, you can set them by calling the .setMaxHarmonics() method with a
// number in range [1,10], where 1 is lowest and 10 is highest accuracy. See
// the skeleton_bw.h file for more insight and the definition of those limits.
//
// Picking a low accuracy mode will stop calculating further harmonics after a
// certain step, which means that if your square wave is at 100 Hz but the
// limit is set to 100, then no harmonic over 100 Hz * 100 = 10.000 Hz will be
// generated. Often times, especially when prototyping, this is good enough.
//
// For when it's not, the highest limit currently ends at 96000 harmonics.
// This means that even at 192 kHz project sample rate, a 1 Hz square wave
// could, theoretically, be 100% accurately generated.
//
// There is no band-limited sine oscillator, since the sine is the purest
// wave type with the least (as in zero) amount of occuring overtones and
// harmonics by itself. One could call the sine band-limited by its nature.
//
/** Band-limited mono per-sample oscillator skeleton class. */
class SkeletonBandlimited : public Helpers::Skeleton
{
public:
    
    /* --------------------------- limit ------ base --> limit --- base -> limit --- base --> limit ---*/
    #define NUM_HARMONICS_MAX_BEST 96000     // 1 Hz --> 96.000 Hz = full 192.0 sample rate spectrum
    #define NUM_HARMONICS_MAX_HARD 44100     // 1 Hz --> 44.100 Hz = full  88.2 sample rate spectrum
    #define NUM_HARMONICS_MAX_FULL 22050     // 1 Hz --> 22.050 Hz = full  44.1 sample rate spectrum
    #define NUM_HARMONICS_MAX_GOOD  2500     // 1 Hz -->  2.500 Hz, 10 --> 25.000 Hz, 100 --> 250.000
    #define NUM_HARMONICS_MAX_OKAY   801     // 1 Hz -->    801 Hz, 10 -->  8.100 Hz, 100 -->  81.000
    #define NUM_HARMONICS_MAX_FAIR   250     // 1 Hz -->    250 Hz, 10 -->  2.500 Hz, 100 -->  25.000
    #define NUM_HARMONICS_MAX_FAST   100     // 1 Hz -->    100 Hz, 10 -->  1.000 Hz, 100 -->  10.000
    #define NUM_HARMONICS_MAX_CENT    25     // 1 Hz -->     25 Hz, 10 -->    250 Hz, 100 -->   2.500
    #define NUM_HARMONICS_MAX_PHEW    10     // 1 Hz -->     10 Hz, 10 -->    100 Hz, 100 -->   1.000
    #define NUM_HARMONICS_MAX_NONE     1     // 1 Hz -->      1 Hz, 10 -->     10 Hz, 100 -->     100
    
    SkeletonBandlimited ()
    {
        accuracy[0] = NUM_HARMONICS_MAX_NONE;
        accuracy[1] = NUM_HARMONICS_MAX_PHEW;
        accuracy[2] = NUM_HARMONICS_MAX_CENT;
        accuracy[3] = NUM_HARMONICS_MAX_FAST;
        accuracy[4] = NUM_HARMONICS_MAX_FAIR;
        accuracy[5] = NUM_HARMONICS_MAX_OKAY;
        accuracy[6] = NUM_HARMONICS_MAX_GOOD;
        accuracy[7] = NUM_HARMONICS_MAX_FULL;
        accuracy[8] = NUM_HARMONICS_MAX_HARD;
        accuracy[9] = NUM_HARMONICS_MAX_BEST;
    }
    
    virtual ~SkeletonBandlimited () {}
    
protected:
    
    // Half the SampleRate in Hz = max. frequency sine to calculate and add
    double nyquist = 0.0;
    
    unsigned int numHarmonics = 0;      // How many harmonics to generate in a tick
    unsigned int maxHarmonics = 8;      // Index to pick from std::vector to limit numHarmonics
    unsigned int dunHarmonics = 0;      // Per-tick counter variable
    
    double harmonicStep = 0.0;        // Multiplication factor used in harmonics generation
    double harmonicPhase = 0.0;         // Per-harmonic phase factor
    
    std::vector<unsigned int> accuracy = std::vector<unsigned int> (10,0);
    
    //==============================================================================
    /** Sets the oscillator sample rate in Hertz. */
    void setSampleRate (const double& SR)
    {
        // Only update and recalculate if new SR value is different
        if (SR != samplerate)
        {
            // Import number of samples per second
            samplerate = SR;
            
            // Calculate the Nyquist frequency (used for band limiting)
            nyquist = SR * 0.5;
            
            // If the SR is changed while a Frequency was already set
            if (frequency > 0.0)
            {
                // Recalculate the per-sample phase modifier
                fractionFrequency = frequency / samplerate;
                
                // Calculate how many harmonics need to be generated for this frequency
                updateNumberOfHarmonics();
            }
            
            // Re-initialize to reset state
            reset();
        }
    }
    
    /** Sets the oscillator center frequency in Hertz. */
    void setFrequency (const double& Hz)
    {
        // Only update and recalculate if new Hz value is different
        if (Hz != frequency)
        {
            // Import new center frequency
            frequency = Hz;
            
            // If the center frequency is changed while SR and Nyquist were already set
            if ((samplerate > 0.0) and (nyquist > 0.0) and (nyquist == samplerate * 0.5))
            {
                // Recalculate the per-sample phase modifier
                fractionFrequency = frequency / samplerate;
                
                // Calculate how many harmonics need to be generated for this frequency
                updateNumberOfHarmonics();
            }
        }
    }
    
    /** Sets the maximum amount of harmonics to generate.
        Use a number between 1 and 10, where 1 is NONE and 10 is BEST.
        If Number outside of [1,10] will default to FULL. */
    void setMaxHarmonics (const unsigned int& Number)
    {
        unsigned int amount = Number;
        
        // If amount outside range [1,10] then default to FULL quality
        if ((amount < 1) or (amount > 10))
        {
            amount = 8;
        }
        
        // Re-scale from 1-10 (user friendly) to 0-9 (index friendly)
        --amount;
        
        if (amount != maxHarmonics)
        {
            maxHarmonics = amount;
            
            updateNumberOfHarmonics();
        }
    }
    
    /** Returns the currently set maximum harmonic quality amount, range [1,10] */
    const unsigned int getMaxHarmonics () const
    {
        return maxHarmonics + 1;
    }
    
private:
    
    /** Used to figure out how many harmonic sine waves should be generated to stay inside
        the Nyquist band limit and/or the desired limit of harmonics to generate. */
    void updateNumberOfHarmonics ()
    {
        // Make sure the values we'll work with are correctly set
        assert(samplerate > 0.0 && "Samplerate not correctly set");
        assert(nyquist == samplerate * 0.5 && "Samplerate not correctly set");
        assert(frequency <= nyquist && "Frequency can't be above nyquist");
        
        // Set the number of harmonics to calculate per tick so reaches nyquist
        numHarmonics = static_cast<unsigned int>(nyquist / frequency);
        
        // Ensure the number of harmonics to generate does not exceed the maximum
        if (numHarmonics > accuracy[maxHarmonics])
        {
            numHarmonics = accuracy[maxHarmonics];
        }
    }
    
}; // class RCH::Helpers::SkeletonBandlimited


// Just cleaning up
#undef NUM_HARMONICS_MAX_BEST
#undef NUM_HARMONICS_MAX_HARD
#undef NUM_HARMONICS_MAX_FULL
#undef NUM_HARMONICS_MAX_GOOD
#undef NUM_HARMONICS_MAX_OKAY
#undef NUM_HARMONICS_MAX_FAIR
#undef NUM_HARMONICS_MAX_FAST
#undef NUM_HARMONICS_MAX_CENT
#undef NUM_HARMONICS_MAX_PHEW
#undef NUM_HARMONICS_MAX_NONE


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_HELPERS_SKELETON_BANDLIMITED_H_INCLUDED
