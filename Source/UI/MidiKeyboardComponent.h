/*
  ==============================================================================

    MidiKeyboardComponent.h
    Created: 14 May 2024 7:59:16pm
    Author:  bogdi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MidiKeyboardComponent  : public juce::Component
{
public:
    MidiKeyboardComponent(juce::MidiKeyboardState& keyboardState);
    ~MidiKeyboardComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::MidiKeyboardComponent keyboard;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiKeyboardComponent)
};
