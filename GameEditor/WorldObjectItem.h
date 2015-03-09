#ifndef WORLD_OBJECT_ITEM_H
#define WORLD_OBJECT_ITEM_H

#include <QtWidgets/QGraphicsItem>
#include <geometry/Rectangle.h>
#include <FlagManager.h>
#include "WorldObjectInfo.h"
#include <QtWidgets/QMenu>

namespace database
{
    class Manager;
}

class WorldObjectItem : public QGraphicsItem, public QObject
{
private:
    QPoint _checkMove(QPoint moveTo) const;
    void _setup();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);

public:
    WorldObjectItem(MAP::MAP_DATA::WorldObjectInfo& info, const database::Manager& DBMgr);

    void setEditable(bool editable = true);

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
    void setWorldObjectInfo(const MAP::MAP_DATA::WorldObjectInfo& info);

    GEOMETRY::Rectangle<int32> getWorldObjectBoundingRect() const { return m_BoundingRect; }
    void setWorldObjectBoundingRect(const GEOMETRY::Rectangle<int32>& rect) { m_BoundingRect = rect; }

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);

    // flags
    enum class Flags
    {
        NONE                    = 0x00,
        DRAW_BOUNDING_RECT      = 0x01
    };

    FlagManager<Flags>& getFlags() { return m_Flags; }
    const FlagManager<Flags>& getFlags() const { return m_Flags; }

    int type() const;
    QRectF boundingRect() const;

private:
    MAP::MAP_DATA::WorldObjectInfo& m_WorldObjectInfo;
    GEOMETRY::Rectangle<int32> m_BoundingRect;
    FlagManager<Flags> m_Flags;
    QPixmap m_Pixmap;

    const database::Manager& m_DBMgr;
};
#endif
