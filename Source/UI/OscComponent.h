/*
  ==============================================================================

    OscComponent.h
    Created: 26 Apr 2024 2:23:15pm
    Author:  bogdi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Data/OscData.h"

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId, juce::String fmWaveSelectorId, juce::String pitchWheelId, OscData& oscData);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;


private:

    void waveformTypeChanged();

    juce::ComboBox oscWaveSelector;
    juce::ComboBox fmOscWaveSelector;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> fmOscWaveSelectorAttachment;

    juce::Slider fmFreqSlider;
    juce::Slider fmDepthSlider;
    juce::Slider pitchSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchSliderAttachment;

    juce::Label fmWaveSelectorLabel{ "FM Wave Type", "FM Wave Type" };
    juce::Label waveSelectorLabel{ "Wave Type", "Wave Type" };
    juce::Label fmFreqLabel{"FM Freq", "FM Freq"};
    juce::Label fmDepthLabel{ "FM Depth", "FM Depth" };
    juce::Label pitchSliderLabel{ "Pitch Wheel", "Pitch Wheel" };

    OscData& oscillator;

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramId);
    void drawWaveform(juce::Graphics& g, juce::Rectangle<int> bounds);
    float getSampleAtPosition(float position, int waveType);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
