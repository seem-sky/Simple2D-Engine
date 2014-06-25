#include "TileMappingMode.h"
#include "MapViewer.h"
#include "MapEditorWidgetBrush.h"
#include "MapException.h"

namespace MAPPING_MODE
{
    Tile::Tile(const BRUSH::MapEditorWidgetBrush* pBrushWidget) : m_pBrushWidget(pBrushWidget)
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
                m_pCurrentBrush = m_pBrushWidget->createBrush(brush, layer);
                auto pos = pViewer->mapToScene(pEvent->pos());
                UInt32Point tilePos(pos.x() / TILE_SIZE, pos.y() / TILE_SIZE);

                // if already same tile, return
                if (_isTileAlreadySet(layer, tilePos, brush))
                    return;

                m_pCurrentBrush->start(tilePos);
                pViewer->getScene()->update();
                m_pMapViewer = pViewer;
            }
            catch (const MAP::EXCEPTION::LayerOutOfRangeException&) {}
        }
    }

    bool Tile::_isTileAlreadySet(const MAP::Layer &layer, const UInt32Point& tilePos, BRUSH::BrushIndex brush)
    {
        try
        {
            auto tileInfo = layer.getMapTile(tilePos).getMapTile();
            return !tileInfo.isValid() || (!tileInfo.isAutoTile() && tileInfo.m_uiTileID == m_pBrushWidget->getBrushInfo(brush).getID());
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

                // if already same tile, return
                if (_isTileAlreadySet(layer, tilePos, brushIndexFromMouseButton(pEvent->button())))
                    return;

                m_pCurrentBrush->start(tilePos);
                pViewer->getScene()->update();
            }
            catch (const MAP::EXCEPTION::LayerOutOfRangeException&) {}
        }
    }
}