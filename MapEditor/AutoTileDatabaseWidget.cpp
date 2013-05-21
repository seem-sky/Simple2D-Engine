#include "AutoTileDatabaseWidget.h"
#include "moc_AutoTileDatabaseWidget.h"
#include <QtGui/QMessageBox>
#include <QtGui/QBitmap>
#include "Config.h"
#include <QtGui/QGraphicsItem>
#include <QtGui/QMouseEvent>

using namespace DATABASE;

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
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
        {
            connect(pLabel, SIGNAL(onDrop(uint32, const Point<int32>&)), this, SLOT(_onTileDrop(uint32, const Point<int32>&)));
            pLabel->resize(TILE_SIZE, TILE_SIZE);
        }
    }
}

TileDropLabel* AutoTileDatabaseWidget::_getLabelForIndex(AutoTilePrototype::AUTO_TILE_INDEX index)
{
    switch (index)
    {
    case AutoTilePrototype::INDEX_TOP_LEFT: return m_pTopLeft;
    case AutoTilePrototype::INDEX_TOP: return m_pTopCenter;
    case AutoTilePrototype::INDEX_TOP_RIGHT: return m_pTopRight;
    case AutoTilePrototype::INDEX_LEFT: return m_pCenterLeft;
    case AutoTilePrototype::INDEX_CENTER: return m_pCenter;
    case AutoTilePrototype::INDEX_RIGHT: return m_pCenterRight;
    case AutoTilePrototype::INDEX_BOTTOM_LEFT: return m_pBottomLeft;
    case AutoTilePrototype::INDEX_BOTTOM: return m_pBottomCenter;
    case AutoTilePrototype::INDEX_BOTTOM_RIGHT: return m_pBottomRight;
    case AutoTilePrototype::INDEX_INNER_CENTER: return m_pInnerEdges;
    }
    return NULL;
}

AutoTilePrototype::AUTO_TILE_INDEX AutoTileDatabaseWidget::_getIndexForLabel(TileDropLabel *pLabel)
{
    if (pLabel)
    {
        for (uint32 i = 0; i < AutoTilePrototype::INDEX_NONE; ++i)
        {
            if (pLabel == _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
                return static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i);
        }
    }
    return AutoTilePrototype::INDEX_NONE;
}

void AutoTileDatabaseWidget::clearWidgets()
{
    DatabaseWidget::clearWidgets();
    // clear labels
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
            pLabel->clear();
    }
}

void AutoTileDatabaseWidget::setTileDB(const ConstTileDatabaseChangerPtr &pDB)
{
    m_pTileDB = pDB;
    m_pTileList->setDB(m_pTileDB);
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
            pLabel->setTileDB(pDB);
    }
}

void AutoTileDatabaseWidget::setFocus()
{
    m_pTileList->fillWithPrototypes();
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
            pLabel->drawCurrentTile();
    }
}

bool AutoTileDatabaseWidget::setWidgetsFromPrototype(const AutoTilePrototypePtr &proto)
{
    if (!DatabaseWidget::setWidgetsFromPrototype(proto) || !m_pTileDB)
        return false;

    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
            pLabel->setCurrentTileID(proto->getTileID(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)));
    }
    return true;
}

bool AutoTileDatabaseWidget::getPrototypeFromWidgets(AutoTilePrototypePtr &proto)
{
    if (!DatabaseWidget::getPrototypeFromWidgets(proto))
        return false;

    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel *pLabel = _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
            proto->setTileID(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pLabel->getCurrentTileID());
    }
    return true;
}

void AutoTileDatabaseWidget::_onTileDrop(uint32 uiID, const Point<int32> &pos)
{
    updateItem();
}