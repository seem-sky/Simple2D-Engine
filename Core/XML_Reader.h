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
    class XML_ReadData;
    typedef std::multimap<std::string, XML_ReadData> ReadChildList;
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

    class XML_Data
    {
    public:
        virtual bool HasChilds() const = 0; 
        virtual bool HasChild(std::string p_sName) const = 0; 

        inline bool HasAttributes() const { return !m_AttributeList.empty(); }
        bool HasAttribute(std::string p_sName) const;
        bool GetAttributeValue(std::string p_sName, CComVariant &p_Value) const;
        inline void SetAttributeList(AttributeList &p_AttrList) { m_AttributeList = p_AttrList; }
        //inline AttributeList* GetAttributeList() { return &m_AttributeList; }
        const inline AttributeList* GetAttributeList() const { return &m_AttributeList; }

    protected:
        AttributeList m_AttributeList;
    };

    class XML_ReadData : public XML_Data
    {
    public:
        bool HasChilds() const { return !m_ChildList.empty(); }
        bool HasChild(std::string p_sName) const;
        XML_ReadData* GetChild(std::string p_sName);
        const XML_ReadData* GetChild(std::string p_sName) const;
        const inline ReadChildList* GetChildList() const { return &m_ChildList; }
        inline void SetChildList(ReadChildList &p_ChildList) { m_ChildList = p_ChildList; }

    private:
        ReadChildList m_ChildList;
    };

    class XML_Reader : public ActiveObject
    {
    public:
        XML_Reader(std::string sData);
        virtual ~XML_Reader(void);

        void Run();
        inline XML_STATE GetReaderState() { return m_XMLState; }
        inline const ReadChildList* GetXMLData() { return &m_ChildList; }
        inline void ClearChildList() { m_ChildList.clear(); }

    protected:
        HRESULT CheckoutChildren(IXMLDOMNodePtr p_pNode, ReadChildList &p_DestList);
        HRESULT CheckoutAttributes(IXMLDOMNodePtr p_pNode, AttributeList &p_DestList);

    private:
        XML_STATE m_XMLState;
        std::string m_sData;
        std::string m_sLogLocationName;

        ReadChildList m_ChildList;
    };
}
#endif