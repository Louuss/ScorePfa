#include "CreateStructs.hpp"


QTime getQtime(double time){
  QTime t(0,0,0);
  t = t.addSecs((int)time);
  t = t.addMSecs((int)(time/1000));
  return t;
}

std::vector<Midi::NoteData> convertToScoreNotes(std::vector<MidiNote> midiNotes, double BPM){
  std::vector<Midi::NoteData> scoreNotes;
  for (int i=0; i<midiNotes.size(); i++){
    MidiNote note = midiNotes[i];
    scoreNotes.emplace_back(note.start, note.duration, note.pitch, note.velocity);
  }
  return scoreNotes;
}



Loop::ProcessModel& ClipEventCreator::createClipEvent(ClipEvent& ce)
{
  constexpr double y = 0.02;

  if(ce.start != prevEnd){
    auto& emptyInterval = macro.createIntervalAfter(scenar, startDot, {getQtime(ce.start), y});
    startDot = Scenario::endState(emptyInterval, scenar).id();
  }

  auto& in = macro.createIntervalAfter(scenar, startDot, {getQtime(ce.end), y});
  endDot = Scenario::endState(in, scenar).id();
  prevEnd = ce.end;

  // Creates a loop
  auto& loop = macro.createProcessInNewSlot<Loop::ProcessModel>(in, {});
  // and returns it
  return loop;

}

void ClipEventCreator::operator()(AudioClipEvent& clip)
{
   auto& loop = createClipEvent(clip);
   // creates the audio process
   std::cout << clip.path << '\n';
   macro.createProcessInNewSlot<Media::Sound::ProcessModel>(loop.interval(), QString::fromStdString(clip.path));
}


void ClipEventCreator::operator()(MidiClipEvent& clip)
{
    auto& loop = createClipEvent(clip);

    // creates the midi process
    auto& midi = macro.createProcessInNewSlot<Midi::ProcessModel>(loop.interval(), {});
    // adds notes
    new Midi::ReplaceNotes(midi, convertToScoreNotes(clip.midiNotes, BPM), 0, 127, getQtime(clip.end - clip.start));
}


void TrackCreator::createTrack(Track& tr)
{
    // base
    constexpr double y = 0.02;
    auto& mainStart = macro.createState(scenar, scenar.startEvent().id(), y);
    // ending dot
    const auto& [t2, e2, trackEnd] = macro.createDot(scenar, {getQtime(tr.length), y});
    // creates the track interval
    const auto& trackInterval = macro.createInterval(scenar, mainStart.id(), trackEnd.id());

    auto& track = macro.createProcessInNewSlot<Scenario::ProcessModel>(trackInterval, {});

    auto& trackStart = macro.createState(track, track.startEvent().id(), y);






    ClipEventCreator cec{track, macro, trackStart.id(), BPM};

    for(int i = 0; i<tr.clipEvents.size(); i++)
    {
      std::visit(cec, tr.clipEvents[i]);
      cec.startDot = cec.endDot;
    }


}
void TrackCreator::operator()(MidiTrack& track)
{
  createTrack(track);
}
void TrackCreator::operator()(AudioTrack& track)
{
  createTrack(track);
}




void AbletonDocumentCreator::createDoc( AbletonDocument aDoc,
  const Scenario::ProcessModel& scenar,
  Scenario::Command::Macro& macro)
  {
    for (int i = 0; i < aDoc.tracks.size(); i++) {
      std::visit(TrackCreator{scenar,macro,aDoc.BPM},  aDoc.tracks[i]);

    }

    macro.commit();
  }
