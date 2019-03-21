#ifndef GETXMLELEMENTS
#define GETXMLELEMENTS

QDomNodeList getNodes(QDomElement& n, QStringList args);
QDomNodeList getList(QDomDocument& doc, QString str);
QDomNodeList getList(QDomElement& elem, QString str);
QDomNodeList getClipEvents(QDomElement& elem);



#endif
