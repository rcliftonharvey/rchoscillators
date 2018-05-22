#ifndef RCHOSC_OSCILLATORS_TEMPLATES_SAW_H_INCLUDED
#define RCHOSC_OSCILLATORS_TEMPLATES_SAW_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// SAWTOOTH OSCILLATOR
//
// Implemented after Demofox' AdvanceOscillator_Saw() function
// https://blog.demofox.org/2012/05/19/diy-synthesizer-chapter-2-common-wave-forms/
//
/** Creates a mono per-sample saw wave oscillator with variable direction. */
class Saw : private RCH::Helpers::Skeleton
{
public:
    
    Saw  () {}
    ~Saw () {}
    
    //==============================================================================
    using RCH::Helpers::Skeleton::reset;
    
    using RCH::Helpers::Skeleton::setSampleRate;
    using RCH::Helpers::Skeleton::setFrequency;
    using RCH::Helpers::Skeleton::setAmplitude;
    using RCH::Helpers::Skeleton::setVolume;
    using RCH::Helpers::Skeleton::setState;
    using RCH::Helpers::Skeleton::setPhaseOffset;
    using RCH::Helpers::Skeleton::setDirection;
    
    using RCH::Helpers::Skeleton::getSampleRate;
    using RCH::Helpers::Skeleton::getFrequency;
    using RCH::Helpers::Skeleton::getAmplitude;
    using RCH::Helpers::Skeleton::getVolume;
    using RCH::Helpers::Skeleton::getState;
    using RCH::Helpers::Skeleton::getPhaseOffset;
    using RCH::Helpers::Skeleton::getDirection;
    
    //==============================================================================
    /** Calculates and returns the next saw wave sample. */
    const double& tick () override
    {
        // Make sure the basic values are correctly set
        assert(samplerate > 0.0 && "Samplerate not correctly set");
        assert(frequency < samplerate && "Frequency not correctly set");
        
        // If direction set to +1 then rising saw wave
        // If direction set to -1 then falling saw wave
        // Increase phase by +1 step
        phase += direction * fractionFrequency;
        
        // Constrain/wrap phase value to sensible boundaries [0,1]
        if (phase > 1.0)
        {
            phase -= 1.0;
        }
        else if (phase < 0.0)
        {
            phase += 1.0;
        }
        
        // Calculate saw value for current phase step and scale to desired volume
        state = ((phase * 2.0) - 1.0) * amplitude;
        
        // Return calculated saw value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Saw


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_OSCILLATORS_TEMPLATES_SAW_H_INCLUDED
