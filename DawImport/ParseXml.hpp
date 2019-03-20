#ifndef PARSEXML
#define PARSEXML
#include <stdlib.h>
#include <iostream>
#include <QtXml/QtXml>
#include "Structs.hpp"
#include <vector>
#include <Midi/MidiNote.hpp>

using namespace std;
QDomNodeList getNodes(QDomElement& n, QStringList args);
QDomElement getElement(QDomElement& n, QStringList args);
QDomNodeList getMidiTracks(QDomDocument& doc);
QDomNodeList getAudioTracks(QDomDocument& doc);
QDomNodeList getSlots(QDomElement& track);
QDomNodeList getAudioClipEvents(QDomElement& track);
QDomNodeList getMidiClipEvents(QDomElement& track);
QDomNodeList getKeyTrack(QDomElement& midiClip);
QDomNodeList getNotes(QDomElement& keyTrack);
QDomElement getMidiClip(QDomElement& slot);
int loadNotes(QDomElement& keyTrack, vector<Midi::NoteData>& notes, Midi::midi_size_t midiKeyValue);

int loadAudioClip(QDomElement& audioClipXml, AudioClip& audioClip);

int loadMidiClip(QDomElement& midiClipXml, MidiClip& midiClip);
int loadSlots(QDomElement& midiTrack, vector<ClipSlot>& clipSlots);

int loadMidiClipEvents(QDomElement& midiTrack, std::vector<MidiClip>& midiClipEvents);
int loadAudioClipEvents(QDomElement& audioTrack, std::vector<AudioClip>& audioClipEvents);

int loadMidiTracks(QDomDocument& docXml, vector<MidiTrack>& midiTracks);

int loadAudioTracks(QDomDocument& docXml, vector<AudioTrack>& audioTracks);


int loadDocument(QDomDocument& docXml, AbletonDocument& abletonDoc);

QString PATH_MIDI_TRACKS        = "LiveSet->Tracks->MidiTrack";
QString PATH_AUDIO_TRACKS       = "LiveSet->Tracks->AudioTrack";
QString PATH_AUDIO_CLIP_EVENTS  = "DeviceChain->MainSequencer->Sample->ArrangerAutomation->Events->AudioClip";
QString PATH_MIDI_CLIP_EVENTS   = "DeviceChain->MainSequencer->ClipTimeable->ArrangerAutomation->Events->MidiClip";
QString PATH_KEY_TRACKS         = "Notes->KeyTracks->KeyTrack";
QString PATH_NOTES              = "Notes->MidiNoteEvent";

#endif
