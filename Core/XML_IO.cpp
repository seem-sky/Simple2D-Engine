#include "XML_IO.h"

using namespace XML_IO;

/*####
# XML
####*/
bool XML::waitForSuccess()
{
    if (isThreaded())
    {
        m_pThread->join();
        m_pThread = ThreadPtr();
        return m_result != DONE;
    }
    return true;
}

void XML::execThreaded(const QString &fileName, const QString &nodeName)
{
    if (m_result == NONE)
    {
        m_pThread = ThreadPtr(new boost::thread(boost::bind(&XML::_execThreaded, this, fileName, nodeName, m_result)));
        m_pThread->start_thread();
    }
}

bool XML::isThreaded() const
{
    return m_pThread;
}

/*####
# XMLReader
####*/
void XMLReader::_execThreaded(const QString &fileName, const QString &nodeName, XML_Result &result)
{
    result = readFile(fileName, nodeName);
}

XML_Result XMLReader::readFile(const QString &fileName, const QString &nodeName)
{
    m_result = IN_PROGRESS;
    QFile xmlFile(fileName);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        ERROR_LOG("Unable to load XML file " + fileName + " .");
        m_result = FAILED;
        return m_result;
    }

    QDomDocument xmlDoc;
    xmlDoc.setContent(&xmlFile);
    if (xmlDoc.isNull())
    {
        ERROR_LOG("Unable to parse XML file " + fileName + " .");
        m_result = FAILED;
        return m_result;
    }

    QDomNode rootNode = getSingleNode(xmlDoc, nodeName);
    if (rootNode.isNull() || !checkoutChildren(rootNode))
    {
        m_result = FAILED;
        return m_result;
    }
    m_result = DONE;
    return m_result;
}

QDomNode XMLReader::getSingleNode(const QDomNode &parentNode, const QString &nodeName)
{
    if (!parentNode.isNull() && !nodeName.isEmpty())
    {
        QDomNodeList childNodes = parentNode.childNodes();
        for (int32 i = 0; i < childNodes.length(); ++i)
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
void XMLStreamWriter::_execThreaded(const QString &fileName, const QString &nodeName, XML_Result &result)
{
    result = writeFile(fileName, nodeName);
}

XML_Result XMLStreamWriter::writeFile(const QString &fileName, const QString &rootNode)
{
    m_result = IN_PROGRESS;
    QFile xmlFile(fileName);
    if (!xmlFile.open(QIODevice::WriteOnly))
    {
        ERROR_LOG("Unable to load XML file " + fileName + " .");
        m_result = FAILED;
        return m_result;
    }

    QXmlStreamWriter xmlWriter(&xmlFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement(rootNode);
    _writeChildren(xmlWriter);
    xmlWriter.writeEndElement();
    m_result = DONE;
    return m_result;
}