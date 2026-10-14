#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <cmath>

inline int findFirstSoundSample (const juce::AudioBuffer<float>& buffer, float threshold)

{
    for (int i = 0; i < buffer.getNumSamples(); ++i)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        if (std::abs (buffer.getSample (ch, i)) >=threshold)
            return i;
    }
}

    return 0;
}