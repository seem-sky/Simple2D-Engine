#include "MapBrushWidget.h"
#include "moc_MapBrushWidget.h"

using namespace MAP;

MapBrushWidget::MapBrushWidget(QWidget *pParent) : QWidget(pParent), MapEditorObject(), Ui_MapBrushWidget(), m_pBrush(new MapTileBrush())
{
    setupUi(this);

    connect(m_pMode, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(_changeDrawMode(const QString&)));
}

void MapBrushWidget::updateObject()
{
    changeBrush(m_pBrush->getTile(), m_pBrush->isAutoTileBrush());

}

void MapBrushWidget::clearWidget()
{
    changeBrush(0, false);
    m_pMode->setCurrentIndex(0);
}

void MapBrushWidget::changeBrush(uint32 uiID, bool isAutoTile)
{
    if (!m_pSharedData)
        return;

    if (isAutoTile)
    {
        ConstAutoTilePtr pAutoTile;
        ConstQPixmapPtr pPixmap;
        if (m_pSharedData->getAutoTileCache()->getItem(uiID, pAutoTile) && pAutoTile->getPixmap(DATABASE::AutoTilePrototype::INDEX_CIRCLE, pPixmap))
        {
            m_pBrush = _getNewAutoTileBrush();
            m_pCurrentTile->setPixmap(*pPixmap.get());
        }
        else
            return;
    }
    else
    {
        ConstQPixmapPtr pPixmap;
        m_pBrush = _getNewTileBrush();
        if (m_pSharedData->getTileCache()->getItem(uiID, pPixmap))
            m_pCurrentTile->setPixmap(*pPixmap.get());
        else
        {
            // if id == 0, set brush
            if (!uiID)
            {
                QPixmap newPixmap(TILE_SIZE, TILE_SIZE);
                newPixmap.fill();
                m_pCurrentTile->setPixmap(newPixmap);
            }
            else return;
        }
    }
    m_pBrush->setTile(uiID);
}

TileBrush::BrushType MapBrushWidget::getBrushType(const QString &mode)
{    
    if (mode == "Fill")
        return TileBrush::BRUSH_FILL;
    return TileBrush::BRUSH_PEN;
}

TileBrush::BrushType MapBrushWidget::getCurrentBrushType() const
{
    return getBrushType(m_pMode->itemText(m_pMode->currentIndex()));
}

void MapBrushWidget::_changeDrawMode(const QString& mode)
{
    m_pBrush->setBrushType(getBrushType(mode));
}

MapBrushPtr MapBrushWidget::_getNewTileBrush()
{
    MapBrushPtr pNewBrush;
    if (m_pSharedData)
    {
        pNewBrush = MapBrushPtr(new MapTileBrush());
        pNewBrush->setBrushType(getCurrentBrushType());
        pNewBrush->setLayer(m_pSharedData->getCurrentLayer());
    }
    return pNewBrush;
}

MapBrushPtr MapBrushWidget::_getNewAutoTileBrush()
{
    MapBrushPtr pNewBrush;
    if (m_pSharedData)
    {
        pNewBrush = MapBrushPtr(new MapAutoTileBrush(m_pSharedData->getAutoTileDatabase()));
        pNewBrush->setBrushType(getCurrentBrushType());
        pNewBrush->setLayer(m_pSharedData->getCurrentLayer());
    }
    return pNewBrush;
}