#include "XML_IO.h"
#include <fstream>

using namespace XML_IO;

/*####
# CoObject
####*/
CoObject::~CoObject()
{
    if (init)
        CoUninitialize();
}

bool CoObject::InitCo(LPVOID pReserved)
{
    if (FAILED(CoInitialize(pReserved)))
        return false;

    init = true;
    return true;
}

/*####
# XMLReader
####*/
bool XMLReader::readFile(const std::string &sFileName, const StdStringVector &sRootNode, const bool threaded)
{
    CoObject coObj;
    if (threaded)
        coObj.InitCo();

    MSXML2::IXMLDOMDocument2Ptr pXMLDom;
    if (FAILED(pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
    {
        ERROR_LOG("Unable to create DOM instance.");
        return false;
    }

    if (!pXMLDom->load(_variant_t(sFileName.c_str())))
    {
        ERROR_LOG("Unable to load XML file.");
        return false;
    }

    IXMLDOMNodePtr pDatabaseNode = changeRootNode(pXMLDom, sRootNode);
    if (pDatabaseNode)
        return checkoutChildren(pDatabaseNode);
    return false;
}

IXMLDOMNodePtr XMLReader::changeRootNode(const IXMLDOMNodePtr pParentNode, const StdStringVector &sNodeName)
{
    if (!pParentNode)
        return NULL;

    IXMLDOMNodePtr resultNode;
    IXMLDOMNodePtr node = pParentNode;
    for (uint32 i = 0; i < sNodeName.size(); ++i)
    {
        if (FAILED(node->selectSingleNode(_bstr_t(sNodeName.at(i).c_str()), &resultNode)))
            return NULL;

        node = resultNode;
    }
    return resultNode;
}

/*####
# XMLWriter
####*/
MSXML2::IXMLDOMElementPtr XMLWriter::createNode(const std::string &sNode)
{
    return m_pXMLDom->createNode(NODE_ELEMENT, _bstr_t(sNode.c_str()), "");
}

void XMLWriter::removeChildren(MSXML2::IXMLDOMNodePtr pNode)
{
    while (MSXML2::IXMLDOMNodePtr pChild = pNode->GetfirstChild())
        pNode->removeChild(pChild);
}

IXMLDOMNodePtr XMLWriter::changeRootNode(const IXMLDOMNodePtr pParentNode, const StdStringVector &sNodeName)
{
    if (!m_pXMLDom || !pParentNode)
        return NULL;

    IXMLDOMNodePtr resultNode;
    IXMLDOMNodePtr node = pParentNode;
    for (uint32 i = 0; i < sNodeName.size(); ++i)
    {
        if (FAILED(node->selectSingleNode(_bstr_t(sNodeName.at(i).c_str()), &resultNode)) || !resultNode)
        {
            resultNode = createNode(sNodeName.at(i));
            if (!resultNode)
                return NULL;
            resultNode = addChildren(resultNode, node);
            if (!resultNode)
                return NULL;
        }

        node = resultNode;
    }
    return resultNode;
}

void XMLWriter::changeAttribute(MSXML2::IXMLDOMElementPtr pNode, CComVariant value, CComBSTR attributeName)
{
    if (!m_pXMLDom)
        return;
    if (FAILED(pNode->setAttribute(attributeName.m_str, value)))
    {
        MSXML2::IXMLDOMAttributePtr attribute = m_pXMLDom->createAttribute(attributeName.m_str);
        attribute->value = value;
        pNode->Getattributes()->setNamedItem(attribute);
    }
}

bool XMLWriter::writeFile(const std::string &sFileName, const StdStringVector &sRootNode, const bool threaded)
{
    CoObject coObj;
    if (threaded)
        coObj.InitCo();

    
    if (FAILED(m_pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
    {
        ERROR_LOG("Unable to create DOM instance.");
        return false;
    }

    while (!m_pXMLDom->load(_variant_t(sFileName.c_str())))
    {
        BASIC_LOG("Unable to load XML file. Create file with root nodes.");
        if (sRootNode.empty())
        {
            BASIC_LOG("Rootnodes are empty, so unable to create xml file.");
            return false;
        }

        // create basic file
        std::ofstream file(sFileName.c_str());
        file << "<" << sRootNode.at(0) << "></" << sRootNode.at(0) << ">";
        file.close();
    }

    MSXML2::IXMLDOMNodePtr pDatabaseNode = changeRootNode(m_pXMLDom, sRootNode);
    if (!pDatabaseNode)
        return false;
    // clear children before writing
    removeChildren(pDatabaseNode);
    writeChildren(pDatabaseNode);
    if (FAILED(m_pXMLDom->save(_variant_t(sFileName.c_str()))))
        return false;
    return true;
}

MSXML2::IXMLDOMNodePtr XMLWriter::addChildren(MSXML2::IXMLDOMNodePtr pNode, MSXML2::IXMLDOMNodePtr pParent)
{
    if (pNode && pParent)
        return pParent->appendChild(pNode);
    return NULL;
}