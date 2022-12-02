/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class OneShotSamplerAudioProcessorEditor
    : public juce::AudioProcessorEditor
    , private juce::Button::Listener
    , private juce::Timer
{
public:
    //==============================================================================
    OneShotSamplerAudioProcessorEditor (OneShotSamplerAudioProcessor&);
    ~OneShotSamplerAudioProcessorEditor();

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    void buttonClicked(juce::Button* button) override;

    //==============================================================================
    virtual void timerCallback() override;

    //==============================================================================
    juce::TextButton sineWaveButton;
    juce::TextButton sampleSelectButton;
    std::unique_ptr<juce::Label> labelSignalBPM;
    std::unique_ptr<juce::Slider> sliderStretcherPitch;
    juce::MidiKeyboardComponent keyboardComponent;

    OneShotSamplerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneShotSamplerAudioProcessorEditor)
};
