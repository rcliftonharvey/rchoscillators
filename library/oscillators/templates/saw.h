#pragma once
#ifndef RCH_OSCILLATORS_TEMPLATES_SAW_H_INCLUDED
#define RCH_OSCILLATORS_TEMPLATES_SAW_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// SAWTOOTH OSCILLATOR
//
// Implemented after Alan Wolfe's AdvanceOscilator_Saw() function
// https://blog.demofox.org/2012/05/19/diy-synthesizer-chapter-2-common-wave-forms/
//
/** Creates a mono per-sample saw wave oscillator with variable direction. */
class Saw : private Helpers::Skeleton
{
public:
    
    Saw  () {}
    ~Saw () {}
    
    //==============================================================================
    using Helpers::Skeleton::reset;
    
    using Helpers::Skeleton::setSampleRate;
    using Helpers::Skeleton::setFrequency;
    using Helpers::Skeleton::setAmplitude;
    using Helpers::Skeleton::setVolume;
    using Helpers::Skeleton::setState;
    using Helpers::Skeleton::setPhaseOffset;
    using Helpers::Skeleton::setDirection;
    
    using Helpers::Skeleton::getSampleRate;
    using Helpers::Skeleton::getFrequency;
    using Helpers::Skeleton::getAmplitude;
    using Helpers::Skeleton::getVolume;
    using Helpers::Skeleton::getState;
    using Helpers::Skeleton::getPhaseOffset;
    using Helpers::Skeleton::getDirection;
    
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
        //
        // if (phase > 1.0)
        // {
        //     phase -= 1.0;
        // }
        // else if (phase < 0.0)
        // {
        //     phase += 1.0;
        // }
        //
        // IF-branches are slower than simple maths in time critical code, this does the same but faster
        phase += ((phase > 1.0) * -1.0) + ((phase < 0.0) * 1.0);
        
        // Calculate saw value for current phase step and scale to desired volume
        state = ((phase * 2.0) - 1.0) * amplitude;
        
        // Return calculated saw value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Saw


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_TEMPLATES_SAW_H_INCLUDED
