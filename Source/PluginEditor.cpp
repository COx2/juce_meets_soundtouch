/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OneShotSamplerAudioProcessorEditor::OneShotSamplerAudioProcessorEditor (OneShotSamplerAudioProcessor& p)
    : juce::AudioProcessorEditor (&p)
    , processor (p)
    , keyboardComponent(p.getMidiKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyboardComponent);
    
    sineWaveButton.setButtonText("Sine Wave");
    sineWaveButton.addListener(this);
    addAndMakeVisible(sineWaveButton);

    sampleSelectButton.setButtonText("Sample Select");
    sampleSelectButton.addListener(this);
    addAndMakeVisible(sampleSelectButton);

    sliderStretcherPitch = std::make_unique<juce::Slider>(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow);
    sliderStretcherPitch->setRange(0.25, 4.0, 0.01);
    sliderStretcherPitch->setSkewFactor(0.725);
    sliderStretcherPitch->onValueChange = [this]() {
        const auto value = sliderStretcherPitch->getValue();
        processor.setStretcherPitch(value);
    };
    sliderStretcherPitch->setValue(1.0, juce::sendNotification);
    addAndMakeVisible(sliderStretcherPitch.get());

    labelStretcherPitch = std::make_unique<juce::Label>();
    labelStretcherPitch->setText("Stretcher Pitch", juce::dontSendNotification);
    labelStretcherPitch->attachToComponent(sliderStretcherPitch.get(), false);
    addAndMakeVisible(labelStretcherPitch.get());

    setSize(800, 600);
}

OneShotSamplerAudioProcessorEditor::~OneShotSamplerAudioProcessorEditor()
{
}

//==============================================================================
void OneShotSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void OneShotSamplerAudioProcessorEditor::resized()
{
    sineWaveButton.setBoundsRelative(0.15, 0.2, 0.2, 0.2);
    sampleSelectButton.setBoundsRelative(0.4, 0.2, 0.2, 0.2);
    sliderStretcherPitch->setBoundsRelative(0.75, 0.2, 0.2, 0.2);
    keyboardComponent.setBoundsRelative(0.0, 0.7, 1.0, 0.3);
}

void OneShotSamplerAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &sineWaveButton)
    {
        processor.loadSineWave();
    }
    else if (button == &sampleSelectButton)
    {
        processor.loadSampleFile();
    }
}
