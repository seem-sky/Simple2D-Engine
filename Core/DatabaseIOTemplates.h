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
        void _getItemFromXML(const QDomNode &node)
        {
            if (node.isNull())
                return;
            QDomNamedNodeMap attributeMap = node.attributes();
            std::shared_ptr<T> proto(new T());
            // get attributes
            for (int32 i = 0; i < attributeMap.length(); ++i)
            {
                QDomNode attributeNode = attributeMap.item(i);
                if (!attributeNode.isNull())
                    getAttributeFromXML(proto, attributeNode.nodeName(), attributeNode.nodeValue());
            }
            // get children
            checkoutChildren(node, proto);
            m_pDB->setItem(proto->getID(), proto);
        }

    protected:
        virtual bool getChildrenFromXML(const QDomNode &node, std::shared_ptr<T> proto, const QString &childName) { return false; }
        bool checkoutChildren(const QDomNode &node, std::shared_ptr<T> proto)
        {
            if (node.isNull())
                return false;
            QDomNodeList childNodeList = node.childNodes();
            for (int32 i = 0; i < childNodeList.length(); ++i)
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
            for (int32 i = 0; i < childNodeList.length(); ++i)
                _getItemFromXML(childNodeList.at(i));
            return true;
        }

        virtual bool getAttributeFromXML(std::shared_ptr<T> proto, const QString &attributeName, const QString &attributeValue)
        {
            if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
                return false;

            if (attributeName == "Name")
            {
                proto->setName(attributeValue);
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
        DatabaseReader(const std::shared_ptr<Database<T>> &pDB) : XMLReader(), m_pDB(pDB) {}

    protected:
        std::shared_ptr<Database<T>> m_pDB;
    };

    template <class T>
    class DatabaseWriter : public XML_IO::XMLStreamWriter
    {
    private:
        bool _writePrototype(uint32 uiID, QXmlStreamWriter &writer)
        {
            std::shared_ptr<T> proto;
            if (!m_pDB->getItem(uiID, proto))
                return false;
            writer.writeStartElement("prototype");
            getXMLFromAttributes(proto, writer);
            writer.writeEndElement();
            return true;
        }

        bool _writeChildren(QXmlStreamWriter &writer)
        {
            for (uint32 i = 1; i <= m_pDB->getSize()+1; ++i)
                _writePrototype(i, writer);
            return true;
        }

    protected:
        virtual void getXMLFromAttributes(std::shared_ptr<T> proto, QXmlStreamWriter &writer)
        {
            if (!proto)
                return;
            writer.writeAttribute("ID", QString::number(proto->getID()));
            writer.writeAttribute("Name", proto->getName());
        }

    public:
        DatabaseWriter(const std::shared_ptr<Database<T>> &pDB) : XMLStreamWriter(), m_pDB(pDB) {}

    protected:
        std::shared_ptr<Database<T>> m_pDB;
    };

    /*#####
    # TextureDatabases
    #####*/
    template <class T>
    class TextureDatabaseReader : public DatabaseReader<T>
    {
    protected:
        virtual bool getAttributeFromXML(std::shared_ptr<T> proto, const QString &attributeName, const QString &attributeValue)
        {
            if (!proto || attributeName.isEmpty() || attributeValue.isEmpty())
                return false;
            if (DatabaseReader::getAttributeFromXML(proto, attributeName, attributeValue))
                return true;

            if (attributeName == "FileName")
            {
                proto->setFileName(attributeValue);
                return true;
            }
            else if (attributeName == "Path")
            {
                proto->setPath(attributeValue);
                return true;
            }
            else if (attributeName == "transparent_color")
            {
                proto->setTransparencyColor(Color(attributeValue));
                return true;
            }
            return false;
        }

    public:
        TextureDatabaseReader(const std::shared_ptr<Database<T>> &pDB) : DatabaseReader(pDB){}
    };

    template <class T>
    class TextureDatabaseWriter : public DatabaseWriter<T>
    {
    protected:
        virtual void getXMLFromAttributes(std::shared_ptr<T> proto, QXmlStreamWriter &writer)
        {
            if (!proto)
                return;
            DatabaseWriter::getXMLFromAttributes(proto, writer);
            writer.writeAttribute("FileName", proto->getFileName());
            writer.writeAttribute("Path", proto->getPath());
            writer.writeAttribute("transparent_color", proto->getTransparencyColor().getColorString());
        }

    public:
        TextureDatabaseWriter(const std::shared_ptr<Database<T>> &pDB) : DatabaseWriter(pDB) {}
    };
}
#endif