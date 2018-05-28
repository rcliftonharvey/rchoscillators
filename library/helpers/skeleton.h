#pragma once
#ifndef RCH_OSCILLATORS_HELPERS_SKELETON_H_INCLUDED
#define RCH_OSCILLATORS_HELPERS_SKELETON_H_INCLUDED
// ---- MODULE CODE STARTS BELOW ---- //

    
/** Generic mono per-sample oscillator skeleton class. */
class Skeleton
{
public:
    
    Skeleton () {}
    virtual ~Skeleton () {}
    
    //==============================================================================
    /** Calculates and returns the next sample for this oscillator type.
        OVERRIDE this in the derived class. */
    virtual const double& tick ()
    {
        // OVERRIDE THIS IN THE OSCILLATOR CLASS
        return state;
    }
    
protected:
    
    double samplerate = 0.0;        // Project samplerate
    double frequency  = 0.0;        // Oscillator frequency
    double fractionFrequency = 0.0; // frequency / samplerate
    
    float amplitude = 0.5f;         // Oscillator volume
    
    double state = 0.0;             // Last oscillator state (output value)
    
    double phase = 0.0;             // Last phase position in range [0,1]
    double phaseOffset = 0.0;       // Phase start offset after oscillator was reset()
    
    double direction = 1.0;         // SAW modifier: 1.0 = rising, -1.0 = falling wave
    double width = 1.0;             // PULSE modifier: pulse width in range [0,1] per half phase
    
    //==============================================================================
    /** Call this whenever the sine stream should restart, e.g. before note on etc.
        Will reset state value to 0.0 and phase value to phaseOffset start value. */
    void reset ()
    {
        state = 0.0;            // Default output value
        phase = phaseOffset;    // Starting phase including offset
    }
    
    //==============================================================================
    /** Sets the oscillator sample rate in Hertz. */
    void setSampleRate (const double& SR)
    {
        // Only update and recalculate if new SR value is different
        if (SR != samplerate)
        {
            // Import number of samples per second
            samplerate = SR;
            
            // If the SR is changed while a Frequency was already set
            if (frequency > 0.0)
            {
                // Recalculate the per-sample phase modifier
                fractionFrequency = frequency / samplerate;
            }
            
            // Revert to reset state
            reset();
        }
    }
    
    /** Sets the oscillator center frequency in Hertz. */
    void setFrequency (const double& Hz)
    {
        // Only update and recalculate if new Hz value is different
        if (Hz != frequency)
        {
            // Import new center frequency
            frequency = Hz;
            
            // If the center frequency is changed while SR was already set
            if (samplerate > 0.0)
            {
                // Recalculate the per-sample phase modifier
                fractionFrequency = frequency / samplerate;
            }
            
            // Revert to reset state
            reset();
        }
    }
    
    /** Sets the oscillator amplitude as float gain (not dB). */
    void setAmplitude (const float& Amplitude)
    {
        // Only update if the new amplitude is different
        if (Amplitude != amplitude)
        {
            amplitude = Amplitude;
        }
    }
    
    /** Sets the oscillator volume in Decibels (use negative values). */
    void setVolume (const double& dB)
    {
        // Convert dB to float gain and send to setAmplitude()
        setAmplitude(Helpers::Decibels::ToGain(dB));
    }
    
    /** Sets the current oscillator sample state to a specific value manually. */
    void setState (const double& State)
    {
        // Only update if the new state value is different from the current state
        if (State != state)
        {
            state = State;
        }
    }
    
    /** Sets a phase starting offset for the oscillator.
        Range is in [0,1] x 1 cycle.
        This is NOT the current phase step/state value. */
    void setPhaseOffset (const double& Offset)
    {
        // Only update if new phase offset value is different
        if (Offset != phaseOffset)
        {
            phaseOffset = Offset;
        }
    }
    
    /** Sets a directional offset for saw oscillator.
        Range is [-1;1] where +1 is rising and -1 is falling wave. */
    void setDirection (const double& Direction)
    {
        if (Direction != direction)
        {
            direction = Direction;
        }
    }
    
    /** Sets the pulse width for a pulse wave oscillator.
        Range is in [0,1] where 0 = silence and 0.5 = square wave. */
    void setPulseWidth (const double& Width)
    {
        if (Width != width)
        {
            width = Width;
        }
    }
    
    /** Sets the pulse width for a square pulse wave oscillator.
        Range is in [0,1] where 0 = silence and 1 = square wave. */
    void setWidth (const double& Width)
    {
        // Needs to be halved, since will be used per 1/2 cycle
        const double newWidth = Width*0.5;
        
        if (newWidth != width)
        {
            width = newWidth;
        }
    }
    
    //==============================================================================
    /** Returns the currently set oscillator sample rate. */
    const double& getSampleRate () const
    {
        return samplerate;
    }
    
    /** Returns the currently set oscillator center frequency. */
    const double& getFrequency () const
    {
        return frequency;
    }
    
    /** Returns the currently set oscillator amplitude as float gain factor. */
    const float& getAmplitude () const
    {
        return amplitude;
    }
    
    /** Returns the currently set oscillator volume in (negative) Decibels. */
    const double getVolume () const
    {
        return Helpers::Decibels::FromGain(amplitude);
    }
    
    /** Returns the current oscillator sample state.
        Does NOT generate a new value, use .tick() for that. */
    const double& getState () const
    {
        return state;
    }
    
    /** Returns the current oscillator phase value.
        This is the actual phase step, not the reset offset. */
    const double& getPhase () const
    {
        return phase;
    }
    
    /** Returns the current oscillator phase reset offset. */
    const double& getPhaseOffset () const
    {
        return phaseOffset;
    }
    
    /** Returns the current directional offset value.
        Only applies to SAW WAVE oscillators. */
    const double& getDirection () const
    {
        return direction;
    }
    
    /** Returns the current pulse width modifier value.
        Only applies to PULSE WAVE oscillators. */
    const double& getPulseWidth () const
    {
        return width;
    }
    
    /** Returns the current square pulse width modifier value.
        Only applies to SQUARE PULSE WAVE oscillators. */
    const double getWidth () const
    {
        // Must be doubled since stored value is per 1/2 cycle
        return width * 2.0;
    }
    
}; // class RCH::Helpers::Skeleton


// ---- MODULE CODE ENDS ABOVE ---- //
#endif // #ifndef RCH_OSCILLATORS_HELPERS_SKELETON_H_INCLUDED
