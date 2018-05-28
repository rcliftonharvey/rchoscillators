#pragma once
#ifndef RCH_OSCILLATORS_HELPERS_WRAPPER_H_INCLUDED
#define RCH_OSCILLATORS_HELPERS_WRAPPER_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //

    
/** Generic multi-channel capable oscillator wrapper.
    This will generate a MONO oscillator wave and duplicate
    it to all channels of a passed sample buffer. */
template <class OSCILLATOR_TEMPLATE>
class Wrapper
{
public:
    
    Wrapper () {}
    virtual ~Wrapper () {}
    
private:
    
    OSCILLATOR_TEMPLATE generator;
    
protected:
    
    /** Resets the current oscillator state to the start of a new cycle. */
    void reset ()
    {
        generator.reset();
    }
    
    //==============================================================================
    /** Sets the oscillator sample rate in Hertz. */
    void setSampleRate (const double& SR)
    {
        generator.setSampleRate(SR);
    }
    
    /** Sets the oscillator center frequency in Hertz. */
    void setFrequency (const double& Hz)
    {
        generator.setFrequency(Hz);
    }
    
    /** Sets the oscillator amplitude as float gain (not dB). */
    void setAmplitude (const float& Amplitude)
    {
        generator.setAmplitude(Amplitude);
    }
    
    /** Sets the oscillator volume in Decibels (use negative values). */
    void setVolume (const double& dB)
    {
        generator.setVolume(dB);
    }
    
    /** Sets the current oscillator sample state to a specific value manually. */
    void setState (const double& State)
    {
        generator.setState(State);
    }
    
    /** Sets a phase starting offset for the oscillator.
        Range is in [0,1] x 1 cycle.
        This is NOT the current phase step/state value. */
    void setPhaseOffset (const double& Offset)
    {
        generator.setPhaseOffset(Offset);
    }
    
    /** Sets a directional offset for saw oscillator.
        Range is [-1;1] where +1 is rising and -1 is falling wave. */
    void setDirection (const double& Direction)
    {
        generator.setDirection(Direction);
    }
    
    /** Sets the pulse width for a pulse wave oscillator.
        Range is in [0,1] where 0 = silence and 0.5 = square wave. */
    void setPulseWidth (const double& PulseWidth)
    {
        generator.setPulseWidth(PulseWidth);
    }
    
    /** Sets the pulse width for a square pulse wave oscillator.
        Range is in [0,1] where 0 = silence and 1 = square wave. */
    void setWidth (const double& PulseWidth)
    {
        generator.setWidth(PulseWidth);
    }
    
    /** Band-limited oscillators only! Sets the amount of harmonics that will be
        calculated. Less = lighter on CPU, more = higher precision, default is 7. */
    void setAccuracy (const unsigned int& Quality)
    {
        generator.setMaxHarmonics(Quality);
    }
    
    //==============================================================================
    /** Convenience function to set up most parameters at once.
        Accepts an optional double Phase Offset parameter at the end. */
    void setup (const double& SR, const double& Hz, const float& Amplitude, const double& Phase=0.0)
    {
        setSampleRate(SR);
        setFrequency(Hz);
        setAmplitude(Amplitude);
        
        if (Phase != 0.0)
        {
            setPhaseOffset(Phase);
        }
    }
    
    /** Convenience function to set up most parameters at once.
        Accepts an optional double Phase Offset parameter at the end. */
    void setup (const double& SR, const double& Hz, const double& Volume, const double& Phase=0.0)
    {
        setSampleRate(SR);
        setFrequency(Hz);
        setVolume(Volume);
        
        if (Phase != 0.0)
        {
            setPhaseOffset(Phase);
        }
    }
    
    //==============================================================================
    /** Returns the currently set oscillator sample rate. */
    const double& getSampleRate () const
    {
        return generator.getSampleRate();
    }
    
    /** Returns the currently set oscillator frequency. */
    const double& getFrequency () const
    {
        return generator.getFrequency();
    }
    
    /** Returns the currently set oscillator amplitude as float gain factor. */
    const float& getAmplitude () const
    {
        return generator.getAmplitude();
    }
    
    /** Returns the currently set oscillator volume in (negative) Decibels. */
    const double& getVolume () const
    {
        return generator.getVolume();
    }
    
    /** Returns the current oscillator sample state.
        Does NOT generate a new value, use .tick() for that. */
    const double& getState () const
    {
        return generator.getState();
    }
    
    /** Returns the current oscillator phase value.
        This is the actual phase step, not the reset offset. */
    const double& getPhase () const
    {
        return generator.getPhase();
    }
    
    /** Returns the current oscillator phase reset offset. */
    const double& getPhaseOffset () const
    {
        return generator.getPhaseOffset();
    }
    
    /** Returns the current directional offset value.
        Only applies to SAW WAVE oscillators. */
    const double& getDirection () const
    {
        return generator.getDirection();
    }
    
    /** Returns the current pulse width modifier value.
        Only applies to PULSE WAVE oscillators. */
    const double& getPulseWidth () const
    {
        return generator.getPulseWidth();
    }
    
    /** Returns the current square pulse width modifier value.
        Only applies to SQUARE PULSE WAVE oscillators. */
    const double& getWidth () const
    {
        return generator.getWidth();
    }
    
    const unsigned int& getAccuracy () const
    {
        return generator.getMaxHarmonics();
    }
    
public:
    
    //==============================================================================
    /** Fills an entire Buffer of DOUBLE samples with the same mono oscillator wave on all channels.
        This will overwrite any signal previously in the Buffer. */
    void fill (double** Buffer, const unsigned int& NumChannels, const unsigned int& NumSamples)
    {
        // If at least one channel present in the Buffer
        if (NumChannels > 0)
        {
            // Cycle through all the samples in the first channel
            for (unsigned int sample=0; sample<NumSamples; ++sample)
            {
                // Fill each sample with the next oscillator tick sample
                Buffer[0][sample] = generator.tick();
            }
        }
        
        // If more than 1 channel present in the Buffer
        if (NumChannels > 1)
        {
            // Cycle through all remaining channels in the buffer
            for (unsigned int channel=1; channel<NumChannels; ++channel)
            {
                // Cycle through all the samples in this channel
                for (unsigned int sample=0; sample<NumSamples; ++sample)
                {
                    // Copy the values of the first channel over to this one
                    Buffer[channel][sample] = Buffer[0][sample];
                }
            }
        }
        
        // The passed Buffer now contains the oscillator signal on all its channels
    }
    
    /** Fills an entire Buffer of FLOAT samples with the same mono oscillator wave on all channels.
        This will overwrite any signal previously in the Buffer. */
    void fill (float** Buffer, const unsigned int& NumChannels, const unsigned int& NumSamples)
    {
        // If at least one channel present in the Buffer
        if (NumChannels > 0)
        {
            // Cycle through all its samples
            for (unsigned int sample=0; sample<NumSamples; ++sample)
            {
                // Fill each sample with the next oscillator tick sample
                Buffer[0][sample] = static_cast<float>(generator.tick());
            }
        }
        
        // If more than 1 channel present in the Buffer
        if (NumChannels > 1)
        {
            // Cycle through all remaining channels in the buffer
            for (unsigned int channel=1; channel<NumChannels; ++channel)
            {
                // Cycle through all the samples in this channel
                for (unsigned int sample=0; sample<NumSamples; ++sample)
                {
                    // Copy the values of the first channel over to this one
                    Buffer[channel][sample] = Buffer[0][sample];
                }
            }
        }
        
        // The passed Buffer now contains the oscillator signal on all its channels
    }
    
    //==============================================================================
    /** Adds the same mono oscillator wave to all channels of the passed Buffer of DOUBLE samples.
        This will keep any signal previously in the Buffer and add to it. */
    void add (double** Buffer, const unsigned int& NumChannels, const unsigned int& NumSamples)
    {
        // Only proceed if at least one channel present in Buffer
        if (NumChannels > 0)
        {
            // Create a temporary channel stream that will contain the oscillator signal
            double signal [NumSamples];
            
            // Cycle through all samples in the signal buffer
            for (unsigned int sample=0; sample<NumSamples; ++sample)
            {
                // Fill each sample with the next oscillator tick sample
                signal[sample] = generator.tick();
            }
            
            // Cycle through all channels in the passed Buffer
            for (unsigned int channel=0; channel<NumChannels; ++channel)
            {
                // Cycle through all the samples in this channel
                for (unsigned int sample=0; sample<NumSamples; ++sample)
                {
                    // Copy the values of the first channel over to this one
                    Buffer[channel][sample] += signal[sample];
                }
            }
        }
        
        // The passed Buffer now contains its original signal plus the oscillator signal on all channels
    }
    
    /** Adds the same mono oscillator wave to all channels of the passed Buffer of FLOAT samples.
        This will keep any signal previously in the Buffer and add to it. */
    void add (float** Buffer, const unsigned int& NumChannels, const unsigned int& NumSamples)
    {
        // Only proceed if at least one channel present in Buffer
        if (NumChannels > 0)
        {
            // Create a temporary channel stream that will contain the oscillator signal
            float signal [NumSamples];
            
            // Cycle through all samples in the signal buffer
            for (unsigned int sample=0; sample<NumSamples; ++sample)
            {
                // Fill each sample with the next oscillator tick sample
                signal[sample] = static_cast<float>(generator.tick());
            }
            
            // Cycle through all channels in the passed Buffer
            for (unsigned int channel=0; channel<NumChannels; ++channel)
            {
                // Cycle through all the samples in this channel
                for (unsigned int sample=0; sample<NumSamples; ++sample)
                {
                    // Copy the values of the first channel over to this one
                    Buffer[channel][sample] += signal[sample];
                }
            }
        }
        
        // The passed Buffer now contains its original signal plus the oscillator signal on all channels
    }
    
}; // end class RCH::Helpers::Wrapper


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_HELPERS_WRAPPER_H_INCLUDED
