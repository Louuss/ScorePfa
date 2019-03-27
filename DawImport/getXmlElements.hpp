#ifndef GETXMLELEMENTS
#define GETXMLELEMENTS

#include "XmlPaths.hpp"
#include <QtXml/QtXml>
#include <vector>

/**
\file getXmlElements.hpp
\brief Search on a xml file interesting data

*/

/**
\fn QDomNodeList getNodes(QDomElement& n, QStringList args)
\brief Searches for Nodes corresponding at the given tags

\param n The QDomElement where the Nodes are searched
\param args A QStringList containing the tags

\return A QDomNodeList containing all the nodes that have been found

*/
QDomNodeList getNodes(QDomElement& n, QStringList args);

/**
\fn QDomNodeList getList(QDomDocument& doc, QString str)
\brief Searches for all the Nodes corresponding to the given path

\param doc The QDomDocument, corresponding to the entire xml file
\param str A QString containing the tags corresponding to the path

\return A QDomNodeList containing all the different nodes corresponding to str
*/
QDomNodeList getList(QDomDocument& doc, QString str);

/**
\fn QDomNodeList getList(QDomElement& elem, QString str);
\brief Does exactly the same thing as the above function; but on a QDomElement instead

*/
QDomNodeList getList(QDomElement& elem, QString str);

/**
\fn QDomNodeList getClipEvents(QDomElement& elem)
\brief Searches for all the ClipEvent in a given QDomElement


\param elem The QDomElement where are located the ClipEvents.
It begins either by an AudioTrack tag or a MidiTrack tag

\return A QDomNodeList containing all the different ClipEvents
*/
QDomNodeList getClipEvents(QDomElement& elem);

/**
\fn QString getItemValue(QDomElement& n, QString path)
\brief Searches for the value of a node at the given path


\param n The QDomElement where the node value is searched
\param path A QString containing the path of the node

\return A QString containing the value of the node
*/
QString getItemValue(QDomElement& n, QString path);

/**
\fn QString getItemValue(QDomDocument& doc, QString path)
\brief The same function as above, but called with a QDomDocument corresponding to the entire Xml file
*/
QString getItemValue(QDomDocument& doc, QString path);

/**
\fn std::string getAudioClipPath(QDomElement& docXml)
\brief Searches for path of an AudioClip

\param docXml The QDomElement where the path is located

\return A string containing the path of the AudioClip
*/
std::string getAudioClipPath(QDomElement& docXml);


#endif
