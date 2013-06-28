#include "MapBrushWidget.h"
#include "moc_MapBrushWidget.h"

using namespace MAP;
using namespace BRUSH;

MapBrushWidget::MapBrushWidget(QWidget *pParent) : QWidget(pParent), MapEditorObject(), Ui_MapBrushWidget(), m_pBrush(new MapTileBrush())
{
    setupUi(this);
    m_pCurrentTile->setScaledContents(true);
    connect(m_pMode, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(_changeDrawMode(const QString&)));
}

void MapBrushWidget::updateObject()
{
    changeBrush(m_pBrush->getTile(), m_pBrush->getBrushType());
}

void MapBrushWidget::clearWidget()
{
    changeBrush(0, BRUSH_TILE);
    m_pMode->setCurrentIndex(0);
}

void MapBrushWidget::changeBrush(uint32 uiID, BrushType type)
{
    if (!m_pSharedData)
        return;

    if (type == BRUSH_AUTO_TILE)
    {
        ConstAutoTilePtr pAutoTile;
        ConstQPixmapPtr pPixmap;
        if (m_pSharedData->getAutoTileCache()->getItem(uiID, pAutoTile) && pAutoTile->getPixmap(DATABASE::AUTO_TILE::INDEX_CIRCLE, pPixmap))
        {
            m_pBrush = _getNewAutoTileBrush();
            m_pCurrentTile->setPixmap(*pPixmap);
        }
        else
            return;
    }
    else  if (type == BRUSH_TILE_SET)
    {
        if (uiID)
        {
            m_pBrush = _getNewTileSetBrush();
            DATABASE::ConstTileSetPrototypePtr proto;
            if (m_pSharedData->getTileSetDatabase() && m_pSharedData->getTileSetDatabase()->getItem(uiID, proto))
                m_pCurrentTile->setPixmap(DATABASE::TILE_SET::createTileSetPixmap(proto, m_pSharedData->getTileDatabase()));
        }
        else
            return;
    }
    else
    {
        ConstQPixmapPtr pPixmap;
        m_pBrush = _getNewTileBrush();
        if (m_pSharedData->getTileCache()->getItem(uiID, pPixmap))
            m_pCurrentTile->setPixmap(*pPixmap);
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

BrushMode MapBrushWidget::getCurrentBrushMode() const
{
    return getBrushMode(m_pMode->itemText(m_pMode->currentIndex()));
}

void MapBrushWidget::_changeDrawMode(const QString& mode)
{
    m_pBrush->setBrushMode(getBrushMode(mode));
}

MapBrushPtr MapBrushWidget::_getNewTileBrush()
{
    MapBrushPtr pNewBrush;
    if (m_pSharedData)
    {
        pNewBrush = MapBrushPtr(new MapTileBrush());
        pNewBrush->setBrushMode(getCurrentBrushMode());
        pNewBrush->setLayer(m_pSharedData->getCurrentLayer());
    }
    return pNewBrush;
}

MapBrushPtr MapBrushWidget::_getNewTileSetBrush()
{
    MapBrushPtr pNewBrush;
    if (m_pSharedData)
    {
        pNewBrush = MapBrushPtr(new MapTileSetBrush(m_pSharedData->getTileSetDatabase()));
        pNewBrush->setBrushMode(getCurrentBrushMode());
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
        pNewBrush->setBrushMode(getCurrentBrushMode());
        pNewBrush->setLayer(m_pSharedData->getCurrentLayer());
    }
    return pNewBrush;
}