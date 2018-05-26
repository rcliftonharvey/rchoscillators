/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RchoscillatorsAudioProcessor::RchoscillatorsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    const double startupTrim = 0.0;
    const double startupFreq = 1000.0;
    const double startupVolume = -64.0;
    
    frequencies.setSkewForCentre(startupFreq);
    const float paramFreq = frequencies.convertTo0to1(startupFreq);
    
    volumes.setSkewForCentre(startupVolume);
    const float paramGain = volumes.convertTo0to1(startupVolume);
    
    trims.setSkewForCentre(startupTrim);
    const float paramTrim = trims.convertTo0to1(startupTrim);
    
    addParameter(bypassed            = new AudioParameterBool ("bypassed",              "Bypass",               false));
    
    addParameter(bandlimited         = new AudioParameterBool ("bandlimited",           "Band-limit: In",       false));
    addParameter(bandlimitQuality    = new AudioParameterInt  ("bandlimitQuality",      "Band-limit: Quality",  1,     10,    7));
    
    addParameter(volumeInput         = new AudioParameterFloat("volumeInput",           "Input: Trim",          0.0f,  1.0f,  paramTrim));
    
    addParameter(sine                = new AudioParameterBool ("sine",                  "Sine: In",             true));
    addParameter(volumeSine          = new AudioParameterFloat("volumeSine",            "Sine: Amplitude",      0.0f,  1.0f,  paramGain));
    addParameter(frequencySine       = new AudioParameterFloat("frequencySine",         "Sine: Frequency",      0.0f,  1.0f,  paramFreq));
    
    addParameter(triangle            = new AudioParameterBool ("triangle",              "Triangle: In",         false));
    addParameter(volumeTriangle      = new AudioParameterFloat("volumeTriangle",        "Triangle: Amplitude",  0.0f,  1.0f,  paramGain));
    addParameter(frequencyTriangle   = new AudioParameterFloat("frequencyTriangle",     "Triangle: Frequency",  0.0f,  1.0f,  paramFreq));
    
    addParameter(saw                 = new AudioParameterBool ("saw",                   "Saw (rise): In",       false));
    addParameter(volumeSaw           = new AudioParameterFloat("volumeSaw",             "Saw (rise): Amplitude",0.0f,  1.0f,  paramGain));
    addParameter(frequencySaw        = new AudioParameterFloat("frequencySaw",          "Saw (rise): Frequency",0.0f,  1.0f,  paramFreq));
    
    addParameter(sawReverse          = new AudioParameterBool ("sawReverse",            "Saw (fall): In",       false));
    addParameter(volumeSawReverse    = new AudioParameterFloat("volumeSawReverse",      "Saw (fall): Amplitude",0.0f,  1.0f,  paramGain));
    addParameter(frequencySawReverse = new AudioParameterFloat("frequencySawReverse",   "Saw (fall): Frequency",0.0f,  1.0f,  paramFreq));
    
    addParameter(square              = new AudioParameterBool ("square",                "Square: In",           false));
    addParameter(volumeSquare        = new AudioParameterFloat("volumeSquare",          "Square: Amplitude",    0.0f,  1.0f,  paramGain));
    addParameter(frequencySquare     = new AudioParameterFloat("frequencySquare",       "Square: Frequency",    0.0f,  1.0f,  paramFreq));
    
    addParameter(squarePulse         = new AudioParameterBool ("squarePulse",           "Square Pulse: In",         false));
    addParameter(volumeSquarePulse   = new AudioParameterFloat("volumeSquarePulse",     "Square Pulse: Amplitude",  0.0f,  1.0f,  paramGain));
    addParameter(frequencySquarePulse= new AudioParameterFloat("frequencySquarePulse",  "Square Pulse: Frequency",  0.0f,  1.0f,  paramFreq));
    addParameter(widthSquarePulse    = new AudioParameterFloat("widthSquarePulse",      "Square Pulse: Width",      0.0f,  1.0f,  0.5f));
    
    addParameter(pulse               = new AudioParameterBool ("pulse",                 "Pulse: In",            false));
    addParameter(volumePulse         = new AudioParameterFloat("volumePulse",           "Pulse: Amplitude",     0.0f,  1.0f,  paramGain));
    addParameter(frequencyPulse      = new AudioParameterFloat("frequencyPulse",        "Pulse: Frequency",     0.0f,  1.0f,  paramFreq));
    addParameter(widthPulse          = new AudioParameterFloat("widthPulse",            "Pulse: PulseWidth",    0.0f,  1.0f,  0.5f));
    
    addParameter(volumeOutput        = new AudioParameterFloat("volumeOutput",          "Output: Trim",         0.0f,  1.0f,  paramTrim));
}

// THIS IS FOR DOUBLE PRECISION BUFFERS (double samples)
void RchoscillatorsAudioProcessor::processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {buffer.clear (i, 0, buffer.getNumSamples());}
    
    // Don't need to process anything if bypassed
    if (*bypassed == true) {return;}
    
    // Convenience variables
    const unsigned int numChannels = buffer.getNumChannels();
    const unsigned int numSamples = buffer.getNumSamples();
    
    // Input trim adjustment
    if (*volumeInput != 0.5f)
    {
        buffer.applyGain(RCH::Helpers::Decibels::ToGain(trims.convertFrom0to1(*volumeInput)));
    }
    
// ----------- OSCILLATOR SECTION STARTS BELOW ------------------------------------------------------------------
    
    // SINE OSCILLATOR
    if (*sine == true)
    {
        oscSine.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySine),*volumeSine);
        oscSine.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
    }
    
    // TRIANGLE OSCILLATOR
    if (*triangle == true)
    {
        if (*bandlimited == false)
        {
            oscTriangle.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencyTriangle),*volumeTriangle);
            oscTriangle.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscTriangleBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencyTriangle),*volumeTriangle);
            oscTriangleBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscTriangleBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
    // SAW OSCILLATOR (rising)
    if (*saw == true)
    {
        if (*bandlimited == false)
        {
            oscSawRise.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySaw),*volumeSaw);
            oscSawRise.setDirection(1.0); // This makes the saw ramp up
            oscSawRise.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscSawRiseBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySaw),*volumeSaw);
            oscSawRiseBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscSawRiseBL.setDirection(1.0); // This makes the saw ramp up
            oscSawRiseBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
    // SAW OSCILLATOR (falling)
    if (*sawReverse == true)
    {
        if (*bandlimited == false)
        {
            oscSawFall.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySawReverse),*volumeSawReverse);
            oscSawFall.setDirection(-1.0); // This makes the saw ramp down
            oscSawFall.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscSawFallBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySawReverse),*volumeSawReverse);
            oscSawFallBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscSawFallBL.setDirection(-1.0); // This makes the saw ramp up
            oscSawFallBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
    // SQUARE OSCILLATOR (full pulse width)
    if (*square == true)
    {
        if (*bandlimited == false)
        {
            oscSquare.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySquare),*volumeSquare);
            oscSquare.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscSquareBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySquare),*volumeSquare);
            oscSquareBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscSquareBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
    // SQUARE PULSE OSCILLATOR ("bipolar", variable pulse width)
    // Cycle --> 0 to 1 to 0 to -1 to 0 ...
    if (*squarePulse == true)
    {
        if (*bandlimited == false)
        {
            oscSquarePulse.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySquarePulse),*volumeSquarePulse);
            oscSquarePulse.setWidth(*widthSquarePulse);
            oscSquarePulse.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscSquarePulseBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySquarePulse),*volumeSquarePulse);
            oscSquarePulseBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscSquarePulseBL.setWidth(*widthSquarePulse);
            oscSquarePulseBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
    // PULSE OSCILLATOR ("unipolar", variable pulse width)
    // Cycle --> -1 to +1 to -1 (rest) to +1 to -1 (rest) ...
    if (*pulse == true)
    {
        if (*bandlimited == false)
        {
            oscPulse.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencyPulse),*volumePulse);
            oscPulse.setPulseWidth(*widthPulse);
            oscPulse.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscPulseBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencyPulse),*volumePulse);
            oscPulseBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscPulseBL.setPulseWidth(*widthPulse);
            oscPulseBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
// ----------- OSCILLATOR SECTION ENDS ABOVE --------------------------------------------------------------------
    
    // Output trim adjustment
    if (*volumeOutput != 0.5f)
    {
        buffer.applyGain(RCH::Helpers::Decibels::ToGain(trims.convertFrom0to1(*volumeOutput)));
    }
}

// THIS IS FOR SINGLE PRECISION BUFFERS (float samples)
void RchoscillatorsAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {buffer.clear (i, 0, buffer.getNumSamples());}
    
    // Don't need to process anything if bypassed
    if (*bypassed == true) {return;}
    
    // Convenience variables
    const unsigned int numChannels = buffer.getNumChannels();
    const unsigned int numSamples = buffer.getNumSamples();
    
    // Input trim adjustment
    if (*volumeInput != 0.5f)
    {
        buffer.applyGain(RCH::Helpers::Decibels::ToGain(trims.convertFrom0to1(*volumeInput)));
    }
    
// ----------- OSCILLATOR SECTION STARTS BELOW ------------------------------------------------------------------
    
    // SINE OSCILLATOR
    if (*sine == true)
    {
        oscSine.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySine),*volumeSine);
        oscSine.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
    }
    
    // TRIANGLE OSCILLATOR
    if (*triangle == true)
    {
        if (*bandlimited == false)
        {
            oscTriangle.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencyTriangle),*volumeTriangle);
            oscTriangle.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscTriangleBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencyTriangle),*volumeTriangle);
            oscTriangleBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscTriangleBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
    // SAW OSCILLATOR (rising)
    if (*saw == true)
    {
        if (*bandlimited == false)
        {
            oscSawRise.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySaw),*volumeSaw);
            oscSawRise.setDirection(1.0); // This makes the saw ramp up
            oscSawRise.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscSawRiseBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySaw),*volumeSaw);
            oscSawRiseBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscSawRiseBL.setDirection(1.0); // This makes the saw ramp up
            oscSawRiseBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
    // SAW OSCILLATOR (falling)
    if (*sawReverse == true)
    {
        if (*bandlimited == false)
        {
            oscSawFall.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySawReverse),*volumeSawReverse);
            oscSawFall.setDirection(-1.0); // This makes the saw ramp down
            oscSawFall.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscSawFallBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySawReverse),*volumeSawReverse);
            oscSawFallBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscSawFallBL.setDirection(-1.0); // This makes the saw ramp up
            oscSawFallBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
    // SQUARE OSCILLATOR (full pulse width)
    if (*square == true)
    {
        if (*bandlimited == false)
        {
            oscSquare.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySquare),*volumeSquare);
            oscSquare.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscSquareBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySquare),*volumeSquare);
            oscSquareBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscSquareBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
    // SQUARE PULSE OSCILLATOR ("bipolar", variable pulse width)
    // Cycle --> 0 to 1 to 0 to -1 to 0 ...
    if (*squarePulse == true)
    {
        if (*bandlimited == false)
        {
            oscSquarePulse.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySquarePulse),*volumeSquarePulse);
            oscSquarePulse.setWidth(*widthSquarePulse);
            oscSquarePulse.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscSquarePulseBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencySquarePulse),*volumeSquarePulse);
            oscSquarePulseBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscSquarePulseBL.setWidth(*widthSquarePulse);
            oscSquarePulseBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
    // PULSE OSCILLATOR ("unipolar", variable pulse width)
    // Cycle --> -1 to +1 to -1 (rest) to +1 to -1 (rest) ...
    if (*pulse == true)
    {
        if (*bandlimited == false)
        {
            oscPulse.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencyPulse),*volumePulse);
            oscPulse.setPulseWidth(*widthPulse);
            oscPulse.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
        else
        {
            oscPulseBL.setup(getSampleRate(),frequencies.convertFrom0to1(*frequencyPulse),*volumePulse);
            oscPulseBL.setAccuracy(static_cast<unsigned int>(*bandlimitQuality));
            oscPulseBL.setPulseWidth(*widthPulse);
            oscPulseBL.add(buffer.getArrayOfWritePointers(),numChannels,numSamples);
        }
    }
    
// ----------- OSCILLATOR SECTION ENDS ABOVE --------------------------------------------------------------------
    
    // Output trim adjustment
    if (*volumeOutput != 0.5f)
    {
        buffer.applyGain(RCH::Helpers::Decibels::ToGain(trims.convertFrom0to1(*volumeOutput)));
    }
}

//==============================================================================
void RchoscillatorsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

//==============================================================================
bool RchoscillatorsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RchoscillatorsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RchoscillatorsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RchoscillatorsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
void RchoscillatorsAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml ("RCHOscillatorsDemoSettings");
    
    for (auto* param : getParameters())
    {
        if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param))
        {
            xml.setAttribute (p->paramID, p->getValue());
        }
    }
    
    copyXmlToBinary (xml, destData);
}

void RchoscillatorsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName ("RCHOscillatorsDemoSettings"))
        {
            for (auto* param : getParameters())
            {
                if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param))
                {
                    p->setValue ((float) xmlState->getDoubleAttribute (p->paramID, p->getValue()));
                }
            }
        }
    }
}

//==============================================================================
AudioProcessorEditor* RchoscillatorsAudioProcessor::createEditor()
{
    return new RchoscillatorsAudioProcessorEditor (*this);
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RchoscillatorsAudioProcessor();
}
