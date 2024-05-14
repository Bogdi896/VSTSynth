/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VSTSynth3AudioProcessorEditor::VSTSynth3AudioProcessorEditor (VSTSynth3AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    osc (audioProcessor.apvts, "OSC1WAVETYPE", "OSC1FMFREQ", "OSC1FMDEPTH", "FMOSCTYPE", "PITCH"),
    adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE"),
    filter (audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES"),
    keyboard (p.keyboardState)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (620, 550); 


    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
    addAndMakeVisible(keyboard);
    
}

VSTSynth3AudioProcessorEditor::~VSTSynth3AudioProcessorEditor()
{
}

//==============================================================================
void VSTSynth3AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

}

void VSTSynth3AudioProcessorEditor::resized()
{
    const auto paddingX = 5;
    const auto paddingY = 35;
    const auto paddingY2 = 235;

    osc.setBounds(paddingX, paddingY, 600, 200);
    //filterAdsr.setBounds(paddingX, paddingY2, 300, 200);
    filter.setBounds(paddingX, paddingY2, 300, 200);
    adsr.setBounds(filter.getRight(), paddingY2, 300, 200);
    keyboard.setBounds(10, 450, getWidth() - 20, 90);
}

