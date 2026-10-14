#pragma once 

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <array>
#include <memory>
#include "SampleData.h"

class SamplerAudioProcessor : public juce::AudioProcessor
{
public:
    SamplerAudioProcessor();
    ~SamplerAudioProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; } 

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; } 
    bool producesMidi() const override { return false; } 
    bool isMidiEffect() const override { return false; } 
    double getTailLengthSeconds() const override { return 0.0; }
    
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock&) override {}
    void setStateInformation (const void*, int) override {}

    bool loadSample (const juce::File& file);
    const juce::String& getSampleName() const { return sampleName; }
juce::MidiKeyboardState& getKeyboardState() { return keyboardState; }

private:
    struct Voice
    {
        bool active = false;
        double position = 0.0;
        double ratio = 1.0;
        float gain = 1.0f;
    };

    void startVoice (int note, float velocity);
    void renderVoices (juce::AudioBuffer<float>& buffer,
    int startSample, int numSamples);

    static constexpr int rootNote = 60;
    juce::AudioFormatManager formatManager;
    std::shared_ptr<const SampleData> currentSample;
    std::shared_ptr<const SampleData> playingSample;
    std::array<Voice, 8> voices;
    juce::String sampleName;
    juce::MidiKeyboardState keyboardState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessor)    
};