#ifndef DATABASE_MODULE_DRAG_DROP_LIST_H
#define DATABASE_MODULE_DRAG_DROP_LIST_H

#include <QtWidgets/QTreeView>
#include "DatabaseModel.h"
#include <QtWidgets/QLabel>
#include <QtCore/QTimer>

class DatabaseModuleTooltipList : public QTreeView
{
    Q_OBJECT    
private:
    void _endTracking();
    virtual QWidget* _setupTooltipWidget(uint32 uiPrototypeID) { return new QWidget(this); }
    //void _beginTracking(QTreeWidgetItem* pItem);

protected:
    virtual void leaveEvent(QEvent* pEvent);
    virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous);

public:
    DatabaseModuleTooltipList(QWidget* pParent = nullptr);

    void setModel(DATABASE::ConstDatabaseModel* pModel);

    // tooltip
    inline void setMaximumTooltipSize(const QSize& size) { m_MaximumTooltipSize = size; }
    inline QSize getMaximumTooltipSize() const { return m_MaximumTooltipSize; }

    void setShowTooltip(bool show = true);
    inline bool showTooltip() const { return m_ShowTooltip; }

    enum class ToolTipPosition
    {
        TOOLTIP_LEFT,
        TOOLTIP_RIGHT
    };
    void setToolTipPosition(ToolTipPosition pos) { m_ToolTipPos = pos; }

    uint32 getSelectedID() const;

private slots:
    void _showTooltip();
    void _onItemEntered(const QModelIndex& index);

signals:
    void changeIndex(int32 index);

private:
    QWidget* m_pToolTip;
    uint32 m_CurrentRow;
    QTimer m_Timer;
    QSize m_MaximumTooltipSize;
    ToolTipPosition m_ToolTipPos; 
    bool m_ShowTooltip;
};

/*#####
# Texture drag&drop
#####*/
class DatabaseModuleTextureDragList : public DatabaseModuleTooltipList
{
private:
    QWidget* _setupTooltipWidget(uint32 uiPrototypeID);

protected:
    void startDrag(Qt::DropActions supportedActions);

public:
    DatabaseModuleTextureDragList(QWidget* pParent = nullptr);
};

class TileDropLabel : public QLabel
{
    Q_OBJECT
protected:
    void dragMoveEvent(QDragMoveEvent* pEvent);
    void dragEnterEvent(QDragEnterEvent* pEvent);
    void dropEvent(QDropEvent* pEvent);
    void paintEvent(QPaintEvent* pEvent);

public:
    TileDropLabel(QWidget* pParent = nullptr);

    inline uint32 getCurrentTileID() const { return m_uiCurrentTileID; }
    inline void setCurrentTileID(uint32 uiTileID) { m_uiCurrentTileID = uiTileID; repaint(); }

    inline void setTileDatabase(const DATABASE::TileDatabase* pDB) { m_pTileDB = pDB; }

signals:
    void onDrop(uint32 uiID, const GEOMETRY::Point<int32>& pos);

private:
    uint32 m_uiCurrentTileID;
    const DATABASE::TileDatabase* m_pTileDB;
};

/*#####
# Animation drag&drop
#####*/
class DatabaseModuleAnimationDragList : public DatabaseModuleTooltipList
{
private:
    QWidget* _setupTooltipWidget(uint32 uiPrototypeID);

protected:
    void startDrag(Qt::DropActions supportedActions);

public:
    DatabaseModuleAnimationDragList(QWidget* pParent = nullptr);

    void setSpriteDatabase(const DATABASE::SpriteDatabase* pSpriteDB);

private:
    const DATABASE::SpriteDatabase* m_pSpriteDB;
};

#endif
