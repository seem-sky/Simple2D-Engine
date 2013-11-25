#ifndef DATABASE_MODULE_DRAG_DROP_LIST_H
#define DATABASE_MODULE_DRAG_DROP_LIST_H

#include <QtWidgets/QTreeView>
#include "DatabaseModel.h"
#include <QtWidgets/QLabel>
#include <QtCore/QTimer>

class DatabaseModuleDragList : public QTreeView
{
    Q_OBJECT    
private:
    void _endTracking();
    virtual QWidget* _setupTooltipWidget(uint32 uiPrototypeID) { return new QWidget(this); }
    //void _beginTracking(QTreeWidgetItem* pItem);

protected:
    virtual void leaveEvent(QEvent *pEvent);

public:
    DatabaseModuleDragList(QWidget *pParent = nullptr);

    void setModel(IDatabaseModel *pModel);

    // tooltip
    inline void setMaximumTooltipSize(const QSize &size) { m_MaximumTooltipSize = size; }
    inline QSize getMaximumTooltipSize() const { return m_MaximumTooltipSize; }

    void setShowTooltip(bool show = true);
    inline bool showTooltip() const { return m_ShowTooltip; }

    enum ToolTipPosition
    {
        TOOLTIP_LEFT,
        TOOLTIP_RIGHT
    };
    void setToolTipPosition(ToolTipPosition pos) { m_ToolTipPos = pos; }

private slots:
    void _showTooltip();
    void _onItemEntered(const QModelIndex &index);

private:
    QWidget *m_pToolTip;
    uint32 m_CurrentRow;
    QTimer m_Timer;
    QSize m_MaximumTooltipSize;
    ToolTipPosition m_ToolTipPos; 
    bool m_ShowTooltip;
};

/*#####
# Texture drag&drop
#####*/
class DatabaseModuleTextureDragList : public DatabaseModuleDragList
{
private:
    QWidget* _setupTooltipWidget(uint32 uiPrototypeID);

protected:
    void startDrag(Qt::DropActions supportedActions);

public:
    DatabaseModuleTextureDragList(QWidget *pParent = nullptr);
};

class TileDropLabel : public QLabel
{
    Q_OBJECT
protected:
    void dragMoveEvent(QDragMoveEvent *pEvent);
    void dragEnterEvent(QDragEnterEvent *pEvent);
    void dropEvent(QDropEvent *pEvent);
    void paintEvent(QPaintEvent *pEvent);

public:
    TileDropLabel(QWidget *pParent = NULL);

    inline uint32 getCurrentTileID() const { return m_uiCurrentTileID; }
    inline void setCurrentTileID(uint32 uiTileID) { m_uiCurrentTileID = uiTileID; }

    inline void setTileDatabase(const DATABASE::TileDatabase *pDB) { m_pTileDB = pDB; }

signals:
    void onDrop(uint32 uiID, const Int32Point &pos);

private:
    uint32 m_uiCurrentTileID;
    const DATABASE::TileDatabase *m_pTileDB;
};

#endif
