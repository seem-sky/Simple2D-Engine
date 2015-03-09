#include "AutoTiles.h"
#include "Simple2D-Global.h"
#include <math_extensions.h>
#include <QtGui/QPainter>
#include "Tiles.h"
#include <log/Log.h>

using namespace CACHE;
using namespace MAP;
using namespace database::prototype;
using namespace DATABASE::PROTOTYPE::AUTO_TILE;

// free functions
void drawOnAtlas(QPainter& painter, const QPixmap& tile, uint32 index)
{
    if (index < AUTO_TILE_SET_COUNT)
        return;

    index -= AUTO_TILE_SET_COUNT;
    GEOMETRY::Point<uint32> pos(index % ATLAS_WIDTH, index / ATLAS_WIDTH);
    painter.drawPixmap(pos.getX() * MAP::TILE_SIZE, pos.getY() * MAP::TILE_SIZE, tile);
}

enum class Position
{
    top,
    bottom,
    left,
    right,

    topLeft,
    topRight,
    bottomLeft,
    bottomRight,

    full
};

void drawOnPixmap(QPainter& painter, const CACHE::TileCacheInfo& info, Position pos)
{
    if (!info.isValid())
        return;

    switch (pos)
    {
    case Position::top:
        painter.drawPixmap(0, 0, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(), TILE_SIZE, TILE_SIZE / 2);
        break;

    case Position::bottom:
        painter.drawPixmap(0, TILE_SIZE / 2, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY() + TILE_SIZE / 2, TILE_SIZE, TILE_SIZE / 2);
        break;

    case Position::left:
        painter.drawPixmap(0, 0, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(), TILE_SIZE / 2, TILE_SIZE);
        break;

    case Position::right:
        painter.drawPixmap(TILE_SIZE / 2, 0, *info.getPixmap(), info.getPosition().getX() + TILE_SIZE / 2, info.getPosition().getY(), TILE_SIZE / 2, TILE_SIZE);
        break;

    case Position::topLeft:
        painter.drawPixmap(0, 0, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(), TILE_SIZE / 2, TILE_SIZE / 2);
        break;

    case Position::topRight:
        painter.drawPixmap(TILE_SIZE / 2, 0, *info.getPixmap(), info.getPosition().getX() + TILE_SIZE / 2, info.getPosition().getY(), TILE_SIZE / 2, TILE_SIZE / 2);
        break;

    case Position::bottomLeft:
        painter.drawPixmap(0, TILE_SIZE / 2, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY() + TILE_SIZE / 2, TILE_SIZE / 2, TILE_SIZE / 2);
        break;

    case Position::bottomRight:
        painter.drawPixmap(TILE_SIZE / 2, TILE_SIZE / 2, *info.getPixmap(), info.getPosition().getX() + TILE_SIZE / 2,
            info.getPosition().getY() + TILE_SIZE / 2, TILE_SIZE / 2, TILE_SIZE / 2);
        break;

    case Position::full:
        painter.drawPixmap(0, 0, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(), TILE_SIZE, TILE_SIZE);
    }
}

void setupMultiInnerEdges(QPainter& painter, const AutoTile* pAutoTile, CACHE::Tiles& tileCache)
{
    auto innerCenter = tileCache.get(pAutoTile->getTileID(AutoTile::Index::InnerCenter));
    auto center = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Center));
    for (uint32 i = INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT; i <= INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT; ++i)
    {
        QPixmap tempPix(TILE_SIZE, TILE_SIZE);
        tempPix.fill(Qt::transparent);
        QPainter pixmapPainter(&tempPix);

        switch (i)
        {
        case INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::topLeft);
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomRight);
            drawOnPixmap(pixmapPainter, center, Position::topRight);
            drawOnPixmap(pixmapPainter, center, Position::bottomLeft);
            break;

        case INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::topRight);
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomLeft);
            drawOnPixmap(pixmapPainter, center, Position::topLeft);
            drawOnPixmap(pixmapPainter, center, Position::bottomRight);
            break;

        case INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::top);
            drawOnPixmap(pixmapPainter, center, Position::bottom);
            break;

        case INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::left);
            drawOnPixmap(pixmapPainter, center, Position::right);
            break;

        case INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::right);
            drawOnPixmap(pixmapPainter, center, Position::left);
            break;

        case INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottom);
            drawOnPixmap(pixmapPainter, center, Position::top);
            break;
        }
        drawOnAtlas(painter, tempPix, i);
    }
}

void setupDoubleSides(QPainter& painter, const AutoTile* pAutoTile, CACHE::Tiles& tileCache)
{
    for (uint32 i = INDEX_SIDE_VERTICAL; i <= INDEX_SIDE_HORIZONTAL; ++i)
    {
        QPixmap tempPix(TILE_SIZE, TILE_SIZE);
        tempPix.fill(Qt::transparent);
        QPainter pixmapPainter(&tempPix);
        switch (i)
        {
        case INDEX_SIDE_VERTICAL:
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::Right)), Position::right);
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::Left)), Position::left);
            break;
        case INDEX_SIDE_HORIZONTAL:
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::Bottom)), Position::bottom);
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::Top)), Position::top);
            break;
        }

        drawOnAtlas(painter, tempPix, i);
    }
}

void setupCircle(QPainter& painter, const AutoTile* pAutoTile, CACHE::Tiles& tileCache)
{
    QPixmap tempPix(TILE_SIZE, TILE_SIZE);
    tempPix.fill(Qt::transparent);
    QPainter pixmapPainter(&tempPix);
    drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::TopLeft)), Position::topLeft);
    drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::TopRight)), Position::topRight);
    drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::BottomLeft)), Position::bottomLeft);
    drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::BottomRight)), Position::bottomRight);
    drawOnAtlas(painter, tempPix, INDEX_CIRCLE);
}

void setupInnerEdges(QPainter& painter, const AutoTile* pAutoTile, CACHE::Tiles& tileCache)
{
    auto innerCenter = tileCache.get(pAutoTile->getTileID(AutoTile::Index::InnerCenter));
    auto center = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Center));
    for (uint32 i = INDEX_INNER_EDGE_TOP_LEFT; i <= INDEX_INNER_EDGE_BOTTOM_RIGHT; ++i)
    {
        QPixmap tempPix(TILE_SIZE, TILE_SIZE);
        tempPix.fill(Qt::transparent);
        QPainter pixmapPainter(&tempPix);
        switch (i)
        {
        case INDEX_INNER_EDGE_TOP_LEFT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::topLeft);
            drawOnPixmap(pixmapPainter, center, Position::topRight);
            drawOnPixmap(pixmapPainter, center, Position::bottom);
            break;
        case INDEX_INNER_EDGE_TOP_RIGHT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::topRight);
            drawOnPixmap(pixmapPainter, center, Position::topLeft);
            drawOnPixmap(pixmapPainter, center, Position::bottom);
            break;
        case INDEX_INNER_EDGE_BOTTOM_LEFT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomLeft);
            drawOnPixmap(pixmapPainter, center, Position::bottomRight);
            drawOnPixmap(pixmapPainter, center, Position::top);
            break;
        case INDEX_INNER_EDGE_BOTTOM_RIGHT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomRight);
            drawOnPixmap(pixmapPainter, center, Position::bottomLeft);
            drawOnPixmap(pixmapPainter, center, Position::top);
            break;
        }
        drawOnAtlas(painter, tempPix, i);
    }
}

void setupT_Tiles(QPainter& painter, const AutoTile* pAutoTile, CACHE::Tiles& tileCache)
{
    auto innerCenter = tileCache.get(pAutoTile->getTileID(AutoTile::Index::InnerCenter));
    for (uint32 i = INDEX_T_TOP; i <= INDEX_T_RIGHT; ++i)
    {
        QPixmap tempPix(TILE_SIZE, TILE_SIZE);
        tempPix.fill(Qt::transparent);
        QPainter pixmapPainter(&tempPix);

        switch (i)
        {
        case INDEX_T_TOP:
        {
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottom);
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::Top)), Position::top);
            break;
        }
        case INDEX_T_BOTTOM:
            drawOnPixmap(pixmapPainter, innerCenter, Position::top);
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::Bottom)), Position::bottom);
            break;
        case INDEX_T_LEFT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::right);
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::Left)), Position::left);
            break;
        case INDEX_T_RIGHT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::left);
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::Right)), Position::right);
            break;
        }
        drawOnAtlas(painter, tempPix, i);
    }
}

void setupY_Tiles(QPainter& painter, const AutoTile* pAutoTile, CACHE::Tiles& tileCache)
{
    auto innerCenter = tileCache.get(pAutoTile->getTileID(AutoTile::Index::InnerCenter));
    for (uint32 i = INDEX_Y_TOP_BOTTOM_LEFT; i <= INDEX_Y_RIGHT_BOTTOM_LEFT; ++i)
    {
        QPixmap tempPix(TILE_SIZE, TILE_SIZE);
        tempPix.fill(Qt::transparent);
        QPainter pixmapPainter(&tempPix);

        switch (i)
        {
        case INDEX_Y_TOP_BOTTOM_LEFT:
        {
            auto side = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Top));
            drawOnPixmap(pixmapPainter, side, Position::top);
            drawOnPixmap(pixmapPainter, side, Position::bottomRight);
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomLeft);
            break;
        }

        case INDEX_Y_TOP_BOTTOM_RIGHT:
        {
            auto side = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Top));
            drawOnPixmap(pixmapPainter, side, Position::top);
            drawOnPixmap(pixmapPainter, side, Position::bottomLeft);
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomRight);
            break;
        }

        case INDEX_Y_RIGHT_BOTTOM_LEFT:
        {
            auto side = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Right));
            drawOnPixmap(pixmapPainter, side, Position::top);
            drawOnPixmap(pixmapPainter, side, Position::bottomRight);
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomLeft);
            break;
        }

        case INDEX_Y_RIGHT_TOP_LEFT:
        {
            auto side = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Right));
            drawOnPixmap(pixmapPainter, side, Position::right);
            drawOnPixmap(pixmapPainter, side, Position::bottomLeft);
            drawOnPixmap(pixmapPainter, innerCenter, Position::topLeft);
            break;
        }

        case INDEX_Y_LEFT_BOTTOM_RIGHT:
        {
            auto side = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Left));
            drawOnPixmap(pixmapPainter, side, Position::left);
            drawOnPixmap(pixmapPainter, side, Position::topRight);
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomRight);
            break;
        }

        case INDEX_Y_LEFT_TOP_RIGHT:
        {
            auto side = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Left));
            drawOnPixmap(pixmapPainter, side, Position::left);
            drawOnPixmap(pixmapPainter, side, Position::bottomRight);
            drawOnPixmap(pixmapPainter, innerCenter, Position::topRight);
            break;
        }

        case INDEX_Y_BOTTOM_TOP_LEFT:
        {
            auto side = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Bottom));
            drawOnPixmap(pixmapPainter, side, Position::bottom);
            drawOnPixmap(pixmapPainter, side, Position::topRight);
            drawOnPixmap(pixmapPainter, innerCenter, Position::topLeft);
            break;
        }
        
        case INDEX_Y_BOTTOM_TOP_RIGHT:
        {
            auto side = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Bottom));
            drawOnPixmap(pixmapPainter, side, Position::bottom);
            drawOnPixmap(pixmapPainter, side, Position::topLeft);
            drawOnPixmap(pixmapPainter, innerCenter, Position::topRight);
            break;
        }
        }

        drawOnAtlas(painter, tempPix, i);
    }
}

void setupCurves(QPainter& painter, const AutoTile* pAutoTile, CACHE::Tiles& tileCache)
{
    auto innerCenter = tileCache.get(pAutoTile->getTileID(AutoTile::Index::InnerCenter));
    for (uint32 i = INDEX_CURVE_TOP_LEFT; i <= INDEX_CURVE_BOTTOM_RIGHT; ++i)
    {
        QPixmap tempPix(TILE_SIZE, TILE_SIZE);
        tempPix.fill(Qt::transparent);
        QPainter pixmapPainter(&tempPix);

        switch (i)
        {
        case INDEX_CURVE_TOP_LEFT:
        {
            CACHE::TileCacheInfo edge = tileCache.get(pAutoTile->getTileID(AutoTile::Index::TopLeft));
            drawOnPixmap(pixmapPainter, edge, Position::top);
            drawOnPixmap(pixmapPainter, edge, Position::bottomLeft);
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomRight);
            break;
        }

        case INDEX_CURVE_TOP_RIGHT:
        {
            CACHE::TileCacheInfo edge = tileCache.get(pAutoTile->getTileID(AutoTile::Index::TopLeft));
            drawOnPixmap(pixmapPainter, edge, Position::top);
            drawOnPixmap(pixmapPainter, edge, Position::bottomRight);
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomLeft);
            break;
        }

        case INDEX_CURVE_BOTTOM_LEFT:
        {
            CACHE::TileCacheInfo edge = tileCache.get(pAutoTile->getTileID(AutoTile::Index::BottomLeft));
            drawOnPixmap(pixmapPainter, edge, Position::bottom);
            drawOnPixmap(pixmapPainter, edge, Position::topLeft);
            drawOnPixmap(pixmapPainter, innerCenter, Position::topRight);
            break;
        }

        case INDEX_CURVE_BOTTOM_RIGHT:
        {
            CACHE::TileCacheInfo edge = tileCache.get(pAutoTile->getTileID(AutoTile::Index::BottomRight));
            drawOnPixmap(pixmapPainter, edge, Position::bottom);
            drawOnPixmap(pixmapPainter, edge, Position::topRight);
            drawOnPixmap(pixmapPainter, innerCenter, Position::topLeft);
            break;
        }
        }

        drawOnAtlas(painter, tempPix, i);
    }
}

void setupTripleInnerEdges(QPainter& painter, const AutoTile* pAutoTile, CACHE::Tiles& tileCache)
{
    auto center = tileCache.get(pAutoTile->getTileID(AutoTile::Index::Center));
    auto innerCenter = tileCache.get(pAutoTile->getTileID(AutoTile::Index::InnerCenter));
    for (uint32 i = INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT; i <= INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT; ++i)
    {
        QPixmap tempPix(TILE_SIZE, TILE_SIZE);
        tempPix.fill(Qt::transparent);
        QPainter pixmapPainter(&tempPix);

        switch (i)
        {
        case INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::top);
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomLeft);
            drawOnPixmap(pixmapPainter, center, Position::bottomRight);
            break;
        case INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::top);
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottomRight);
            drawOnPixmap(pixmapPainter, center, Position::bottomLeft);
            break;
        case INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottom);
            drawOnPixmap(pixmapPainter, innerCenter, Position::topLeft);
            drawOnPixmap(pixmapPainter, center, Position::topRight);
            break;
        case INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT:
            drawOnPixmap(pixmapPainter, innerCenter, Position::bottom);
            drawOnPixmap(pixmapPainter, innerCenter, Position::topRight);
            drawOnPixmap(pixmapPainter, center, Position::topLeft);
            break;
        }
        drawOnAtlas(painter, tempPix, i);
    }
}

void setupSideEnds(QPainter& painter, const AutoTile* pAutoTile, CACHE::Tiles& tileCache)
{
    for (uint32 i = INDEX_SIDE_END_TOP; i <= INDEX_SIDE_END_RIGHT; ++i)
    {
        QPixmap tempPix(TILE_SIZE, TILE_SIZE);
        tempPix.fill(Qt::transparent);
        QPainter pixmapPainter(&tempPix);

        switch (i)
        {
        case INDEX_SIDE_END_TOP:
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::TopRight)), Position::right);
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::TopLeft)), Position::left);
            break;

        case INDEX_SIDE_END_BOTTOM:
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::BottomRight)), Position::right);
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::BottomLeft)), Position::left);
            break;

        case INDEX_SIDE_END_LEFT:
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::TopLeft)), Position::top);
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::BottomLeft)), Position::bottom);
            break;

        case INDEX_SIDE_END_RIGHT:
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::TopRight)), Position::top);
            drawOnPixmap(pixmapPainter, tileCache.get(pAutoTile->getTileID(AutoTile::Index::BottomRight)), Position::bottom);
            break;
        }

        drawOnAtlas(painter, tempPix, i);
    }
}

/*#####
# AutoTiles
#####*/
AutoTiles::AutoTiles(Tiles& tileCache, const database::Manager& DBMgr)
    : m_TileCache(tileCache), TileBase(DBMgr)
{}

TileCacheInfo AutoTiles::get(uint32 ID, AutoTile::Index index) const
{
    try
    {
        if (auto pAutoTile = m_DBMgr.getAutoTileDatabase().getPrototype(ID))
        {
            // if basic texture return from tile cache
            if (static_cast<uint32>(index) < AUTO_TILE_SET_COUNT)
                return m_TileCache.get(pAutoTile->getTileID(index));

            if (isEmpty(ID))
            {
                auto info = const_cast<AutoTiles&>(*this)._setupAutoTile(ID);
                STANDARD_MESSAGE(std::string("AutoTileCache: ID: ") + std::to_string(pAutoTile->getID()) + " // name: " + pAutoTile->getName().toStdString() + "\n" +
                    "Added into texture atlas at position " + std::to_string(info.getPosition().getX()) + "/" + std::to_string(info.getPosition().getY()) + ".");
            }


            TileCacheInfo info(m_Positions.at(ID - 1).getPixmap(), GEOMETRY::Point<uint32>(m_Positions.at(ID - 1).getPosition().getX() * ATLAS_WIDTH * TILE_SIZE,
                m_Positions.at(ID - 1).getPosition().getY() * ATLAS_HEIGHT * TILE_SIZE));
            return TileCacheInfo(info.getPixmap(),
                GEOMETRY::Point<uint32>(info.getPosition().getX() + (static_cast<uint32>(index)-AUTO_TILE_SET_COUNT) % ATLAS_WIDTH * TILE_SIZE, info.getPosition().getY() +
                (static_cast<uint32>(index)-AUTO_TILE_SET_COUNT) / ATLAS_WIDTH * TILE_SIZE));
        }
    }
    catch (const std::runtime_error&) {}

    WARNING_MESSAGE(std::string("AutoTileCache: ID: ") + std::to_string(ID) + "\n" +
        "Unable to add tile to texture atlas.");
    return TileCacheInfo(nullptr, GEOMETRY::Point<uint32>(MATH::maximum<uint32>(), MATH::maximum<uint32>()));
}

TileCacheInfo AutoTiles::_setupAutoTile(uint32 ID)
{
    auto pAutoTile = m_DBMgr.getAutoTileDatabase().getPrototype(ID);
    if (!pAutoTile)
        throw std::runtime_error("Unable to get AutoTilePrototype.");

    QPixmap atlas(ATLAS_WIDTH * TILE_SIZE, ATLAS_HEIGHT * TILE_SIZE);
    atlas.fill(Qt::transparent);
    QPainter painter(&atlas);

    setupCircle(painter, pAutoTile, m_TileCache);
    setupInnerEdges(painter, pAutoTile, m_TileCache);
    setupMultiInnerEdges(painter, pAutoTile, m_TileCache);
    setupTripleInnerEdges(painter, pAutoTile, m_TileCache);

    setupY_Tiles(painter, pAutoTile, m_TileCache);
    setupT_Tiles(painter, pAutoTile, m_TileCache);

    setupDoubleSides(painter, pAutoTile, m_TileCache);
    setupSideEnds(painter, pAutoTile, m_TileCache);
    setupCurves(painter, pAutoTile, m_TileCache);

    return add(ID, atlas);
}
