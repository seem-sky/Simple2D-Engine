#ifndef XML_READER_H
#define XML_READER_H

#include "Thread.h"
#include <string>
#include <map>
#include <msxml.h>

#import <msxml4.dll>

struct XML_Data;
typedef std::multimap<std::string, XML_Data> ChildList;
typedef std::map<std::string, VARIANT> AttributeList;

struct XML_Data
{
    ChildList m_ChildList;
    AttributeList m_AttributeList;

    inline bool HasChilds() { return m_ChildList.empty() ? false : true; }
    inline bool HasAttributes() { return m_AttributeList.empty() ? false : true; }
};

class XML_Reader : public ActiveObject
{
public:
    enum XML_STATE
    {
        XML_NONE,
        XML_FAILED,
        XML_NO_FILE,
        XML_CORRUPT_FILE,
        XML_IN_PROGRESS,
        XML_DONE,
    };

    XML_Reader(std::string sData);
    ~XML_Reader(void);

    void Run();
    inline XML_STATE GetReaderState() { return m_XMLState; }
    inline ChildList* GetXMLData() { return m_pChildList; }

private:
    XML_STATE m_XMLState;
    std::string m_sData;
    std::string m_sLogLocationName;

    ChildList *m_pChildList;

    ChildList* CheckoutChilds(IXMLDOMNodePtr p_pNode, HRESULT &p_hr);
    AttributeList* CheckoutAttributes(IXMLDOMNodePtr p_pNode, HRESULT &p_hr);
};

#endif