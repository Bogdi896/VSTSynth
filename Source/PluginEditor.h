/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"

//==============================================================================
/**
*/
class VSTSynth3AudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::MidiKeyboardStateListener, private juce::Timer
  
{
public:
    VSTSynth3AudioProcessorEditor (VSTSynth3AudioProcessor&);
    ~VSTSynth3AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

private:

    VSTSynth3AudioProcessor& audioProcessor;
    OscComponent osc;
    AdsrComponent adsr;
    FilterComponent filter;
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboard;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VSTSynth3AudioProcessorEditor)
};
