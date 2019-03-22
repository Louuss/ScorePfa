#include "CreateStructs.hpp"


struct TrackCreator{


};

struct ClipEventCreator {

  const Scenario::ProcessModel& scenar;
  Scenario::Command::Macro& macro;
  Scenario::StateModel& startDot;
  Scenario::StateModel& endDot;

    void createClipEvent(ClipEvent& ce)
      {

        constexpr double y = 0.02;


        // Create Interval
        auto& interval = macro.createIntervalAfter(track, startNode.id(), {2s, y});;

        endDot = Scenario::endState(interval, track);

        // Create a loop
        auto& loop = macro.createProcessInNewSlot<Loop::ProcessModel>(i1, {});
      }

      void operator()(AudioClipEvent& audioClipEvent)
      {
        createClipEvent(scenar, macro, audioClipEvent);
        //create the sound process with the path
        macro.createProcessInNewSlot<Media::Sound::ProcessModel>(loop.interval(), audioClipEvent.path);
      }
      void operator()(MidiClipEvent& midiClipEvent)
      {
        createClipEvent(scenar, macro, midiClipEvent);
        // create the midi process
        auto& midi = macro.createProcessInNewSlot<Midi::ProcessModel>(loop.interval(), {});
        // add notes
        new Midi::ReplaceNotes(midi, convertToScoreNotes(clipEvent.midiNotes), 0, 127, getQtime(clipEvent.end - clipEvent.start));
      }

  };



std::vector<Midi::NoteData> convertToScoreNotes(std::vector<MidiNote> midiNotes){
  std::vector<Midi::NoteData> scoreNotes;
  for (int i=0; i<midiNotes.size(); i++){
    MidiNote note = midiNotes[i];
    scoreNotes.emplace_back(note.start, note.duration, note.pitch, note.velocity);
  }
  return scoreNotes;
}
