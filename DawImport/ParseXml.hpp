#ifndef PARSEXML
#define PARSEXML
#include <stdlib.h>
#include <iostream>
#include <QtXml/QtXml>
#include "Structs.hpp"
#include <vector>
#include <Midi/MidiNote.hpp>

using namespace std;
QDomNodeList getMidiTracks(QDomDocument& doc);

QDomNodeList getSlots(QDomNode& track);

QDomNode getMidiClip(QDomNode& slot);
QDomNodeList getAudioClipEvents(QDomNode& track);
QDomNodeList getMidiClipEvents(QDomNode& track);

QDomNodeList getKeyTracks(QDomNode& midiClip);

QDomNodeList getNotes(QDomNode& keyTrack);

int loadNotes(QDomNode& keyTrack, vector<Midi::NoteData>& notes, int midiKeyValue);
int loadMidiClip(QDomNode& midiClipXml, MidiClip& midiClip);
int loadAudioClip(QDomNode& audioClipXml, AudioClip& audioClip);
int loadSlots(QDomNode& midiTrack, vector<ClipSlot*>& clipSlots);


int loadMidiTracks(QDomDocument& docXml, vector<MidiTrack *>& midiTracks);
int loadAudioTracks(QDomDocument& docXml, vector<AudioTrack *>& audioTracks);

int loadDocument(QDomDocument& docXml, AbletonDocument& abletonDoc);
#endif
