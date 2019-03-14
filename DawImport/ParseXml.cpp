#include "ParseXml.hpp"

using namespace std;


QDomNodeList getMidiTracks(QDomDocument& doc){
  return doc.firstChildElement("Ableton").firstChildElement("LiveSet").firstChildElement("Tracks").elementsByTagName("MidiTrack");
}

QDomNodeList getAudioTracks(QDomDocument& doc){
  return doc.firstChildElement("Ableton").firstChildElement("LiveSet").firstChildElement("Tracks").elementsByTagName("AudioTrack");
}

QDomNodeList getSlots(QDomNode& track){
  return track.firstChildElement("DeviceChain").firstChildElement("MainSequencer").firstChildElement("ClipSlotList").elementsByTagName("ClipSlot");
}

QDomNode getMidiClip(QDomNode& slot){
  return slot.firstChildElement("ClipSlot").firstChildElement("Value").firstChildElement("MidiClip");
}

QDomNodeList getAudioClipEvents(QDomNode& track){
  return track.firstChildElement("DeviceChain").firstChildElement("MainSequencer").firstChildElement("Sample").firstChildElement("ArrangerAutomation").firstChildElement("Events").elementsByTagName("AudioClip");
}

QDomNodeList getMidiClipEvents(QDomNode& track){
  return track.firstChildElement("DeviceChain").firstChildElement("MainSequencer").firstChildElement("ClipTimeable").firstChildElement("ArrangerAutomation").firstChildElement("Events").elementsByTagName("MidiClip");
}

QDomNodeList getKeyTracks(QDomNode& midiClip){
  return midiClip.firstChildElement("Notes").firstChildElement("KeyTracks").elementsByTagName("KeyTrack");

}

QDomNodeList getNotes(QDomNode& keyTrack){
  return keyTrack.firstChildElement("Notes").elementsByTagName("MidiNoteEvent");
}

int loadNotes(QDomNode& keyTrack, vector<Midi::NoteData>& notes, Midi::midi_size_t midiKeyValue){
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

    /*NoteData(double s, double d, midi_size_t p, midi_size_t v)
      : m_start{s}, m_duration{d}, m_pitch{p}, m_velocity{v}
      {*/

    notes.emplace_back(time, duration, midiKeyValue, velocity);

  }
  return 0;
}

int loadAudioClip(QDomNode& audioClipXml, AudioClip& audioClip){
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


int loadMidiClip(QDomNode& midiClipXml, MidiClip& midiClip){
  QDomNodeList keyTrackListXml = getKeyTracks(midiClipXml);
  cout << "---------MIDICLIP--------" << endl;

  for (int i = 0; i < keyTrackListXml.length(); i++) {

    Midi::midi_size_t midiKeyValue = stoi(keyTrackListXml.item(i).firstChildElement("MidiKey").attributes().item(0).nodeValue().toStdString());
    QDomNode a = keyTrackListXml.item(i);
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
int loadSlots(QDomNode& midiTrack, vector<ClipSlot*>& clipSlots){
  QDomNodeList slotsXml = getSlots(midiTrack);
  for (int i = 0; i < slotsXml.length(); i++) {
    QDomNode a = slotsXml.item(i);
    QDomNode slotMidiClip = getMidiClip(a);
    if(!(slotMidiClip.isNull())){
      clipSlots.push_back(new ClipSlot());
      loadMidiClip(slotMidiClip, clipSlots[clipSlots.size()-1]->midiClip);
    }
  }
  return 0;
}

int loadMidiClipEvents(QDomNode& midiTrack, std::vector<MidiClip *>& midiClipEvents){
  QDomNodeList midiClipEventsXml = getMidiClipEvents(midiTrack);
  for (int i = 0; i < midiClipEventsXml.length(); i++) {
    QDomNode a = midiClipEventsXml.item(i);
    midiClipEvents.push_back(new MidiClip());
    loadMidiClip(a, *(midiClipEvents[midiClipEvents.size()-1]));
  }
  return 0;
}

int loadAudioClipEvents(QDomNode& audioTrack, std::vector<AudioClip *>& audioClipEvents){
  QDomNodeList audioClipEventsXml = getAudioClipEvents(audioTrack);
  for (int i = 0; i < audioClipEventsXml.length(); i++) {
    QDomNode a = audioClipEventsXml.item(i);
    audioClipEvents.push_back(new AudioClip());
    loadAudioClip(a, *(audioClipEvents[audioClipEvents.size()-1]));
  }
  return 0;
}

int loadMidiTracks(QDomDocument& docXml, vector<MidiTrack *>& midiTracks){
  QDomNodeList midiTracksXml = getMidiTracks(docXml);
  std::cout<< "NB Midi Tracks: " << midiTracksXml.length() << std::endl;
  for (int i = 0; i < midiTracksXml.length(); i++) {
    cout << "---------MIDITRACK--------" << endl;

    midiTracks.push_back(new MidiTrack());
    QDomNode a = midiTracksXml.item(i);
    loadSlots(a, midiTracks[i]->clipSlots);
    loadMidiClipEvents(a, midiTracks[i]->midiClipEvents);
    cout << "--------------------------" << endl;

  }

  return 0;
}

int loadAudioTracks(QDomDocument& docXml, vector<AudioTrack *>& audioTracks){
  QDomNodeList audioTracksXml = getAudioTracks(docXml);
  std::cout<< "NB Midi Tracks: " << audioTracksXml.length() << std::endl;
  for (int i = 0; i < audioTracksXml.length(); i++) {
    cout << "---------AUDIOTRACK--------" << endl;

    audioTracks.push_back(new AudioTrack());
    QDomNode a = audioTracksXml.item(i);
    loadAudioClipEvents(a, audioTracks[i]->audioClipEvents);
    cout << "--------------------------" << endl;

  }

  return 0;
}


int loadDocument(QDomDocument& docXml, AbletonDocument& abletonDoc){
  loadMidiTracks(docXml, abletonDoc.midiTracks);
  loadAudioTracks(docXml, abletonDoc.audioTracks);
  return 0;
}
