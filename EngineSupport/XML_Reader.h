#ifndef XML_READER_H
#define XML_READER_H

#include "Thread.h"
#include <string>
#include <map>
#include <atlcomcli.h>
#include <atlbase.h>

#include <msxml.h>
#import <msxml6.dll>

namespace XML
{
    struct XML_Data;
    typedef std::multimap<std::string, XML_Data> ChildList;
    typedef std::map<std::string, CComVariant> AttributeList;

    enum XML_STATE
    {
        XML_NONE,
        XML_FAILED,
        XML_NO_FILE,
        XML_CORRUPT_FILE,
        XML_IN_PROGRESS,
        XML_DONE,
    };

    struct XML_Data
    {
        ChildList m_ChildList;
        AttributeList m_AttributeList;

        inline bool HasChilds() { return !m_ChildList.empty(); }
        inline bool HasAttributes() { return !m_AttributeList.empty(); }
        bool GetAttributeValue(std::string p_sName, VARIANT &p_Value);
    };

    class XML_Reader : public ActiveObject
    {
    public:
        XML_Reader(std::string sData);
        virtual ~XML_Reader(void);

        void Run();
        inline XML_STATE GetReaderState() { return m_XMLState; }
        inline ChildList* GetXMLData() { return m_pChildList; }
        inline void ClearChildList() { m_pChildList->clear(); }

    protected:
        ChildList* CheckoutChildren(IXMLDOMNodePtr p_pNode, HRESULT &p_hr);
        AttributeList* CheckoutAttributes(IXMLDOMNodePtr p_pNode, HRESULT &p_hr);

    private:
        XML_STATE m_XMLState;
        std::string m_sData;
        std::string m_sLogLocationName;

        ChildList *m_pChildList;
    };
}
#endif