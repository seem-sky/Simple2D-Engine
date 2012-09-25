#include "XML_Writer.h"
#include "Logfile.h"

namespace XML
{
    #define returnRun CoUninitialize(); return;

    XML_Writer::XML_Writer(std::string p_sFileName, XML_WriteData *p_pData) : m_XMLState(XML_NONE), m_sFileName(p_sFileName), m_pData(p_pData), m_sLogLocationName(LOGFILE_ENGINE_LOG_NAME + "XML_Writer : "),
        ActiveObject()
    {
        _thread.Resume();
    }

    XML_Writer::~XML_Writer(void)
    {
        if (m_pData)
        {
            delete m_pData;
            m_pData = NULL;
        }
    }

    void XML_Writer::Run()
    {
        if (!m_pData)
        {
            m_XMLState = XML_FAILED;
            return;
        }

        m_XMLState = XML_IN_PROGRESS;
        if (FAILED(CoInitialize(NULL)))
        {
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

        if (!t_pXMLDom->load(m_sFileName.c_str()))
        {
            ERROR_LOG(m_sLogLocationName + "Unable to load " + m_sFileName + ".");
            m_XMLState = XML_FAILED;
            returnRun;
        }

        AddChildNodes(t_pXMLDom, t_pXMLDom, m_pData->GetChildList());
        _variant_t t_Path = m_sFileName.c_str();
        t_pXMLDom->save(t_Path);
        m_XMLState = XML_DONE;
        returnRun;
    }

    IXMLDOMNodePtr XML_Writer::GetChildNodeByAttribute(IXMLDOMNodePtr p_pNode, std::string p_sNodeName, std::string p_sAttributeName, CComVariant p_value)
    {
        IXMLDOMNodeListPtr t_pChildren;
        p_pNode->selectNodes(_bstr_t(p_sNodeName.c_str()), &t_pChildren);
        LONG t_lLength = 0;
        t_pChildren->get_length(&t_lLength);
        // if no nodes in list, return
        if (!t_lLength)
            return NULL;

        p_value.ChangeType(VT_BSTR);
        IXMLDOMNodePtr t_pTempNode;
        BSTR t_sNodeName;
        // check children
        for (LONG t_i = 0;t_i < t_lLength ; ++t_i)
        {
            t_pChildren->get_item(t_i, &t_pTempNode);
            IXMLDOMNamedNodeMapPtr t_pAttributeList;
            t_pTempNode->get_attributes(&t_pAttributeList);
            LONG t_lAttributeLength = 0;
            t_pAttributeList->get_length(&t_lAttributeLength);
            // check attributes
            for (LONG t_j = 0; t_j < t_lAttributeLength; ++t_j)
            {
                IXMLDOMNodePtr t_pAttribute;
                t_pAttributeList->get_item(t_j, &t_pAttribute);
                t_pAttribute->get_nodeName(&t_sNodeName);
                std::string t_sCheckString = bstr_t(t_sNodeName);
                if (p_sAttributeName == t_sCheckString)
                {
                    CComVariant t_ValueCheck;
                    t_pAttribute->get_nodeValue(&t_ValueCheck);
                    if (bstr_t(p_value.bstrVal) == bstr_t(t_ValueCheck.bstrVal))
                        return t_pTempNode;
                    else
                        break;
                }
            }
        }

        return NULL;
    }

    void XML_Writer::AddChildNode(MSXML2::IXMLDOMDocument2Ptr p_pDOM, MSXML2::IXMLDOMElementPtr p_pParent, std::string p_sNodeName, const AttributeList *p_plAttributes)
    {
        if (!p_pDOM)
            return;

        if (!p_pParent)
            return;

        MSXML2::IXMLDOMElementPtr t_Node(p_pDOM->createNode(NODE_ELEMENT, p_sNodeName.c_str(), ""));

        // add attributes
        if (p_plAttributes)
        {
            MSXML2::IXMLDOMAttributePtr t_Attribute;
            for (AttributeList::const_iterator t_Itr = p_plAttributes->begin(); t_Itr != p_plAttributes->end(); ++t_Itr)
            {
                t_Attribute = p_pDOM->createAttribute(t_Itr->first.c_str());
                t_Attribute->value = t_Itr->second;
                t_Node->Getattributes()->setNamedItem(t_Attribute);
            }
        }

        p_pParent->appendChild(t_Node);
    }

    void XML_Writer::AddChildNodes(MSXML2::IXMLDOMDocument2Ptr p_pDOM, IXMLDOMNodePtr p_pParent, const WriteChildList *p_plChildren)
    {
        if (!p_pDOM || !p_pParent || !p_plChildren)
            return;

        for (WriteChildList::const_iterator t_Itr = p_plChildren->begin(); t_Itr != p_plChildren->end(); ++t_Itr)
        {
            IXMLDOMNodePtr t_pNewNode = NULL;
            // if node has no attribute named "ID" its a single node
            CComVariant t_Value;
            if (t_Itr->second.GetAttributeValue("ID", t_Value))
            {
                 if (FAILED(t_Value.ChangeType(VT_UINT)))
                    continue;

                 t_pNewNode = GetChildNodeByAttribute(p_pParent, t_Itr->first, "ID", t_Value);
            }
            else
                p_pParent->selectSingleNode(_bstr_t(t_Itr->first.c_str()), &t_pNewNode);

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

            case XML_WRITE_CHANGE:
            case XML_WRITE_ADD:
                if (t_pNewNode)
                    ChangeNode(t_pNewNode, t_Itr->second.GetAttributeList());

                else
                {
                    AddChildNode(p_pDOM, p_pParent, t_Itr->first, t_Itr->second.GetAttributeList());
                    p_pParent->selectSingleNode(_bstr_t(t_Itr->first.c_str()), &t_pNewNode);
                }
                break;

            case XML_WRITE_NONE:
            default:
                break;
            }

            if (t_Itr->second.GetChildList() && t_Itr->second.GetChildList()->size())
                AddChildNodes(p_pDOM, t_pNewNode, t_Itr->second.GetChildList());
        }
    }

    void XML_Writer::ChangeNode(IXMLDOMNodePtr p_pNode, const AttributeList *p_plAttributes)
    {
        if (!p_pNode || !p_plAttributes)
            return;

        IXMLDOMElementPtr p_pElement = p_pNode;

        for (AttributeList::const_iterator t_Itr = p_plAttributes->begin(); t_Itr != p_plAttributes->end(); ++t_Itr)
            p_pElement->setAttribute(_bstr_t(t_Itr->first.c_str()), t_Itr->second);
    }

    bool XML_WriteData::HasChild(std::string p_sName) const
    {
        WriteChildList::const_iterator t_Itr = m_ChildList.find(p_sName);
        return t_Itr != m_ChildList.end() ? true : false;
    }

    XML_WriteData* XML_WriteData::GetChild(std::string p_sName)
    {
        WriteChildList::iterator t_Itr = m_ChildList.find(p_sName);
        return t_Itr != m_ChildList.end() ? &t_Itr->second : NULL;
    }

    const XML_WriteData* XML_WriteData::GetChild(std::string p_sName) const
    {
        WriteChildList::const_iterator t_Itr = m_ChildList.find(p_sName);
        return t_Itr != m_ChildList.end() ? &t_Itr->second : NULL;
    }

    void XML_WriteData::AddChild(std::string p_sName, XML::XML_WriteData p_NewChild)
    {
        m_ChildList.insert(std::make_pair(p_sName, p_NewChild));
    }

    void XML_WriteData::AddAttribute(std::string p_sName, CComVariant p_NewAttribute)
    {
        m_AttributeList.insert(std::make_pair(p_sName, p_NewAttribute));
    }
}