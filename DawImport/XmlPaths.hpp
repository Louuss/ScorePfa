#ifndef XML_PATHS
#define XML_PATHS

#include <QString>

QString PATH_MIDI_TRACKS = "LiveSet->Tracks->MidiTrack";
QString PATH_AUDIO_TRACKS = "LiveSet->Tracks->AudioTrack";
QString PATH_SLOTS = "DeviceChain->MainSequencer->ClipSlotList->ClipSlot";
QString PATH_AUDIO_CLIP_EVENTS
    = "DeviceChain->MainSequencer->Sample->ArrangerAutomation->Events->"
      "AudioClip";
QString PATH_MIDI_CLIP_EVENTS
    = "DeviceChain->MainSequencer->ClipTimeable->ArrangerAutomation->Events->"
      "MidiClip";
QString PATH_KEY_TRACKS = "Notes->KeyTracks->KeyTrack";
QString PATH_NOTES = "Notes->MidiNoteEvent";
QString PATH_AUDIO_CLIP_PATH
    = "SampleRef->FileRef->SearchHint->PathHint->RelativePathElement";

QString PATH_AUDIO_CLIP_NAME = "SampleRef->FileRef->Name->Value";
QString PATH_VST_NAME
    = "DeviceChain->DeviceChain->Devices->PluginDevice->PluginDesc->"
      "VstPluginInfo->PlugName->Value";
QString PATH_TRACK_NAME = "Name->EffectiveName->Value";
QString PATH_BPM_VALUE
    = "LiveSet->MasterTrack->DeviceChain->Mixer->Tempo->Manual->Value";
QString PATH_MIDIKEY_VALUE = "MidiKey->Value";

QString PATH_START_VALUE = "CurrentStart->Value";
QString PATH_END_VALUE = "CurrentEnd->Value";
QString PATH_STARTRELATIVE_VALUE = "Loop->StartRelative->Value";
QString PATH_LOOPEND_VALUE = "Loop->LoopEnd->Value";

QString NAME_VELOCITY = "Velocity";
QString NAME_DURATION = "Duration";
QString NAME_TIME = "Time";

#endif
