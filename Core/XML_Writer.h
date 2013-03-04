#ifndef XML_WRITER_H
#define XML_WRITER_H

#include "XML_Reader.h"
#include <msxml.h>

#import <msxml6.dll>

namespace XML
{
    enum XMLWriteState
    {
        XML_WRITE_NONE,
        XML_WRITE_ADD,
        XML_WRITE_DELETE,
        XML_WRITE_CHANGE,
        XML_WRITE_APPEND,
        XML_WRITE_OVERWRITE,
    };

    class XML_WriteData;
    typedef std::multimap<std::string, XML_WriteData> XMLWriteChildMultimap;

    class XML_WriteData : public XML_Data
    {
    public:
        XML_WriteData(XMLWriteState writeState = XML_WRITE_NONE) : m_WriteState(writeState), XML_Data() {}

        inline XMLWriteState GetWriteState() const { return m_WriteState; }
        inline void SetWriteState(XMLWriteState state) { m_WriteState = state; }

        void AddChild(const std::string &sName, const XML_WriteData &newChild);
        bool HasChilds() const { return !m_ChildList.empty(); }
        bool HasChild(std::string sName) const;
        XML_WriteData* GetChild(std::string sName);
        const XML_WriteData* GetChild(std::string sName) const;
        const inline XMLWriteChildMultimap* GetChildList() const { return &m_ChildList; }
        inline void SetChildList(XMLWriteChildMultimap &childList) { m_ChildList = childList; }

        void AddAttribute(const std::string &sName, const CComVariant &newAttribute);

    private:
        XMLWriteState m_WriteState;
        XMLWriteChildMultimap m_ChildList;
    };

    class XML_Writer
    {
    private:
        IXMLDOMNodePtr _getChildNodeByAttribute(IXMLDOMNodePtr pNode, std::string sNodeName, std::string sAttributeName, CComVariant value);
        IXMLDOMNodePtr _addChildNode(MSXML2::IXMLDOMDocument2Ptr pDOM, MSXML2::IXMLDOMElementPtr pParent, std::string sNodeName, const AttributeMap *plAttributes);
        void _addChildNodes(MSXML2::IXMLDOMDocument2Ptr pDOM, IXMLDOMNodePtr pParent, const XMLWriteChildMultimap *plChildren);
        void _changeNode(IXMLDOMNodePtr pElement, const AttributeMap *plAttributes);

    public:
        XML_Writer();
        bool startWriting(const std::string &sFileName, XML_WriteData &data);

    private:
        std::string m_sLogLocationName;
    };
}
#endif