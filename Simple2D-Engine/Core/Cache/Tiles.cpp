#include "Tiles.h"
#include "Simple2D-Global.h"
#include <math_extensions.h>
#include <QtGui/QPainter>
#include "Config.h"
#include "QtGlobal.h"
#include "Database/Manager.h"
#include <log/Log.h>
#include <math.h>

using namespace CACHE;

Tiles::Tiles(const database::Manager& DBMgr) : TileBase(DBMgr)
{}

TileCacheInfo Tiles::get(uint32 ID) const
{
    if (!isEmpty(ID))
    {
        auto& info = m_Positions.at(ID - 1);
        return TileCacheInfo(info.getPixmap(), info.getPosition() * MAP::TILE_SIZE);
    }

    if (auto pTexture = m_DBMgr.getTileDatabase().getPrototype(ID))
    {
        QPixmap pixmap;
        if (createPixmap(Config::get()->getProjectDirectory() + "/Textures/" + pTexture->getPathName(), pTexture->getTransparencyColor(), pixmap))
        {
            auto info = const_cast<Tiles&>(*this).add(ID, pixmap);
            STANDARD_MESSAGE(std::string("TileCache: ID: ") + std::to_string(pTexture->getID()) + " // name: " + pTexture->getName().toStdString() + " // path: " +
                pTexture->getPath().toStdString() + "\n" + "Added into texture atlas at position " +
                std::to_string(info.getPosition().getX()) + "/" + std::to_string(info.getPosition().getY()) + ".");
            return TileCacheInfo(info.getPixmap(), info.getPosition() * MAP::TILE_SIZE);
        }
    }
    
    if (ID)
        WARNING_MESSAGE(std::string("TileCache: ID: ") + std::to_string(ID) + "\nUnable to add tile to texture atlas.");
    return TileCacheInfo(nullptr, GEOMETRY::Point<uint32>(MATH::maximum<uint32>(), MATH::maximum<uint32>()));
}
