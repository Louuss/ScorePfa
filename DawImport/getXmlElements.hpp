#ifndef GETXMLELEMENTS
#define GETXMLELEMENTS

#include "XmlPaths.hpp"
#include <QtXml/QtXml>
#include <vector>

QDomNodeList getNodes(QDomElement& n, QStringList args);
QDomNodeList getList(QDomDocument& doc, QString str);
QDomNodeList getList(QDomElement& elem, QString str);
QDomNodeList getClipEvents(QDomElement& elem);
QString getItemValue(QDomElement& n, QString path);
std::string getAudioClipPath(QDomElement& docXml);


#endif
