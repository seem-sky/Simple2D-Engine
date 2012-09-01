#include "EditorConfig.h"

EditorConfig::EditorConfig(void) : TSingleton()
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "EditorConfig : ";
}

EditorConfig::~EditorConfig(void)
{
}
