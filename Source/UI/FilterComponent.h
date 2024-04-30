/*
  ==============================================================================

    FilterComponent.h
    Created: 27 Apr 2024 2:56:45pm
    Author:  bogdi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterCutoffId, juce::String filterResonanceId);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;

    juce::ComboBox filterTypeSelector{ "Filter Type" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectorAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;

    juce::Label filterSelectorLabel{ "Filter Type", "Filter Type" };
    juce::Label filterCutoffLabel{ "Filter Cutoff", "Filter Cutoff" };
    juce::Label filterResonanceLabel{ "Filter Res", "Filter Res" };

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramId);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
