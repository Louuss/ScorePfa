#ifndef CREATESTRUCTS
#define CREATESTRUCTS
#include "Structs.hpp"
#include <Scenario/Process/Algorithms/Accessors.hpp>
#include <Scenario/Process/Algorithms/ContainersAccessors.hpp>
#include <Scenario/Process/ScenarioModel.hpp>
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentModel.hpp>
#include <Scenario/Document/BaseScenario/BaseScenario.hpp>
#include <score/document/DocumentInterface.hpp>
#include <score/actions/Menu.hpp>
#include <score/actions/MenuManager.hpp>
#include <core/document/Document.hpp>
#include <Scenario/Commands/CommandAPI.hpp>

#include <Midi/MidiProcess.hpp>
#include <Midi/Commands/AddNote.hpp>

#include <Media/Effect/EffectProcessModel.hpp>
#include <Media/Effect/VST/VSTEffectModel.hpp>
#include <Media/Commands/InsertVST.hpp>
#include <Media/ApplicationPlugin.hpp>

#include <Media/Sound/SoundModel.hpp>
#include <Loop/LoopProcessModel.hpp>

#include <QMenu>
#include <QtXml/QtXml>

#include "ParseStructs.hpp"

#include <math.h>

struct ClipEventCreator {

  const Scenario::ProcessModel& scenar;
  Scenario::Command::Macro& macro;
  Id<Scenario::StateModel> startDot;
  double BPM;
  Id<Scenario::StateModel> endDot = Id<Scenario::StateModel>();
  double prevEnd = 0;



  Loop::ProcessModel& createClipEvent(ClipEvent& ce);
  void operator()(AudioClipEvent& audioClipEvent);
  void operator()(MidiClipEvent& midiClipEvent);
  Loop::ProcessModel& createLoop(ClipEvent& clipEvent);

};


struct TrackCreator{

  const Scenario::ProcessModel& scenar;
  Scenario::Command::Macro& macro;
  double BPM;
  double y;
  void createTrack(Track& tr);
  void operator()(MidiTrack& track);
  void operator()(AudioTrack& track);

};

struct AbletonDocumentCreator {
  void createDoc( AbletonDocument aDoc,
                  const Scenario::ProcessModel& scenar,
                  Scenario::Command::Macro& macro);

};


#endif
