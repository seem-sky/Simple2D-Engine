#include "TileMappingMode.h"
#include "moc_TileMappingMode.h"
#include "MapViewerScene.h"
#include "MapException.h"
#include <DatabaseMgr.h>
#include <BrushFactory.h>
#include "MapEditorWidgetBrush.h"

namespace MAPPING_MODE
{
    Tile::Tile(const DATABASE::DatabaseMgr& DBMgr, QObject* pParent) : m_DBMgr(DBMgr), Interface(pParent)
    {}

    void Tile::_finishBrush()
    {
        // push only if there are changes
        if (m_pCurrentBrush && m_pCurrentBrush->hasChanges() && m_pCurrentScene)
            m_pCurrentScene->addRevert(m_pCurrentBrush->takeBrushRevert());

        m_pCurrentScene = nullptr;
        m_pCurrentBrush.reset();
    }

    BRUSH::BrushIndex Tile::brushIndexFromMouseButton(Qt::MouseButton button)
    {
        auto brush = BRUSH::BrushIndex::BRUSH_LEFT;
        if (button == Qt::RightButton)
            brush = BRUSH::BrushIndex::BRUSH_RIGHT;
        return brush;
    }

    void Tile::press(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button)
    {
        if (!pScene)
            return;

        if (button == Qt::RightButton || button == Qt::LeftButton)
        {
            // create brush
            auto brush = brushIndexFromMouseButton(button);

            // if there is an old brush, release it
            _finishBrush();
            try
            {
                m_pCurrentBrush = MAP::BRUSH::BrushFactory::createBrush(m_BrushInfos.at(static_cast<std::size_t>(brush)), m_DBMgr, pScene->getMapData().getMapLayer(),
                    pScene->getLayerType(), pScene->getLayerIndex() - 1);
                GEOMETRY::Point<uint32> tilePos(pos.x() / TILE_SIZE, pos.y() / TILE_SIZE);

                // if tileSet mapping, set brush size equal tileSet size
                if (m_BrushInfos.at(static_cast<std::size_t>(brush)).getType() == MAP::BRUSH::BrushInfo::Type::TILE_SET)
                {
                    if (auto pTileSet = m_DBMgr.getTileSetDatabase()->getOriginalPrototype(m_BrushInfos.at(static_cast<std::size_t>(brush)).getID()))
                        m_pCurrentBrush->setBrushSize(pTileSet->getTileSetSize());
                }

                m_pCurrentBrush->start(tilePos);
                pScene->update();
                m_pCurrentScene = pScene;
            }
            catch (const MAP::EXCEPTION::LayerOutOfRangeException&) {}
        }
    }

    bool Tile::_isTileAlreadySet(const MAP::Layer &layer, const GEOMETRY::Point<uint32>& tilePos, const MAP::BRUSH::BrushInfo& brushInfo)
    {
        try
        {
            auto tileInfo = layer.getMapTile(tilePos).getMapTile();
            return !tileInfo.isValid() || brushInfo.getType() == MAP::BRUSH::BrushInfo::Type::TILE && tileInfo.m_uiTileID == brushInfo.getID() ||
                brushInfo.getType() == MAP::BRUSH::BrushInfo::Type::AUTO_TILE && tileInfo.m_uiAutoTileSetID == brushInfo.getID();
        }
        catch (const MAP::EXCEPTION::TileOutOfRangeException&) {}
        return true;
    }

    void Tile::release(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button)
    {
        _finishBrush();
    }

    void Tile::move(MapViewerScene* pScene, QPoint pos)
    {
        if (m_pCurrentScene && pScene == m_pCurrentScene)
        {
            try
            {
                GEOMETRY::Point<uint32> tilePos(pos.x() / TILE_SIZE, pos.y() / TILE_SIZE);

                m_pCurrentBrush->start(tilePos);
                m_pCurrentScene->update();
            }
            catch (const MAP::EXCEPTION::LayerOutOfRangeException&) {}
        }
        else
            _finishBrush();
    }

    void Tile::onBrushInfoChanged(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo info)
    {
        m_BrushInfos.at(static_cast<std::size_t>(brush)) = info;
    }

    void Tile::copy(MapViewerScene* pScene, QPoint pos)
    {
    }

    void Tile::insert(MapViewerScene* pScene, QPoint pos)
    {

    }

    void Tile::cutOut(MapViewerScene* pScene, QPoint pos)
    {

    }
}
