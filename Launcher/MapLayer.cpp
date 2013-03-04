#include "MapLayer.h"
#include "Game.h"

MapLayer::MapLayer(void) : Layer()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "MapLayer : ";
    m_LayerType         = LAYER_TYPE_MAP;
}

MapLayer::~MapLayer(void)
{
}

void MapLayer::UpdateLayer(const uint32 uicCurTime, const uint32 uiDiff)
{
    /*if (m_pMap)
    {
        D3DXVECTOR3 v3Pos = m_pMap->GetPosition();
        v3Pos.x -= 1;
        v3Pos.y -= 2;
        m_pMap->SetPosition(v3Pos);
    }*/
}
