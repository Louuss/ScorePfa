/**
  \file Structs.hpp
  \brief Structs annalogue to to those of Ableton

  *These structures act as an etermediary between Ableton objects and Score Objects
  */


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

struct MidiNote{
  double start;
  double duration;
  uint8_t pitch;
  uint8_t velocity;
  MidiNote(double s, double d, uint8_t p, uint8_t v):start(s), duration(d), pitch(p), velocity(v){};

};

struct MidiClip{
  std::vector<MidiNote> midiNotes;
  double start;
  double end;
  double startRelative;
  double clipLength;
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
