#include "EditorGlobal.h"
#include <Database/Derived.h>
#include <QtGui/QPainter>
#include <Core/Cache/Tiles.h>

namespace database
{
    namespace prototype
    {
        QPixmap createPixmap(const TileSet* pTileSet, CACHE::Tiles& tileCache)
        {
            QPixmap pixmap;
            if (pTileSet)
            {
                pixmap = QPixmap(pTileSet->getSize().getWidth() * MAP::TILE_SIZE, pTileSet->getSize().getHeight() * MAP::TILE_SIZE);
                pixmap.fill();
                QPainter painter(&pixmap);
                GEOMETRY::Point<uint32> pos;
                for (pos.getX() = 0; pos.getX() < pTileSet->getSize().getWidth(); ++pos.getX())
                {
                    for (pos.getY() = 0; pos.getY() < pTileSet->getSize().getHeight(); ++pos.getY())
                    {
                        auto info = tileCache.get(pTileSet->getTileID(pos));
                        if (info.isValid())
                            painter.drawPixmap(pos.getX()*MAP::TILE_SIZE, pos.getY()*MAP::TILE_SIZE, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(),
                            MAP::TILE_SIZE, MAP::TILE_SIZE);
                    }
                }
            }
            return pixmap;
        }
    }
}
