/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FirstEQAudioProcessorEditor::FirstEQAudioProcessorEditor (FirstEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    for(auto comp : getComps()){
        addAndMakeVisible(comp);
    }
    
    setSize (600, 400);
}

FirstEQAudioProcessorEditor::~FirstEQAudioProcessorEditor()
{
}

//==============================================================================
void FirstEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void FirstEQAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto bounds = getLocalBounds();
    
    auto responseArea = bounds.removeFromTop(bounds.getHeight()*0.33);
    
    auto lowCutArea = bounds.removeFromLeft(bounds.getWidth()*0.33);
    auto highCutArea = bounds.removeFromRight(bounds.getWidth()*0.5);
    auto eqArea = bounds;
    
    lowCutFreqSlider.setBounds(lowCutArea.removeFromTop(lowCutArea.getHeight()*0.5));
    lowCutSlopeSlider.setBounds(lowCutArea);
    
    
    highCutFreqSlider.setBounds(highCutArea.removeFromTop(highCutArea.getHeight()*0.5));
    highCutSlopeSlider.setBounds(highCutArea);
    
    
    
    peakFreqSlider.setBounds(eqArea.removeFromTop(eqArea.getHeight()*0.33));
    peakGainSlider.setBounds(eqArea.removeFromTop(eqArea.getHeight()*0.5));
    peakQualitySlider.setBounds(eqArea);

}

std::vector<juce::Component*> FirstEQAudioProcessorEditor::getComps()
{
    return {
        &peakFreqSlider, &peakGainSlider, &peakQualitySlider, &lowCutFreqSlider, &highCutFreqSlider, &lowCutSlopeSlider, &highCutSlopeSlider
    };
}
