#include "XML_Reader.h"
#include "Logfile.h"
#include <fstream>

namespace XML
{
    #define FAILED_CHECK if (p_hr == S_FALSE) { m_XMLState = XML_FAILED; return NULL; }
    #define returnRun CoUninitialize(); return;

    XML_Reader::XML_Reader(std::string sData) : m_XMLState(XML_IN_PROGRESS), m_sData(sData), m_sLogLocationName(LOGFILE_ENGINE_LOG_NAME + "XML_Reader : "),
        m_pChildList(NULL), ActiveObject()
    {
        _thread.Resume();
    }

    XML_Reader::~XML_Reader(void)
    {
        delete m_pChildList;
    }

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

        HRESULT p_hr;
        m_pChildList = CheckoutChildren(t_pXMLDom, p_hr);
        if (FAILED(p_hr))
        {
            m_XMLState = XML_FAILED;
            returnRun;
        }

        m_XMLState = XML_DONE;

        returnRun;
    }

    ChildList* XML_Reader::CheckoutChildren(IXMLDOMNodePtr p_pNode, HRESULT &p_hr)
    {
        if (!p_pNode)
            return NULL;

        ChildList *t_pNewData = new ChildList();

        // get all childs
        IXMLDOMNodeListPtr t_pChildList;
        p_hr = p_pNode->get_childNodes(&t_pChildList);
        FAILED_CHECK

            // get child count
            LONG t_ChildrenCount = 0;
        p_hr = t_pChildList->get_length(&t_ChildrenCount);
        FAILED_CHECK
            IXMLDOMNodePtr t_pTempNode;
        XML_Data t_NewChild;
        BSTR sNodeName;
        std::string sName;
        for (LONG i = 0; i < t_ChildrenCount; ++i)
        {
            p_hr = t_pChildList->get_item(i, &t_pTempNode);
            FAILED_CHECK
                // get children
                ChildList *t_pTempData = CheckoutChildren(t_pTempNode, p_hr);
            FAILED_CHECK
                if (t_pTempData)
                    t_NewChild.m_ChildList = *t_pTempData;

            delete t_pTempData;

            // get attributes
            AttributeList *t_pTempAttributes = CheckoutAttributes(t_pTempNode, p_hr);
            FAILED_CHECK
                if (t_pTempData)
                    t_NewChild.m_AttributeList = *t_pTempAttributes;

            delete t_pTempAttributes;

            // insert child in data map
            p_hr = t_pTempNode->get_nodeName(&sNodeName);
            FAILED_CHECK
                sName = _bstr_t(sNodeName);
            t_pNewData->insert(std::make_pair(sName, t_NewChild));
        }

        return t_pNewData;
    }

    AttributeList* XML_Reader::CheckoutAttributes(IXMLDOMNodePtr p_pNode, HRESULT &p_hr)
    {
        if (!p_pNode)
            return NULL;

        AttributeList *t_pNewData = new AttributeList();

        // get all attributes
        IXMLDOMNamedNodeMapPtr t_pAttributeList;
        p_hr = p_pNode->get_attributes(&t_pAttributeList);
        FAILED_CHECK

            // get attribute count
            LONG iAttributes = 0;
        p_hr = t_pAttributeList->get_length(&iAttributes);
        FAILED_CHECK
            IXMLDOMNodePtr t_pTempNode;
        VARIANT value;
        VariantInit(&value);
        BSTR sNodeName;
        std::string sName;
        for (LONG i = 0; i < iAttributes; ++i)
        {
            p_hr = t_pAttributeList->get_item(i, &t_pTempNode);
            FAILED_CHECK
                p_hr = t_pTempNode->get_nodeValue(&value);
            FAILED_CHECK
                p_hr = t_pTempNode->get_nodeName(&sNodeName);
            FAILED_CHECK
                sName = _bstr_t(sNodeName);
            t_pNewData->insert(std::make_pair(sName, value));
        }

        return t_pNewData;
    }

    bool XML_Data::GetAttributeValue(std::string p_sName, VARIANT &p_Value)
    {
        AttributeList::iterator t_Itr = m_AttributeList.find(p_sName);
        if (t_Itr != m_AttributeList.end())
        {
            p_Value = t_Itr->second;
            return true;
        }

        return false;
    }
}