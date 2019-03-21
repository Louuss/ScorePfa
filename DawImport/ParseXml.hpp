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
QDomNodeList getClipEvents(QDomElement& elem);
QDomNodeList getList(QDomDocument& doc, QString str);
QDomNodeList getList(QDomElement& elem, QString str);
/*
QDomNodeList getMidiTracks(QDomDocument& doc);
QDomNodeList getAudioTracks(QDomDocument& doc);
QDomNodeList getSlots(QDomElement& track);
QDomNodeList getAudioClipEvents(QDomElement& track);
QDomNodeList getMidiClipEvents(QDomElement& track);
QDomNodeList getKeyTrack(QDomElement& midiClip);
QDomNodeList getNotes(QDomElement& keyTrack);
*/
int loadNotes(QDomElement& keyTrack, vector<Midi::NoteData>& notes, Midi::midi_size_t midiKeyValue);

int loadAudioClip(QDomElement& audioClipXml, AudioClip& audioClip);

int loadMidiClip(QDomElement& midiClipXml, MidiClip& midiClip);
int loadSlots(QDomElement& midiTrack, vector<ClipSlot>& clipSlots);

int loadMidiClipEvents(QDomElement& midiTrack, std::vector<MidiClip>& midiClipEvents);
int loadAudioClipEvents(QDomElement& audioTrack, std::vector<AudioClip>& audioClipEvents);

int loadMidiTracks(QDomDocument& docXml, vector<MidiTrack>& midiTracks);

int loadAudioTracks(QDomDocument& docXml, vector<AudioTrack>& audioTracks);


int loadDocument(QDomDocument& docXml, AbletonDocument& abletonDoc);
#endif
