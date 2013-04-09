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
    connectWidgets();
    m_pTileList->sortByColumn(0, Qt::AscendingOrder);

    connect(m_pTileList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(_tileDoubleClicked(QTreeWidgetItem*, int)));
    connect(m_pTileList, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(_changeSelectedTile(QTreeWidgetItem*, QTreeWidgetItem*)));
    // install event filter for auto tile labels
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (QLabel *pLabel = _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
        {
            pLabel->installEventFilter(this);
            pLabel->resize(TILE_SIZE, TILE_SIZE);
        }
    }
}

QLabel* AutoTileDatabaseWidget::_getLabelForIndex(AutoTilePrototype::AUTO_TILE_INDEX index)
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

AutoTilePrototype::AUTO_TILE_INDEX AutoTileDatabaseWidget::_getIndexForLabel(QLabel *pLabel)
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

bool AutoTileDatabaseWidget::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (!pObj || !pEvent)
        return false;

    if (pEvent->type() == QEvent::MouseButtonPress)
    {
        if (QMouseEvent *pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent))
        {
            if (pMouseEvent->button() == Qt::LeftButton)
            {
                for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
                {
                    if (pObj == _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
                        _setCurrentLabel(_getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)));
                }
            }
        }
    }

    return QWidget::eventFilter(pObj, pEvent);
}

void AutoTileDatabaseWidget::_setCurrentLabel(QLabel *pLabel)
{
    if (!pLabel || m_pCurrentLabel == pLabel)
        return;

    if (m_pCurrentLabel)
        m_pCurrentLabel->setLineWidth(1);
    m_pCurrentLabel = pLabel;
    m_pCurrentLabel->setLineWidth(3);
}

void AutoTileDatabaseWidget::_tileDoubleClicked(QTreeWidgetItem *pItem, int column)
{
    AutoTilePrototype::AUTO_TILE_INDEX index = _getIndexForLabel(m_pCurrentLabel);
    if (!pItem || index == AUTO_TILE_SET_COUNT)
        return;

    uint32 uiTileID = pItem->text(0).toUInt();
    ConstTilePrototypePtr pTileProto;
    AutoTilePrototypePtr autoTileProto;
    if (!m_pTileDB->getPrototype(uiTileID, pTileProto))
        return;

    m_uiAutoTileSet[index] = pTileProto->getID();
    m_pCurrentLabel->setPixmap(_createTilePixmap(pTileProto));
    change();
}

void AutoTileDatabaseWidget::_changeSelectedTile(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
{
    ConstTilePrototypePtr proto;
    if (pCurrent && m_pTileDB && m_pTileDB->getPrototype(pCurrent->text(0).toUInt(), proto))
        _showTilePixmap(proto);
}

void AutoTileDatabaseWidget::_fillTileList()
{
    m_pTileList->clear();
    if (!m_pTileDB)
        return;

    UInt32StdStringMap tileList;
    m_pTileDB->getPrototypeShortInfos(tileList);
    for (UInt32StdStringMap::const_iterator itr = tileList.begin(); itr != tileList.end(); ++itr)
    {
        QStringList stringList;
        stringList.push_back(QString::number(itr->first));
        stringList.push_back(QString::fromStdString(itr->second));
        m_pTileList->addTopLevelItem(new PrototypeTreeWidgetItem(stringList));
    }

    // select topLevelItem
    if (QTreeWidgetItem *pItem = m_pTileList->topLevelItem(0))
        m_pTileList->setCurrentItem(pItem);
}

void AutoTileDatabaseWidget::_showTilePixmap(const ConstTilePrototypePtr &proto)
{
    QGraphicsScene *pScene = new QGraphicsScene();
    pScene->addPixmap(_createTilePixmap(proto));
    m_pTileView->setScene(pScene);    
}

QPixmap AutoTileDatabaseWidget::_createTilePixmap(const DATABASE::ConstTilePrototypePtr &proto)
{
    QPixmap pixmap;
    if (!proto)
        return pixmap;
    pixmap = QPixmap(QString::fromStdString(Config::Get()->getProjectDirectory() + "/Textures/" + proto->getPathName()));
    if (!pixmap.isNull())
    {
        // set transparency color
        Color color(proto->getTransparencyColor());
        if (color.hasValidColor())
            pixmap.setMask(pixmap.createMaskFromColor(QColor(color.getRed(), color.getGreen(), color.getBlue())));
    }
    return pixmap;
}

void AutoTileDatabaseWidget::clearWidgets()
{
    DatabaseWidget::clearWidgets();
    // clear labels
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (QLabel *pLabel = _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
            pLabel->clear();
    }
}

void AutoTileDatabaseWidget::setFocus()
{
    _fillTileList();
}

bool AutoTileDatabaseWidget::setWidgetsFromPrototype(const AutoTilePrototypePtr &proto)
{
    if (!DatabaseWidget::setWidgetsFromPrototype(proto) || !m_pTileDB)
        return false;

    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        m_uiAutoTileSet[i] = proto->getTileID(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i));
        ConstTilePrototypePtr tileProto;
        if (m_pTileDB->getPrototype(m_uiAutoTileSet[i], tileProto))
        {
            if (QLabel *pLabel = _getLabelForIndex(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)))
                pLabel->setPixmap(_createTilePixmap(tileProto));
        }
    }
    return true;
}

bool AutoTileDatabaseWidget::getPrototypeFromWidgets(AutoTilePrototypePtr &proto)
{
    if (!DatabaseWidget::getPrototypeFromWidgets(proto))
        return false;

    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
        proto->setTileID(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), m_uiAutoTileSet[i]);

    return true;
}