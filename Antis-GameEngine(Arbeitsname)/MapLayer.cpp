#include "MapLayer.h"

MapLayer::MapLayer(void) : Layer()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "MapLayer : ";
    m_LayerType         = LAYER_TYPE_MAP;
}

MapLayer::~MapLayer(void)
{
    if (m_pSprite)
        m_pSprite->Release();
}
