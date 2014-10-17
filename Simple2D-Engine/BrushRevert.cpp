#include "BrushRevert.h"
#include "MapException.h"
#include "MapLayer.h"

using namespace MAP;
using namespace BRUSH::REVERT;

BrushRevert::BrushRevert(uint8 layerIndex, LayerType layerType, LayerContainer& layer) : m_MapLayer(layer), m_LayerIndex(layerIndex), m_LayerType(layerType),
    MAP::REVERT::Interface()
{}

void BrushRevert::revert()
{
    auto& layer = m_MapLayer.getLayer(m_LayerType, m_LayerIndex);
    for (auto& info : m_Tiles)
    {
        try
        {
            layer.setMapTile(info);
        }
        catch (const MAP::EXCEPTION::TileOutOfRangeException&) {}
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
    m_LayerIndex = 0;
    m_LayerType = LayerType::LAYER_BACKGROUND;
}
