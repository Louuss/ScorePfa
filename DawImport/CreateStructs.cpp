#include "CreateStructs.hpp"


QTime getQtime(double time){
  QTime t(0,0,0);
  t = t.addSecs((int)time);
  t = t.addMSecs((int)(time/1000));
  return t;
}

std::vector<Midi::NoteData> convertToScoreNotes(std::vector<MidiNote> midiNotes){
  std::vector<Midi::NoteData> scoreNotes;
  for (int i=0; i<midiNotes.size(); i++){
    MidiNote note = midiNotes[i];
    scoreNotes.emplace_back(note.start, note.duration, note.pitch, note.velocity);
  }
  return scoreNotes;
}



void ClipEventCreator::createClipEvent(ClipEvent& ce)
{
constexpr double y = 0.02;

// // Create Interval
// auto& in = macro.createIntervalAfter(scenar, startDot->id(), {getQtime(ce.end), y});
// startDot = &Scenario::endState(in, scenar);
//
// // Create a loop
// auto& loop = macro.createProcessInNewSlot<Loop::ProcessModel>(in, {});
}

void ClipEventCreator::operator()(AudioClipEvent& audioClipEvent)
{
    constexpr double y = 0.02;

    // Create Interval
    auto& in = macro.createIntervalAfter(scenar, startDot.id(), {getQtime(audioClipEvent.end), y});


    //startDot = Scenario::endState(in, scenar);


    // Create a loop
    auto& loop = macro.createProcessInNewSlot<Loop::ProcessModel>(in, {});

    //createClipEvent(audioClipEvent);
    //create the sound process with the path
    macro.createProcessInNewSlot<Media::Sound::ProcessModel>(loop.interval(), QString::fromStdString(audioClipEvent.path));
}


void ClipEventCreator::operator()(MidiClipEvent& midiClipEvent)
{
    constexpr double y = 0.02;

    // Create Interval
    auto& in = macro.createIntervalAfter(scenar, startDot.id(), {getQtime(midiClipEvent.end), y});


    //startDot = Scenario::endState(in, scenar);

    // Create a loop
    auto& loop = macro.createProcessInNewSlot<Loop::ProcessModel>(in, {});



    //  createClipEvent(midiClipEvent);
    // create the midi process

    auto& midi = macro.createProcessInNewSlot<Midi::ProcessModel>(loop.interval(), {});
    // add notes
    new Midi::ReplaceNotes(midi, convertToScoreNotes(midiClipEvent.midiNotes), 0, 127, getQtime(midiClipEvent.end - midiClipEvent.start));
}


void TrackCreator::createTrack(Track tr)
{
    using namespace std::literals;
    // base

    constexpr double y = 0.02;
    auto& start1 = macro.createState(scenar, scenar.startEvent().id(), y);
    // ending dot
    const auto& [t2, e2, end1] = macro.createDot(scenar, {getQtime(tr.length), y});
    // creates the track interval
    const auto& interval1 = macro.createInterval(scenar, start1.id(), end1.id());

    auto& track = macro.createProcessInNewSlot<Scenario::ProcessModel>(interval1, {});

    ClipEventCreator cec {track, macro, start1};

    for(int i = 0; i<tr.clipEvents.size(); i++)
    {
      std::visit(cec,tr.clipEvents[i]);
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
   std::cout<<"JE VEUX CREER LA TRACK : " << i <<std::endl;
   std::visit(TrackCreator{scenar,macro},  aDoc.tracks[i]);

}
}
