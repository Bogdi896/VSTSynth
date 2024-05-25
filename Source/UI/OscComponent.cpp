#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId, juce::String fmWaveSelectorId, juce::String pitchWheelId, OscData& oscData)
    : oscillator(oscData)
{
    juce::StringArray choices{ "Sine", "Saw", "Square", "Experimental 1" };
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);
    oscWaveSelector.onChange = [this] { waveformTypeChanged(); };

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

void OscComponent::waveformTypeChanged()
{
    repaint();  // Trigger a repaint when the waveform type changes
}

OscComponent::~OscComponent() {}

void OscComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.fillAll(juce::Colour(0xff4d5d6b)); // Darker, muted blue background
    g.setColour(juce::Colours::white); // White text color
    g.setFont(20.0f);
    g.drawText("Oscillator", labelSpace.withX(5), juce::Justification::centred);

    // Draw the overall background for knobs and labels
    g.setColour(juce::Colour(0xff2f3b45)); // Darker gray for rounded rectangle fill
    g.fillRoundedRectangle(bounds.toFloat(), 10.0f); // Increase corner radius

    // Calculate waveform area bounds
    auto waveformBounds = bounds.removeFromRight(bounds.getWidth() / 2.5).reduced(10);
    //waveformBounds.reduce(1, 10); // Further reduce to make waveform display smaller

    // Draw the darker background for the waveform display
    g.setColour(juce::Colour(0xff1e242b)); // Even darker gray for the waveform area background
    g.fillRoundedRectangle(waveformBounds.toFloat(), 10.0f);

    // Draw waveform in the specified area
    drawWaveform(g, waveformBounds);
}

void OscComponent::resized()
{
    const auto startY = 55;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    const int sliderSpacing = -20;  // Reduced space between sliders

    oscWaveSelector.setBounds(10, startY + 5, 90, 30);
    waveSelectorLabel.setBounds(10, startY - labelYOffset, 90, labelHeight);

    fmOscWaveSelector.setBounds(10, oscWaveSelector.getBottom() + 45, 90, 30);
    fmWaveSelectorLabel.setBounds(10, fmOscWaveSelector.getY() - labelYOffset, 90, labelHeight);

    fmFreqSlider.setBounds(oscWaveSelector.getRight(), startY, sliderWidth, sliderHeight);
    fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() - labelYOffset, fmFreqSlider.getWidth(), labelHeight);

    fmDepthSlider.setBounds(fmFreqSlider.getRight() + sliderSpacing, startY, sliderWidth, sliderHeight);
    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - labelYOffset, fmDepthSlider.getWidth(), labelHeight);

    pitchSlider.setBounds(fmDepthSlider.getRight() + sliderSpacing, startY, sliderWidth, sliderHeight);
    pitchSliderLabel.setBounds(pitchSlider.getX(), pitchSlider.getY() - labelYOffset, pitchSlider.getWidth(), labelHeight);
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

void OscComponent::drawWaveform(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    // Reduce the rectangle slightly to ensure the border doesn't overlap the waveform
    auto innerBounds = bounds.reduced(1);

    // Define colors
    auto strokeColor = juce::Colour::fromString("0xFF0099FF"); // Light blue for the waveform outline
    auto fillColor = juce::Colour::fromString("0xFF0099FF").withAlpha(0.2f); // Semi-transparent darker blue for the fill

    // Create the waveform path for stroking
    juce::Path strokePath;
    strokePath.startNewSubPath(innerBounds.getX(), innerBounds.getCentreY());

    // Create the waveform path for filling
    juce::Path fillPath;
    fillPath.startNewSubPath(innerBounds.getX(), innerBounds.getCentreY());

    const int numSamples = innerBounds.getWidth();
    int waveType = oscillator.getWaveType();

    for (int i = 0; i < numSamples; ++i)
    {
        float x = static_cast<float>(i) / numSamples;
        float y = getSampleAtPosition(x, waveType);
        float yCoord = juce::jmap(y, -1.0f, 1.0f, static_cast<float>(innerBounds.getBottom()), static_cast<float>(innerBounds.getY()));

        strokePath.lineTo(innerBounds.getX() + i, yCoord);
        fillPath.lineTo(innerBounds.getX() + i, yCoord);
    }

    // Close the fill path back to the start at the center line
    fillPath.lineTo(innerBounds.getRight(), innerBounds.getCentreY());
    fillPath.closeSubPath();

    // Fill the path with semi-transparent color
    g.setColour(fillColor);
    g.fillPath(fillPath);

    // Stroke the outline with a solid color
    g.setColour(strokeColor);
    g.strokePath(strokePath, juce::PathStrokeType(2.0f));

}



float OscComponent::getSampleAtPosition(float position, int waveType)
{
    switch (waveType)
    {
    case 0: // Sine wave
        return std::sin(position * juce::MathConstants<float>::twoPi);
    case 1: // Saw wave
        return 2.0f * (position - std::floor(position + 0.5f));
    case 2: // Square wave
        return (position < 0.5f) ? 1.0f : -1.0f;
    case 3: // Experimental 1
        return std::sin(juce::MathConstants<float>::pi * position * position * 32 * std::log(position + 1));
    default:
        return 0.0f;
    }
}
