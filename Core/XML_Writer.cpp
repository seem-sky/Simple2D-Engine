#include "XML_Writer.h"
#include "Logfile.h"
#include <iostream>

#define returnRun CoUninitialize(); return

using namespace XML;

XML_Writer::XML_Writer() : m_sLogLocationName(LOGFILE_ENGINE_LOG_NAME + "XML_Writer : ")
{ }

bool XML_Writer::startWriting(const std::string &sFileName, XML_WriteData &data)
{
    if (FAILED(CoInitialize(NULL)))
    {
        returnRun false;
    }

    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    if (FAILED(pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
    {
        ERROR_LOG(m_sLogLocationName + "Unable to create DOM instance.");
        returnRun false;
    }

    pXMLDom->load(_variant_t(sFileName.c_str()));
    _addChildNodes(pXMLDom, pXMLDom, data.GetChildList());
    if (SUCCEEDED(pXMLDom->save(_variant_t(sFileName.c_str()))))
    {
        returnRun true;
    }

    returnRun false;
}

IXMLDOMNodePtr XML_Writer::_getChildNodeByAttribute(IXMLDOMNodePtr p_pNode, std::string p_sNodeName, std::string sAttributeName, CComVariant p_value)
{
    IXMLDOMNodeListPtr pChildren;
    p_pNode->selectNodes(_bstr_t(p_sNodeName.c_str()), &pChildren);
    LONG length = 0;
    pChildren->get_length(&length);
    // if no nodes in list, return
    if (!length)
        return NULL;

    p_value.ChangeType(VT_BSTR);
    IXMLDOMNodePtr pTempNode;
    BSTR sNodeName;
    // check children
    for (LONG i = 0;i < length ; ++i)
    {
        pChildren->get_item(i, &pTempNode);
        IXMLDOMNamedNodeMapPtr pAttributeList;
        pTempNode->get_attributes(&pAttributeList);
        LONG attributeLength = 0;
        pAttributeList->get_length(&attributeLength);
        // check attributes
        for (LONG j = 0; j < attributeLength; ++j)
        {
            IXMLDOMNodePtr pAttribute;
            pAttributeList->get_item(j, &pAttribute);
            pAttribute->get_nodeName(&sNodeName);
            std::string sCheckString = bstr_t(sNodeName);
            if (sAttributeName == sCheckString)
            {
                CComVariant t_ValueCheck;
                pAttribute->get_nodeValue(&t_ValueCheck);
                if (bstr_t(p_value.bstrVal) == bstr_t(t_ValueCheck.bstrVal))
                    return pTempNode;
                else
                    break;
            }
        }
    }

    return NULL;
}

IXMLDOMNodePtr XML_Writer::_addChildNode(MSXML2::IXMLDOMDocument2Ptr p_pDOM, MSXML2::IXMLDOMElementPtr p_pParent, std::string p_sNodeName, const AttributeMap *p_plAttributes)
{
    if (!p_pDOM || !p_pParent)
        return NULL;

    MSXML2::IXMLDOMElementPtr t_Node(p_pDOM->createNode(NODE_ELEMENT, p_sNodeName.c_str(), ""));

    // add attributes
    if (p_plAttributes)
    {
        MSXML2::IXMLDOMAttributePtr t_Attribute;
        for (AttributeMap::const_iterator t_Itr = p_plAttributes->begin(); t_Itr != p_plAttributes->end(); ++t_Itr)
        {
            t_Attribute = p_pDOM->createAttribute(t_Itr->first.c_str());
            t_Attribute->value = t_Itr->second;
            t_Node->Getattributes()->setNamedItem(t_Attribute);
        }
    }

    return p_pParent->appendChild(t_Node);
}

void XML_Writer::_addChildNodes(MSXML2::IXMLDOMDocument2Ptr p_pDOM, IXMLDOMNodePtr p_pParent, const XMLWriteChildMultimap *p_plChildren)
{
    if (!p_pDOM || !p_pParent || !p_plChildren)
        return;

    for (XMLWriteChildMultimap::const_iterator t_Itr = p_plChildren->begin(); t_Itr != p_plChildren->end(); ++t_Itr)
    {
        IXMLDOMNodePtr t_pNewNode = NULL;
        if (t_Itr->second.GetWriteState() != XML_WRITE_APPEND)
        {
            // if node has no attribute named "ID" its a single node
            CComVariant t_Value;
            if (t_Itr->second.GetAttributeValue("ID", t_Value))
            {
                if (FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;

                t_pNewNode = _getChildNodeByAttribute(p_pParent, t_Itr->first, "ID", t_Value);
            }
            else
                p_pParent->selectSingleNode(_bstr_t(t_Itr->first.c_str()), &t_pNewNode);
        }

        switch (t_Itr->second.GetWriteState())
        {
        case XML_WRITE_DELETE:
            if (t_pNewNode)
            {
                IXMLDOMNodePtr t_pRemovedChild = NULL;
                p_pParent->removeChild(t_pNewNode, &t_pRemovedChild);
            }

            // deleted nodes can not have child nodes, so do continue with next node
        	continue;

            // delete all child nodes first
        case XML_WRITE_OVERWRITE:
            if (t_pNewNode)
            {
                IXMLDOMNodePtr t_pRemovedChild = NULL;
                p_pParent->removeChild(t_pNewNode, &t_pRemovedChild);
                t_pNewNode = NULL;
            }

        case XML_WRITE_APPEND:
        case XML_WRITE_CHANGE:
        case XML_WRITE_ADD:
            if (t_pNewNode)
                _changeNode(t_pNewNode, t_Itr->second.GetAttributeList());

            else
                t_pNewNode = _addChildNode(p_pDOM, p_pParent, t_Itr->first, t_Itr->second.GetAttributeList());
            break;

        case XML_WRITE_NONE:
        default:
            break;
        }

        if (t_Itr->second.GetChildList() && t_Itr->second.GetChildList()->size())
            _addChildNodes(p_pDOM, t_pNewNode, t_Itr->second.GetChildList());
    }
}

void XML_Writer::_changeNode(IXMLDOMNodePtr p_pNode, const AttributeMap *p_plAttributes)
{
    if (!p_pNode || !p_plAttributes)
        return;

    IXMLDOMElementPtr p_pElement = p_pNode;

    for (AttributeMap::const_iterator t_Itr = p_plAttributes->begin(); t_Itr != p_plAttributes->end(); ++t_Itr)
        p_pElement->setAttribute(_bstr_t(t_Itr->first.c_str()), t_Itr->second);
}

bool XML_WriteData::HasChild(std::string p_sName) const
{
    XMLWriteChildMultimap::const_iterator t_Itr = m_ChildList.find(p_sName);
    return t_Itr != m_ChildList.end() ? true : false;
}

XML_WriteData* XML_WriteData::GetChild(std::string p_sName)
{
    XMLWriteChildMultimap::iterator t_Itr = m_ChildList.find(p_sName);
    return t_Itr != m_ChildList.end() ? &t_Itr->second : NULL;
}

const XML_WriteData* XML_WriteData::GetChild(std::string p_sName) const
{
    XMLWriteChildMultimap::const_iterator t_Itr = m_ChildList.find(p_sName);
    return t_Itr != m_ChildList.end() ? &t_Itr->second : NULL;
}

void XML_WriteData::AddChild(const std::string &p_sName, const XML::XML_WriteData &p_NewChild)
{
    m_ChildList.insert(std::make_pair(p_sName, p_NewChild));
}

void XML_WriteData::AddAttribute(const std::string &p_sName, const CComVariant &p_NewAttribute)
{
    m_AttributeList.insert(std::make_pair(p_sName, p_NewAttribute));
}