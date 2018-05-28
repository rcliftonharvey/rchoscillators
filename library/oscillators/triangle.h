#pragma once
#ifndef RCH_OSCILLATORS_TRIANGLE_H_INCLUDED
#define RCH_OSCILLATORS_TRIANGLE_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// Convenience definitions for this oscillator
#define OSCILLATOR_TYPE     Oscillators::Templates::Triangle
#define OSCILLATOR_WRAPPER  Helpers::Wrapper


/** Creates a multi-channel capable triangle wave generator.
    This will generate a MONO oscillator wave and duplicate
    it to all channels of a passed sample buffer. */
class Triangle : private OSCILLATOR_WRAPPER<OSCILLATOR_TYPE>
{
public:
    
    Triangle  () {}
    ~Triangle () {}
    
    using OSCILLATOR_WRAPPER<OSCILLATOR_TYPE>::reset;
    using OSCILLATOR_WRAPPER<OSCILLATOR_TYPE>::setup;
    
    using OSCILLATOR_WRAPPER<OSCILLATOR_TYPE>::fill;
    using OSCILLATOR_WRAPPER<OSCILLATOR_TYPE>::add;
    
}; // end class RCH::Oscillators::Triangle


// Just cleaning up
#undef OSCILLATOR_WRAPPER
#undef OSCILLATOR_TYPE


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_TRIANGLE_H_INCLUDED
