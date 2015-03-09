#ifndef EDITOR_GLOBAL_H
#define EDITOR_GLOBAL_H

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPixmap>

namespace MAPPING_MODE
{
    enum class Type
    {
        TILE_MAPPING,
        OBJECT_MAPPING,
        SCRIPT_AREA_MAPPING,
        PRESENTATION
    };

    enum QGraphicsItemType
    {
        ITEM_WORLD_OBJECT = QGraphicsItem::UserType + 1,
        ITEM_SCRIPT_AREA = QGraphicsItem::UserType + 2,
        ITEM_MOVE_POINT = QGraphicsItem::UserType + 3
    };
}

namespace CACHE
{
    class Tiles;
}

namespace database
{
    namespace prototype
    {
        class TileSet;
        QPixmap createPixmap(const TileSet* pTileSet, CACHE::Tiles& tileCache);
    }
}

#endif
