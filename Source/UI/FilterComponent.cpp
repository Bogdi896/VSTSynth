/*
  ==============================================================================

    FilterComponent.cpp
    Created: 27 Apr 2024 2:56:45pm
    Author:  bogdi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterCutoffId, juce::String filterResonanceId)
{

    juce::StringArray choices{ "Low-Pass", "Band-pass", "High-pass" };
    filterTypeSelector.addItemList(choices, 1);
    addAndMakeVisible(filterTypeSelector);

    filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectorId, filterTypeSelector);

    filterSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterSelectorLabel.setFont(15.0f);
    filterSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(filterSelectorLabel);


    setSliderWithLabel(filterCutoffSlider, filterCutoffLabel, filterCutoffAttachment, apvts, filterCutoffId);
    setSliderWithLabel(filterResonanceSlider, filterResonanceLabel, filterResonanceAttachment, apvts, filterResonanceId);
}

FilterComponent::~FilterComponent()
{

}

void FilterComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.fillAll(juce::Colour(0xff4d5d6b)); // Darker, muted blue background
    g.setColour(juce::Colours::white); // White text color
    g.setFont(20.0f);
    g.drawText("Filter", labelSpace.withX(5), juce::Justification::centred);

    g.setColour(juce::Colour(0xff2f3b45)); // Darker gray for rounded rectangle fill
    g.fillRoundedRectangle(bounds.toFloat(), 10.0f); // Increase corner radius

    g.setColour(juce::Colour(0xff1e242b)); // Even darker gray for rounded rectangle border
    g.drawRoundedRectangle(bounds.toFloat(), 10.0f, 2.0f);
}



void FilterComponent::resized()
{
    const auto startY = 55;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    filterTypeSelector.setBounds(10, startY + 5, 90, 30);
    filterSelectorLabel.setBounds(10, startY - labelYOffset, 90, labelHeight);

    filterCutoffSlider.setBounds(filterTypeSelector.getRight(), startY, sliderWidth, sliderHeight);
    filterCutoffLabel.setBounds(filterCutoffSlider.getX(), filterCutoffSlider.getY() - labelYOffset, filterCutoffSlider.getWidth(), labelHeight);

    filterResonanceSlider.setBounds(filterCutoffSlider.getRight(), startY, sliderWidth, sliderHeight);
    filterResonanceLabel.setBounds(filterResonanceSlider.getX(), filterResonanceSlider.getY() - labelYOffset, filterResonanceSlider.getWidth(), labelHeight);
}

void FilterComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramId)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramId, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setJustificationType(juce::Justification::centred);
    label.setFont(15.0f);
    addAndMakeVisible(label);
}
