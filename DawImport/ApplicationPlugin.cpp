#include <DawImport/ApplicationPlugin.hpp>

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

#include <QMenu>
#include <QtXml/QtXml>

#include "Structs.hpp"
#include "ParseXml.hpp"
#include "CreateScore.hpp"


namespace DawImport
{

static void generateScore(
    const Scenario::ScenarioDocumentModel& root,
    const Scenario::ProcessModel& scenar,
    Scenario::Command::Macro& macro,
    const score::GUIApplicationContext& context)
{







}


ApplicationPlugin::ApplicationPlugin(const score::GUIApplicationContext& app):
    score::GUIApplicationPlugin{app}
{
  m_generate = new QAction{tr("Generate a score"), nullptr};
  connect(m_generate, &QAction::triggered, this, &ApplicationPlugin::generate);
}

score::GUIElements ApplicationPlugin::makeGUIElements()
{
    auto& m = context.menus.get().at(score::Menus::Export());
    QMenu* menu = m.menu();
    menu->addAction(m_generate);
    return {};
}

void ApplicationPlugin::generate()
{
  auto doc = currentDocument();
  if(!doc)
      return;
  Scenario::ScenarioDocumentModel& base = score::IDocument::get<Scenario::ScenarioDocumentModel>(*doc);

  const auto& baseInterval = base.baseScenario().interval();
  if(baseInterval.processes.size() == 0)
      return;

  auto firstScenario = dynamic_cast<Scenario::ProcessModel*>(&*baseInterval.processes.begin());
  if(!firstScenario)
      return;

  Scenario::Command::Macro m{new GenerateAScore, doc->context()};

  QDomDocument docXml;
  QFile file(PLUGIN_SOURCE_DIR "/DawImport/resources/audio1.xml");
  if (!file.open(QIODevice::ReadOnly) || !docXml.setContent(&file)){
      //std::cout << "ERROR: error oppenning xml file" << std::endl
    }


  AbletonDocument abletonDoc;

  loadDocument(docXml, abletonDoc);

  createAudioClip(base, *firstScenario, m, this->context, abletonDoc.audioTracks[0].audioClipEvents[0]);

  //Scenario::Command::Macro m2{new GenerateAScore, doc->context()};

  //createMidiClip(base, *firstScenario, m2, this->context, abletonDoc.midiTracks[0]->midiClipEvents[1]);


}



}
