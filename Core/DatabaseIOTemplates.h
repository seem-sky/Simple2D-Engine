#ifndef DATABASE_IOTEMPLATES_H
#define DATABASE_IOTEMPLATES_H

#include "XML_IO.h"
#include "DatabaseMgr.h"

namespace DATABASE
{
    /*####
    # Database
    ####*/
    template <class T>
    class DatabaseReader : public XML_IO::XMLReader
    {
    private:
        void _getPrototypeFromXML(const QDomNode &node)
        {
            if (node.isNull())
                return;
            QDomNamedNodeMap attributeMap = node.attributes();
            boost::shared_ptr<T> proto(new T());
            // get attributes
            for (uint32 i = 0; i < attributeMap.length(); ++i)
            {
                QDomNode attributeNode = attributeMap.item(i);
                if (!attributeNode.isNull())
                    getAttributeFromXML(proto, attributeNode.nodeName(), attributeNode.nodeValue());
            }
            // get children
            checkoutChildren(node, proto);
            m_pDB->setPrototype(proto->getID(), proto);
        }

    protected:
        virtual bool getChildrenFromXML(const QDomNode &node, boost::shared_ptr<T> proto, const QString &childName) { return false; }
        bool checkoutChildren(const QDomNode &node, boost::shared_ptr<T> proto)
        {
            if (node.isNull())
                return false;
            QDomNodeList childNodeList = node.childNodes();
            for (uint32 i = 0; i < childNodeList.length(); ++i)
            {
                if (!childNodeList.at(i).isNull())
                    getChildrenFromXML(childNodeList.at(i), proto, childNodeList.at(i).nodeName());
            }
            return true;
        }

        bool checkoutChildren(const QDomNode &parentNode)
        {
            if (parentNode.isNull())
                return false;
            QDomNodeList childNodeList = parentNode.childNodes();
            for (uint32 i = 0; i < childNodeList.length(); ++i)
                _getPrototypeFromXML(childNodeList.at(i));
            return true;
        }

        virtual bool getAttributeFromXML(boost::shared_ptr<T> proto, const QString &attributeName, const QString &attributeValue)
        {
            if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
                return false;

            if (attributeName == "Name")
            {
                proto->setName(attributeValue.toStdString());
                return true;
            }
            else if (attributeName == "ID")
            {
                proto->setID(attributeValue.toUInt());
                return true;
            }
            return false;
        }

    public:
        DatabaseReader(const boost::shared_ptr<Database<T>> &pDB) : XMLReader(), m_pDB(pDB) {}

    protected:
        boost::shared_ptr<Database<T>> m_pDB;
    };

    template <class T>
    class DatabaseWriter : public XML_IO::XMLStreamWriter
    {
    private:
        bool _writePrototype(uint32 uiID, QXmlStreamWriter &writer)
        {
            boost::shared_ptr<T> proto;
            if (!m_pDB->getPrototype(uiID, proto))
                return false;
            writer.writeStartElement("prototype");
            getXMLFromAttributes(proto, writer);
            writer.writeEndElement();
            return true;
        }

        bool _writeChildren(QXmlStreamWriter &writer)
        {
            for (uint32 i = 1; i <= m_pDB->getDBSize()+1; ++i)
                _writePrototype(i, writer);
            return true;
        }

    protected:
        virtual void getXMLFromAttributes(boost::shared_ptr<T> proto, QXmlStreamWriter &writer)
        {
            if (!proto)
                return;
            writer.writeAttribute("ID", QString::number(proto->getID()));
            writer.writeAttribute("Name", QString::fromStdString(proto->getName()));
        }

    public:
        DatabaseWriter(const boost::shared_ptr<Database<T>> &pDB) : XMLStreamWriter(), m_pDB(pDB) {}

    protected:
        boost::shared_ptr<Database<T>> m_pDB;
    };

    /*#####
    # TextureDatabases
    #####*/
    template <class T>
    class TextureDatabaseReader : public DatabaseReader<T>
    {
    protected:
        virtual bool getAttributeFromXML(boost::shared_ptr<T> proto, const QString &attributeName, const QString &attributeValue)
        {
            if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
                return false;
            if (DatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
                return true;

            if (attributeName == "FileName")
            {
                proto->setFileName(attributeValue.toStdString());
                return true;
            }
            else if (attributeName == "Path")
            {
                proto->setPath(attributeValue.toStdString());
                return true;
            }
            else if (attributeName == "transparent_color")
            {
                proto->setTransparencyColor(Color(attributeValue.toStdString()));
                return true;
            }
            return false;
        }

    public:
        TextureDatabaseReader(const boost::shared_ptr<Database<T>> &pDB) : DatabaseReader(pDB){}
    };

    template <class T>
    class TextureDatabaseWriter : public DatabaseWriter<T>
    {
    protected:
        virtual void getXMLFromAttributes(boost::shared_ptr<T> proto, QXmlStreamWriter &writer)
        {
            if (!proto)
                return;
            DatabaseWriter::getXMLFromAttributes(proto, writer);
            writer.writeAttribute("FileName", QString::fromStdString(proto->getFileName()));
            writer.writeAttribute("Path", QString::fromStdString(proto->getPath()));
            writer.writeAttribute("transparent_color", QString::fromStdString(proto->getTransparencyColor().getColorString()));
        }

    public:
        TextureDatabaseWriter(const boost::shared_ptr<Database<T>> &pDB) : DatabaseWriter(pDB) {}
    };
}
#endif