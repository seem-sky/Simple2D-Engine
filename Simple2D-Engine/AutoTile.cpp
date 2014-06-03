#include "AutoTile.h"
#include <QtGui/QPainter>
#include "QtGlobal.h"
#include "Config.h"

using namespace DATABASE;
using namespace PROTOTYPE::AUTO_TILE;
using namespace MAP;

typedef std::unique_ptr<QPixmap> QPixmapPtr;

AutoTile::AutoTile(const DATABASE::DatabaseMgr& DBMgr, uint32 ID) : m_DBMgr(DBMgr), m_ID(ID)
{
    _createPixmaps();
}

void AutoTile::_createPixmaps()
{
    _clearPixmaps();
    auto pAutoTile = m_DBMgr.getAutoTileDatabase()->getOriginalPrototype(m_ID);
    if (!pAutoTile)
        return;

    // setup the easy ones
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (auto pNewPixmap = GTileCache::get()->getItem(pAutoTile->getTileID(static_cast<AUTO_TILE_INDEX>(i))))
            _setPixmap(static_cast<AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // inner edges
    for (uint32 i = INDEX_INNER_EDGE_TOP_LEFT; i <= INDEX_INNER_EDGE_BOTTOM_RIGHT; ++i)
    {
        auto pInnerCenter = GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_INNER_CENTER));
        auto pCenter = GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_CENTER));
        if (!pInnerCenter || !pCenter)
            break;
        QPixmapPtr pNewPixmap(new QPixmap(QSize(TILE_SIZE, TILE_SIZE)));
        pNewPixmap->fill(Qt::transparent);
        QPainter pixmapPainter(&*pNewPixmap);
        switch (i)
        {
        case INDEX_INNER_EDGE_TOP_LEFT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_INNER_EDGE_TOP_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_INNER_EDGE_BOTTOM_LEFT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_INNER_EDGE_BOTTOM_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        }
        _setPixmap(static_cast<AUTO_TILE_INDEX>(i), pNewPixmap.release());
    }

    // multi inner edges
    for (uint32 i = INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT; i <= INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT; ++i)
    {
        auto pInnerCenter = GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_INNER_CENTER));
        auto pCenter = GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_CENTER));
        if (!pInnerCenter || !pCenter)
            break;
        QPixmapPtr pNewPixmap(new QPixmap(QSize(TILE_SIZE, TILE_SIZE)));
        pNewPixmap->fill(Qt::transparent);
        QPainter pixmapPainter(pNewPixmap.get());
        switch (i)
        {
        case INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;

        case INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;

        case INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;

        case INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;

        case INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;

        case INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        }
        _setPixmap(static_cast<AUTO_TILE_INDEX>(i), pNewPixmap.release());
    }

    // double sides
    for (uint32 i = INDEX_SIDE_VERTICAL; i <= INDEX_SIDE_HORIZONTAL; ++i)
    {
        QSize size(TILE_SIZE, TILE_SIZE);
        std::array<AUTO_TILE_INDEX, 2> uiIndex;
        switch (i)
        {
        case INDEX_SIDE_VERTICAL:
            uiIndex[0] = INDEX_RIGHT;
            uiIndex[1] = INDEX_LEFT;
            size.setWidth(TILE_SIZE/2);
            break;
        case INDEX_SIDE_HORIZONTAL:
            uiIndex[0] = INDEX_BOTTOM;
            uiIndex[1] = INDEX_TOP;
            size.setHeight(TILE_SIZE/2);
            break;
        }
        std::array<const QPixmap*, 2> pSide = { GTileCache::get()->getItem(pAutoTile->getTileID(uiIndex[0])),
            GTileCache::get()->getItem(pAutoTile->getTileID(uiIndex[1])) };
        if (!pSide.at(1) || !pSide.at(1))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(QSize(TILE_SIZE, TILE_SIZE)));
        pNewPixmap->fill(Qt::transparent);
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(size.width() % TILE_SIZE, size.height() % TILE_SIZE, size.width(), size.height(),* pSide[0],
            size.width() % TILE_SIZE, size.height() % TILE_SIZE);
        pixmapPainter.drawTiledPixmap(0, 0, size.width(), size.height(),* pSide[1], 0, 0);
        _setPixmap(static_cast<AUTO_TILE_INDEX>(i), pNewPixmap.release());
    }

    // T tiles
    for (uint32 i = INDEX_T_TOP; i <= INDEX_T_RIGHT; ++i)
    {
        AUTO_TILE_INDEX uiIndex;
        switch (i)
        {
        case INDEX_T_TOP: uiIndex = INDEX_TOP; break;
        case INDEX_T_BOTTOM: uiIndex = INDEX_BOTTOM; break;
        case INDEX_T_LEFT: uiIndex = INDEX_LEFT; break;
        case INDEX_T_RIGHT: uiIndex = INDEX_RIGHT; break;
        }
        auto pSide = GTileCache::get()->getItem(pAutoTile->getTileID(uiIndex));
        auto pInnerCenter = GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_INNER_CENTER));
        if (!pSide || !pInnerCenter)
            break;
        QPixmapPtr pNewPixmap(new QPixmap(QSize(TILE_SIZE, TILE_SIZE)));
        pNewPixmap->fill(Qt::transparent);
        QPainter pixmapPainter(pNewPixmap.get());
        switch (i)
        {
        case INDEX_T_TOP:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE, TILE_SIZE/2,* pSide, 0, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE, TILE_SIZE/2,* pInnerCenter, 0, TILE_SIZE/2);
            break;
        case INDEX_T_BOTTOM:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE, TILE_SIZE/2,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE, TILE_SIZE/2,* pSide, 0, TILE_SIZE/2);
            break;
        case INDEX_T_LEFT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE,* pSide, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE,* pInnerCenter, TILE_SIZE/2, 0);
            break;
        case INDEX_T_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE,* pSide, TILE_SIZE/2, 0);
            break;
        }
        _setPixmap(static_cast<AUTO_TILE_INDEX>(i), pNewPixmap.release());
    }

    // Y tiles
    for (uint32 i = INDEX_Y_TOP_BOTTOM_LEFT; i <= INDEX_Y_RIGHT_BOTTOM_LEFT; ++i)
    {
        AUTO_TILE_INDEX uiIndex;
        QPoint pos(0,0);
        switch (i)
        {
        case INDEX_Y_TOP_BOTTOM_LEFT:
            uiIndex = INDEX_TOP;
            pos.setY(TILE_SIZE/2);
            break;
        case INDEX_Y_TOP_BOTTOM_RIGHT:
            uiIndex = INDEX_TOP;
            pos.setY(TILE_SIZE/2);
            pos.setX(TILE_SIZE/2);
            break;
        case INDEX_Y_BOTTOM_TOP_LEFT:
            uiIndex = INDEX_BOTTOM;
            break;
        case INDEX_Y_BOTTOM_TOP_RIGHT:
            uiIndex = INDEX_BOTTOM;
            pos.setX(TILE_SIZE/2);
            break;
        case INDEX_Y_LEFT_TOP_RIGHT:
            uiIndex = INDEX_LEFT;
            pos.setX(TILE_SIZE/2);
            break;
        case INDEX_Y_LEFT_BOTTOM_RIGHT:
            uiIndex = INDEX_LEFT;
            pos.setX(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        case INDEX_Y_RIGHT_TOP_LEFT:
            uiIndex = INDEX_RIGHT;
            break;
        case INDEX_Y_RIGHT_BOTTOM_LEFT:
            uiIndex = INDEX_RIGHT;
            pos.setY(TILE_SIZE/2);
            break;
        }
        auto pSide = GTileCache::get()->getItem(pAutoTile->getTileID(uiIndex));
        auto pInnerCenter = GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_INNER_CENTER));
        if (!pSide || !pInnerCenter)
            break;
        QPixmapPtr pNewPixmap(new QPixmap(QSize(TILE_SIZE, TILE_SIZE)));
        pNewPixmap->fill(Qt::transparent);
        QPainter pixmapPainter(pNewPixmap.get());
        switch (i)
        {
        case INDEX_Y_TOP_BOTTOM_LEFT:
        case INDEX_Y_RIGHT_BOTTOM_LEFT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pSide, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pSide, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pSide, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_Y_TOP_BOTTOM_RIGHT:
        case INDEX_Y_LEFT_BOTTOM_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pSide, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pSide, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pSide, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_Y_BOTTOM_TOP_LEFT:
        case INDEX_Y_RIGHT_TOP_LEFT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pSide, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pSide, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pSide, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_Y_BOTTOM_TOP_RIGHT:
        case INDEX_Y_LEFT_TOP_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pSide, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pSide, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pSide, TILE_SIZE/2, TILE_SIZE/2);
            break;
        }
        _setPixmap(static_cast<AUTO_TILE_INDEX>(i), pNewPixmap.release());
    }

    // curves
    for (uint32 i = INDEX_CURVE_TOP_LEFT; i <= INDEX_CURVE_BOTTOM_RIGHT; ++i)
    {
        AUTO_TILE_INDEX uiIndex;
        switch (i)
        {
        case INDEX_CURVE_TOP_LEFT: uiIndex = INDEX_TOP_LEFT; break;
        case INDEX_CURVE_TOP_RIGHT: uiIndex = INDEX_TOP_RIGHT; break;
        case INDEX_CURVE_BOTTOM_LEFT: uiIndex = INDEX_BOTTOM_LEFT; break;
        case INDEX_CURVE_BOTTOM_RIGHT: uiIndex = INDEX_BOTTOM_RIGHT; break;
        }

        auto pEdge = GTileCache::get()->getItem(pAutoTile->getTileID(uiIndex));
        auto pInnerCenter = GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_INNER_CENTER));
        if (!pEdge || !pInnerCenter)
            break;
        QPixmapPtr pNewPixmap(new QPixmap(QSize(TILE_SIZE, TILE_SIZE)));
        pNewPixmap->fill(Qt::transparent);
        QPainter pixmapPainter(pNewPixmap.get());
        switch (i)
        {
        case INDEX_CURVE_TOP_LEFT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pEdge, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pEdge, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pEdge, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_CURVE_TOP_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pEdge, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pEdge, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pEdge, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_CURVE_BOTTOM_LEFT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pEdge, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pEdge, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pEdge, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_CURVE_BOTTOM_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pEdge, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pEdge, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pEdge, TILE_SIZE/2, TILE_SIZE/2);
            break;
        }
        _setPixmap(static_cast<AUTO_TILE_INDEX>(i), pNewPixmap.release());
    }

    // triple inner edges
    for (uint32 i = INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT; i <= INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT; ++i)
    {
        auto pCenter = GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_CENTER));
        auto pInnerCenter = GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_INNER_CENTER));
        if (!pCenter || !pInnerCenter)
            break;
        QPixmapPtr pNewPixmap(new QPixmap(QSize(TILE_SIZE, TILE_SIZE)));
        pNewPixmap->fill(Qt::transparent);
        QPainter pixmapPainter(pNewPixmap.get());
        switch (i)
        {
        case INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        case INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT:
            pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pCenter, 0, 0);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, 0);
            pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, 0, TILE_SIZE/2);
            pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pInnerCenter, TILE_SIZE/2, TILE_SIZE/2);
            break;
        }
        _setPixmap(static_cast<AUTO_TILE_INDEX>(i), pNewPixmap.release());
    }

    // side ends
    for (uint32 i = INDEX_SIDE_END_TOP; i <= INDEX_SIDE_END_RIGHT; ++i)
    {
        QSize size(TILE_SIZE, TILE_SIZE);
        std::array<AUTO_TILE_INDEX, 2> uiIndex;
        switch (i)
        {
        case INDEX_SIDE_END_TOP:
            uiIndex[0] = INDEX_TOP_RIGHT;
            uiIndex[1] = INDEX_TOP_LEFT;
            size.setWidth(TILE_SIZE/2);
            break;
        case INDEX_SIDE_END_BOTTOM:
            uiIndex[0] = INDEX_BOTTOM_RIGHT;
            uiIndex[1] = INDEX_BOTTOM_LEFT;
            size.setWidth(TILE_SIZE/2);
            break;
        case INDEX_SIDE_END_LEFT:
            uiIndex[0] = INDEX_BOTTOM_LEFT;
            uiIndex[1] = INDEX_TOP_LEFT;
            size.setHeight(TILE_SIZE/2);
            break;
        case INDEX_SIDE_END_RIGHT:
            uiIndex[0] = INDEX_BOTTOM_RIGHT;
            uiIndex[1] = INDEX_TOP_RIGHT;
            size.setHeight(TILE_SIZE/2);
            break;
        }
        std::array<const QPixmap*, 2> pSide = { GTileCache::get()->getItem(pAutoTile->getTileID(uiIndex[0])),
            GTileCache::get()->getItem(pAutoTile->getTileID(uiIndex[1])) };
        if (!pSide.at(0) || !pSide.at(1))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(QSize(TILE_SIZE, TILE_SIZE)));
        pNewPixmap->fill(Qt::transparent);
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(size.width() % TILE_SIZE, size.height() % TILE_SIZE, size.width(), size.height(),* pSide.at(0),
            size.width() % TILE_SIZE, size.height() % TILE_SIZE);
        pixmapPainter.drawTiledPixmap(0, 0, size.width(), size.height(),* pSide.at(1), 0, 0);
        _setPixmap(static_cast<AUTO_TILE_INDEX>(i), pNewPixmap.release());
    }

    // circle
    std::array<const QPixmap*, 4> pSide = { GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_TOP_LEFT)),
        GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_TOP_RIGHT)),
        GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_BOTTOM_LEFT)),
        GTileCache::get()->getItem(pAutoTile->getTileID(INDEX_BOTTOM_RIGHT)) };

    for (auto pointer : pSide)
    {
        if (!pointer)
            return;
    }

    QPixmapPtr pNewPixmap(new QPixmap(QSize(TILE_SIZE, TILE_SIZE)));
    pNewPixmap->fill(Qt::transparent);
    QPainter pixmapPainter(pNewPixmap.get());
    pixmapPainter.drawTiledPixmap(0, 0, TILE_SIZE/2, TILE_SIZE/2,* pSide.at(0), 0, 0);
    pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2,* pSide.at(1), TILE_SIZE/2, 0);
    pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pSide.at(2), 0, TILE_SIZE/2);
    pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2,* pSide.at(3), TILE_SIZE/2, TILE_SIZE/2);
    _setPixmap(INDEX_CIRCLE, pNewPixmap.release());
}
