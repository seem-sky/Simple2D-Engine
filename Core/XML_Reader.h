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
    /*####
    # XML_Data
    ####*/
    class XML_ReadData;
    typedef std::multimap<std::string, XML_ReadData> ReadChildList;
    typedef std::map<std::string, CComVariant> AttributeMap;

    class XML_Data
    {
    public:
        virtual bool HasChilds() const = 0; 
        virtual bool HasChild(std::string p_sName) const = 0; 

        inline bool HasAttributes() const { return !m_AttributeList.empty(); }
        bool HasAttribute(std::string p_sName) const;
        bool GetAttributeValue(std::string p_sName, CComVariant &p_Value) const;
        inline void SetAttributeList(AttributeMap &p_AttrList) { m_AttributeList = p_AttrList; }
        //inline AttributeList* GetAttributeList() { return &m_AttributeList; }
        const inline AttributeMap* GetAttributeList() const { return &m_AttributeList; }

    protected:
        AttributeMap m_AttributeList;
    };

    /*####
    # XML_ReadData
    ####*/
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

    /*####
    # CoObject
    ####*/
    class CoObject
    {
    public:
        ~CoObject();

        bool InitCo(LPVOID p_pReserved = NULL);
    };

    /*####
    # XML_Reader
    ####*/
    enum XMLState
    {
        XML_NONE,
        XML_DONE,
        XML_FAILED,
        XML_IN_PROGRESS,
    };

    class XML_Reader
    {
    public:
        XML_Reader(std::string p_sFileName);

        bool ReadFile();
        inline ReadChildList& GetXMLData() { return m_ChildList; }
        inline void ClearChildList() { m_ChildList.clear(); }

    protected:
        HRESULT CheckoutChildren(IXMLDOMNodePtr p_pNode, ReadChildList &p_DestList);
        HRESULT CheckoutAttributes(IXMLDOMNodePtr p_pNode, AttributeMap &p_DestList);

    private:
        XMLState m_XMLState;
        std::string m_sFileName;
        std::string m_sLogLocationName;

        ReadChildList m_ChildList;
    };
}
#endif