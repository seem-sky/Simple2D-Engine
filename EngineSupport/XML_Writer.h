#ifndef XML_WRITER_H
#define XML_WRITER_H

#include "XML_Reader.h"
#include <msxml.h>

#import <msxml4.dll>

namespace XML
{
    class XML_Writer : ActiveObject
    {
    public:
        XML_Writer(std::string p_sFileName, bool p_bNew);
        ~XML_Writer(void);

        void Run();
        bool GetChildNode(IXMLDOMNodePtr p_pNode, std::string p_sNodeName, IXMLDOMNodePtr &p_pResultNode);
        bool GetChildNodeByAttribute(IXMLDOMNodePtr p_pNode, std::string p_sNodeName, std::string p_sAttributeName, VARIANT p_value, IXMLDOMNodePtr &p_pResultNode);

        IXMLDOMNodePtr* AddChildNode(IXMLDOMNodePtr p_pNode, std::string p_sNodeName);

    private:
        XML_STATE m_XMLState;
        std::string m_sFileName;
        bool m_bNew;
    };
}
#endif