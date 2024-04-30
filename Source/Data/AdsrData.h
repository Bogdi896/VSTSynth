/*
  ==============================================================================

    AdsrData.h
    Created: 26 Apr 2024 1:14:13pm
    Author:  bogdi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AdsrData : public juce::ADSR
{
public:
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

private:

    juce::ADSR::Parameters adsrParams;

};