#ifndef PARSESTRUCTS
#define PARSESTRUCTS

#include <QMenu>
#include <QtXml/QtXml>
#include "Structs.hpp"
#include "XmlPaths.hpp"
#include "getXmlElements.hpp"


struct ClipEventLoader{
  QDomElement& clipXml;

  int loadNotes(QDomElement& keyTrack, std::vector<MidiNote>& notes, uint8_t midiKeyValue);
  void loadClipAtributes(ClipEvent& clipEvent);
  void operator()(MidiClipEvent& midiClipEvent);
  void operator()(AudioClipEvent& audioClipEvent);
};

struct TrackLoader{
  QDomElement& trackXml;
  int trackType;
  void loadTrack(Track& track);
  void operator()(MidiTrack& track);
  void operator()(AudioTrack& track);
};

struct AbletonDocumentLoader{
  void loadTracks(QDomDocument& docXml,
                  std::vector<std::variant<AudioTrack,
                  MidiTrack>>& tracks,
                  QString path,
                  int trackType);
  void loadAbletonDocument(QDomDocument& docXml, AbletonDocument& abletonDoc);
};

#endif
