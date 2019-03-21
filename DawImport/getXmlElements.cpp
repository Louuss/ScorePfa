#include "getXmlElements.hpp"

using namespace std;



QDomNodeList getNodes(QDomElement& n, QStringList args){
  QDomElement currentElement = n;

  for(int i=0;i<args.length()-1;i++){
    cout<<args.at(i).toStdString()<<endl;
    currentElement = currentElement.firstChildElement(args.at(i));
  }

  return currentElement.elementsByTagName(args.at(args.length()-1));
}


QDomNodeList getList(QDomDocument& doc, QString str){
  QDomElement n = doc.firstChildElement("Ableton");
  return getNodes(n, str.split("->"));
}

QDomNodeList getList(QDomElement& elem, QString str){
  return getNodes(elem, str.split("->"));
}

QDomNodeList getClipEvents(QDomElement& elem){
  //get type (audio or midi) and call getList accordingly
  std::cout<< elem.tagName().toStdString() << std::endl;
  if(elem.tagName().toStdString() == "AudioTrack"){
    return getList(elem, PATH_AUDIO_TRACKS);
  }
  else if (elem.tagName().toStdString() == "MidiTrack"){
    return getList(elem, PATH_MIDI_TRACKS);
  }
  QDomNodeList a;
  return a;
}
