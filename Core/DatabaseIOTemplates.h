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
        void _getPrototypeFromXML(const IXMLDOMNodePtr &pParentNode)
        {
            IXMLDOMNamedNodeMapPtr pAttributes;
            LONG attributeCount = 0;
            if (FAILED(pParentNode->get_attributes(&pAttributes)) || FAILED(pAttributes->get_length(&attributeCount)))
                return;

            CComBSTR attributeText;
            CComVariant value;
            boost::shared_ptr<T> proto(new T());
            for (LONG i = 0; i < attributeCount; ++i)
            {
                IXMLDOMNodePtr pNodeTemp = NULL;
                if (FAILED(pAttributes->get_item(i, &pNodeTemp)) || FAILED(pNodeTemp->get_nodeName(&attributeText)) ||
                    FAILED(pNodeTemp->get_nodeValue(&value)))
                    continue;

                std::string sAttributeText = _bstr_t(attributeText);
                getAttributeFromXML(proto, sAttributeText, value);
            }
            m_pDB->setPrototype(proto->getID(), proto);
        }

    protected:
        bool checkoutChildren(const IXMLDOMNodePtr &pParentNode)
        {
            IXMLDOMNodeListPtr pChildNodes;
            if (FAILED(pParentNode->get_childNodes(&pChildNodes)))
                return false;

            LONG nodeLength = 0;
            if (FAILED(pChildNodes->get_length(&nodeLength)))
                return false;

            IXMLDOMNodePtr pNodeTemp = NULL;
            for (LONG i = 0; i < nodeLength; ++i)
            {
                if (SUCCEEDED(pChildNodes->get_item(i, &pNodeTemp)))
                    _getPrototypeFromXML(pNodeTemp);
            }
            return true;
        }

        virtual bool getAttributeFromXML(boost::shared_ptr<T> proto, const std::string sAttributeName, CComVariant value)
        {
            if (!proto)
                return false;

            if (sAttributeName == "Name")
            {
                proto->setName(std::string(_bstr_t(value)));
                return true;
            }
            else if (sAttributeName == "ID")
            {
                if (SUCCEEDED(value.ChangeType(VT_UINT)))
                {
                    proto->setID(value.uintVal);
                    return true;
                }
            }
            return false;
        }

    public:
        DatabaseReader(const boost::shared_ptr<Database<T>> &pDB) : XMLReader(), m_pDB(pDB) {}

    protected:
        boost::shared_ptr<Database<T>> m_pDB;
    };

    template <class T>
    class DatabaseWriter : public XML_IO::XMLWriter
    {
    private:
        virtual std::string _getNodeName() const = 0;

        bool _getXMLFromPrototype(uint32 uiID, MSXML2::IXMLDOMNodePtr &pNewNode)
        {
            boost::shared_ptr<T> proto;
            if (!pNewNode || !m_pDB->getPrototype(uiID, proto))
                return false;
            getXMLFromAttributes(proto, pNewNode);
            return true;
        }

        bool writeChildren(MSXML2::IXMLDOMNodePtr &parentNode)
        {
            for (uint32 i = 1; i <= m_pDB->getDBSize()+1; ++i)
            {
                MSXML2::IXMLDOMNodePtr newNode = createNode(_getNodeName());
                if (_getXMLFromPrototype(i, newNode))
                    addChildren(newNode, parentNode);
            }
            return true;
        }

    protected:
        virtual void getXMLFromAttributes(boost::shared_ptr<T> proto, MSXML2::IXMLDOMNodePtr &pNewNode)
        {
            if (!proto)
                return;
            changeAttribute(pNewNode, proto->getID(), "ID");
            changeAttribute(pNewNode, proto->getName().c_str(), "Name");
        }

    public:
        DatabaseWriter(const boost::shared_ptr<Database<T>> &pDB) : XMLWriter(), m_pDB(pDB) {}

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
        virtual bool getAttributeFromXML(boost::shared_ptr<T> proto, const std::string sAttributeName, CComVariant value)
        {
            if (!proto)
                return false;
            if (DatabaseReader::getAttributeFromXML(proto, sAttributeName, value))
                return true;

            if (sAttributeName == "FileName")
            {
                proto->setFileName(std::string(_bstr_t(value)));
                return true;
            }
            else if (sAttributeName == "Path")
            {
                proto->setPath(std::string(_bstr_t(value)));
                return true;
            }
            else if (sAttributeName == "transparent_color")
            {
                proto->setTransparencyColor(Color(std::string(_bstr_t(value))));
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
        virtual void getXMLFromAttributes(boost::shared_ptr<T> proto, MSXML2::IXMLDOMNodePtr &pNewNode)
        {
            if (!proto)
                return;
            DatabaseWriter::getXMLFromAttributes(proto, pNewNode);
            changeAttribute(pNewNode, proto->getFileName().c_str(), "FileName");
            changeAttribute(pNewNode, proto->getPath().c_str(), "Path");
            changeAttribute(pNewNode, proto->getTransparencyColor().getColorString().c_str(), "transparent_color");
        }

    public:
        TextureDatabaseWriter(const boost::shared_ptr<Database<T>> &pDB) : DatabaseWriter(pDB) {}
    };
}
#endif