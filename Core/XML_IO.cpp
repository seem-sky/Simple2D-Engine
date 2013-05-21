#include "XML_IO.h"

using namespace XML_IO;

/*####
# XMLReader
####*/
bool XMLReader::readFile(const QString &fileName, const QString &nodeName)
{
    QFile xmlFile(fileName);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        ERROR_LOG("Unable to load XML file " + fileName.toStdString() + " .");
        return false;
    }

    QDomDocument xmlDoc;
    xmlDoc.setContent(&xmlFile);
    if (xmlDoc.isNull())
    {
        ERROR_LOG("Unable to parse XML file " + fileName.toStdString() + " .");
        return false;
    }

    QDomNode rootNode = getSingleNode(xmlDoc, nodeName);
    if (!rootNode.isNull())
        return checkoutChildren(rootNode);
    return false;
}

QDomNode XMLReader::getSingleNode(const QDomNode &parentNode, const QString &nodeName)
{
    if (!parentNode.isNull() && !nodeName.isEmpty())
    {
        QDomNodeList childNodes = parentNode.childNodes();
        for (uint32 i = 0; i < childNodes.length(); ++i)
        {
            if (childNodes.at(i).nodeName() == nodeName)
                return childNodes.at(i);
        }
    }
    return QDomNode();
}

QDomNode XMLReader::changeRootNode(const QDomNode &parentNode, const QStringList &nodeNameList)
{
    if (parentNode.isNull())
        return QDomNode();

    QDomNode node = parentNode;
    for (int32 j = 0; j < nodeNameList.length(); ++j)
    {
        node = getSingleNode(node, nodeNameList.at(j));
        if (node.isNull())
            return QDomNode();
    }
    return node;
}

/*####
# XMLStreamWriter
####*/
bool XMLStreamWriter::writeFile(const QString &fileName, const QString &rootNode)
{
    QFile xmlFile(fileName);
    if (!xmlFile.open(QIODevice::WriteOnly))
    {
        ERROR_LOG("Unable to load XML file " + fileName.toStdString() + " .");
        return false;
    }

    QXmlStreamWriter xmlWriter(&xmlFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement(rootNode);
    _writeChildren(xmlWriter);
    return true;
}