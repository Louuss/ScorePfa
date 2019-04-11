#ifndef CREATESTRUCTS
#define CREATESTRUCTS
#include "ParseStructs.hpp"
#include "Structs.hpp"

#include <Loop/LoopProcessModel.hpp>
#include <Media/ApplicationPlugin.hpp>
#include <Media/Commands/InsertVST.hpp>
#include <Media/Effect/EffectProcessModel.hpp>
#include <Media/Effect/VST/VSTEffectModel.hpp>
#include <Media/Sound/SoundModel.hpp>
#include <Midi/Commands/AddNote.hpp>
#include <Midi/MidiProcess.hpp>
#include <Scenario/Commands/CommandAPI.hpp>
#include <Scenario/Document/BaseScenario/BaseScenario.hpp>
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentModel.hpp>
#include <Scenario/Process/Algorithms/Accessors.hpp>
#include <Scenario/Process/Algorithms/ContainersAccessors.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

#include <score/actions/Menu.hpp>
#include <score/actions/MenuManager.hpp>
#include <score/document/DocumentInterface.hpp>

#include <core/document/Document.hpp>

#include <QMenu>
#include <QtXml/QtXml>

#include <math.h>

/**
\file CreateStructs.hpp
\brief Create Score structures from our own structures defined in Structs.hpp

*/

struct ClipEventCreator
{

  const Scenario::ProcessModel& scenar;
  Scenario::Command::Macro& macro;
  Id<Scenario::StateModel> startDot;
  double BPM;
  const Scenario::ScenarioDocumentModel& root;


  Id<Scenario::StateModel> endDot = Id<Scenario::StateModel>();
  double prevEnd = 0;


  void operator()(AudioClipEvent& audioClipEvent);
  void operator()(MidiClipEvent& midiClipEvent);

  /**
  \fn Loop::ProcessModel& createLoop(ClipEvent& clipEvent)
  \brief

  \param clipEvent

  */
  Loop::ProcessModel& createLoop(ClipEvent& clipEvent);
};

struct TrackCreator
{

  const Scenario::ProcessModel& scenar;
  Scenario::Command::Macro& macro;
  double BPM;
  double y;
  const score::GUIApplicationContext& context;
  const Scenario::ScenarioDocumentModel& root;
  Id<Scenario::IntervalModel> trackId = Id<Scenario::IntervalModel>() ;

  /**
  \fn void createTrack(Track& tr)
  \brief

  \param tr

  */
  void addFX(MidiTrack& tr, const Scenario::ProcessModel& trackScenario);
  const Scenario::ProcessModel& createTrack(Track& tr);
  void operator()(MidiTrack& track);
  void operator()(AudioTrack& track);
};

struct AbletonDocumentCreator
{
  /**
  \fn void createDoc(AbletonDocument aDoc, const Scenario::ProcessModel& scenar, Scenario::Command::Macro& macro)
  \brief Fills the given ClipEvent with the values found in the clipXml

  \param aDoc The AbletonDocument from where the score structures are built
  \param scenar
  \param macro

  */
  void createDoc(
      AbletonDocument aDoc,
      const Scenario::ProcessModel& scenar,
      Scenario::Command::Macro& macro,
      const score::GUIApplicationContext& context,
      const Scenario::ScenarioDocumentModel& root);
};

#endif
