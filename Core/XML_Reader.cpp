#include "XML_Reader.h"
#include "Logfile.h"
#include <fstream>

using namespace XML;

/*####
# CoObject
####*/
CoObject::~CoObject()
{
    CoUninitialize();
}

bool CoObject::InitCo(LPVOID pReserved)
{
    if (FAILED(CoInitialize(pReserved)))
        return false;

    return true;
}

/*####
# XML_Reader
####*/
XML_Reader::XML_Reader(std::string sFileName) :  m_sLogLocationName(LOGFILE_ENGINE_LOG_NAME + "XML_Reader : "), m_sFileName(sFileName),
    m_XMLState(XML_NONE)
{

}

bool XML_Reader::ReadFile()
{
    if (m_sFileName.empty())
        return false;

    CoObject coObj;
    coObj.InitCo();

    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    if (FAILED(pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
    {
        ERROR_LOG(m_sLogLocationName + "Unable to create DOM instance.");
        m_XMLState = XML_FAILED;
        return false;
    }

    if (!pXMLDom->load(_variant_t(m_sFileName.c_str())))
    {
        ERROR_LOG(m_sLogLocationName + "Unable to load XML file.");
        m_XMLState = XML_FAILED;
        return false;
    }

    HRESULT t_HR = CheckoutChildren(pXMLDom, m_ChildList);
    if (FAILED(t_HR))
    {
        m_XMLState = XML_FAILED;
        return false;
    }

    m_XMLState = XML_DONE;
    return true;
}

HRESULT XML_Reader::CheckoutChildren(IXMLDOMNodePtr p_pNode, ReadChildList &p_DestList)
{
    p_DestList.clear();
    if (!p_pNode)
        return -1;

    // get all childs
    IXMLDOMNodeListPtr t_pChildList;
    HRESULT t_HR = p_pNode->get_childNodes(&t_pChildList);
    if (FAILED(t_HR))
        return t_HR;

    // get child count
    LONG t_ChildrenCount = 0;
    t_HR = t_pChildList->get_length(&t_ChildrenCount);
    if (FAILED(t_HR))
        return t_HR;

    IXMLDOMNodePtr t_pTempNode;
    XML_ReadData t_NewChild;
    BSTR sNodeName;
    std::string sName;
    for (LONG i = 0; i < t_ChildrenCount; ++i)
    {
        t_HR = t_pChildList->get_item(i, &t_pTempNode);
        if (FAILED(t_HR))
            return t_HR;

        // get children
        ReadChildList t_TempData;
        t_HR = CheckoutChildren(t_pTempNode, t_TempData);
        if (FAILED(t_HR))
            return t_HR;
        t_NewChild.SetChildList(t_TempData);

        // get attributes
        AttributeMap t_TempAttributes;
        t_HR = CheckoutAttributes(t_pTempNode, t_TempAttributes);
        if (FAILED(t_HR))
            return t_HR;
        t_NewChild.SetAttributeList(t_TempAttributes);

        // insert child in data map
        t_HR = t_pTempNode->get_nodeName(&sNodeName);
        if (FAILED(t_HR))
            return t_HR;
        sName = _bstr_t(sNodeName);
        p_DestList.insert(std::make_pair(sName, t_NewChild));
    }

    return t_HR;
}

HRESULT XML_Reader::CheckoutAttributes(IXMLDOMNodePtr p_pNode, AttributeMap &p_DestList)
{
    p_DestList.clear();

    if (!p_pNode)
        return -1;

    // get all attributes
    IXMLDOMNamedNodeMapPtr t_pAttributeList;
    HRESULT t_HR = p_pNode->get_attributes(&t_pAttributeList);
    if (FAILED(t_HR))
        return t_HR;

    // get attribute count
    LONG iAttributes = 0;
    t_HR = t_pAttributeList->get_length(&iAttributes);
    if (FAILED(t_HR))
        return t_HR;

    IXMLDOMNodePtr t_pTempNode;
    CComVariant t_Value;
    BSTR sNodeName;
    std::string sName;
    for (LONG i = 0; i < iAttributes; ++i)
    {
        t_HR = t_pAttributeList->get_item(i, &t_pTempNode);
        if (FAILED(t_HR))
            return t_HR;

        t_HR = t_pTempNode->get_nodeValue(&t_Value);
        if (FAILED(t_HR))
            return t_HR;

        t_HR = t_pTempNode->get_nodeName(&sNodeName);
        if (FAILED(t_HR))
            return t_HR;

        sName = _bstr_t(sNodeName);
        p_DestList.insert(std::make_pair(sName, t_Value));
    }

    return t_HR;
}

/*####
# XML_Data
####*/
bool XML_Data::GetAttributeValue(std::string p_sName, CComVariant &p_Value) const
{
    AttributeMap::const_iterator t_Itr = m_AttributeList.find(p_sName);
    if (t_Itr != m_AttributeList.end())
    {
        p_Value = t_Itr->second;
        return true;
    }

    return false;
}

bool XML_Data::HasAttribute(std::string p_sName) const
{
    AttributeMap::const_iterator t_Itr = m_AttributeList.find(p_sName);
    return t_Itr != m_AttributeList.end() ? true : false;
}

/*####
# XML_ReadData
####*/
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