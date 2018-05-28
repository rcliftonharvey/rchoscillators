#pragma once
#ifndef RCH_OSCILLATORS_TEMPLATES_SQUARE_H_INCLUDED
#define RCH_OSCILLATORS_TEMPLATES_SQUARE_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// SQUARE WAVE OSCILLATOR
//
// Implemented after Alan Wolfe's AdvanceOscilator_Square() function
// https://blog.demofox.org/2012/05/19/diy-synthesizer-chapter-2-common-wave-forms/
//
/** Creates a mono per-sample square wave oscillator. */
class Square : private Helpers::Skeleton
{
private:
    
    // Primitive wave table for the square.
    // Index 0 = negative wave flank, index 1 = positive wave flank.
    const double wavetable [2] = {-1.0,1.0};
    
public:
    
    Square  () {}
    ~Square () {}
    
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
    /** Calculates and returns the next square wave sample. */
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
        
        // Evaluate wavetable index: if phase < 0.5 --> 0, else --> 1
        //
        // It would also be possible to do IF-branching here, but branching in time-critical
        // per-sample methods is to be discouraged, so I wanted to do it differently here.
        //
        // This is possible but clunky, since it uses template functions over simple bool 0/1 multipliers.
        // const unsigned int index = static_cast<unsigned int>(std::ceil(phase - 0.5));
        //
        // This is a lot simpler/faster to compute than conditional branching or ceilings.
        // A TRUE condition results in index 1, a FALSE condition results in index 0.
        const unsigned int index = static_cast<unsigned int>(phase < 0.5);
        
        // Fetch square wave value from wavetable and scale to desired volume
        state = wavetable[index] * amplitude;
        
        // Return calculated square wave value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Square


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_TEMPLATES_SQUARE_H_INCLUDED
