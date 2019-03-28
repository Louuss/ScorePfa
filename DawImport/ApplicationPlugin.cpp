#include "CreateStructs.hpp"
#include "Debug.hpp"
#include "ParseStructs.hpp"
#include "Structs.hpp"
#include "gzipSupport.hpp"

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

#include <DawImport/ApplicationPlugin.hpp>

namespace DawImport
{

ApplicationPlugin::ApplicationPlugin(const score::GUIApplicationContext& app)
    : score::GUIApplicationPlugin{app}
{
  m_generate = new QAction{tr("Ableton Project"), nullptr};
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
  if (!doc)
    return;

  Scenario::ScenarioDocumentModel& base
      = score::IDocument::get<Scenario::ScenarioDocumentModel>(*doc);

  const auto& baseInterval = base.baseScenario().interval();
  if (baseInterval.processes.size() == 0)
    return;

  auto firstScenario = dynamic_cast<Scenario::ProcessModel*>(
      &*baseInterval.processes.begin());
  if (!firstScenario)
    return;

  Scenario::Command::Macro m{new GenerateAScore, doc->context()};

  QString caption = QString::fromStdString("Import a document");
  QString filter = QString::fromStdString("Live Files (*.als)");
  QFileDialog liveFile{nullptr, caption, QString(), filter};
  liveFile.setFileMode(QFileDialog::ExistingFile);

  if (liveFile.exec())
  {
    QString liveFilePath = liveFile.selectedFiles().first();

    QFile file(liveFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
    }

    QByteArray input = file.readAll();
    QByteArray output;
    gzipDecompress(input, output);

    QDomDocument docXml;
    docXml.setContent(output);

    AbletonDocument abletonDoc;

    AbletonDocumentLoader ADL;

    ADL.loadAbletonDocument(docXml, abletonDoc);

    displayAbletonDoc(abletonDoc);

    AbletonDocumentCreator adc;
    adc.createDoc(abletonDoc, *firstScenario, m);
  }
}

}
