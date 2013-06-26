#include "AutoTileDatabaseWidget.h"
#include "moc_AutoTileDatabaseWidget.h"
#include <QtWidgets/QMessageBox>
#include <QtGui/QBitmap>
#include "Config.h"
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QMouseEvent>

using namespace DATABASE;
using namespace AUTO_TILE;

AutoTileDatabaseWidget::AutoTileDatabaseWidget(QWidget *pParent) : DatabaseWidget(pParent), Ui_AutoTileDatabaseWidget(), m_pCurrentLabel(NULL)
{
    memset(&m_uiAutoTileSet, NULL, sizeof(m_uiAutoTileSet));
    Ui_AutoTileDatabaseWidget::setupUi(this);
    m_ModObj.setWidget(m_pTileList, MODIFY_RESIZE, QPoint(0, 5), MODIFY_DIRECTION_HEIGHT);
    connectWidgets();
    m_pTileList->sortByColumn(0, Qt::AscendingOrder);

    connect(m_pTileList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(_tileDoubleClicked(QTreeWidgetItem*, int)));
    // connect auto tile labels
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AUTO_TILE_INDEX>(i)))
        {
            connect(pLabel, SIGNAL(onDrop(uint32, const Point<int32>&)), this, SLOT(_onTileDrop(uint32, const Point<int32>&)));
            pLabel->resize(TILE_SIZE, TILE_SIZE);
        }
    }
}

TileDropLabel* AutoTileDatabaseWidget::_getLabelForIndex(AUTO_TILE::AUTO_TILE_INDEX index)
{
    switch (index)
    {
    case INDEX_TOP_LEFT: return m_pTopLeft;
    case INDEX_TOP: return m_pTopCenter;
    case INDEX_TOP_RIGHT: return m_pTopRight;
    case INDEX_LEFT: return m_pCenterLeft;
    case INDEX_CENTER: return m_pCenter;
    case INDEX_RIGHT: return m_pCenterRight;
    case INDEX_BOTTOM_LEFT: return m_pBottomLeft;
    case INDEX_BOTTOM: return m_pBottomCenter;
    case INDEX_BOTTOM_RIGHT: return m_pBottomRight;
    case INDEX_INNER_CENTER: return m_pInnerEdges;
    }
    return NULL;
}

AUTO_TILE_INDEX AutoTileDatabaseWidget::_getIndexForLabel(TileDropLabel *pLabel)
{
    if (pLabel)
    {
        for (uint32 i = 0; i < INDEX_NONE; ++i)
        {
            if (pLabel == _getLabelForIndex(static_cast<AUTO_TILE_INDEX>(i)))
                return static_cast<AUTO_TILE_INDEX>(i);
        }
    }
    return INDEX_NONE;
}

void AutoTileDatabaseWidget::clearWidgets()
{
    DatabaseWidget::clearWidgets();
    // clear labels
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AUTO_TILE_INDEX>(i)))
            pLabel->clear();
    }
}

void AutoTileDatabaseWidget::setTileDB(const ConstTileDatabaseChangerPtr &pDB)
{
    m_pTileDB = pDB;
    m_pTileList->setDB(m_pTileDB);
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AUTO_TILE_INDEX>(i)))
            pLabel->setTileDB(pDB);
    }
}

void AutoTileDatabaseWidget::setFocus()
{
    m_pTileList->fillWithPrototypes();
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AUTO_TILE_INDEX>(i)))
            pLabel->drawCurrentTile();
    }
}

bool AutoTileDatabaseWidget::setWidgetsFromPrototype(const AutoTilePrototypePtr &proto)
{
    if (!DatabaseWidget::setWidgetsFromPrototype(proto) || !m_pTileDB)
        return false;

    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AUTO_TILE_INDEX>(i)))
            pLabel->setCurrentTileID(proto->getTileID(static_cast<AUTO_TILE_INDEX>(i)));
    }
    return true;
}

bool AutoTileDatabaseWidget::getItemFromWidgets(AutoTilePrototypePtr &proto)
{
    if (!DatabaseWidget::getItemFromWidgets(proto))
        return false;

    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AUTO_TILE_INDEX>(i)))
            proto->setTileID(static_cast<AUTO_TILE_INDEX>(i), pLabel->getCurrentTileID());
    }
    return true;
}

void AutoTileDatabaseWidget::_onTileDrop(uint32 uiID, const Point<int32> &pos)
{
    updateItem();
}