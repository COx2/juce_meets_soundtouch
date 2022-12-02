/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <BinaryData.h>

//==============================================================================
OneShotSamplerAudioProcessor::OneShotSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    audioFormatManager = std::make_unique<juce::AudioFormatManager>();
    audioFormatManager->registerBasicFormats();

    bpmDetector = std::make_unique<soundtouch::BPMDetect>(0, 48000);

    soundStretcher = std::make_unique<soundtouch::SoundTouch>();
    DBG(soundStretcher->getVersionString());
}

OneShotSamplerAudioProcessor::~OneShotSamplerAudioProcessor()
{
}

//==============================================================================
const juce::String OneShotSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OneShotSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OneShotSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OneShotSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
return false;
#endif
}

double OneShotSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OneShotSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int OneShotSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OneShotSamplerAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String OneShotSamplerAudioProcessor::getProgramName(int index)
{
    return {};
}

void OneShotSamplerAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void OneShotSamplerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    keyboardState.reset();

    auto num_channels = getMainBusNumOutputChannels();
    bpmDetector = std::make_unique<soundtouch::BPMDetect>(1, sampleRate);

    soundStretcher->setChannels(num_channels);
    soundStretcher->setSampleRate((juce::uint32)sampleRate);
}

void OneShotSamplerAudioProcessor::releaseResources()
{
    keyboardState.allNotesOff(0);
    keyboardState.reset();

    soundStretcher->clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OneShotSamplerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void OneShotSamplerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (isSynthChanging) { return; }

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    bpmDetector->inputSamples(buffer.getReadPointer(1), buffer.getNumSamples());
    outputSignalBPM = bpmDetector->getBpm();

    {
        std::vector<float> sending_samples;
        for (int sample_idx = 0; sample_idx < buffer.getNumSamples(); sample_idx++)
        {
            for (int ch_idx = 0; ch_idx < buffer.getNumChannels(); ch_idx++)
            {
                sending_samples.push_back(buffer.getSample(ch_idx, sample_idx));
            }
        }

        soundStretcher->putSamples(sending_samples.data(), buffer.getNumSamples());
    }

    if (buffer.getNumSamples() <= (int)soundStretcher->numSamples())
    {
        std::vector<float> returning_samples(buffer.getNumChannels() * buffer.getNumSamples());
        soundStretcher->receiveSamples(returning_samples.data(), buffer.getNumSamples());

        int read_counter = 0;
        for (int sample_idx = 0; sample_idx < buffer.getNumSamples(); sample_idx++)
        {
            for (int ch_idx = 0; ch_idx < buffer.getNumChannels(); ch_idx++)
            {
                buffer.setSample(ch_idx, sample_idx, returning_samples.at(read_counter));
                read_counter++;
            }
        }
    }
}

//==============================================================================
bool OneShotSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OneShotSamplerAudioProcessor::createEditor()
{
    return new OneShotSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void OneShotSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OneShotSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OneShotSamplerAudioProcessor();
}


void OneShotSamplerAudioProcessor::setupSampler(juce::AudioFormatReader& newReader)
{
    isSynthChanging = true;

    synth.clearSounds();
    synth.clearVoices();

    juce::BigInteger allNotes;
    allNotes.setRange(0, 128, true);

    synth.addSound(new juce::SamplerSound("default", newReader, allNotes, 60, 0, 0.1, 10.0));

    for (int i = 0; i < 128; i++) 
    {
        synth.addVoice(new juce::SamplerVoice());
    }

    isSynthChanging = false;
}

void OneShotSamplerAudioProcessor::loadSineWave()
{
    auto inputStream = std::make_unique<juce::MemoryInputStream>(BinaryData::sine_wav, BinaryData::sine_wavSize, true);
    auto reader = 
        std::unique_ptr<juce::AudioFormatReader>(audioFormatManager->createReaderFor(std::move(inputStream)));
    if (reader != nullptr)
    {
        setupSampler(*reader.get());
    }
}

void OneShotSamplerAudioProcessor::loadSampleFile()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Open audio file to play.", 
        juce::File(), 
        audioFormatManager->getWildcardForAllFormats());

    const auto flag = juce::FileBrowserComponent::FileChooserFlags::openMode;
    fileChooser->launchAsync(flag, 
        [this](const juce::FileChooser& chooserWithResult) {
            auto reader =
            std::unique_ptr<juce::AudioFormatReader>(audioFormatManager->createReaderFor(chooserWithResult.getResult()));
            if (reader != nullptr)
            {
                setupSampler(*reader);
            }
        }
    );
}

void OneShotSamplerAudioProcessor::setStretcherPitch(double pitch)
{
    soundStretcher->setPitch(pitch);
}
