#pragma once
#ifndef RCH_OSCILLATORS_TEMPLATES_SINE_H_INCLUDED
#define RCH_OSCILLATORS_TEMPLATES_SINE_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// SINE WAVE OSCILLATOR
//
// Implemented after Alan Wolfe's AdvanceOscilator_Sine() function
// https://blog.demofox.org/2012/05/19/diy-synthesizer-chapter-2-common-wave-forms/
//
/** Creates a mono per-sample sine wave oscillator. */
class Sine : private Helpers::Skeleton
{
public:
    
    Sine  () {}
    ~Sine () {}
    
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
    /** Calculates and returns the next sine wave sample. */
    const double& tick () override
    {
        // Make sure the basic values are correctly set
        assert(samplerate > 0.0 && "Samplerate not correctly set");
        assert(frequency < samplerate && "Frequency not correctly set");
        
        // Increase phase by +1 step
        phase += M_2PI * fractionFrequency;
        
        // Constrain/wrap phase value to sensible boundaries [0,2PI]
        //
        // if (phase >= M_2PI)
        // {
        //     phase -= M_2PI;
        // }
        // else if (phase < 0.0)
        // {
        //     phase += M_2PI;
        // }
        //
        // IF-branches are slower than simple maths in time critical code, this does the same but faster
        phase += ((phase >= M_2PI) * -M_2PI) + ((phase < 0.0) * M_2PI);
        
        // Calculate sine value for current phase step and scale to desired volume. I tried using fast
        // sine approximations and lookup tables instead, but they added audible harmonic imperfections.
        state = std::sin(phase) * amplitude;
        
        // Return calculated sine value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Sine


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_TEMPLATES_SINE_H_INCLUDED
