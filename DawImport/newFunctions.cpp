// ============= HPP ================

QDomNodeList getClipEvents(QDomElement& elem);
QDomNodeList getList(QDomDocument& doc, QString str);
QDomNodeList getList(QDomElement& elem, QString str);


// ============= CPP ================

QDomNodeList getList(QDomDocument& doc, QString str){
  return getNodes(doc.firstChildElement("Ableton"), str.split("->"));
}

QDomNodeList getList(QDomElement& elem, QString str){
  return getNodes(elem, str.split("->"));
}

QDomNodeList getClipEvents(QDomElement& elem){
  //get type (audio or midi) and call getList accordingly
  QDomElement type = elem.firstChildElement();
  if(type.hasAttribute("AudioTrack")){
    return getList(elem, PATH_AUDIO_TRACKS);
  }
  else if (type.hasAttribute("MidiTrack")){
    return getList(elem, PATH_MIDI_TRACKS);
}
