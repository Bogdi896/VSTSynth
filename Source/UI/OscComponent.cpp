/*
  ==============================================================================

    OscComponent.cpp
    Created: 26 Apr 2024 2:23:15pm
    Author:  bogdi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId, juce::String fmWaveSelectorId, juce::String pitchWheelId)
{
    juce::StringArray choices{ "Sine", "Saw", "Square", "Triangle", "Experimental 1"};
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);

    juce::StringArray choices2{ "Sine", "Saw", "Square" };
    fmOscWaveSelector.addItemList(choices2, 1);
    addAndMakeVisible(fmOscWaveSelector);

    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);
    fmOscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, fmWaveSelectorId, fmOscWaveSelector);

    waveSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    waveSelectorLabel.setFont(15.0f);
    waveSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(waveSelectorLabel);

    fmWaveSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    fmWaveSelectorLabel.setFont(15.0f);
    fmWaveSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(fmWaveSelectorLabel);


    setSliderWithLabel(fmFreqSlider, fmFreqLabel, fmFreqAttachment, apvts, fmFreqId);
    setSliderWithLabel(fmDepthSlider, fmDepthLabel, fmDepthAttachment, apvts, fmDepthId);
    setSliderWithLabel(pitchSlider, pitchSliderLabel, pitchSliderAttachment, apvts, pitchWheelId);

}

OscComponent::~OscComponent()
{
}

void OscComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.fillAll(juce::Colour(0xff4d5d6b)); // Darker, muted blue background
    g.setColour(juce::Colours::white); // White text color
    g.setFont(20.0f);
    g.drawText("Oscillator", labelSpace.withX(5), juce::Justification::centred);

    g.setColour(juce::Colour(0xff2f3b45)); // Darker gray for rounded rectangle fill
    g.fillRoundedRectangle(bounds.toFloat(), 10.0f); // Increase corner radius

    g.setColour(juce::Colour(0xff1e242b)); // Even darker gray for rounded rectangle border
    g.drawRoundedRectangle(bounds.toFloat(), 10.0f, 2.0f);
}


void OscComponent::resized()
{
    const auto startY = 55;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    oscWaveSelector.setBounds(10, startY + 5, 90, 30);
    waveSelectorLabel.setBounds(10, startY - labelYOffset, 90, labelHeight);

    fmOscWaveSelector.setBounds(10, oscWaveSelector.getBottom() + 45, 90, 30);
    fmWaveSelectorLabel.setBounds(10, fmOscWaveSelector.getY() - labelYOffset, 90, labelHeight);

    fmFreqSlider.setBounds(oscWaveSelector.getRight(), startY, sliderWidth, sliderHeight);
    fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() - labelYOffset, fmFreqSlider.getWidth(), labelHeight);

    fmDepthSlider.setBounds(fmFreqSlider.getRight(), startY, sliderWidth, sliderHeight);
    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - labelYOffset, fmDepthSlider.getWidth(), labelHeight);

    pitchSlider.setBounds(fmDepthSlider.getRight(), startY, sliderWidth, sliderHeight);
    pitchSliderLabel.setBounds(pitchSlider.getX(), pitchSlider.getY() - labelYOffset, pitchSlider.getWidth(), labelHeight);

    //DBG("oscWaveSelector bounds: " + oscWaveSelector.getBounds().toString());
    //DBG("fmOscWaveSelector bounds: " + fmOscWaveSelector.getBounds().toString());

}

void OscComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramId)
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