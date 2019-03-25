#include "CreateStructs.hpp"


QTime getQtime(double time, double BPM){
  time = time/(BPM/60);
  QTime t(0,0,0);
  t = t.addSecs((int)time);
  t = t.addMSecs((int)((time-floor(time))*1000));
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

  auto& loop = createLoop(midiClipEvent);
  auto& midi = macro.createProcessInNewSlot<Midi::ProcessModel>(loop.interval(), {});
  // add notes
  macro.submit(new Midi::ReplaceNotes(midi, convertToScoreNotes(midiClipEvent.midiNotes), 0, 127, getQtime(1, BPM)));
}


Loop::ProcessModel& ClipEventCreator::createLoop(ClipEvent& clipEvent)
{
    constexpr double y = 0.02;

    if(clipEvent.start != prevEnd){
      auto& emptyInterval = macro.createIntervalAfter(scenar, startDot, {getQtime(clipEvent.start, BPM), y});
      startDot = Scenario::endState(emptyInterval, scenar).id();
    }

    // Create Interval
    auto& loopInterval = macro.createIntervalAfter(scenar, startDot, {getQtime(clipEvent.end, BPM), y});
    endDot = Scenario::endState(loopInterval, scenar).id();
    prevEnd = clipEvent.end;
    // Create a loop
    auto& loop = macro.createProcessInNewSlot<Loop::ProcessModel>(loopInterval, {});

    Scenario::IntervalDurations::Algorithms::changeAllDurations(
                                              loop.interval(), getQtime(clipEvent.clipLength, BPM));
    loop.endEvent().setDate(getQtime(clipEvent.clipLength, BPM));
    loop.endTimeSync().setDate(getQtime(clipEvent.clipLength, BPM));
    return loop;
}


void TrackCreator::createTrack(Track& tr)
{
    // base
    constexpr double y = 0.02;
    auto& mainStart = macro.createState(scenar, scenar.startEvent().id(), y);
    // ending dot
    const auto& [t2, e2, trackEnd] = macro.createDot(scenar, {getQtime(tr.length, BPM), y});
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
    double BPM = aDoc.BPM;
    for (int i = 0; i < aDoc.tracks.size(); i++) {
      std::cout<<"JE VEUX CREER LA TRACK : " << i <<std::endl;
      std::visit(TrackCreator{scenar,macro, BPM},  aDoc.tracks[i]);

    }

    macro.commit();
  }
