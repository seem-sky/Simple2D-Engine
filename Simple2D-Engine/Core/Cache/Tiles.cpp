#include "Tiles.h"
#include "Simple2D-Global.h"
#include <math_extensions.h>
#include <QtGui/QPainter>
#include "Config.h"
#include "QtGlobal.h"
#include "DatabaseMgr.h"
#include <log/Log.h>

using namespace CACHE;

Tiles::Tiles(const DATABASE::DatabaseMgr& DBMgr) : m_DBMgr(DBMgr), m_Atlas(1024, 1024)
{}

TileCacheInfo Tiles::get(uint32 ID) const
{
    if (!m_Atlas.isEmpty(ID))
        return TileCacheInfo(&m_Atlas.getTexture(), m_Atlas.get(ID) * MAP::TILE_SIZE);

    if (auto pTexture = m_DBMgr.getTileDatabase()->getOriginalPrototype(ID))
    {
        QPixmap pixmap;
        if (createPixmap(Config::get()->getProjectDirectory(), pTexture->getPathName(), pTexture->getTransparencyColor(), pixmap))
        {
            const_cast<CACHE::TextureAtlas<MAP::TILE_SIZE, MAP::TILE_SIZE>&>(m_Atlas).add(ID, pixmap);
            auto pos = m_Atlas.get(ID);
            STANDARD_MESSAGE(std::string("TileCache: ID: ") + std::to_string(pTexture->getID()) + " // name: " + pTexture->getName().toStdString() + " // path: " +
                pTexture->getPath().toStdString() + "\n" + "Added into texture atlas at position " + std::to_string(pos.getX()) + "/" + std::to_string(pos.getY()) + ".");
            return TileCacheInfo(&m_Atlas.getTexture(), pos * MAP::TILE_SIZE);
        }
    }
    if (ID)
        WARNING_MESSAGE(std::string("TileCache: ID: ") + std::to_string(ID) + "\nUnable to add tile to texture atlas.");
    return TileCacheInfo(nullptr, GEOMETRY::Point<uint32>(MATH::maximum<uint32>(), MATH::maximum<uint32>()));
}

void Tiles::clear()
{
    m_Atlas.clear();
}
