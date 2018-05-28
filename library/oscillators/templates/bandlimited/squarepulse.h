#pragma once
#ifndef RCH_OSCILLATORS_TEMPLATES_BANDLIMITED_SQUAREPULSE_H_INCLUDED
#define RCH_OSCILLATORS_TEMPLATES_BANDLIMITED_SQUAREPULSE_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //


// BAND-LIMITED SQUARE PULSE WAVE OSCILLATOR
//
// In contrast to the "true" pulse oscillator, this will (intendedly) generate
// a square wave at width = 1.0 and NOT at width = 0.5.
//
/** Creates a band-limited mono per-sample square pulse wave oscillator. */
class SquarePulse : private Helpers::SkeletonBandlimited
{
private:
    
    // These are all used for the shifted 2nd wave
    double shiftedPhase2 = 0.0;
    double harmonicPhase2 = 0.0;
    
public:
    
    SquarePulse  () {}
    ~SquarePulse () {}
    
    //==============================================================================
    using Helpers::SkeletonBandlimited::reset;
    
    using Helpers::SkeletonBandlimited::setSampleRate;
    using Helpers::SkeletonBandlimited::setFrequency;
    using Helpers::SkeletonBandlimited::setAmplitude;
    using Helpers::SkeletonBandlimited::setVolume;
    using Helpers::SkeletonBandlimited::setState;
    using Helpers::SkeletonBandlimited::setPhaseOffset;
    using Helpers::SkeletonBandlimited::setWidth;
    using Helpers::SkeletonBandlimited::setMaxHarmonics;
    
    using Helpers::SkeletonBandlimited::getSampleRate;
    using Helpers::SkeletonBandlimited::getFrequency;
    using Helpers::SkeletonBandlimited::getAmplitude;
    using Helpers::SkeletonBandlimited::getVolume;
    using Helpers::SkeletonBandlimited::getState;
    using Helpers::SkeletonBandlimited::getPhaseOffset;
    using Helpers::SkeletonBandlimited::getWidth;
    using Helpers::SkeletonBandlimited::getMaxHarmonics;
    
    //==============================================================================
    /** Calculates and returns the next square pulse wave sample. */
    const double& tick () override
    {
        // Make sure the values we'll work with are correctly set
        assert(samplerate > 0.0 && "Samplerate not correctly set");
        assert(nyquist == samplerate * 0.5 && "Samplerate not correctly set");
        assert(frequency <= nyquist && "Frequency can't be above nyquist");
        
        // Increase phase by +1 step
        phase += fractionFrequency;
        shiftedPhase2 = phase + width;
        
        // Constrain/wrap phase value to sensible boundaries [0,1]
        phase += ((phase >= 1.0) * -1.0) + ((phase < 0.0) * 1.0);
        shiftedPhase2 += ((shiftedPhase2 >= 1.0) * -1.0) + ((shiftedPhase2 < 0.0) * 1.0);
        
        // Reset the output value to 0, generated harmonics will be added to this later
        state = 0.0;
        
        // Reset the number of already calculated harmonics to zero
        dunHarmonics = 0;
        
        // Iterate through as many harmonic overtones as should be generated
        for (unsigned int harmonic=1; dunHarmonics<numHarmonics; harmonic+=2)
        {
            // This is the number of the currently generated harmonic step,
            // can be used as amplitude modifier later.
            harmonicStep = static_cast<double>(harmonic);
            
            // The phase positions for the currently generated harmonic.
            //
            // In this case, to get an alternating pulse shape, two square waves
            // with inverted polarities will be generated, shifted and summed.
            harmonicPhase = phase * harmonicStep;
            harmonicPhase2 = -shiftedPhase2 * harmonicStep;
            
            // Generate the harmonic sine waves, scale them down by harmonicStep
            // and add them to the already existing wave.
            state += (std::sin(harmonicPhase * M_2PI)       // first square wave
                  +   std::sin(harmonicPhase2 * M_2PI))     // second square wave
                  /   harmonicStep;                         // volume factor
            
            // Inrement the number of generated harmonics.
            dunHarmonics+=2;
        }
        
        // Adjust the volume --> M_2_PI = 2 * M_4_PI, since we have 2 waves
        state *= M_2_PI * amplitude;
        
        // Return the generated sample value
        return state;
    }
    
}; // end class RCH::Oscillators::Templates::Bandlimited::SquarePulse


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_TEMPLATES_BANDLIMITED_SQUAREPULSE_H_INCLUDED
