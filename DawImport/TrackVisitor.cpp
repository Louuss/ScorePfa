struct TrackVisitor{


  void addClipEvent(Track& track, QDomElement& clipEventXml){
      ClipEventVisitor cev;
      addElement(track.the_track.clipEvents.push_back());
      cev.loadClipEvent(track.the_track.clipEvents.back().the_clip, clipEventXml);
  }


  void loadTrack(Track& track, QDomElement& trackXml){

    QDomNodeList clipEventsXml = getClipEvents(trackXml);

    for (int i = 0; i < clipEventsXml.length(); i++) {
      QDomElement a = clipEventsXml.item(i).toElement();
      addClipEvent(track, a);
    }
  }


};

struct ClipEventVisitor{
  void loadClipEvent(MidiClipEvent& midiClipEvent, QDomElement& midiClipEventXml){
    QDomNodeList keyTrackListXml = getKeyTracks(midiClipEventXml);
    cout << "---------MIDICLIP--------" << endl;

    for (int i = 0; i < keyTrackListXml.length(); i++) {

      uint8_t midiKeyValue = stoi(keyTrackListXml.item(i).firstChildElement("MidiKey").attributes().item(0).nodeValue().toStdString());
      QDomElement a = keyTrackListXml.item(i).toElement();
      loadNotes(a, midiClip.midiNotes, midiKeyValue);
      double start = stod(midiClipXml.firstChildElement("CurrentStart").attributes().item(0).nodeValue().toStdString());
      double end = stod(midiClipXml.firstChildElement("CurrentEnd").attributes().item(0).nodeValue().toStdString());;
      double startRelative = stod(midiClipXml.firstChildElement("Loop").firstChildElement("StartRelative").attributes().item(0).nodeValue().toStdString());
      double clipLength = stod(midiClipXml.firstChildElement("Loop").firstChildElement("LoopEnd").attributes().item(0).nodeValue().toStdString());

      midiClipEvent.start = start;
      midiClipEvent.end = end;
      midiClipEvent.startRelative = startRelative;
      midiClipEvent.clipLength = clipLength;

      cout << start << endl;
      cout << end << endl;
      cout << startRelative << endl;
      cout << clipLength << endl;
    }

    cout << "-------------------------" << endl;

  }

  void loadClipEvent(AudioClipEvent& audioClipEvent, QDomElement& audioClipEventXml){



  }

};
