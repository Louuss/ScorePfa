#include "getXmlElements.hpp"

using namespace std;

QDomNodeList getList(QDomDocument& doc, QString str)
{
  QDomElement n = doc.firstChildElement("Ableton");
  return getList(n, str);
}

QDomNodeList getList(QDomElement& elem, QString str)
{
  QStringList args = str.split("->");
  QDomElement currentElement = elem;

  for (int i = 0; i < args.length() - 1; i++)
  {
    cout << args.at(i).toStdString() << endl;
    currentElement = currentElement.firstChildElement(args.at(i));
  }

  return currentElement.elementsByTagName(args.at(args.length() - 1));
}

QDomNodeList getClipEvents(QDomElement& elem)
{
  // get type (audio or midi) and call getList accordingly
  std::cout << elem.tagName().toStdString() << std::endl;
  if (elem.tagName().toStdString() == "AudioTrack")
  {
    return getList(elem, PATH_AUDIO_CLIP_EVENTS);
  }
  else if (elem.tagName().toStdString() == "MidiTrack")
  {
    return getList(elem, PATH_MIDI_CLIP_EVENTS);
  }
  QDomNodeList a;
  return a;
}

QString getItemValue(QDomElement& n, QString path)
{
  QStringList args = path.split("->");
  QDomElement currentElement = n;

  for (int i = 0; i < args.length() - 1; i++)
  {
    cout << args.at(i).toStdString() << endl;
    currentElement = currentElement.firstChildElement(args.at(i));
  }

  return currentElement.attributes()
      .namedItem(args.at(args.length() - 1))
      .nodeValue();
}
QString getItemValue(QDomDocument& doc, QString path)
{
  QDomElement n = doc.firstChildElement("Ableton");
  return getItemValue(n, path);
}

std::string getAudioClipPath(QDomElement& clipEventXml)
{
  QDomNodeList pathList = getList(clipEventXml, PATH_AUDIO_CLIP_PATH);
  QString path;
  for (int i = 0; i < pathList.size(); i++)
  {
    path += pathList.at(i).attributes().item(0).nodeValue();
    path += "/";
  }
  path += getItemValue(clipEventXml, PATH_AUDIO_CLIP_NAME);

  return path.toStdString();
}
