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
    juce::TextButton sineWaveButton;
    juce::TextButton sampleSelectButton;
    std::unique_ptr<juce::Slider> sliderStretcherPitch;
    std::unique_ptr<juce::Label> labelStretcherPitch;
    juce::MidiKeyboardComponent keyboardComponent;

    OneShotSamplerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneShotSamplerAudioProcessorEditor)
};
