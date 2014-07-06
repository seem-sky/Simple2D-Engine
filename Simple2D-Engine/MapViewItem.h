#ifndef MAP_VIEW_ITEM_H
#define MAP_VIEW_ITEM_H

#include <QtWidgets/QGraphicsPixmapItem>
#include "WorldObjectInfo.h"

class MapViewItem : public QGraphicsPixmapItem
{
private:
    QPoint _checkMove(QPoint moveTo) const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    void keyPressEvent(QKeyEvent* pEvent);

public:
    MapViewItem(MAP::MAP_DATA::WorldObjectInfo& info, const QPixmap& pixmap);

    void setTopLeftPos(Int32Point pos);
    void setTopLeftPos(int32 x, int32 y);
    Int32Point getTopLeftPos() const;
    void setBottomRightPos(Int32Point pos);
    void setBottomRightPos(int32 x, int32 y);
    Int32Point getBottomRightPos() const;
    void setCenterPos(Int32Point pos);
    void setCenterPos(int32 x, int32 y);
    Int32Point getCenterPos() const;

    const MAP::MAP_DATA::WorldObjectInfo& getWorldObjectInfo() const { return m_WorldObjectInfo; }

    Int32Rect getWorldObjectBoundingRect() const { return m_BoundingRect; }
    void setWorldObjectBoundingRect(const Int32Rect& rect) { m_BoundingRect = rect; }

private:
    MAP::MAP_DATA::WorldObjectInfo& m_WorldObjectInfo;
    Int32Rect m_BoundingRect;
};
#endif
