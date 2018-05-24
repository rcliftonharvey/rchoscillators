#ifndef RCHOSC_OSCILLATORS_TEMPLATES_TRIANGLE_BANDLIMITED_H_INCLUDED
#define RCHOSC_OSCILLATORS_TEMPLATES_TRIANGLE_BANDLIMITED_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// BAND-LIMITED TRIANGLE WAVE OSCILLATOR
//
// Implemented after Barry Truax' Handbook for Acoustic Ecology
// https://www.sfu.ca/sonic-studio/handbook/index.html
//
/** Creates a band-limited mono per-sample triangle wave oscillator. */
class Triangle : private RCH::Helpers::SkeletonBandlimited
{
public:
    
    Triangle  () {}
    ~Triangle () {}
    
    //==============================================================================
    using RCH::Helpers::SkeletonBandlimited::reset;
    
    using RCH::Helpers::SkeletonBandlimited::setSampleRate;
    using RCH::Helpers::SkeletonBandlimited::setFrequency;
    using RCH::Helpers::SkeletonBandlimited::setAmplitude;
    using RCH::Helpers::SkeletonBandlimited::setVolume;
    using RCH::Helpers::SkeletonBandlimited::setState;
    using RCH::Helpers::SkeletonBandlimited::setPhaseOffset;
    using RCH::Helpers::SkeletonBandlimited::setMaxHarmonics;
    
    using RCH::Helpers::SkeletonBandlimited::getSampleRate;
    using RCH::Helpers::SkeletonBandlimited::getFrequency;
    using RCH::Helpers::SkeletonBandlimited::getAmplitude;
    using RCH::Helpers::SkeletonBandlimited::getVolume;
    using RCH::Helpers::SkeletonBandlimited::getState;
    using RCH::Helpers::SkeletonBandlimited::getPhaseOffset;
    using RCH::Helpers::SkeletonBandlimited::getMaxHarmonics;
    
    //==============================================================================
    /** Calculates and returns the next triangle wave sample. */
    const double& tick () override
    {
        // Make sure the values we'll work with are correctly set
        assert(samplerate > 0.0 && "Samplerate not correctly set");
        assert(nyquist == samplerate * 0.5 && "Samplerate not correctly set");
        assert(frequency <= nyquist && "Frequency can't be above nyquist");
        
        // Increase phase by +1 step
        phase += fractionFrequency;
        
        // Constrain/wrap phase value to sensible boundaries [0,1]
        phase += ((phase >= 1.0) * -1.0) + ((phase < 0.0) * 1.0);
        
        // Reset the output value to 0, generated harmonics will be added to this later
        state = 0.0;
        
        // Reset the number of already calculated harmonics to zero
        dunHarmonics = 0;
        
        // Iterate through as many harmonic overtones as should be generated
        for (unsigned int harmonic=1; dunHarmonics<=numHarmonics; harmonic += 2)
        {
            // This is the number of the currently generated harmonic step,
            // can be used as amplitude modifier later.
            harmonicStep = static_cast<double>(harmonic);
            
            // The phase position for the currently generated harmonic. It's
            // basically multiplying the frequency, doubling the speed of the
            // rotation, etc. which results in a shifted wave.
            harmonicPhase = (phase * direction) * harmonicStep;
            
            // Generate this harmonic sine wave, scale it down by harmonicStep
            // and add it to the already existing wave.
            state += std::cos(harmonicPhase * M_2PI) / (harmonicStep * harmonicStep);
            
            // Inrement the number of generated harmonics. Since we only need
            // the odd harmonics, increment by +2 to skip the even ones.
            dunHarmonics += 2;
        }
        
        // Adjust the volume
        state *= M_8_PIPI * amplitude;
        
        // Return the generated sample value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Bandlimited::Triangle


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_OSCILLATORS_TEMPLATES_TRIANGLE_BANDLIMITED_H_INCLUDED
