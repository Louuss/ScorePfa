#ifndef STRUCTS
#define STRUCTS

#include<stdlib.h>
#include<vector>
#include <Midi/MidiNote.hpp>
#include <QString>

struct AudioClip{
  QString path;
  double start;
  double end;
  double startRelative;
};


struct MidiClip{
  std::vector<Midi::NoteData> midiNotes;
  double start;
  double end;
  double startRelative;
};

struct ClipSlot{
  MidiClip midiClip;
};

struct MidiTrack{
  std::vector<ClipSlot> clipSlots;
  std::vector<MidiClip> midiClipEvents;

};

struct AudioTrack{
  std::vector<AudioClip> audioClipEvents;

};

struct AbletonDocument{
  std::vector<MidiTrack> midiTracks;
  std::vector<AudioTrack> audioTracks;


};

#endif
