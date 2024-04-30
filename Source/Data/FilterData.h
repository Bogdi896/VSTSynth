/*
  ==============================================================================

    FilterData.h
    Created: 27 Apr 2024 2:18:00pm
    Author:  bogdi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterData
{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(const int filterType, const float frequency, const float resonance);
    void reset();

private:
    juce::dsp::StateVariableTPTFilter<float> filter;
    bool isPrepared{ false };
};
