#include "RevertInfo.h"

using namespace MAP::BRUSH;
using namespace REVERT;

RevertInfo::RevertInfo(const UInt32Point& layerSize, uint32 layerIndex, MAP::Layer layerType)
    : m_Check(layerSize.x * layerSize.y), m_LayerIndex(layerIndex), m_LayerType(layerType), m_LayerSize(layerSize)
{
}

RevertInfo::~RevertInfo()
{
    m_Check.reset();
}

void RevertInfo::revert(MAP::MapLayer &mapLayer)
{
    for (auto& pair : m_Tiles)
    {
        try
        {
            mapLayer.setMapTile(UInt32Point3D(pair.first, m_LayerIndex), m_LayerType, pair.second);
        }
        catch (const std::exception& e)
        {
        }
    }
}

void RevertInfo::addTile(const UInt32Point& pos, const MAP::MapTile& tile)
{
    uint32 index = pos.x*m_LayerSize.y + pos.y;
    if (index > m_LayerSize.x*m_LayerSize.y || m_Check[index])
        return;
    m_Check[index] = true;
    m_Tiles.push_back(std::make_pair(pos, tile));
}
