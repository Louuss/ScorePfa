#include "ParseStucts.hpp"

struct ClipEventLoader{
  QDomElement& clipXml;

  int loadNotes(QDomElement& keyTrack, vector<MidiNote>& notes, uint8_t midiKeyValue){
    QDomNodeList notesListXml = getList(keyTrack, PATH_NOTES);

    for (int i = 0; i < notesListXml.length(); i++) {
      uint8_t velocity = stoi(notesListXml.item(i).attributes().namedItem("Velocity").nodeValue().toStdString());
      double duration = stof(notesListXml.item(i).attributes().namedItem("Duration").nodeValue().toStdString());
      double time = stof(notesListXml.item(i).attributes().namedItem("Time").nodeValue().toStdString());


      notes.emplace_back(time, duration, midiKeyValue, velocity);

    }
    return 0;
  }


  void loadClipAtributes(ClipEvent& clipEvent){
    double start = stod(clipXml.firstChildElement("CurrentStart").attributes().item(0).nodeValue().toStdString());
    double end = stod(clipXml.firstChildElement("CurrentEnd").attributes().item(0).nodeValue().toStdString());;
    double startRelative = stod(clipXml.firstChildElement("Loop").firstChildElement("StartRelative").attributes().item(0).nodeValue().toStdString());;
    double clipLength = stod(midiClipXml.firstChildElement("Loop").firstChildElement("LoopEnd").attributes().item(0).nodeValue().toStdString());


    clipEvent.start = start;
    clipEvent.end = end;
    clipEvent.startRelative = startRelative;
    midiClip.clipLength = clipLength;

  }

  void operator()(MidiClipEvent& midiClipEvent){
    loadClipAtributes(midiClipEvent);

    QDomNodeList keyTrackListXml = getList(clipXml, PATH_KEY_TRACKS);

    for (int i = 0; i < keyTrackListXml.length(); i++) {
      uint8_t midiKeyValue = stoi(keyTrackListXml.item(i).firstChildElement("MidiKey").attributes().item(0).nodeValue().toStdString());
      QDomElement a = keyTrackListXml.item(i).toElement();
      loadNotes(a, midiClipEvent.midiNotes, midiKeyValue);
    }
  }

  void operator()(AudioClipEvent& audioClipEvent){
    loadClipAtributes(audioClipEvent);
    QString name = audioClipXml.firstChildElement("SampleRef").firstChildElement("FileRef").firstChildElement("Name").attributes().item(0).nodeValue();
    audioClip.path = name;

  }
};

struct TrackLoader{
  QDomElement& trackXml;
  int trackType;
  void loadTrack(Track& track){
    QDomNodeList clipEventsXml = getClipEvents(trackXml);

    for (int i = 0; i < clipEventsXml.length(); i++) {
      QDomElement a = clipEventsXml.item(i).toElement();
      ClipEventVisitor cev;
      if (trackType == 1){
        track.clipEvents.push_back(MidiClipEvent());
      }else if (trackType == 2){
        track.clipEvents.push_back(AudioClipEvent());
      }
      cev.loadClipEvent(track.clipEvents.back(), a);
    }

  }

  void operator()(MidiTrack& track){
    trackType =1;
    loadTrack(track);
  }

  void operator()(AudioTrack& track){
    trackType =2;
    loadTrack(track);
  }

};

struct AbletonDocumentLoader{
  void loadTracks(QDomDocument& docXml,
                  std::vector<std::variant<AudioTrack,
                  MidiTrack>>& tracks,
                  QString path,
                  int trackType){

    QDomNodeList tracksXml = getList(docXml, path);
    for (int i = 0; i < tracksXml.length(); i++) {
      if(trackType == 1){
        tracks.emplace_back(AudioTrack());
      }else if(trackType == 2){
        tracks.emplace_back(MidiTrack());
      }

      QDomElement a = tracksXml.item(i).toElement();
      std::visit(TrackLoader{a}, tracks.back());

    }

  }


  void loadAbletonDocument(QDomDocument& docXml, AbletonDocument& abletonDoc){
    loadTracks(docXml, abletonDoc.tracks, PATH_MIDI_TRACKS, 2);
    loadTracks(docXml, abletonDoc.tracks, PATH_AUDIO_TRACKS, 1);
  }
};
