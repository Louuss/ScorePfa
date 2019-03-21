#ifndef XML_PATHS
#define XML_PATHS

#include <QString>

QString PATH_MIDI_TRACKS        = "LiveSet->Tracks->MidiTrack";
QString PATH_AUDIO_TRACKS       = "LiveSet->Tracks->AudioTrack";
QString PATH_SLOTS              = "DeviceChain->MainSequencer->ClipSlotList->ClipSlot";
QString PATH_AUDIO_CLIP_EVENTS  = "DeviceChain->MainSequencer->Sample->ArrangerAutomation->Events->AudioClip";
QString PATH_MIDI_CLIP_EVENTS   = "DeviceChain->MainSequencer->ClipTimeable->ArrangerAutomation->Events->MidiClip";
QString PATH_KEY_TRACKS         = "Notes->KeyTracks->KeyTrack";
QString PATH_NOTES              = "Notes->MidiNoteEvent";

#endif
