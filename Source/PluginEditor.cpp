/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FirstEQAudioProcessorEditor::FirstEQAudioProcessorEditor (FirstEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
peakFreqSliderAttachment(audioProcessor.apvts, "Peak Freq", peakFreqSlider),
peakGainSliderAttachment(audioProcessor.apvts, "Peak Gain", peakGainSlider),
peakQualitySliderAttachment(audioProcessor.apvts, "Peak Quality", peakQualitySlider),
lowCutFreqSliderAttachment(audioProcessor.apvts, "LowCut Freq", lowCutFreqSlider),
highCutFreqSliderAttachment(audioProcessor.apvts, "HighCut Freq", highCutFreqSlider),
lowCutSlopeSliderAttachment(audioProcessor.apvts, "LowCut Slope", lowCutSlopeSlider),
highCutSlopeSliderAttachment(audioProcessor.apvts, "HighCut Slope", highCutSlopeSlider)
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
    
    using namespace juce;
    
    g.fillAll (Colours::black);
    
    auto bounds = getLocalBounds();
    auto responseArea = bounds.removeFromTop(bounds.getHeight()*0.33);
    
    auto w = responseArea.getWidth();
    
    auto &lowcut = monoChain.get<ChainPossitions::LowCut>();
    auto &peak = monoChain.get<ChainPossitions::Peak>();
    auto &highcut = monoChain.get<ChainPossitions::HighCut>();
    
    auto sampleRate = audioProcessor.getSampleRate();
    
    std::vector<double> mags;
    
    mags.resize(w);
    
    for(int i=0; i<w; i++){
        double mag = 1.f;
        auto freq = mapToLog10((double)i / (double)w, 20.0, 20000.0);
        
        if(!monoChain.isBypassed<ChainPossitions::Peak>())
            mag *= peak.coefficients->getMagnitudeForFrequency(freq, sampleRate);
        
        if(!lowcut.isBypassed<0>())
            mag *= lowcut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        if(!lowcut.isBypassed<1>())
            mag *= lowcut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        if(!lowcut.isBypassed<2>())
            mag *= lowcut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        if(!lowcut.isBypassed<3>())
            mag *= lowcut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        
        if(!highcut.isBypassed<0>())
            mag *= highcut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        if(!highcut.isBypassed<1>())
            mag *= highcut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        if(!highcut.isBypassed<2>())
            mag *= highcut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        if(!highcut.isBypassed<3>())
            mag *= highcut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        
        mags[i]=Decibels::gainToDecibels(mag);
    }
    
    Path responseCurve;
    
    const double outputMin = responseArea.getBottom();
    const double outputMax = responseArea.getY();
    auto map = [outputMin, outputMax](double input){
        return jmap(input, -24.0, 24.0, outputMin, outputMax);
    };
    
    responseCurve.startNewSubPath(responseArea.getX(), map(mags.front()));
    
    for(size_t i=1; i<mags.size(); i++)
    responseCurve.lineTo(responseArea.getX()+i, map(mags[i]));
    
    g.setColour(Colours::orange);
    g.drawRoundedRectangle(responseArea.toFloat(), 20.f, 1.f);
    
    g.setColour(Colours::white);
    g.strokePath(responseCurve, PathStrokeType(2.f));
    
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

void FirstEQAudioProcessorEditor::parameterValueChanged(int parameterIndex, float newValue){
    parametersChanged.set(true);
}

void FirstEQAudioProcessorEditor::timerCallback(){
    if (parametersChanged.compareAndSetBool(false, true)){
        //update monochain
        //signal repaint
    }
}

std::vector<juce::Component*> FirstEQAudioProcessorEditor::getComps()
{
    return {
        &peakFreqSlider, &peakGainSlider, &peakQualitySlider, &lowCutFreqSlider, &highCutFreqSlider, &lowCutSlopeSlider, &highCutSlopeSlider
    };
}
