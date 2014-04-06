#include "RevertInfo.h"
#include "MapException.h"

using namespace MAP;
using namespace BRUSH;
using namespace REVERT;

RevertInfo::RevertInfo(LayerType type, uint8 index) : m_LayerType(type), m_LayerIndex(index)
{
}

void RevertInfo::revert(MAP::LayerContainer &mapLayer)
{
    for (auto& pair : m_Tiles)
    {
        try
        {
            auto& layer = mapLayer.getLayer(m_LayerType, m_LayerIndex);
            layer.setMapTile(UInt32Point3D(pair.first), pair.second);
        }
        catch (const MAP::EXCEPTION::TileRangeException& e)
        {
        }
    }
}

void RevertInfo::addTile(const UInt32Point& pos, const MAP::MapTile& tile)
{
    if (m_Check.get(pos))
        return;
    m_Check.set(pos);
    m_Tiles.push_back(std::make_pair(pos, tile));
}
