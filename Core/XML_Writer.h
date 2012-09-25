#ifndef XML_WRITER_H
#define XML_WRITER_H

#include "XML_Reader.h"
#include <msxml.h>

#import <msxml6.dll>

namespace XML
{
    enum XML_WRITE_STATE
    {
        XML_WRITE_NONE,
        XML_WRITE_ADD,
        XML_WRITE_DELETE,
        XML_WRITE_CHANGE,
    };

    class XML_WriteData;
    typedef std::multimap<std::string, XML_WriteData> WriteChildList;

    class XML_WriteData : public XML_Data
    {
    public:
        XML_WriteData(XML_WRITE_STATE p_WriterState = XML_WRITE_NONE) : m_WriteState(p_WriterState), XML_Data() {}

        inline XML_WRITE_STATE GetWriteState() const { return m_WriteState; }
        inline void SetWriteState(XML_WRITE_STATE p_State) { m_WriteState = p_State; }

        void AddChild(std::string p_sName, XML_WriteData p_NewChild);
        bool HasChilds() const { return !m_ChildList.empty(); }
        bool HasChild(std::string p_sName) const;
        XML_WriteData* GetChild(std::string p_sName);
        const XML_WriteData* GetChild(std::string p_sName) const;
        const inline WriteChildList* GetChildList() const { return &m_ChildList; }
        inline void SetChildList(WriteChildList &p_ChildList) { m_ChildList = p_ChildList; }

        void AddAttribute(std::string p_sName, CComVariant p_NewAttribute);

    private:
        XML_WRITE_STATE m_WriteState;
        WriteChildList m_ChildList;
    };

    class XML_Writer : public ActiveObject
    {
    public:
        XML_Writer(std::string p_sData, XML_WriteData *p_tData);
        ~XML_Writer(void);

        void Run();
        XML_STATE GetWriterState() const { return m_XMLState; }
        IXMLDOMNodePtr GetChildNodeByAttribute(IXMLDOMNodePtr p_pNode, std::string p_sNodeName, std::string p_sAttributeName, CComVariant p_value);

        void AddChildNode(MSXML2::IXMLDOMDocument2Ptr p_pDOM, MSXML2::IXMLDOMElementPtr p_pParent, std::string p_sNodeName, const AttributeList *p_plAttributes);
        void AddChildNodes(MSXML2::IXMLDOMDocument2Ptr p_pDOM, IXMLDOMNodePtr p_pParent, const WriteChildList *p_plChildren);
        void ChangeNode(IXMLDOMNodePtr p_pElement, const AttributeList *p_plAttributes);

    private:
        XML_STATE m_XMLState;
        std::string m_sFileName;
        XML_WriteData *m_pData;

        std::string m_sLogLocationName;
    };
}
#endif