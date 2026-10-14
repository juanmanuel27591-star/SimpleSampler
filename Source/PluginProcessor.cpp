#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>
#include "SilenceTrimmer.h"

SamplerAudioProcessor::SamplerAudioProcessor()
    :AudioProcessor (BusesProperties()
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
        {
            formatManager.registerBasicFormats();
        }

void SamplerAudioProcessor::prepareToPlay (double, int)
{    
}

void SamplerAudioProcessor::releaseResources()
{    
}

void SamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
keyboardState.processNextMidiBuffer (midi, 0, buffer.getNumSamples(), true);
    auto latest = std::atomic_load (&currentSample);

    if (latest != playingSample)
    {
        for (auto& voice : voices)
            voice.active = false;

            playingSample = latest;
    }
    
    int position = 0;

    for (const auto metadata : midi)
    {
        auto eventPosition = metadata.samplePosition;
        renderVoices (buffer, position, eventPosition - position);
        position = eventPosition;

        auto message = metadata.getMessage();

        if (message.isNoteOn())
            startVoice (message.getNoteNumber(),
        message.getFloatVelocity());
    }

        renderVoices (buffer, position, buffer.getNumSamples() - position);
}

void SamplerAudioProcessor::startVoice (int note, float velocity)
{
    if (playingSample == nullptr || getSampleRate() <= 0.0)
    return;

    Voice* voice = &voices[0];

    for (auto& candidate : voices)
    {
        if (! candidate.active)
        {
            voice = &candidate;
            break;
        }
    }

    auto semitones = (double) (note - rootNote);
    auto pitchRatio = std::pow (2.0, semitones / 12.0);
    auto rateRatio = playingSample->sampleRate / getSampleRate();

    voice->active = true;
    voice->position = (double) playingSample->soundStart;
    voice->ratio = pitchRatio * rateRatio;
    voice->gain = velocity;
}

void SamplerAudioProcessor::renderVoices (juce::AudioBuffer<float>& buffer,
int startSample, int numSamples)
{
    if (playingSample == nullptr)
    return;

    auto& data = playingSample->buffer;
    auto length = data.getNumSamples();
    auto dataChannels = data.getNumChannels();

    if (length < 2 || dataChannels < 1)
    return;

    for (auto& voice : voices)
    {
        if (! voice.active)
        continue;

        for (int i = 0; i < numSamples; ++i)
        {
            auto index = (int) voice.position;

            if (index + 1 >= length)
            {
                voice.active = false;
                break;
            }

            auto fraction = (float) (voice.position - index);

            for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            {
                auto* source = data.getReadPointer (
                    juce::jmin (ch, dataChannels - 1));
                    auto first = source[index];
                    auto second = source[index + 1];
                    auto value = first + fraction * (second - first);
                    buffer.addSample (ch, startSample + i, 
                    value * voice.gain);
            }

            voice.position += voice.ratio;
        }
    }
}

bool SamplerAudioProcessor::loadSample (const juce::File& file)
{
    std::unique_ptr<juce::AudioFormatReader> reader (
        formatManager.createReaderFor (file));
    
    if (reader == nullptr)
        return false;
    
        
    auto length = (int) reader->lengthInSamples;
    auto newSample = std::make_shared<SampleData>();
    newSample->sampleRate = reader->sampleRate;
    newSample->buffer.setSize ((int) reader->numChannels, length);
    reader->read (&newSample->buffer, 0, length, 0, true, true);
    
    newSample->soundStart = findFirstSoundSample (
        newSample->buffer, 0.01f);


    std::atomic_store (&currentSample,
        std::shared_ptr<const SampleData> (newSample));
    sampleName = file.getFileName();
    return true;    
}

juce::AudioProcessorEditor* SamplerAudioProcessor::createEditor()
{
    return new SamplerAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplerAudioProcessor();
}