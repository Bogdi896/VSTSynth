/*
  ==============================================================================

    OscData.cpp
    Created: 26 Apr 2024 1:58:51pm
    Author:  bogdi

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    fmOsc.prepare(spec);
    prepare(spec);
}

void OscData::setWaveType(const int choice)
{
    // return std::sin (x); // Sine Wave
    // return x / juce::MathConstants<float>::pi; // Saw Wave
    // return x < 0.0f ? -1.0f : 1.0f; // Square Wave

    switch (choice)
    {
    case 0:
        // Sine 
        initialise([](float x) {return std::sin(x); });
        break;
    case 1:
        // Saw
        initialise([](float x) {return x / juce::MathConstants<float>::pi; });
        break;
    case 2:
        // Square Wave
        initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f; });
        break;
    case 3:
        // Triangle Wave
        initialise([](float x) {
            float phase = std::fmod(x, juce::MathConstants<float>::twoPi) / juce::MathConstants<float>::twoPi;
            return 2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f;
            });
        break;
    case 4:
        // Pulse Wave (SQ4 TEST)
        initialise([](float x) {
            float phase = std::fmod(x, juce::MathConstants<float>::twoPi) / juce::MathConstants<float>::twoPi;
            return phase < 0.25f ? 1.0f : -1.0f;
            });
        break;


    default:
        jassertfalse;
        break;
    }
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    lastMidiNote = midiNoteNumber;
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
            fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
        }
    }
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OscData::setFmParams(const float depth, const float freq)
{
    fmOsc.setFrequency(freq);
    fmDepth = depth;
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;
    setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}

void OscData::setFmWaveType(const int choice)
{
    switch (choice)
    {
    case 0: // Sine
        fmOsc.initialise([](float x) { return std::sin(x); });
        break;
    case 1: // Saw
        fmOsc.initialise([](float x) { return x / juce::MathConstants<float>::pi; });
        break;
    case 2: // Square
        fmOsc.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        break;
    default:
        jassertfalse; // This shouldn't happen!
        break;
    }
}
