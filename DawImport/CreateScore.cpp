/**
  \file CreateScore.cpp
  \brief Create objects in Score.

  *Create objects in Score based on Ableton structures that have been generated from the xml.
  */

#include "CreateScore.hpp"




void createMidiClip(
    const Scenario::ScenarioDocumentModel& root,
    const Scenario::ProcessModel& scenar,
    Scenario::Command::Macro& macro,
    const score::GUIApplicationContext& context,
    MidiClip & midiClip)
{


  using namespace std::literals;

  // Création d'une structure de base
  constexpr double y = 0.02;
  auto& s1 = macro.createState(scenar, scenar.startEvent().id(), y);

  const auto& [t2, e2, s2] = macro.createDot(scenar, {10s, y});
  const auto& i1 = macro.createInterval(scenar, s1.id(), s2.id());

  // Ajoutons un premier process
  auto& midi = macro.createProcessInNewSlot<Midi::ProcessModel>(i1, {});

  /////////////////////////////////////////////////////////////////////////////


  for (int i = 0; i < 3; i++) {



      //n.firstChildElement("Name").firstChildElement("EffectiveName").attributes().item(0).nodeValue().toStdString()

      //QDomElement from = n.firstChildElement("from");
      //QDomElement to = n.firstChildElement("to");
      //QDomElement conversion = n.firstChildElement("conversion");

  }

  /////////////////////////////////////////////////////////////////////////////
  macro.submit(new Midi::ReplaceNotes(midi, midiClip.midiNotes, 55, 85, 10000ms));


  // Et un autre
  const auto& vsts = context.applicationPlugin<Media::ApplicationPlugin>().vst_infos;
  auto& fx_chain = macro.createProcessInNewSlot<Media::Effect::ProcessModel>(i1, {});

  // On va voir les VSTs qu'il y a sur le système
  for(const auto& vst : vsts)
  {
    // On ajoute le premier synthé qu'on trouve
    // (J'ai désactivé Carla car c'est un VST qui bug chez moi...)
    if(vst.isSynth && vst.isValid && !vst.prettyName.contains("Carla"))
    {
      macro.submit(new Media::InsertGenericEffect<Media::VST::VSTEffectModel>(
                     fx_chain,
                     QString::number(vst.uniqueID),
                     0));
      break;
    }
  }


  // On connecte un cable entre le midi et le vst

  Process::CableData cable;
    cable.source = *midi.outlet;
    cable.sink = *fx_chain.inlet;
    cable.type = Process::CableType::ImmediateGlutton;
    macro.createCable(root, std::move(cable));



  // Et un fichier son
  /*const auto& i2 = macro.createIntervalAfter(scenar, s2.id(), {5000ms, y});
  macro.createProcessInNewSlot<Media::Sound::ProcessModel>(i2, PLUGIN_SOURCE_DIR "/DawImport/resources/test.wav");
*/
  macro.commit();
}













void createAudioClip(
    const Scenario::ScenarioDocumentModel& root,
    const Scenario::ProcessModel& scenar,
    Scenario::Command::Macro& macro,
    const score::GUIApplicationContext& context,
    AudioClip& audioClip)
{


  using namespace std::literals;

  // Création d'une structure de base
  constexpr double y = 0.02;
  auto& s1 = macro.createState(scenar, scenar.startEvent().id(), y);

  const auto& [t2, e2, s2] = macro.createDot(scenar, {20s, y});
  const auto& i1 = macro.createInterval(scenar, s1.id(), s2.id());
  QString path  = PLUGIN_SOURCE_DIR "/DawImport/resources/" + audioClip.path;

  auto& loop = macro.createProcessInNewSlot<Loop::ProcessModel>(i1, {});
  macro.createProcessInNewSlot<Media::Sound::ProcessModel>(loop.interval(), path);


  //macro.createProcessInNewSlot<Media::Sound::ProcessModel> ( i1, path);

  macro.commit();

}
