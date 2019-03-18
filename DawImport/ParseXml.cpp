/**
  \file ParseXml.cpp
  \brief Parse an xml from Ableton

  *Use a QDomDocument of an xml from ableton to initalise the structures from Struct.hpp that will be loaded into Score via functions in CreateScore.cpp
  */

#include "ParseXml.hpp"

using namespace std;



QDomNodeList getNodes(QDomElement& n, QStringList args){
  QDomElement currentElement = n;

  for(int i=0;i<args.length()-1;i++){
    cout<<args.at(i).toStdString()<<endl;
    currentElement = currentElement.firstChildElement(args.at(i));
  }

  return currentElement.elementsByTagName(args.at(args.length()-1));
}

QDomElement getElement(QDomElement& n, QStringList args){
  QDomElement currentElement = n;

  for(int i=0;i<args.length();i++){
    cout<<args.at(i).toStdString()<<endl;
    currentElement = currentElement.firstChildElement(args.at(i));
  }

  return currentElement;
}

QDomNodeList getMidiTracks(QDomDocument& doc){
  QString str = "LiveSet->Tracks->MidiTrack";
  QStringList args = str.split("->");
  QDomElement docElem = doc.firstChildElement("Ableton");
  return getNodes(docElem,args);
}

QDomNodeList getAudioTracks(QDomDocument& doc){
  QString str = "LiveSet->Tracks->AudioTrack";
  QStringList args = str.split("->");
  QDomElement docElem = doc.firstChildElement("Ableton");
  return getNodes(docElem,args);
}

QDomNodeList getSlots(QDomElement& track){
  QString str = "DeviceChain->MainSequencer->ClipSlotList->ClipSlot";
  QStringList args = str.split("->");
  return getNodes(track, args);
}


QDomNodeList getAudioClipEvents(QDomElement& track){
  QString str = "DeviceChain->MainSequencer->Sample->ArrangerAutomation->Events->AudioClip";
  QStringList args = str.split("->");
  return getNodes(track, args);
}

QDomNodeList getMidiClipEvents(QDomElement& track){
  QString str = "DeviceChain->MainSequencer->ClipTimeable->ArrangerAutomation->Events->MidiClip";
  QStringList args = str.split("->");
  return getNodes(track, args);
}

QDomNodeList getKeyTracks(QDomElement& midiClip){
  QString str = "Notes->KeyTracks->KeyTrack";
  QStringList args = str.split("->");
  return getNodes(midiClip, args);
}

QDomNodeList getNotes(QDomElement& keyTrack){
  QString str = "Notes->MidiNoteEvent";
  QStringList args = str.split("->");
  return getNodes(keyTrack, args);
}

QDomElement getMidiClip(QDomElement& slot){
  QString str = "ClipSlot->Value->MidiClip";
  QStringList args = str.split("->");
  return getElement(slot, args);
}

int loadNotes(QDomElement& keyTrack, vector<Midi::NoteData>& notes, Midi::midi_size_t midiKeyValue){
  QDomNodeList notesListXml = getNotes(keyTrack);

  for (int i = 0; i < notesListXml.length(); i++) {
    //GET NOTE ATTRIBUTES
    cout << "---------NOTE--------" << endl;
    cout << "Midi KeyValue: " << midiKeyValue << endl;
    cout << "Midi Velocity: " << notesListXml.item(i).attributes().namedItem("Velocity").nodeValue().toStdString() << endl;
    cout << "Midi Duration: " << notesListXml.item(i).attributes().namedItem("Duration").nodeValue().toStdString() << endl;
    cout << "Midi Time: " << notesListXml.item(i).attributes().namedItem("Time").nodeValue().toStdString() << endl;
    cout << "---------------------" << endl;
    Midi::midi_size_t velocity = stoi(notesListXml.item(i).attributes().namedItem("Velocity").nodeValue().toStdString());
    double duration = stof(notesListXml.item(i).attributes().namedItem("Duration").nodeValue().toStdString());
    double time = stof(notesListXml.item(i).attributes().namedItem("Time").nodeValue().toStdString());


    notes.emplace_back(time, duration, midiKeyValue, velocity);

  }
  return 0;
}

int loadAudioClip(QDomElement& audioClipXml, AudioClip& audioClip){
  cout << "---------AUDIOCLIP--------" << endl;

  QString name = audioClipXml.firstChildElement("SampleRef").firstChildElement("FileRef").firstChildElement("Name").attributes().item(0).nodeValue();

  double start = stod(audioClipXml.firstChildElement("CurrentStart").attributes().item(0).nodeValue().toStdString());
  double end = stod(audioClipXml.firstChildElement("CurrentEnd").attributes().item(0).nodeValue().toStdString());;
  double startRelative = stod(audioClipXml.firstChildElement("Loop").firstChildElement("StartRelative").attributes().item(0).nodeValue().toStdString());;

  audioClip.path = name;
  audioClip.start = start;
  audioClip.end = end;
  audioClip.startRelative = startRelative;

  cout << name.toStdString() << endl;
  cout << start << endl;
  cout << end << endl;
  cout << startRelative << endl;


  cout << "-------------------------" << endl;

  return 0;
}


int loadMidiClip(QDomElement& midiClipXml, MidiClip& midiClip){
  QDomNodeList keyTrackListXml = getKeyTracks(midiClipXml);
  cout << "---------MIDICLIP--------" << endl;

  for (int i = 0; i < keyTrackListXml.length(); i++) {

    Midi::midi_size_t midiKeyValue = stoi(keyTrackListXml.item(i).firstChildElement("MidiKey").attributes().item(0).nodeValue().toStdString());
    QDomElement a = keyTrackListXml.item(i).toElement();
    loadNotes(a, midiClip.midiNotes, midiKeyValue);
    double start = stod(midiClipXml.firstChildElement("CurrentStart").attributes().item(0).nodeValue().toStdString());
    double end = stod(midiClipXml.firstChildElement("CurrentEnd").attributes().item(0).nodeValue().toStdString());;
    double startRelative = stod(midiClipXml.firstChildElement("Loop").firstChildElement("StartRelative").attributes().item(0).nodeValue().toStdString());;

    midiClip.start = start;
    midiClip.end = end;
    midiClip.startRelative = startRelative;

    cout << start << endl;
    cout << end << endl;
    cout << startRelative << endl;
  }

  cout << "-------------------------" << endl;

  return 0;
}
int loadSlots(QDomElement& midiTrack, vector<ClipSlot>& clipSlots){
  QDomNodeList slotsXml = getSlots(midiTrack);
  for (int i = 0; i < slotsXml.length(); i++) {
    QDomElement a = slotsXml.item(i).toElement();
    QDomElement slotMidiClip = getMidiClip(a);
    if(!(slotMidiClip.isNull())){
      clipSlots.emplace_back();
      loadMidiClip(slotMidiClip, clipSlots[clipSlots.size()-1].midiClip);
    }
  }
  return 0;
}

int loadMidiClipEvents(QDomElement& midiTrack, std::vector<MidiClip>& midiClipEvents){
  QDomNodeList midiClipEventsXml = getMidiClipEvents(midiTrack);
  for (int i = 0; i < midiClipEventsXml.length(); i++) {
    QDomElement a = midiClipEventsXml.item(i).toElement();
    midiClipEvents.emplace_back();
    loadMidiClip(a, midiClipEvents[midiClipEvents.size()-1]);
  }
  return 0;
}

int loadAudioClipEvents(QDomElement& audioTrack, std::vector<AudioClip>& audioClipEvents){
  QDomNodeList audioClipEventsXml = getAudioClipEvents(audioTrack);
  for (int i = 0; i < audioClipEventsXml.length(); i++) {
    QDomElement a = audioClipEventsXml.item(i).toElement();
    audioClipEvents.emplace_back();
    loadAudioClip(a, audioClipEvents[audioClipEvents.size()-1]);
  }
  return 0;
}

int loadMidiTracks(QDomDocument& docXml, vector<MidiTrack>& midiTracks){
  QDomNodeList midiTracksXml = getMidiTracks(docXml);
  std::cout<< "NB Midi Tracks: " << midiTracksXml.length() << std::endl;
  for (int i = 0; i < midiTracksXml.length(); i++) {
    cout << "---------MIDITRACK--------" << endl;

    midiTracks.emplace_back();
    QDomElement a = midiTracksXml.item(i).toElement();
    loadSlots(a, midiTracks[i].clipSlots);
    loadMidiClipEvents(a, midiTracks[i].midiClipEvents);
    cout << "--------------------------" << endl;

  }

  return 0;
}

int loadAudioTracks(QDomDocument& docXml, vector<AudioTrack>& audioTracks){
  QDomNodeList audioTracksXml = getAudioTracks(docXml);
  std::cout<< "NB Audio Tracks: " << audioTracksXml.length() << std::endl;
  for (int i = 0; i < audioTracksXml.length(); i++) {
    cout << "---------AUDIOTRACK--------" << endl;

    audioTracks.emplace_back();
    QDomElement a = audioTracksXml.item(i).toElement();
    loadAudioClipEvents(a, audioTracks[i].audioClipEvents);
    cout << "--------------------------" << endl;

  }

  return 0;
}


int loadDocument(QDomDocument& docXml, AbletonDocument& abletonDoc){
  loadMidiTracks(docXml, abletonDoc.midiTracks);
  loadAudioTracks(docXml, abletonDoc.audioTracks);
  return 0;
}
