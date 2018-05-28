#pragma once
#ifndef RCH_OSCILLATORS_TEMPLATES_SQUAREPULSE_H_INCLUDED
#define RCH_OSCILLATORS_TEMPLATES_SQUAREPULSE_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// SQUARE PULSE WAVE OSCILLATOR
//
// Bit of a hybrid, but essentially a square wave with adjustable duty cycles.
//
// A regular pulse wave would be unipolar, meaning it would rest at its negative
// extreme all the time and only shortly burst up to its positive extreme. I know
// that theoretically, mathematically, such a pulse would also be bipolar since it
// has positive and negative sample values. But the point is that it only ever does
// upward jumps from its negative resting value, it never oscillates down below it.
//
// This square pulse is a bit different. Like a basic square wave, it alternates
// between its positive and negative extremes. The positive/negative extreme values
// last for a fraction of 1/2 cycle each, the silence state is at 0, in the middle,
// making this a bipolar oscillator.
//
/** Creates a mono per-sample square wave oscillator with variable pulse width. */
class SquarePulse : private Helpers::Skeleton
{
public:
    
    SquarePulse  () {}
    ~SquarePulse () {}
    
    //==============================================================================
    using Helpers::Skeleton::reset;
    
    using Helpers::Skeleton::setSampleRate;
    using Helpers::Skeleton::setFrequency;
    using Helpers::Skeleton::setAmplitude;
    using Helpers::Skeleton::setVolume;
    using Helpers::Skeleton::setState;
    using Helpers::Skeleton::setPhaseOffset;
    using Helpers::Skeleton::setWidth;
    
    using Helpers::Skeleton::getSampleRate;
    using Helpers::Skeleton::getFrequency;
    using Helpers::Skeleton::getAmplitude;
    using Helpers::Skeleton::getVolume;
    using Helpers::Skeleton::getState;
    using Helpers::Skeleton::getPhaseOffset;
    using Helpers::Skeleton::getWidth;
    
    //==============================================================================
    /** Calculates and returns the next square pulse wave sample. */
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
        // If phase between [0.5,0.5+width] then output state is negative
        // else if ((phase >= 0.5) and (phase < 0.5 + width))
        // {
        //     state = -amplitude;
        // }
        // If phase outside of value range that should generate signal
        // else
        // {
        //     state = 0.0;
        // }
        //
        // IF-branches are slower than simple maths in time critical code, this does the same but faster
        state = ((phase < width) * amplitude) + ((phase >= 0.5) * (phase < 0.5 + width) * -amplitude);
        
        // Return calculated pulse wave value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::SquarePulse


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_TEMPLATES_SQUAREPULSE_H_INCLUDED
