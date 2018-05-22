#ifndef RCHOSC_OSCILLATORS_TEMPLATES_SINE_H_INCLUDED
#define RCHOSC_OSCILLATORS_TEMPLATES_SINE_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// SINE WAVE OSCILLATOR
//
// Implemented after Demofox' AdvanceOscillator_Sine() function
// https://blog.demofox.org/2012/05/19/diy-synthesizer-chapter-2-common-wave-forms/
//
/** Creates a mono per-sample sine wave oscillator. */
class Sine : private RCH::Helpers::Skeleton
{
public:
    
    Sine  () {}
    ~Sine () {}
    
    //==============================================================================
    using RCH::Helpers::Skeleton::reset;
    
    using RCH::Helpers::Skeleton::setSampleRate;
    using RCH::Helpers::Skeleton::setFrequency;
    using RCH::Helpers::Skeleton::setAmplitude;
    using RCH::Helpers::Skeleton::setVolume;
    using RCH::Helpers::Skeleton::setState;
    using RCH::Helpers::Skeleton::setPhaseOffset;
    
    using RCH::Helpers::Skeleton::getSampleRate;
    using RCH::Helpers::Skeleton::getFrequency;
    using RCH::Helpers::Skeleton::getAmplitude;
    using RCH::Helpers::Skeleton::getVolume;
    using RCH::Helpers::Skeleton::getState;
    using RCH::Helpers::Skeleton::getPhaseOffset;
    
    //==============================================================================
    /** Calculates and returns the next sine wave sample. */
    const double& tick () override
    {
        // Make sure the basic values are correctly set
        assert(samplerate > 0.0 && "Samplerate not correctly set");
        assert(frequency < samplerate && "Frequency not correctly set");
        
        // Increase phase by +1 step
        phase += M_2PI * fractionFrequency;
        
        // Constrain/wrap phase value to sensible boundaries [0,2PI]
        if (phase >= M_2PI)
        {
            phase -= M_2PI;
        }
        else if (phase < 0.0)
        {
            phase += M_2PI;
        }
        
        // Calculate sine value for current phase step and scale to desired volume
        state = std::sin(phase) * amplitude;
        
        // Return calculated sine value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Sine


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_OSCILLATORS_TEMPLATES_SINE_H_INCLUDED
