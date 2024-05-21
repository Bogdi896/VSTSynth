/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthSound.h"
#include "SynthVoice.h"

//==============================================================================
VSTSynth3AudioProcessor::VSTSynth3AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());

}

VSTSynth3AudioProcessor::~VSTSynth3AudioProcessor()
{

}

//==============================================================================
const juce::String VSTSynth3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VSTSynth3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VSTSynth3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VSTSynth3AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VSTSynth3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VSTSynth3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VSTSynth3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void VSTSynth3AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VSTSynth3AudioProcessor::getProgramName (int index)
{
    return {};
}

void VSTSynth3AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VSTSynth3AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    // Clear existing voices (if any) and add new ones
    synth.clearVoices();
    int numVoices = 8;  // Adjust number based on the maximum polyphony required
    for (int i = 0; i < numVoices; ++i)
    {
        synth.addVoice(new SynthVoice());
    }

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
    filter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());

    midiMessageCollector.reset(sampleRate);
}


void VSTSynth3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VSTSynth3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void VSTSynth3AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    midiMessageCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());

    auto& pitchWheelValue = *apvts.getRawParameterValue("PITCH");

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            // Osc controls
            // ADSR
            // LFO

            voice->getOscillator().setPitchBend(pitchWheelValue);

            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");

            auto& fmOscWaveType = *apvts.getRawParameterValue("FMOSCTYPE");
            auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
            auto& fmDepth = *apvts.getRawParameterValue("OSC1FMDEPTH");
            auto& fmFreq = *apvts.getRawParameterValue("OSC1FMFREQ");

            voice->getOscillator().setWaveType(oscWaveChoice);
            voice->getOscillator().setFmWaveType(fmOscWaveType);
            voice->getOscillator().setFmParams(fmDepth, fmFreq);
            voice->update(attack.load(), decay.load(), sustain.load(), release.load());
            
        }
    }


    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    /*for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
        float* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            channelData[sample] = softClip(channelData[sample]);
        }
    }*/  // Not sure if this works

    auto& filterType = *apvts.getRawParameterValue("FILTERTYPE");
    auto& cutoff = *apvts.getRawParameterValue("FILTERCUTOFF");
    auto& resonance = *apvts.getRawParameterValue("FILTERRES");

    filter.updateParameters(filterType, cutoff, resonance);

    filter.process(buffer);
}

//==============================================================================
bool VSTSynth3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VSTSynth3AudioProcessor::createEditor()
{
    return new VSTSynth3AudioProcessorEditor (*this);
}

//==============================================================================
void VSTSynth3AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VSTSynth3AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VSTSynth3AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout VSTSynth3AudioProcessor::createParams()
{

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    // OSC
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC", "Oscillator", juce::StringArray{ "Sine", "Saw", "Square", "Triangle", "Experimental 1" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PITCH", "Pitch Wheel Value", -12.0f, 12.0f, 0.0f));

    // Upper and Lower Limits for Pitch Wheel
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PITCHUPP", "Pitch Wheel Upper Limit", 1.0f, 12.0f, 2.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PITCHLOW", "Pitch Wheel Lower Limit", -12.0f, -1.0f, -2.0f));

    // FM
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1FMFREQ", "FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1FMDEPTH", "FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FMOSCTYPE", "FM Oscillator Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));


    // ADSR

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>(0.01f, 3.0f, 0.01f, 0.4f),
        0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>(0.01f, 1.0f, 0.01f, 0.4f),
        0.1f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>(0.01f, 3.0f, 0.01f, 0.4f),
        0.4f));

    params.push_back(std::make_unique <juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray{ "Sine", "Saw", "Square", "Triangle", "Experimental 1" }, 0));

    // Filter 

    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray{ "Low-pass", "Band-pass", "High-pass" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float> {20.0f, 20000.0f, 0.1f, 0.6f}, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float> {1.0f, 10.0f, 0.1f}, 1.0f));

    return { params.begin(), params.end() };

}

float VSTSynth3AudioProcessor::softClip(float input) { 
    // Threshold at which clipping starts
    const float threshold = 0.8f; // Adjust threshold according to your need
    const float maxLimit = 1.0f;  // Maximum output value

    if (input > threshold) {
        return maxLimit - (maxLimit - threshold) * exp(-(input - threshold));
    }
    else if (input < -threshold) {
        return -maxLimit + (maxLimit - threshold) * exp((input + threshold));
    }
    return input;
}
