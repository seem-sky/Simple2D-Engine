#include "TileMappingMode.h"
#include "moc_TileMappingMode.h"
#include "MapEditor.h"
#include "MapException.h"
#include <Database/Manager.h>
#include <BrushFactory.h>
#include "MapEditorWidgetBrush.h"

namespace MAPPING_MODE
{
    Tile::Tile(const database::Manager& DBMgr, QObject* pParent)
        : m_DBMgr(DBMgr), Interface(pParent)
    {}

    void Tile::_finishBrush()
    {
        // push only if there are changes
        if (m_pCurrentBrush && m_pCurrentBrush->hasChanges() && m_pCurrentEditor)
            m_pCurrentEditor->addRevert(m_pCurrentBrush->takeBrushRevert());

        m_pCurrentEditor = nullptr;
        m_pCurrentBrush.reset();
    }

    BRUSH::BrushIndex Tile::brushIndexFromMouseButton(Qt::MouseButton button)
    {
        auto brush = BRUSH::BrushIndex::BRUSH_LEFT;
        if (button == Qt::RightButton)
            brush = BRUSH::BrushIndex::BRUSH_RIGHT;
        return brush;
    }

    void Tile::press(MapEditor& editor, QMouseEvent* pEvent)
    {
        if (pEvent->button() == Qt::RightButton || pEvent->button() == Qt::LeftButton)
        {
            // create brush
            auto brush = brushIndexFromMouseButton(pEvent->button());
            auto pos = editor.mapToScene(pEvent->pos()).toPoint();
            // if there is an old brush, release it
            _finishBrush();
            try
            {
                m_pCurrentBrush = MAP::BRUSH::BrushFactory::createBrush(m_BrushInfos.at(static_cast<std::size_t>(brush)), m_DBMgr, editor.getMapData().getMapLayer(),
                    editor.getLayerType(), editor.getLayerIndex() - 1);
                GEOMETRY::Point<uint32> tilePos(pos.x() / MAP::TILE_SIZE, pos.y() / MAP::TILE_SIZE);

                // if tileSet mapping, set brush size equal tileSet size
                if (m_BrushInfos.at(static_cast<std::size_t>(brush)).getType() == MAP::BRUSH::BrushInfo::Type::TILE_SET)
                {
                    if (auto pTileSet = m_DBMgr.getTileSetDatabase().getPrototype(m_BrushInfos.at(static_cast<std::size_t>(brush)).getID()))
                        m_pCurrentBrush->setBrushSize(pTileSet->getSize());
                }

                m_pCurrentBrush->start(tilePos);
                editor.scene()->update();
                m_pCurrentEditor = &editor;
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

    void Tile::release(MapEditor& editor, QMouseEvent* pEvent)
    {
        _finishBrush();
    }

    void Tile::move(MapEditor& editor, QMouseEvent* pEvent)
    {
        if (m_pCurrentEditor && &editor == m_pCurrentEditor)
        {
            try
            {
                auto pos = editor.mapToScene(pEvent->pos()).toPoint();
                GEOMETRY::Point<uint32> tilePos(pos.x() / MAP::TILE_SIZE, pos.y() / MAP::TILE_SIZE);

                m_pCurrentBrush->start(tilePos);
                m_pCurrentEditor->scene()->update();
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
}
