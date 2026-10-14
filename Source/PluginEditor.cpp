#include "PluginEditor.h"

SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (
SamplerAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p),
keyboard (p.getKeyboardState(),
    juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible (loadButton);
    addAndMakeVisible (nameLabel);
    addAndMakeVisible (keyboard);
    keyboard.setAvailableRange (48, 84);
    keyboard.setKeyWidth (20.0f);
    nameLabel.setText ("No sample loaded", juce::dontSendNotification);
    nameLabel.setJustificationType (juce::Justification::centred);
    loadButton.onClick = [this] { chooseFile(); };

    setSize (500, 300);
}

void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText ("Simple Sampler", getLocalBounds(),
                juce::Justification::centred);
}

void SamplerAudioProcessorEditor::resized()
{
 loadButton.setBounds (150, 20, 200, 40);
keyboard.setBounds (20, 240, 460, 100);
 nameLabel.setBounds (20, 240, 460, 30);  
}

void SamplerAudioProcessorEditor::chooseFile()
{
    chooser = std::make_unique<juce::FileChooser> (
        "Select a sample", juce::File(), "*.wav;*.aiff;*.aif;*.flac");

        chooser->launchAsync (
            juce::FileBrowserComponent::openMode
            | juce::FileBrowserComponent::canSelectFiles,
            [this] (const juce::FileChooser& fc)
            {
                auto file = fc.getResult();

                if (file == juce::File())
                    return;

                if (audioProcessor.loadSample (file))
                nameLabel.setText (audioProcessor.getSampleName(),
                        juce::dontSendNotification);
                else
                    nameLabel.setText ("Could not read file",
                                        juce::dontSendNotification);    
            });
}