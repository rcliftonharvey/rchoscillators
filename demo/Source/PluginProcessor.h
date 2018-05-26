/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// ONLY INCLUDE FOR RCH::OSCILLATORS LIBRARY
#include "../../library/rchosc.h"

//==============================================================================
/**
*/
class RchoscillatorsAudioProcessor  : public AudioProcessor
{
private:
    
    //==============================================================================
    // OSCILLATORS
    RCH::Oscillators::Sine          oscSine;        // multi-channel sine wave
    RCH::Oscillators::Triangle      oscTriangle;    // multi-channel triangle
    RCH::Oscillators::Saw           oscSawRise;     // multi-channel sawtooth
    RCH::Oscillators::Saw           oscSawFall;     // multi-channel sawtooth
    RCH::Oscillators::Square        oscSquare;      // multi-channel square wave
    RCH::Oscillators::SquarePulse   oscSquarePulse; // multi-channel square pulse wave
    RCH::Oscillators::Pulse         oscPulse;       // multi-channel pulse wave
    
    RCH::Oscillators::Bandlimited::Triangle     oscTriangleBL;
    RCH::Oscillators::Bandlimited::Saw          oscSawRiseBL;
    RCH::Oscillators::Bandlimited::Saw          oscSawFallBL;
    RCH::Oscillators::Bandlimited::Square       oscSquareBL;
    RCH::Oscillators::Bandlimited::SquarePulse  oscSquarePulseBL;
    RCH::Oscillators::Bandlimited::Pulse        oscPulseBL;
    
    //==============================================================================
    // HOST AUTOMATION PARAMETERS
    AudioParameterBool* bypassed = nullptr;
    
    AudioParameterBool* bandlimited = nullptr;
    AudioParameterInt* bandlimitQuality = nullptr;
    
    AudioParameterFloat* volumeInput = nullptr;
    
    AudioParameterBool* sine = nullptr;
    AudioParameterFloat* volumeSine = nullptr;
    AudioParameterFloat* frequencySine = nullptr;
    
    AudioParameterBool* triangle = nullptr;
    AudioParameterFloat* volumeTriangle = nullptr;
    AudioParameterFloat* frequencyTriangle = nullptr;
    
    AudioParameterBool* saw = nullptr;
    AudioParameterFloat* volumeSaw = nullptr;
    AudioParameterFloat* frequencySaw = nullptr;
    
    AudioParameterBool* sawReverse = nullptr;
    AudioParameterFloat* volumeSawReverse = nullptr;
    AudioParameterFloat* frequencySawReverse = nullptr;
    
    AudioParameterBool* square = nullptr;
    AudioParameterFloat* volumeSquare = nullptr;
    AudioParameterFloat* frequencySquare = nullptr;
    
    AudioParameterBool* squarePulse = nullptr;
    AudioParameterFloat* volumeSquarePulse = nullptr;
    AudioParameterFloat* frequencySquarePulse = nullptr;
    AudioParameterFloat* widthSquarePulse = nullptr;
    
    AudioParameterBool* pulse = nullptr;
    AudioParameterFloat* volumePulse = nullptr;
    AudioParameterFloat* frequencyPulse = nullptr;
    AudioParameterFloat* widthPulse = nullptr;
    
    AudioParameterFloat* volumeOutput = nullptr;
    
    //==============================================================================
    // SCALING RANGES
    NormalisableRange<double> frequencies = NormalisableRange<double>(0.0,22000.0,1.0);
    NormalisableRange<double> volumes = NormalisableRange<double> (-180.0,0.0,0.001);
    NormalisableRange<double> trims = NormalisableRange<double> (-12.0,12.0,0.001);
    
public:
    //==============================================================================
    RchoscillatorsAudioProcessor ();
    ~RchoscillatorsAudioProcessor () {}

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    bool supportsDoublePrecisionProcessing () const override {return true;}
    void processBlock (AudioBuffer<double>&, MidiBuffer&) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override {return false;}
    
    //==============================================================================
    const String getName() const override {return JucePlugin_Name;}

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override {return 0.0;}

    //==============================================================================
    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram (int index) override {}
    const String getProgramName (int index) override {return {};}
    void changeProgramName (int index, const String& newName) override {}

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
private:
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RchoscillatorsAudioProcessor)
};
