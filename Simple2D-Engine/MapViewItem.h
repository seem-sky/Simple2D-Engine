#ifndef MAP_VIEW_ITEM_H
#define MAP_VIEW_ITEM_H

#include <QtWidgets/QGraphicsPixmapItem>
#include <geometry/Rectangle.h>
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

    void setTopLeftPos(GEOMETRY::Point<int32> pos);
    void setTopLeftPos(int32 x, int32 y);
    GEOMETRY::Point<int32> getTopLeftPos() const;
    void setBottomRightPos(GEOMETRY::Point<int32> pos);
    void setBottomRightPos(int32 x, int32 y);
    GEOMETRY::Point<int32> getBottomRightPos() const;
    void setCenterPos(GEOMETRY::Point<int32> pos);
    void setCenterPos(int32 x, int32 y);
    GEOMETRY::Point<int32> getCenterPos() const;

    const MAP::MAP_DATA::WorldObjectInfo& getWorldObjectInfo() const { return m_WorldObjectInfo; }

    GEOMETRY::Rectangle<int32> getWorldObjectBoundingRect() const { return m_BoundingRect; }
    void setWorldObjectBoundingRect(const GEOMETRY::Rectangle<int32>& rect) { m_BoundingRect = rect; }

private:
    MAP::MAP_DATA::WorldObjectInfo& m_WorldObjectInfo;
    GEOMETRY::Rectangle<int32> m_BoundingRect;
};
#endif
