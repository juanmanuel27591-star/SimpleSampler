1 # Simple Sampler
2 
3 A simple sampler plugin (VST3 + Standalone) built with C++ and JUCE.
4 
5 Load an audio file, the silence at the start is trimmed automatically,
6 and the sample plays pitched across the keyboard. Play it with a MIDI
7 controller or with the on-screen keyboard.
8 
9 ## Features
10 
11 - Load .wav, .aiff or .flac files with a button
12 - Automatic trimming of the initial silence
13 - Pitch follows the MIDI note (root note 60)
14 - 8 voices, sample-accurate note timing
15 - On-screen keyboard
16 
17 ## Build (Windows)
18 
19 Requires Visual Studio with C++ tools, CMake and Git.
20 JUCE is downloaded automatically with FetchContent.
21 
22 1. Open the Developer Command Prompt for VS
23 2. cd into the project folder
24 3. cmake -B build
25 4. cmake --build build --config Release
26 
27 ## Known limitations
28 
29 - Note off is ignored (one-shot playback, no envelope)
30 - The loaded sample is not saved with the DAW project
31 - Fixed silence threshold (-40 dB), start of the file only
32 - Whole file is loaded into memory
33 
34 ## License
35 
36 Built with JUCE. Check JUCE's license terms before choosing one.