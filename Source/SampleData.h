#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

struct SampleData
{
    juce::AudioBuffer<float> buffer;
    double sampleRate = 0.0;
    int soundStart = 0;
};