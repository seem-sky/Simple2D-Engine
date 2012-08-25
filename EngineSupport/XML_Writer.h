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

    struct XML_WriteData;
    typedef std::multimap<std::string, XML_WriteData> WriteChildList;

    struct XML_WriteData : XML_Data
    {
        XML_WriteData(XML_WRITE_STATE p_WriterState = XML_WRITE_NONE) : m_WriteState(p_WriterState), XML_Data() {}

        inline XML_WRITE_STATE GetWriteState() { return m_WriteState; }
        inline void SetWriteState(XML_WRITE_STATE p_State) { m_WriteState = p_State; }

        virtual bool HasChilds() { return !m_ChildList.empty(); }
        virtual bool HasChild(std::string p_sName);
        virtual XML_Data* GetChild(std::string p_sName);

        XML_WRITE_STATE m_WriteState;
        WriteChildList m_ChildList;
    };

    class XML_Writer : public ActiveObject
    {
    public:
        XML_Writer(std::string p_sData, XML_WriteData *p_tData);
        ~XML_Writer(void);

        void Run();
        XML_STATE GetWriterState() { return m_XMLState; }
        IXMLDOMNodePtr GetChildNodeByAttribute(IXMLDOMNodePtr p_pNode, std::string p_sNodeName, std::string p_sAttributeName, CComVariant p_value);

        void AddChildNode(MSXML2::IXMLDOMDocument2Ptr p_pDOM, MSXML2::IXMLDOMElementPtr p_pParent, std::string p_sNodeName, AttributeList *p_plAttributes);
        void AddChildNodes(MSXML2::IXMLDOMDocument2Ptr p_pDOM, IXMLDOMNodePtr p_pParent, WriteChildList *p_plChildren);
        void ChangeNode(IXMLDOMNodePtr p_pElement, AttributeList *p_plAttributes);

    private:
        XML_STATE m_XMLState;
        std::string m_sFileName;
        XML_WriteData *m_pData;

        std::string m_sLogLocationName;
    };
}
#endif