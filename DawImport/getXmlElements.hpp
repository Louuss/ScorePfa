#ifndef GETXMLELEMENTS
#define GETXMLELEMENTS

#include "XmlPaths.hpp"
#include <QtXml/QtXml>
#include <vector>

QDomNodeList getList(QDomDocument& doc, QString str);
QDomNodeList getList(QDomElement& elem, QString str);
QDomNodeList getClipEvents(QDomElement& elem);
QString getItemValue(QDomElement& n, QString path);
QString getItemValue(QDomDocument& doc, QString path);
std::string getAudioClipPath(QDomElement& clipEventXml);


#endif
