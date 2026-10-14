cmake_minimum_required(VERSION 3.22)

project(SimpleSampler VERSION 1.0.0)

set(CMAKE_CXX_STNDARD 17)
set(CMAKE_CXX_STNDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SimpleSampler
    COMPANY_NAME "Portfolio"
    PRODUCT_NAME "Simple Sampler"
    PLUGIN_MANUFACTURER_CODE Port
    PLUGIN_CODE Smpl 
    FORMATS VST3 Standalone
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(SimpleSampler PRIVATE
Source/PluginProcessor.cpp
Source/PluginProcessor.h 
Source/PluginEditor.cpp
Source/PluginEditor.h
)

target_compile_definitions(SimpleSampler PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(SimpleSampler
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags        
)