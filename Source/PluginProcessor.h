/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <soundtouch/SoundTouch.h>
#include <soundtouch/BPMDetect.h>

//==============================================================================
class OneShotSamplerAudioProcessor
    : public juce::AudioProcessor
{
public:
    //==============================================================================
    OneShotSamplerAudioProcessor();
    ~OneShotSamplerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    void setupSampler(juce::AudioFormatReader& newReader);
    void loadSineWave();
    void loadSampleFile();

    juce::MidiKeyboardState& getMidiKeyboardState() { return keyboardState; };
    float getOutputSignalBPM() { return outputSignalBPM; }

    void setStretcherPitch(double pitch);

private:
    juce::Synthesiser synth;
    juce::MidiKeyboardState keyboardState;
    std::atomic<bool> isSynthChanging;
    std::unique_ptr<juce::FileChooser> fileChooser;
    std::unique_ptr<juce::AudioFormatManager> audioFormatManager;

    std::unique_ptr<soundtouch::BPMDetect> bpmDetector;
    float outputSignalBPM {0};

    std::unique_ptr<soundtouch::SoundTouch> soundStretcher;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneShotSamplerAudioProcessor)
};
