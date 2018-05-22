#ifndef RCHOSC_OSCILLATORS_TEMPLATES_TRIANGLE_H_INCLUDED
#define RCHOSC_OSCILLATORS_TEMPLATES_TRIANGLE_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// TRIANGLE WAVE OSCILLATOR
//
// Implemented after Demofox' AdvanceOscillator_Triangle() function
// https://blog.demofox.org/2012/05/19/diy-synthesizer-chapter-2-common-wave-forms/
//
/** Creates a mono per-sample triangle wave oscillator. */
class Triangle : private RCH::Helpers::Skeleton
{
public:
    
    Triangle  () {}
    ~Triangle () {}
    
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
    /** Calculates and returns the next triangle wave sample. */
    const double& tick () override
    {
        // Make sure the basic values are correctly set
        assert(samplerate > 0.0 && "Samplerate not correctly set");
        assert(frequency < samplerate && "Frequency not correctly set");
        
        // Increase phase by +1 step
        phase += fractionFrequency;
        
        // Constrain/wrap phase value to sensible boundaries [0,1]
        if (phase > 1.0)
        {
            phase -= 1.0;
        }
        else if (phase < 0.0)
        {
            phase += 1.0;
        }
        
        // Calculate triangle value for current phase step
        if (phase <= 0.5)
        {
            state = phase * 4.0 - 1.0;
        }
        else
        {
            state = (1.0 - phase) * 4.0 - 1.0;
        }
        
        // Scale to desired output volume
        state *= amplitude;
        
        // Return calculated triangle value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Triangle


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCHOSC_OSCILLATORS_TEMPLATES_TRIANGLE_H_INCLUDED
