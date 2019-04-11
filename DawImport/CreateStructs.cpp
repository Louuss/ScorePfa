#include "CreateStructs.hpp"

QTime getQtime(double time, double BPM)
{
  time = time / (BPM / 60);
  QTime t(0, 0, 0);
  t = t.addSecs((int)time);
  t = t.addMSecs((int)((time - floor(time)) * 1000));
  return t;
}

std::vector<Midi::NoteData>
convertToScoreNotes(std::vector<MidiNote> midiNotes)
{
  std::vector<Midi::NoteData> scoreNotes;
  for (int i = 0; i < midiNotes.size(); i++)
  {
    MidiNote note = midiNotes[i];
    scoreNotes.emplace_back(
        note.start, note.duration, note.pitch, note.velocity);
  }
  return scoreNotes;
}


void ClipEventCreator::operator()(MidiClipEvent& midiClipEvent)
{

  auto& loop = createLoop(midiClipEvent);

  // On connecte un cable entre le track et le vst
  {
    Process::CableData cable;
    cable.source = *loop.outlet;
    cable.sink = *scenar.inlet;
    cable.type = Process::CableType::ImmediateGlutton;
    macro.createCable(root, std::move(cable));
  }

  auto& midi
      = macro.createProcessInNewSlot<Midi::ProcessModel>(loop.interval(), {});
  // add notes

  {
    Process::CableData cable;
    cable.source = *midi.outlet;
    cable.sink = *loop.inlet;
    cable.type = Process::CableType::ImmediateGlutton;
    macro.createCable(root, std::move(cable));
  }


  macro.submit(new Midi::ReplaceNotes(
      midi,
      convertToScoreNotes(midiClipEvent.midiNotes),
      0,
      127,
      getQtime(1, BPM)));

}


void ClipEventCreator::operator()(AudioClipEvent& audioClipEvent)
{
  auto& loop = createLoop(audioClipEvent);

  macro.createProcessInNewSlot<Media::Sound::ProcessModel>(
      loop.interval(), QString::fromStdString(audioClipEvent.path));
}

Loop::ProcessModel& ClipEventCreator::createLoop(ClipEvent& clipEvent)
{
  constexpr double y = 0.02;

  if (clipEvent.start != prevEnd)
  {
    auto& emptyInterval = macro.createIntervalAfter(
        scenar, startDot, {getQtime(clipEvent.start, BPM), y});
    startDot = Scenario::endState(emptyInterval, scenar).id();
  }

  // Create Interval
  auto& loopInterval = macro.createIntervalAfter(
      scenar, startDot, {getQtime(clipEvent.end, BPM), y});
  endDot = Scenario::endState(loopInterval, scenar).id();
  prevEnd = clipEvent.end;
  // Create a loop
  auto& loop
      = macro.createProcessInNewSlot<Loop::ProcessModel>(loopInterval, {});

  Scenario::IntervalDurations::Algorithms::changeAllDurations(
      loop.interval(), getQtime(clipEvent.clipLength, BPM));
  loop.endEvent().setDate(getQtime(clipEvent.clipLength, BPM));
  loop.endTimeSync().setDate(getQtime(clipEvent.clipLength, BPM));
  return loop;
}

const Scenario::ProcessModel& TrackCreator::createTrack(Track& tr)
{

  auto& mainStart = macro.createState(scenar, scenar.startEvent().id(), y);
  // ending dot
  const auto& [t2, e2, trackEnd]
      = macro.createDot(scenar, {getQtime(tr.length, BPM), y});
  // creates the track interval
  const auto& trackInterval = macro.createInterval(scenar, mainStart.id(), trackEnd.id());
  trackId = trackInterval.id();
  const auto& track = macro.createProcessInNewSlot<Scenario::ProcessModel>(
      trackInterval, {});

  auto& trackStart = macro.createState(track, track.startEvent().id(), y);

  ClipEventCreator cec{track, macro, trackStart.id(), BPM, root};

  for (int i = 0; i < tr.clipEvents.size(); i++)
  {
    std::visit(cec, tr.clipEvents[i]);
    cec.startDot = cec.endDot;
  }

  return track;
}

void TrackCreator::addFX(MidiTrack& tr, const Scenario::ProcessModel& trackScenario){
  const auto& vsts
      = context.applicationPlugin<Media::ApplicationPlugin>().vst_infos;
  auto& fx_chain
      = macro.createProcessInNewSlot<Media::Effect::ProcessModel>(scenar.interval(trackId), {});
  // On va voir les VSTs qu'il y a sur le système
  if(tr.VST!=""){
  for (const auto& vst : vsts)
    {
      // On ajoute le premier synthé qu'on trouve
      // (J'ai désactivé Carla car c'est un VST qui bug chez moi...)
      if (vst.isSynth && vst.isValid && vst.prettyName.contains(QString::fromStdString(tr.VST)))
      {
        macro.submit(new Media::InsertGenericEffect<Media::VST::VSTEffectModel>(
            fx_chain, QString::number(vst.uniqueID), 0));
        break;
      }
    }
  }

  // On connecte un cable entre le track et le vst
  {
    Process::CableData cable;
    cable.source = *trackScenario.outlet;
    cable.sink = *fx_chain.inlet;
    cable.type = Process::CableType::ImmediateGlutton;
    macro.createCable(root, std::move(cable));
  }
}



void TrackCreator::operator()(MidiTrack& track)
{
  const Scenario::ProcessModel& trackScenario = createTrack(track);


  addFX(track, trackScenario);

}


void TrackCreator::operator()(AudioTrack& track)
{
  createTrack(track);
}

void AbletonDocumentCreator::createDoc(
    AbletonDocument aDoc,
    const Scenario::ProcessModel& scenar,
    Scenario::Command::Macro& macro,
    const score::GUIApplicationContext& context,
    const Scenario::ScenarioDocumentModel& root)
{
  double BPM = aDoc.BPM;
  for (int i = 0; i < aDoc.tracks.size(); i++)
  {
    std::visit(TrackCreator{scenar, macro, BPM, i * 0.2, context, root}, aDoc.tracks[i]);
  }

  macro.commit();
}
