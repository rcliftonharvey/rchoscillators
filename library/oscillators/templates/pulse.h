#pragma once
#ifndef RCH_OSCILLATORS_TEMPLATES_PULSE_H_INCLUDED
#define RCH_OSCILLATORS_TEMPLATES_PULSE_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// PULSE WAVE OSCILLATOR
//
// This oscillator rests at its negative amplitude extreme all the time. Only at the
// beginning of each cycle will it shortly ramp up to its positive amplitude extreme
// and then fall down to its negative amplitude extreme again, where it will remain
// until the next cycle starts.
//
// How long the positive pulse lasts, a.k.a. "duty cycle", is set by the width parameter.
// The value of the width parameter is a fraction of the full cycle, so 1.0 means the
// wave stays ramped up to the positive amplitude extreme for the entire cycle, and at
// value 0.5 it would generate a regular square wave.
//
/** Creates a mono per-sample pulse wave oscillator with variable pulse width. */
class Pulse : private Helpers::Skeleton
{
public:
    
    Pulse  () {}
    ~Pulse () {}
    
    //==============================================================================
    using Helpers::Skeleton::reset;
    
    using Helpers::Skeleton::setSampleRate;
    using Helpers::Skeleton::setFrequency;
    using Helpers::Skeleton::setAmplitude;
    using Helpers::Skeleton::setVolume;
    using Helpers::Skeleton::setState;
    using Helpers::Skeleton::setPhaseOffset;
    using Helpers::Skeleton::setPulseWidth;
    
    using Helpers::Skeleton::getSampleRate;
    using Helpers::Skeleton::getFrequency;
    using Helpers::Skeleton::getAmplitude;
    using Helpers::Skeleton::getVolume;
    using Helpers::Skeleton::getState;
    using Helpers::Skeleton::getPhaseOffset;
    using Helpers::Skeleton::getPulseWidth;
    
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
        //
        // If phase between [0.0,width] then output state is positive
        // if (phase < width)
        // {
        //     state = amplitude;
        // }
        // If phase between [width,1.0] then output state is negative
        // else if (phase >= width)
        // {
        //     state = -amplitude;
        // }
        //
        // IF-branches are slower than simple maths in time critical code, this does the same but faster
        state = -amplitude + (phase < width) * (2.0 * amplitude);
        
        // Return calculated pulse wave value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Pulse


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_TEMPLATES_PULSE_H_INCLUDED
