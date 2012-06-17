#include "XML_Reader.h"
#include "Logfile.h"
#include <fstream>

#import <msxml4.dll>

#define FAILED_CHECK if (p_hr == S_FALSE) { m_XMLState = XML_FAILED; return NULL; }

XML_Reader::XML_Reader(std::string sData) : m_XMLState(XML_IN_PROGRESS), m_sData(sData), m_sLogLocationName(LOGFILE_ENGINE_LOG_NAME + "XML_Reader : "),
    m_pChildList(NULL), ActiveObject()
{
    _thread.Resume();
}

XML_Reader::~XML_Reader(void)
{
    delete m_pChildList;
    CoUninitialize();
}

void XML_Reader::Run()
{
    if (FAILED(CoInitialize(NULL)))
        ERROR_LOG(m_sLogLocationName + "Unable to initialize COM.");

    // this parse the XML file:
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    HRESULT p_hr = pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument40));
    if (p_hr == S_FALSE)
    {
        m_XMLState = XML_FAILED;
        return;
    }

    // interpret string
    pXMLDom->loadXML(m_sData.c_str());

    m_pChildList = CheckoutChilds(pXMLDom, p_hr);
    if (p_hr == S_FALSE)
    {
        m_XMLState = XML_FAILED;
        return;
    }

    m_XMLState = XML_DONE;
}

ChildList* XML_Reader::CheckoutChilds(IXMLDOMNodePtr p_pNode, HRESULT &p_hr)
{
    if (!p_pNode)
        return NULL;

    ChildList *t_pNewData = new ChildList();

    // get all childs
    IXMLDOMNodeListPtr t_pChildList;
    p_hr = p_pNode->get_childNodes(&t_pChildList);
    FAILED_CHECK

    // get child count
    LONG iChilds = 0;
    p_hr = t_pChildList->get_length(&iChilds);
    FAILED_CHECK
    IXMLDOMNodePtr t_pTempNode;
    XML_Data t_NewChild;
    BSTR sNodeName;
    std::string sName;
    for (LONG i = 0; i < iChilds; ++i)
    {
        p_hr = t_pChildList->get_item(i, &t_pTempNode);
        FAILED_CHECK
        // get childs
        ChildList *t_pTempData = CheckoutChilds(t_pTempNode, p_hr);
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