#include "TileMappingMode.h"
#include "MapViewer.h"
#include "MapEditorWidgetBrush.h"

namespace MAPPING_MODE
{
    Tile::Tile(const BRUSH::MapEditorWidgetBrush* pBrushWidget) : m_pBrushWidget(pBrushWidget)
    {}

    void Tile::_finishBrush()
    {
        // push only if there are changes
        if (m_pCurrentBrush && m_pCurrentBrush->getRevertInfo().hasChanges() && m_pMapViewer)
            m_pMapViewer->addRevertInfo(m_pCurrentBrush->getRevertInfo());
        m_pCurrentBrush.reset();
        m_pMapViewer = nullptr;
    }

    void Tile::press(MapViewer* pViewer, const QMouseEvent* pEvent)
    {
        if (!pViewer || !pViewer->getScene() || !pEvent)
            return;

        if (pEvent->button() == Qt::RightButton || pEvent->button() == Qt::LeftButton)
        {
            // create brush
            auto brush = BRUSH::BrushIndex::BRUSH_LEFT;
            if (pEvent->button() == Qt::RightButton)
                brush = BRUSH::BrushIndex::BRUSH_RIGHT;

            // if there is an old brush, release it
            if (m_pCurrentBrush)
                _finishBrush();
            m_pCurrentBrush = m_pBrushWidget->createBrush(brush, pViewer->getScene()->getMapData().getMapLayer(), pViewer->getScene()->getLayerType(),
                pViewer->getScene()->getLayerIndex());
            auto pos = pViewer->mapToScene(pEvent->pos());
            m_pCurrentBrush->draw(UInt32Point(pos.x() / TILE_SIZE, pos.y() / TILE_SIZE));
            pViewer->getScene()->update();
            m_pMapViewer = pViewer;
        }
    }

    void Tile::release(MapViewer* pViewer, const QMouseEvent* pEvent)
    {
        _finishBrush();
    }

    void Tile::move(MapViewer* pViewer, const QMouseEvent* pEvent)
    {
        if (m_pCurrentBrush && pViewer && pViewer->getScene())
        {
            auto pos = pViewer->mapToScene(pEvent->pos());
            m_pCurrentBrush->draw(UInt32Point(pos.x() / TILE_SIZE, pos.y() / TILE_SIZE));
            pViewer->getScene()->update();
        }
    }
}