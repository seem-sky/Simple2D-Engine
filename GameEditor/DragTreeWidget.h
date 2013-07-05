#ifndef DRAG_TREE_WIDGET_H
#define DRAG_TREE_WIDGET_H

#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QLabel>
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include "DatabaseChanger.h"
#include "AnimationViewWidget.h"
#include "Config.h"
#include "PrototypeTreeWidgetItem.h"

/*#####
# PixmapTooltipTreeWidget
#####*/
typedef std::shared_ptr<QWidget> QWidgetPtr;
class PixmapTooltipTreeWidget : public QTreeWidget
{
    Q_OBJECT
private:
    void _endTracking();
    //void _beginTracking(QTreeWidgetItem* pItem);

protected:
    virtual void leaveEvent(QEvent *pEvent);
    virtual QWidget* setTooltipWidget(uint32 uiPrototypeID) { return new QWidget(this); };

public:
    PixmapTooltipTreeWidget(QWidget *pParent = NULL);

    inline void setTooltipSize(const QSize &size) { m_TooltipSize = size; }
    inline QSize getTooltipSize() const { return m_TooltipSize; }

    inline void setShowTooltip(bool show = true) { m_ShowTooltip = show; }
    inline bool showTooltip() const { return m_ShowTooltip; }

    enum ToolTipPosition
    {
        TOOLTIP_LEFT,
        TOOLTIP_RIGHT
    };
    void setToolTipPosition(ToolTipPosition pos) { m_ToolTipPos = pos; }

private slots:
    void _showPixmapTooltip();
    void _onItemEnter(QTreeWidgetItem *pItem, int column);

protected:
    QWidgetPtr m_pTooltipViewer;

private:
    QTreeWidgetItem *m_pCurrentItem;
    QTimer m_Timer;
    QSize m_TooltipSize;
    ToolTipPosition m_ToolTipPos; 
    bool m_ShowTooltip;
};

/*#####
# PrototypeTreeWidget
#####*/
template <class T>
class PrototypeTreeWidget : public PixmapTooltipTreeWidget
{
public:
    PrototypeTreeWidget(QWidget *pParent = NULL) : PixmapTooltipTreeWidget(pParent), m_showNULLPrototype(false)
    {
        setSortingEnabled(true);
        QStringList sHeader;
        sHeader.push_back("ID");
        sHeader.push_back("Name");
        setColumnCount(0);
        setHeaderLabels(sHeader);
        sortByColumn(0, Qt::AscendingOrder);
        setColumnWidth(0, 50);
    }

    void fillWithPrototypes()
    {
        clear();
        if (!m_pDB)
            return;

        if (m_showNULLPrototype)
        {
            QStringList stringList;
            stringList.push_back(QString::number(0));
            stringList.push_back("empty prototype");
            addTopLevelItem(new PrototypeTreeWidgetItem(stringList));
        }

        UInt32StringMap spriteList;
        m_pDB->getItemShortInfos(spriteList);
        for (UInt32StringMap::const_iterator itr = spriteList.begin(); itr != spriteList.end(); ++itr)
        {
            QStringList stringList;
            stringList.push_back(QString::number(itr->first));
            stringList.push_back(itr->second);
            addTopLevelItem(new PrototypeTreeWidgetItem(stringList));
        }

        // select topLevelItem
        if (QTreeWidgetItem *pItem = topLevelItem(0))
            setCurrentItem(pItem);
    }

    inline void setDB(std::shared_ptr<const T> pDB) { m_pDB = pDB; fillWithPrototypes(); }

    void showNULLPrototype(bool show = true)
    {
        if (show != m_showNULLPrototype)
        {
            m_showNULLPrototype = show;
            fillWithPrototypes();
        }
    }

private:
    bool m_showNULLPrototype;

protected:
    std::shared_ptr<const T> m_pDB;
};
typedef PrototypeTreeWidget<DATABASE::DatabaseChanger<DATABASE::ObjectAnimationTypePrototype>> ObjectAnimationTypeTreeWidget;

/*#####
# AnimationPrototypeDragTreeWidget
#####*/
class AnimationPrototypeDragTreeWidget : public PrototypeTreeWidget<DATABASE::DatabaseChanger<DATABASE::AnimationPrototype>>
{
protected:
    QWidget* setTooltipWidget(uint32 uiPrototypeID);

public:
    AnimationPrototypeDragTreeWidget(QWidget *pParent = NULL);

    inline void setSpriteDB(DATABASE::ConstSpriteDatabaseChangerPtr pSpriteDB) { m_pSpriteDB = pSpriteDB; }

private:
    DATABASE::ConstSpriteDatabaseChangerPtr m_pSpriteDB;
};

/*#####
# TexturePrototypeDragTreeWidget
#####*/
template<class T>
class TexturePrototypeDragTreeWidget : public PrototypeTreeWidget<DATABASE::DatabaseChanger<T>>
{
protected:
    void startDrag(Qt::DropActions supportedActions)
    {
        if (QTreeWidgetItem *pItem = currentItem())
        {
            QPixmap pixmap;
            std::shared_ptr<const T> proto;
            if (m_pDB && m_pDB->getItem(pItem->text(0).toUInt(), proto) && createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), proto, pixmap))
            {
                QDrag* pDrag = new QDrag(this);
                pDrag->setPixmap(pixmap);
                pDrag->setMimeData(new QMimeData());
                pDrag->exec(supportedActions);
                return;
            }
        }
        QTreeWidget::startDrag(supportedActions);
    }

    QWidget* setTooltipWidget(uint32 uiPrototypeID)
    {
        std::shared_ptr<const T> proto;
        QPixmap pixmap;
        if (m_pDB && m_pDB->getItem(uiPrototypeID, proto) && createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), proto, pixmap))
        {
            QGraphicsView *pViewer = new QGraphicsView(this);
            pViewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            pViewer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            QGraphicsScene *pScene = new QGraphicsScene();
            pScene->addPixmap(pixmap);
            pViewer->setScene(pScene);
            setTooltipSize(pixmap.size());
            return pViewer;
        }
        return NULL;
    }

public:
    TexturePrototypeDragTreeWidget(QWidget *pParent = NULL) : PrototypeTreeWidget(pParent)
    {
        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);
    }
};

/*#####
# TileSetPrototypeDragTreeWidget
#####*/
class TileSetPrototypeDragTreeWidget : public PrototypeTreeWidget<DATABASE::Database<DATABASE::TILE_SET::TileSetPrototype>>
{
    Q_OBJECT
protected:
    QWidget* setTooltipWidget(uint32 uiPrototypeID);
    void mousePressEvent(QMouseEvent *pEvent);

public:
    TileSetPrototypeDragTreeWidget(QWidget *pParent = NULL);

    inline void setAdditionalDBs(DATABASE::ConstTileDatabasePtr pTileDB)
    {
        m_pTileDB = pTileDB;
    }

signals:
    void itemClicked(uint32 uiID, Qt::MouseButton button);

private:
    DATABASE::ConstTileDatabasePtr m_pTileDB;
};

/*#####
# WorldObjectPrototypeDragTreeWidget
#####*/
class WorldObjectPrototypeDragTreeWidget : public PrototypeTreeWidget<DATABASE::Database<DATABASE::WorldObjectPrototype>>
{
    Q_OBJECT
protected:
    QWidget* setTooltipWidget(uint32 uiPrototypeID);
    void leaveEvent(QEvent *pEvent);

public:
    WorldObjectPrototypeDragTreeWidget(QWidget *pParent = NULL);

    void setAdditionalDBs(DATABASE::ConstSpriteDatabasePtr pSpriteDB, DATABASE::ConstAnimationDatabasePtr pAnimationDB)
    {
        m_pSpriteDB = pSpriteDB;
        m_pAnimationDB = pAnimationDB;
    }

signals:
    void newCursorSet(const QCursor &cursor);

private:
    DATABASE::ConstSpriteDatabasePtr m_pSpriteDB;
    DATABASE::ConstAnimationDatabasePtr m_pAnimationDB;
};

class TileDropLabel : public QLabel
{
    Q_OBJECT

protected:
    void dragMoveEvent(QDragMoveEvent *pEvent);
    void dragEnterEvent(QDragEnterEvent *pEvent);
    void dropEvent(QDropEvent *pEvent);

public:
    TileDropLabel(QWidget *pParent = NULL);

    void drawCurrentTile();
    inline void setTileDB(const DATABASE::ConstTileDatabaseChangerPtr &pTileDB) { m_pTileDB = pTileDB; drawCurrentTile(); }

    inline uint32 getCurrentTileID() const { return m_uiCurrentTileID; }
    inline void setCurrentTileID(uint32 uiTileID) { m_uiCurrentTileID = uiTileID; drawCurrentTile(); }

signals:
    void onDrop(uint32 uiID, const Int32Point &pos);

private:
    DATABASE::ConstTileDatabaseChangerPtr m_pTileDB;
    uint32 m_uiCurrentTileID;
};
typedef TexturePrototypeDragTreeWidget<DATABASE::TilePrototype> TilePrototypeDragTreeWidget;
typedef TexturePrototypeDragTreeWidget<DATABASE::SpritePrototype> SpritePrototypeDragTreeWidget;
#endif