#include "BrushRevert.h"
#include "MapException.h"
#include "MapLayer.h"

using namespace MAP;
using namespace BRUSH;
using namespace REVERT;

void BrushRevert::revert(LayerContainer& mapLayer)
{
    auto& layer = mapLayer.getLayer(m_Info.getLayerType(), m_Info.getLayerIndex());
    for (auto& info : m_Tiles)
    {
        try
        {
            layer.setMapTile(info);
        }
        catch (const MAP::EXCEPTION::TileRangeException&)
        {
        }
    }
}

void BrushRevert::addTile(const MapTileInfo& info)
{
    if (m_Check.get(info.getPosition()))
        return;
    m_Check.set(info.getPosition());
    m_Tiles.push_back(info);
}

void BrushRevert::addTiles(const MapTileInfoVec& tileInfos)
{
    for (auto& info : tileInfos)
        addTile(info);
}

void BrushRevert::clear()
{
    m_Tiles.clear();
    m_Check = Bitset2D();
    m_Info = BrushRevertInfo();
}
