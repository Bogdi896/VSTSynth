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
#include "UI/MidiKeyboardComponent.h"

//==============================================================================
/**
*/
class VSTSynth3AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    VSTSynth3AudioProcessorEditor (VSTSynth3AudioProcessor&);
    ~VSTSynth3AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    VSTSynth3AudioProcessor& audioProcessor;
    OscComponent osc;
    AdsrComponent adsr;
    FilterComponent filter;
    MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VSTSynth3AudioProcessorEditor)
};
