#include "XML_Reader.h"
#include "Logfile.h"
#include <fstream>

namespace XML
{
    #define FAILED_CHECK if (FAILED(t_HR)) { m_XMLState = XML_FAILED; return t_HR; }
    
    XML_Reader::XML_Reader(std::string sData) : m_XMLState(XML_IN_PROGRESS), m_sData(sData), m_sLogLocationName(LOGFILE_ENGINE_LOG_NAME + "XML_Reader : "),
        ActiveObject()
    {
        _thread.Resume();
    }

    XML_Reader::~XML_Reader(void)
    {
    }

    #define returnRun CoUninitialize(); return;
    void XML_Reader::Run()
    {
        if (FAILED(CoInitialize(NULL)))
        {
            ERROR_LOG(m_sLogLocationName + "Unable to initialize COM.");
            m_XMLState = XML_FAILED;
            returnRun;
        }

        MSXML2::IXMLDOMDocument2Ptr t_pXMLDom = NULL;
        if (FAILED(t_pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
        {
            ERROR_LOG(m_sLogLocationName + "Unable to create DOM instance.");
            m_XMLState = XML_FAILED;
            returnRun;
        }

        if (!t_pXMLDom->loadXML(_bstr_t(m_sData.c_str())))
        {
            ERROR_LOG(m_sLogLocationName + "Unable to load XML string.");
            m_XMLState = XML_FAILED;
            returnRun;
        }

        HRESULT t_HR = CheckoutChildren(t_pXMLDom, m_ChildList);
        if (FAILED(t_HR))
        {
            m_XMLState = XML_FAILED;
            returnRun;
        }

        m_XMLState = XML_DONE;

        returnRun;
    }

    HRESULT XML_Reader::CheckoutChildren(IXMLDOMNodePtr p_pNode, ReadChildList &p_DestList)
    {
        p_DestList.clear();
        if (!p_pNode)
            return NULL;

        // get all childs
        IXMLDOMNodeListPtr t_pChildList;
        HRESULT t_HR = p_pNode->get_childNodes(&t_pChildList);
        FAILED_CHECK

        // get child count
        LONG t_ChildrenCount = 0;
        t_HR = t_pChildList->get_length(&t_ChildrenCount);
        FAILED_CHECK

        IXMLDOMNodePtr t_pTempNode;
        XML_ReadData t_NewChild;
        BSTR sNodeName;
        std::string sName;
        for (LONG i = 0; i < t_ChildrenCount; ++i)
        {
            t_HR = t_pChildList->get_item(i, &t_pTempNode);
            FAILED_CHECK

            // get children
            ReadChildList t_TempData;
            t_HR = CheckoutChildren(t_pTempNode, t_TempData);
            FAILED_CHECK

            t_NewChild.SetChildList(t_TempData);

            // get attributes
            AttributeList t_TempAttributes;
            t_HR = CheckoutAttributes(t_pTempNode, t_TempAttributes);
            FAILED_CHECK

            t_NewChild.SetAttributeList(t_TempAttributes);

            // insert child in data map
            t_HR = t_pTempNode->get_nodeName(&sNodeName);
            FAILED_CHECK

            sName = _bstr_t(sNodeName);
            p_DestList.insert(std::make_pair(sName, t_NewChild));
        }

        return t_HR;
    }

    HRESULT XML_Reader::CheckoutAttributes(IXMLDOMNodePtr p_pNode, AttributeList &p_DestList)
    {
        p_DestList.clear();

        if (!p_pNode)
            return NULL;

        // get all attributes
        IXMLDOMNamedNodeMapPtr t_pAttributeList;
        HRESULT t_HR = p_pNode->get_attributes(&t_pAttributeList);
        FAILED_CHECK

        // get attribute count
        LONG iAttributes = 0;
        t_HR = t_pAttributeList->get_length(&iAttributes);
        FAILED_CHECK

        IXMLDOMNodePtr t_pTempNode;
        CComVariant t_Value;
        BSTR sNodeName;
        std::string sName;
        for (LONG i = 0; i < iAttributes; ++i)
        {
            t_HR = t_pAttributeList->get_item(i, &t_pTempNode);
            FAILED_CHECK

            t_HR = t_pTempNode->get_nodeValue(&t_Value);
            FAILED_CHECK

            t_HR = t_pTempNode->get_nodeName(&sNodeName);
            FAILED_CHECK

            sName = _bstr_t(sNodeName);
            p_DestList.insert(std::make_pair(sName, t_Value));
        }

        return t_HR;
    }

    bool XML_Data::GetAttributeValue(std::string p_sName, CComVariant &p_Value) const
    {
        AttributeList::const_iterator t_Itr = m_AttributeList.find(p_sName);
        if (t_Itr != m_AttributeList.end())
        {
            p_Value = t_Itr->second;
            return true;
        }

        return false;
    }

    bool XML_Data::HasAttribute(std::string p_sName) const
    {
        AttributeList::const_iterator t_Itr = m_AttributeList.find(p_sName);
        return t_Itr != m_AttributeList.end() ? true : false;
    }

    bool XML_ReadData::HasChild(std::string p_sName) const
    {
        ReadChildList::const_iterator t_Itr = m_ChildList.find(p_sName);
        return t_Itr != m_ChildList.end() ? true : false;
    }

    XML_ReadData* XML_ReadData::GetChild(std::string p_sName)
    {
        ReadChildList::iterator t_Itr = m_ChildList.find(p_sName);
        return t_Itr != m_ChildList.end() ? &t_Itr->second : NULL;
    }

    const XML_ReadData* XML_ReadData::GetChild(std::string p_sName) const
    {
        ReadChildList::const_iterator t_Itr = m_ChildList.find(p_sName);
        return t_Itr != m_ChildList.end() ? &t_Itr->second : NULL;
    }
}