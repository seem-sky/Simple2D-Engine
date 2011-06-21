#include "MapInput.h"
#include <XmlLite.h>
//#include <atlbase.h>

MapInput::MapInput(void) : TFileInput()
{
    LOGFILE_ENGINE_LOG_NAME + "MapInput : ";
    m_sFileDirectory = "Maps/";
}

MapInput::~MapInput(void)
{
}

bool MapInput::CreateMapFromFile()
{
    //CComPtr<IXmlReader> reader;
    return true;
}
