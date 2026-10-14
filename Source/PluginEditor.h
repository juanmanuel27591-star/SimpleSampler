#pragma once

#include "PluginProcessor.h"
#include <juce_audio_utils/juce_audio_utils.h>
class SamplerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SamplerAudioProcessorEditor (SamplerAudioProcessor&);
    ~SamplerAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void chooseFile();


    SamplerAudioProcessor& audioProcessor;
    juce::TextButton loadButton { "load sample" };
    juce::Label nameLabel;
    std::unique_ptr<juce::FileChooser> chooser;
    juce::MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessorEditor)
};