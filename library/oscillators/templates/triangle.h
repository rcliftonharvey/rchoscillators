#pragma once
#ifndef RCH_OSCILLATORS_TEMPLATES_TRIANGLE_H_INCLUDED
#define RCH_OSCILLATORS_TEMPLATES_TRIANGLE_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// TRIANGLE WAVE OSCILLATOR
//
// Implemented after Alan Wolfe's AdvanceOscilator_Triangle() function
// https://blog.demofox.org/2012/05/19/diy-synthesizer-chapter-2-common-wave-forms/
//
/** Creates a mono per-sample triangle wave oscillator. */
class Triangle : private Helpers::Skeleton
{
public:
    
    Triangle  () {}
    ~Triangle () {}
    
    //==============================================================================
    using Helpers::Skeleton::reset;
    
    using Helpers::Skeleton::setSampleRate;
    using Helpers::Skeleton::setFrequency;
    using Helpers::Skeleton::setAmplitude;
    using Helpers::Skeleton::setVolume;
    using Helpers::Skeleton::setState;
    using Helpers::Skeleton::setPhaseOffset;
    
    using Helpers::Skeleton::getSampleRate;
    using Helpers::Skeleton::getFrequency;
    using Helpers::Skeleton::getAmplitude;
    using Helpers::Skeleton::getVolume;
    using Helpers::Skeleton::getState;
    using Helpers::Skeleton::getPhaseOffset;
    
    //==============================================================================
    /** Calculates and returns the next triangle wave sample. */
    const double& tick () override
    {
        // Make sure the basic values are correctly set
        assert(samplerate > 0.0 && "Samplerate not correctly set");
        assert(frequency < samplerate && "Frequency not correctly set");
        
        // Increase phase by +1 step
        phase += fractionFrequency;
        
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
        
        // Calculate triangle value for current phase step
        //
        // if (phase < 0.5)
        // {
        //     state = phase * 4.0 - 1.0;
        // }
        // else
        // {
        //     state = (1.0 - phase) * 4.0 - 1.0;
        // }
        //
        // IF-branches are slower than simple maths in time critical code, this does the same but faster
        state = ((phase < 0.5) * (4.0 * phase - 1.0)) + ((phase >= 0.5) * (1.0 - 4.0 * (phase - 0.5)));
        
        // Scale to desired output volume
        state *= amplitude;
        
        // Return calculated triangle value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Triangle


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_TEMPLATES_TRIANGLE_H_INCLUDED
