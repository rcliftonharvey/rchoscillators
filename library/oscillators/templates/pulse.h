#ifndef RCHOSC_OSCILLATORS_TEMPLATES_PULSE_H_INCLUDED
#define RCHOSC_OSCILLATORS_TEMPLATES_PULSE_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// PULSE WAVE OSCILLATOR
//
// Implemented after Demofox' AdvanceOscillator_Square() function
// https://blog.demofox.org/2012/05/19/diy-synthesizer-chapter-2-common-wave-forms/
//
/** Creates a mono per-sample pulse wave oscillator with variable pulse width. */
class Pulse : private RCH::Helpers::Skeleton
{
public:
    
    Pulse  () {}
    ~Pulse () {}
    
    //==============================================================================
    using RCH::Helpers::Skeleton::reset;
    
    using RCH::Helpers::Skeleton::setSampleRate;
    using RCH::Helpers::Skeleton::setFrequency;
    using RCH::Helpers::Skeleton::setAmplitude;
    using RCH::Helpers::Skeleton::setVolume;
    using RCH::Helpers::Skeleton::setState;
    using RCH::Helpers::Skeleton::setPhaseOffset;
    using RCH::Helpers::Skeleton::setWidth;
    
    using RCH::Helpers::Skeleton::getSampleRate;
    using RCH::Helpers::Skeleton::getFrequency;
    using RCH::Helpers::Skeleton::getAmplitude;
    using RCH::Helpers::Skeleton::getVolume;
    using RCH::Helpers::Skeleton::getState;
    using RCH::Helpers::Skeleton::getPhaseOffset;
    using RCH::Helpers::Skeleton::getWidth;
    
    //==============================================================================
    /** Calculates and returns the next pulse wave sample. */
    const double& tick () override
    {
        // Make sure the basic values are correctly set
        assert(samplerate > 0.0 && "Samplerate not correctly set");
        assert(frequency < samplerate && "Frequency not correctly set");
        
        // Increase phase by +1 step
        phase += fractionFrequency;
        
        // Constrain/wrap phase value to sensible boundaries [0,1]
        //
        // if (phase >= 1.0)
        // {
        //     phase -= 1.0;
        // }
        // else if (phase < 0.0)
        // {
        //     phase += 1.0;
        // }
        //
        // IF-branches are slower than simple maths in time critical code, this does the same but faster
        phase += ((phase >= 1.0) * -1.0) + ((phase < 0.0) * 1.0);
        
        // Calculate pulse value for current phase step
        // For simplicity, this will stay an IF conditional branch
        //
        // (Use just amplitude instead of (+/- 1.0 * amplitude) to save a
        //  multiplication step when utilizing slow conditional branching.)
        //
        // If phase between [0.0,width] then output state is positive
        // if (phase < width)
        // {
        //     state = amplitude;
        // }
        // If phase between [0.5,0.5+width] then output state is negative
        // else if ((phase >= 0.5) and (phase < 0.5 + width))
        // {
        //     state = -amplitude;
        // }
        // If phase outside of value range that should generate a pulse
        // else
        // {
        //     state = 0.0;
        // }
        // IF-branches are slower than simple maths in time critical code, this does the same but faster
        state = ((phase < width) * amplitude) + ((phase >= 0.5) * (phase < 0.5 + width) * -amplitude);
        
        // Return calculated pulse wave value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Pulse


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_OSCILLATORS_TEMPLATES_PULSE_H_INCLUDED
