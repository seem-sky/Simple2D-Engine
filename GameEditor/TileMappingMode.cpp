#include "TileMappingMode.h"
#include "MapViewer.h"
#include "MapEditorWidgetBrush.h"
#include "MapException.h"
#include <DatabaseMgr.h>

namespace MAPPING_MODE
{
    Tile::Tile(const BRUSH::MapEditorWidgetBrush& brushWidget) : m_BrushWidget(brushWidget)
    {}

    void Tile::_finishBrush()
    {
        // push only if there are changes
        if (m_pCurrentBrush && m_pCurrentBrush->getBrushRevert().hasChanges() && m_pMapViewer)
            m_pMapViewer->addBrushRevert(m_pCurrentBrush->getBrushRevert());
        m_pCurrentBrush.reset();
        m_pMapViewer = nullptr;
    }

    BRUSH::BrushIndex Tile::brushIndexFromMouseButton(Qt::MouseButton button)
    {
        auto brush = BRUSH::BrushIndex::BRUSH_LEFT;
        if (button == Qt::RightButton)
            brush = BRUSH::BrushIndex::BRUSH_RIGHT;
        return brush;
    }

    void Tile::press(MapViewer* pViewer, const QMouseEvent* pEvent)
    {
        if (!pViewer || !pViewer->getScene() || !pEvent)
            return;

        if (pEvent->button() == Qt::RightButton || pEvent->button() == Qt::LeftButton)
        {
            // create brush
            auto brush = brushIndexFromMouseButton(pEvent->button());

            // if there is an old brush, release it
            _finishBrush();
            try
            {
                auto& layer = pViewer->getScene()->getMapData().getMapLayer().getLayer(pViewer->getScene()->getLayerType(), pViewer->getScene()->getLayerIndex() - 1);
                m_pCurrentBrush = m_BrushWidget.createBrush(brush, layer);
                auto pos = pViewer->mapToScene(pEvent->pos());
                UInt32Point tilePos(pos.x() / TILE_SIZE, pos.y() / TILE_SIZE);

                auto brushInfo = m_BrushWidget.getBrushInfo(brush);

                // if tileSet mapping, set brush size equal tileSet size
                if (brushInfo.getType() == MAP::BRUSH::BrushInfo::Type::TILE_SET)
                {
                    if (auto pTileSet = m_BrushWidget.getDatabaseMgr().getTileSetDatabase()->getOriginalPrototype(brushInfo.getID()))
                        m_pCurrentBrush->setBrushSize(pTileSet->getTileSetSize());
                }

                // think thats not so good ?!
                //// if already same tile, return
                //if (_isTileAlreadySet(layer, tilePos, m_pBrushWidget->getBrushInfo(brush)))
                //    return;

                m_pCurrentBrush->start(tilePos);
                pViewer->getScene()->update();
                m_pMapViewer = pViewer;
            }
            catch (const MAP::EXCEPTION::LayerOutOfRangeException&) {}
        }
    }

    bool Tile::_isTileAlreadySet(const MAP::Layer &layer, const UInt32Point& tilePos, const MAP::BRUSH::BrushInfo& brushInfo)
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

    void Tile::release(MapViewer* pViewer, const QMouseEvent* pEvent)
    {
        _finishBrush();
    }

    void Tile::move(MapViewer* pViewer, const QMouseEvent* pEvent)
    {
        if (m_pCurrentBrush && pViewer && pViewer->getScene())
        {
            try
            {
                auto& layer = pViewer->getScene()->getMapData().getMapLayer().getLayer(pViewer->getScene()->getLayerType(), pViewer->getScene()->getLayerIndex() - 1);
                auto pos = pViewer->mapToScene(pEvent->pos());
                UInt32Point tilePos(pos.x() / TILE_SIZE, pos.y() / TILE_SIZE);

                // think thats not so good ?!
                //// if already same tile, return
                //if (_isTileAlreadySet(layer, tilePos, m_pBrushWidget->getBrushInfo(brushIndexFromMouseButton(pEvent->button()))))
                //    return;

                m_pCurrentBrush->start(tilePos);
                pViewer->getScene()->update();
            }
            catch (const MAP::EXCEPTION::LayerOutOfRangeException&) {}
        }
    }
}