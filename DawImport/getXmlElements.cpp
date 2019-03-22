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
    return getList(elem, PATH_AUDIO_CLIP_EVENTS);
  }
  else if (elem.tagName().toStdString() == "MidiTrack"){
    return getList(elem, PATH_MIDI_CLIP_EVENTS);
  }
  QDomNodeList a;
  return a;
}


std::string getPath(QDomElement& clipEventXml){
  QDomNodeList pathList = getList(clipEventXml, PATH_AUDIO_CLIP_PATH);
  QString path;
  for(int i = 0; i < pathList.size(); i++){
    path += pathList.at(i).attributes().item(0).nodeValue();
    path += "/";
  }
  path+= clipEventXml.firstChildElement("SampleRef").firstChildElement("FileRef").firstChildElement("Name").attributes().item(0).nodeValue();

  return path.toStdString();
}
