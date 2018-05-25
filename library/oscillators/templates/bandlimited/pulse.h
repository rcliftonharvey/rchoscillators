#ifndef RCHOSC_OSCILLATORS_TEMPLATES_PULSE_BANDLIMITED_H_INCLUDED
#define RCHOSC_OSCILLATORS_TEMPLATES_PULSE_BANDLIMITED_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// BAND-LIMITED PULSE WAVE OSCILLATOR
//
/** Creates a band-limited mono per-sample pulse wave oscillator. */
class Pulse : private Helpers::SkeletonBandlimited
{
private:
    
    // These are all used for the shifted 2nd wave
    double shiftedPhase = 0.0;
    double harmonicPhase2 = 0.0;
    
public:
    
    Pulse  () {}
    ~Pulse () {}
    
    //==============================================================================
    using Helpers::SkeletonBandlimited::reset;
    
    using Helpers::SkeletonBandlimited::setSampleRate;
    using Helpers::SkeletonBandlimited::setFrequency;
    using Helpers::SkeletonBandlimited::setAmplitude;
    using Helpers::SkeletonBandlimited::setVolume;
    using Helpers::SkeletonBandlimited::setState;
    using Helpers::SkeletonBandlimited::setPhaseOffset;
    using Helpers::SkeletonBandlimited::setPulseWidth;
    using Helpers::SkeletonBandlimited::setMaxHarmonics;
    
    using Helpers::SkeletonBandlimited::getSampleRate;
    using Helpers::SkeletonBandlimited::getFrequency;
    using Helpers::SkeletonBandlimited::getAmplitude;
    using Helpers::SkeletonBandlimited::getVolume;
    using Helpers::SkeletonBandlimited::getState;
    using Helpers::SkeletonBandlimited::getPhaseOffset;
    using Helpers::SkeletonBandlimited::getPulseWidth;
    using Helpers::SkeletonBandlimited::getMaxHarmonics;
    
    //==============================================================================
    /** Calculates and returns the next square wave sample. */
    const double& tick () override
    {
        // Make sure the values we'll work with are correctly set
        assert(samplerate > 0.0 && "Samplerate not correctly set");
        assert(nyquist == samplerate * 0.5 && "Samplerate not correctly set");
        assert(frequency <= nyquist && "Frequency can't be above nyquist");
        
        // Increase phase by +1 step
        phase += fractionFrequency;
        shiftedPhase = phase + width;
        
        // Constrain/wrap phase value to sensible boundaries [0,1]
        phase += ((phase >= 1.0) * -1.0) + ((phase < 0.0) * 1.0);
        shiftedPhase += ((shiftedPhase >= 1.0) * -1.0) + ((shiftedPhase < 0.0) * 1.0);
        
        // Reset the output value to 0, generated harmonics will be added to this later
        state = 0.0;
        
        // Reset the number of already calculated harmonics to zero
        dunHarmonics = 0;
        
        // Iterate through as many harmonic overtones as should be generated
        for (unsigned int harmonic=1; dunHarmonics<=numHarmonics; ++harmonic)
        {
            // This is the number of the currently generated harmonic step,
            // can be used as amplitude modifier later.
            harmonicStep = static_cast<double>(harmonic);
            
            // The phase positions for the currently generated harmonic.
            //
            // In this case, to get a pulse shape, two saw waves with different
            // directions (1x rising + 1x falling) will be generated and summed.
            harmonicPhase = (phase * -direction) * harmonicStep;
            harmonicPhase2 = (shiftedPhase * direction) * harmonicStep;
            
            // Generate the harmonic sine waves, scale them down by harmonicStep
            // and add them to the already existing wave.
            state += (std::sin(harmonicPhase  * M_2PI)      // first saw wave
                  +   std::sin(harmonicPhase2 * M_2PI))     // second saw wave (inverted)
                  /   harmonicStep;                         // volume factor
            
            // Inrement the number of generated harmonics.
            ++dunHarmonics;
        }
        
        // This adds a width dependent dynamic DC offset that will be added to the state,
        // it will stop the waveform from drifting towards one polarity if width != 0.5
        //
        // I got this irrational seeming number from Maximilian Matthé's comment here:
        // https://dsp.stackexchange.com/questions/37236/?answertab=votes#tab-top
        //
        // Since the factor would eventually end up being added to the same state twice,
        // I just went ahead and hard-multiplied it by 2 to save a few multiplications.
        state += width * 1.56 + (1.0 - width) * -1.56;
        
        // Adjust the volume --> M_2_PI = 2 * M_4_PI, since we have 2 waves
        state *= M_2_PI * amplitude;
        
        // Return the generated sample value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Bandlimited::Pulse


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_OSCILLATORS_TEMPLATES_PULSE_BANDLIMITED_H_INCLUDED
