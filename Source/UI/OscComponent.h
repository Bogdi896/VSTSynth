/*
  ==============================================================================

    OscComponent.h
    Created: 26 Apr 2024 2:23:15pm
    Author:  bogdi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId, juce::String fmOscWaveSelectorId);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ComboBox oscWaveSelector;
    juce::ComboBox fmOscWaveSelector;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> fmOscWaveSelectorAttachment;

    juce::Slider fmFreqSlider;
    juce::Slider fmDepthSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmDepthAttachment;

    juce::Label fmWaveSelectorLabel{ "FM Wave Type", "FM Wave Type" };
    juce::Label waveSelectorLabel{ "Wave Type", "Wave Type" };
    juce::Label fmFreqLabel{"FM Freq", "FM Freq"};
    juce::Label fmDepthLabel{ "FM Depth", "FM Depth" };

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramId);



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
