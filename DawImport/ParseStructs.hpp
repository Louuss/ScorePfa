#ifndef PARSESTRUCTS
#define PARSESTRUCTS

#include "Structs.hpp"
#include "XmlPaths.hpp"
#include "getXmlElements.hpp"

#include <QMenu>
#include <QtXml/QtXml>

/**
\file ParseStructs.hpp
\brief Parse xml files to obtain structures given in Structs.hpp

*/

struct ClipEventLoader
{
  QDomElement& clipXml;
  double end = 0;

  /**
  \fn int loadNotes(QDomElement& keyTrack, std::vector<MidiNote>& notes,
  uint8_t midiKeyValue) \brief Fills the MidiNote vector for a given keyTrack

  \param keyTrack The keyTrack where the notes are selected
  \param notes The vector where the notes are added
  \param midiKeyValue The value of the pitch for all the notes of the given
  keyTrack \return 0 if success
  */
  int loadNotes(
      QDomElement& keyTrack,
      std::vector<MidiNote>& notes,
      uint8_t midiKeyValue);

  /**
  \fn void loadClipAtributes(ClipEvent& clipEvent)
  \brief Fills the given ClipEvent with the values found in the clipXml

  \param clipEvent The ClipEvent which is going to be completed

  */
  void loadClipAtributes(ClipEvent& clipEvent);
  void operator()(MidiClipEvent& midiClipEvent);
  void operator()(AudioClipEvent& audioClipEvent);
};

struct TrackLoader
{
  QDomElement& trackXml;
  int trackType = NULLTRACKTYPE;

  /**
  \fn void loadTrack(Track& track)
  \brief Fills the given track with the values found in the trackXml

  \param track The Track which is going to be completed

  */
  void loadTrack(Track& track);
  void operator()(MidiTrack& track);
  void operator()(AudioTrack& track);
};

struct AbletonDocumentLoader
{

  /**
  \fn void loadTracks(QDomDocument& docXml,
  std::vector<std::variant<AudioTrack,
  MidiTrack>>& tracks,
  QString path,
  int trackType);
  \brief Fills the given tracks vector with the given Audio or Midi Traks

  \param docXml The xml file where the tracks are taken
  \param tracks The vector which is filled
  \param path The path where the tracks will be found
  \param trackType 1 for AudioTracks, 2 for MidiTracks

  */
  void loadTracks(
      QDomDocument& docXml,
      std::vector<std::variant<AudioTrack, MidiTrack>>& tracks,
      QString path,
      int trackType);

  /**
  \fn void loadAbletonDocument(QDomDocument& docXml, AbletonDocument&
  abletonDoc) \brief Fills the given AbletonDocument structure

  \param doxXml The Xml file where the BPM attribute is found
  \param abletonDoc The AbletonDocument which is going to have its attributes
  initialized

  */
  void loadAbletonDocument(QDomDocument& docXml, AbletonDocument& abletonDoc);
};

#endif
