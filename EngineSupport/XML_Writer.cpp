#include "XML_Writer.h"

namespace XML
{
    #define returnRun CoUninitialize(); return;

    XML_Writer::XML_Writer(std::string p_sFileName, bool t_pNew) : m_XMLState(XML_NONE), m_sFileName(p_sFileName), m_bNew(t_pNew), ActiveObject()
    {
        _thread.Resume();
    }

    XML_Writer::~XML_Writer(void)
    {
        //CoUninitialize();
    }

    void XML_Writer::Run()
    {
        if (FAILED(CoInitialize(NULL)))
        {
            m_XMLState = XML_FAILED;
            returnRun;
        }

        MSXML2::IXMLDOMDocument2Ptr t_pXMLDom = NULL;
        HRESULT p_hr = t_pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60));
        if (p_hr == S_FALSE)
        {
            m_XMLState = XML_FAILED;
            returnRun;
        }
        t_pXMLDom->loadXML(m_sFileName.c_str());

        IXMLDOMNodePtr t_pNode;
        GetChildNode(t_pXMLDom, "Database", t_pNode);
        GetChildNode(t_pNode, "SpriteDatabase", t_pNode);

        returnRun;
    }

    bool XML_Writer::GetChildNode(IXMLDOMNodePtr p_pNode, std::string p_sNodeName, IXMLDOMNodePtr &p_pResultNode)
    {
        IXMLDOMNodeListPtr t_pChildren;
        p_pNode->get_childNodes(&t_pChildren);
        LONG t_lLength = 0;
        t_pChildren->get_length(&t_lLength);
        IXMLDOMNodePtr t_pTempNode;
        BSTR t_sNodeName;
        for (LONG t_i = 0; t_lLength < t_i; ++t_i)
        {
            t_pChildren->get_item(t_i, &t_pTempNode);
            t_pTempNode->get_nodeName(&t_sNodeName);
            std::string t_sCheckString = bstr_t(t_sNodeName);
            if (p_sNodeName == t_sCheckString)
            {
                p_pResultNode = t_pTempNode;
                return true;
            }
        }

        return false;
    }

    bool XML_Writer::GetChildNodeByAttribute(IXMLDOMNodePtr p_pNode, std::string p_sNodeName, std::string p_sAttributeName, VARIANT p_value, IXMLDOMNodePtr &p_pResultNode)
    {
        IXMLDOMNodeListPtr t_pChildren;
        p_pNode->get_childNodes(&t_pChildren);
        LONG t_lLength = 0;
        t_pChildren->get_length(&t_lLength);
        IXMLDOMNodePtr t_pTempNode;
        BSTR t_sNodeName;
        // check children
        for (LONG t_i = 0; t_lLength < t_i; ++t_i)
        {
            t_pChildren->get_item(t_i, &t_pTempNode);
            t_pTempNode->get_nodeName(&t_sNodeName);
            std::string t_sCheckString = bstr_t(t_sNodeName);
            if (p_sNodeName == t_sCheckString)
            {
                IXMLDOMNamedNodeMapPtr t_pAttributeList;
                t_pTempNode->get_attributes(&t_pAttributeList);
                LONG t_lAttributeLength = 0;
                t_pAttributeList->get_length(&t_lAttributeLength);
                // check attributes
                for (LONG t_j = 0; t_j < t_lAttributeLength; ++t_j)
                {
                    IXMLDOMNodePtr t_pAttribute;
                    t_pAttributeList->get_item(t_i, &t_pAttribute);
                    t_pAttribute->get_nodeName(&t_sNodeName);
                    t_sCheckString = bstr_t(t_sNodeName);
                    if (p_value.bstrVal == t_sNodeName)
                    {
                        p_pResultNode = t_pTempNode;
                        return true;
                    }
                }
            }
        }

        return false;
    }

    IXMLDOMNodePtr XML_Writer::AddChildNode(IXMLDOMNodePtr p_pNode, std::string p_sNodeName)
    {
        //IXMLDOMNodePtr t_pNewNode = new IXMLDOMNode(;
        return NULL;
    }
}