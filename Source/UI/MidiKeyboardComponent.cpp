/*
  ==============================================================================

    MidiKeyboardComponent.cpp
    Created: 14 May 2024 7:59:16pm
    Author:  bogdi

  ==============================================================================
*/

#include "MidiKeyboardComponent.h"

#include "MidiKeyboardComponent.h"

MidiKeyboardComponent::MidiKeyboardComponent(juce::MidiKeyboardState& keyboardState)
    : keyboard(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyboard);
    keyboard.setAvailableRange(24, 96);  // C1 to C7
    keyboard.setKeyWidth(16);
    keyboard.setColour(juce::MidiKeyboardComponent::whiteNoteColourId, juce::Colours::white);
    keyboard.setColour(juce::MidiKeyboardComponent::blackNoteColourId, juce::Colours::black);
    keyboard.setColour(juce::MidiKeyboardComponent::keySeparatorLineColourId, juce::Colours::grey);
    keyboard.setVelocity(1.0, true);
}

MidiKeyboardComponent::~MidiKeyboardComponent()
{
}

void MidiKeyboardComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));  // Clear the background
}

void MidiKeyboardComponent::resized()
{
    keyboard.setBounds(getLocalBounds());
}
