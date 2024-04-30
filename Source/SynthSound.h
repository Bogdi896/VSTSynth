/*
  ==============================================================================

    SynthSound.h
    Created: 22 Apr 2024 3:15:13pm
    Author:  bogdi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override
    {
        return true;
    }
    bool appliesToChannel(int midiChannel) override
    {
        return true;
    }

};
