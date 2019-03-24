#include "Debug.hpp"

using namespace std;

void printMidiNote(MidiNote n){
  cout << "          Start: " << n.start << endl;
  cout << "          Duration: " << n.duration << endl;
  cout << "          Pitch: " << unsigned(n.pitch) << endl;
  cout << "          Velocity: " << unsigned(n.velocity) << endl;
}

void printClipEventInfo(ClipEvent ce){
  cout << "      Start: " << ce.start << endl;
  cout << "      End: " << ce.end << endl;
  cout << "      StartRelative: " << ce.startRelative << endl;
  cout << "      ClipLength: " << ce.clipLength << endl;
}

struct displayClipEvent {
  void operator()(MidiClipEvent ce){
    cout << "    MidiClipEvent" << endl;
    printClipEventInfo(ce);
    cout << "      MidiNotes" << endl;
    for(int i=0; i<ce.midiNotes.size(); i++){
      cout << "        MidiNote " << i << endl;
      printMidiNote(ce.midiNotes[i]);
    }
  }
  void operator()(AudioClipEvent ce){
    cout << "    AudioClipEvent" << endl;
    printClipEventInfo(ce);
    cout << "      Path: " << ce.path << endl;
  }
};

void printTrackInfo(Track track) {
  cout << "  Name:" << track.name << endl;
  cout << "  Length:" << track.length << endl;
  cout << "  ClipEvents" << endl;
  for(int i=0; i<track.clipEvents.size(); i++){
    std::visit(displayClipEvent{}, track.clipEvents[i]);
  }
}

struct displayTrack {
  void operator()(MidiTrack t){
    cout << "  MidiTrack" << endl;
    cout << "  VST: " << t.VST << endl;
    printTrackInfo(t);
  }
  void operator()(AudioTrack t){
    cout << "  AudioTrack" << endl;
    printTrackInfo(t);
  }
};

void displayAbletonDoc(AbletonDocument& doc){
  cout << "== BPM: " << doc.BPM << " ==" << endl;
  cout << "== Tracks: " << doc.tracks.size() << " ==" << endl;
  for(int i=0; i<doc.tracks.size(); i++){
    cout << "Track " << i << endl;
    std::visit(displayTrack{}, doc.tracks[i]);
  }
}

/*
int main(){
  AbletonDocument doc;
  MidiNote note1 = {0.0, 1.0, 1, 1};
  MidiNote note2 = {5.9, 7.3, 12, 108};

  MidiClipEvent midiClip;
  midiClip.start = 2;
  midiClip.end = 4;
  midiClip.startRelative = 0;
  midiClip.clipLength = 12;
  midiClip.midiNotes.push_back(note1);
  midiClip.midiNotes.push_back(note2);

  MidiTrack mt1;
  mt1.trackName = "mt1";
  mt1.clipEvents.push_back(midiClip);

  AudioClipEvent audioClip = {1, 3, .5, 8, "path/to/clip"};

  AudioTrack at1;
  at1.trackName = "Track Test";
  at1.clipEvents.push_back(audioClip);

  doc.tracks.push_back(at1);
  doc.tracks.push_back(mt1);
  cout << "Calling" << endl;
  displayAbletonDoc(doc);
  return 0;
}
*/
