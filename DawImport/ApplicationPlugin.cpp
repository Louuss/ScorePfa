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
#include "ParseStructs.hpp"

#include "gzipSupport.hpp"

#include "Debug.hpp"
#include "CreateStructs.hpp"

#include "CreateStructs.hpp"

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


      QString caption = QString::fromStdString("Import a document");
      QString filter = QString::fromStdString("Live Files (*.als)");
      QFileDialog liveFile{nullptr, caption, QString(), filter};
      liveFile.setFileMode(QFileDialog::ExistingFile);

      if (liveFile.exec())
      {
        QString liveFilePath = liveFile.selectedFiles().first();

        QFile file(liveFilePath);
        if (!file.open(QIODevice::ReadOnly)){}

        QByteArray input = file.readAll();
        QByteArray output;
        gzipDecompress(input , output);

        QDomDocument docXml;
        docXml.setContent(output);


        AbletonDocument abletonDoc;

        AbletonDocumentLoader ADL;
        std::cout<<" ~~~~~~~~Avant le load~~~~~~~~~~~~"<<std::endl;
        ADL.loadAbletonDocument(docXml, abletonDoc);
        //createTrack(base, *firstScenario, m, this->context, 10,0.02, abletonDoc.midiTracks[0].midiClipEvents[0]);
<<<<<<< HEAD
        displayAbletonDoc(abletonDoc);
        std::cout << "Hihhihiiihishhshshshshshshshs" << std::endl;

        //AbletonDocumentCreator ADC{};
        //ADC.createDoc(abletonDoc, *firstScenario, m);
=======

        std::cout<<" ~~~~~~~~~~AAAPPRES LE LOAD ~~~~~~~~~~"<< std::endl;
        //displayAbletonDoc(abletonDoc);
        std::cout<<" ~~~~~~~///////////////////~~~~~~~~"<<std::endl;
        AbletonDocumentCreator adc ;
        adc.createDoc(abletonDoc, *firstScenario, m);
>>>>>>> b1e71da3f1ba550cf6bff21c6091fbdcd2f8e28b

      }

    }

}
