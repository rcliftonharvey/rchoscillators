#ifndef RCHOSC_OSCILLATORS_TEMPLATES_SQUAREPULSE_H_INCLUDED
#define RCHOSC_OSCILLATORS_TEMPLATES_SQUAREPULSE_H_INCLUDED
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
class SquarePulse : private RCH::Helpers::Skeleton
{
public:
    
    SquarePulse  () {}
    ~SquarePulse () {}
    
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
    
}; // end class RCH::Oscillators::Templates::SquarePulse


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_OSCILLATORS_TEMPLATES_SQUAREPULSE_H_INCLUDED
