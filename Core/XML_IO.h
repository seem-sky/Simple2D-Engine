#ifndef XML_IO_H
#define XML_IO_H

#include "Global.h"
#include "Logfile.h"
#include <atlcomcli.h>
#include <atlbase.h>

#include <msxml.h>
#import <msxml6.dll>

namespace XML_IO
{
    /*####
    # CoObject
    ####*/
    class CoObject
    {
    public:
        CoObject() : init(false) {}
        ~CoObject();

        bool InitCo(LPVOID p_pReserved = NULL);

    private:
        bool init;
    };

    class XMLReader
    {
    private:
        IXMLDOMNodePtr changeRootNode(const IXMLDOMNodePtr pParentNode, const StdStringVector &sNodeName);
        virtual bool checkoutChildren(const IXMLDOMNodePtr &parentNode) = 0;

    protected:
        bool readFile(const std::string &sFileName, const StdStringVector &sRootNode, const bool threaded = false);
    };

    class XMLWriter
    {
    private:
        void removeChildren(MSXML2::IXMLDOMNodePtr pNode);
        IXMLDOMNodePtr changeRootNode(const IXMLDOMNodePtr pParentNode, const StdStringVector &sNodeName);
        virtual bool writeChildren(MSXML2::IXMLDOMNodePtr &parentNode) = 0;

    protected:
        MSXML2::IXMLDOMElementPtr createNode(const std::string &sNode);
        MSXML2::IXMLDOMNodePtr addChildren(MSXML2::IXMLDOMNodePtr pNode, MSXML2::IXMLDOMNodePtr pParent);
        void changeAttribute(MSXML2::IXMLDOMElementPtr pNode, CComVariant value, CComBSTR attributeName);
        bool writeFile(const std::string &sFileName, const StdStringVector &sRootNode, const bool threaded = false);

    public:
        XMLWriter() : m_pXMLDom(NULL) {}

    private:
        MSXML2::IXMLDOMDocument2Ptr m_pXMLDom;
    };
}
#endif