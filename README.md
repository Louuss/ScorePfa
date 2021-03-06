# Daw Import For OSSIA Score

This is an addon for OSSIA Score that allows users to import projects from Ableton Live.

## Getting Started

To use this adddon you need to add it to the addons directory in the Score src directory and recompile Score.


## Functionalities

In this early development phase, this addon is capable of creating midi tracks and audio tracks based on those in the Ableton Live sequencer. It will attempt to find audio clips in audio tracks and VSTs for midi tracks based on the paths given by Ableton. It will not however configure the VSTs for instruments or add any effects that were in Ableton.

## Useful parts

The Structs.hpp found in DawImport is meant to be the foundation of a generic architecture to represent Ableton Projects. It uses std::variants for type unions to be visited with std::visit. To remain generic, this structure base must never be dependent on any non standard libraries.

## Documentation

This project uses Doxygen comments for it's documentation.
To compile documentation run `doxygen Doxyfile` from the documentation folder.

## TODO

* Emprove score integration to include the use of relativeStart in ClipEvents to start loops from a point in the clip.
* Implement the Device Chain system used in Ableton Live to copy all effects
* Import VST configuration from Ableton




## Acknowledgments

* We would like to thank Jean-Michaël Celerier for his help overseeing the development of this addon
