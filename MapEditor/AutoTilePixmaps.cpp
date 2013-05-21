#include "AutoTilePixmaps.h"
#include "MapEditor.h"

using namespace DATABASE;
using namespace MAP;

void AutoTilePixmaps::_createPixmaps()
{
    _clearPixmaps();
    if (!m_pProto || !m_pEditor)
        return;

    // setup the easy ones
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        QPixmapPtr pNewPixmap;
        if (m_pEditor->getTilePixmap(m_pProto->getTileID(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)), pNewPixmap))
            _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // inner edges
    for (uint32 i = AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT; i <= AutoTilePrototype::INDEX_INNER_EDGE_BOTTOM_RIGHT; ++i)
    {
        QPoint pos(0, 0);
        switch (i)
        {
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_RIGHT:
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_INNER_EDGE_BOTTOM_LEFT:
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_INNER_EDGE_BOTTOM_RIGHT:
            pos.setX(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pInnerCenter;
        QPixmapPtr pCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_CENTER), pCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pCenter.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos.x(), pos.y(), TILE_SIZE/2, TILE_SIZE/2, *pInnerCenter.get(), pos.x(), pos.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // multi inner edges
    for (uint32 i = AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT; i <= AutoTilePrototype::INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT; ++i)
    {
        QPoint pos1(0, 0);
        QPoint pos2(0, 0);
        switch (i)
        {
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT:
            pos2.setX(TILE_SIZE/2);
            pos2.setY(TILE_SIZE/2);
            break;

        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT:
            pos1.setX(TILE_SIZE/2);
            pos2.setY(TILE_SIZE/2);
            break;

        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT:
            pos2.setX(TILE_SIZE/2);
            break;

        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT:
            pos2.setY(TILE_SIZE/2);
            break;

        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT:
            pos1.setX(TILE_SIZE/2);
            pos2.setX(TILE_SIZE/2);
            pos2.setY(TILE_SIZE/2);
            break;

        case AutoTilePrototype::INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT:
            pos1.setY(TILE_SIZE/2);
            pos2.setX(TILE_SIZE/2);
            pos2.setY(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pInnerCenter;
        QPixmapPtr pCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_CENTER), pCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pCenter.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos1.x(), pos1.y(), TILE_SIZE/2, TILE_SIZE/2, *pInnerCenter.get(), pos1.x(), pos1.y());
        pixmapPainter.drawTiledPixmap(pos2.x(), pos2.y(), TILE_SIZE/2, TILE_SIZE/2, *pInnerCenter.get(), pos2.x(), pos2.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // double sides
    for (uint32 i = AutoTilePrototype::INDEX_SIDE_VERTICAL; i <= AutoTilePrototype::INDEX_SIDE_HORIZONTAL; ++i)
    {
        QSize size(TILE_SIZE, TILE_SIZE);
        AutoTilePrototype::AUTO_TILE_INDEX uiIndex[2];
        switch (i)
        {
        case AutoTilePrototype::INDEX_SIDE_VERTICAL:
            uiIndex[0] = AutoTilePrototype::INDEX_RIGHT;
            uiIndex[1] = AutoTilePrototype::INDEX_LEFT;
            size.setWidth(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_SIDE_HORIZONTAL:
            uiIndex[0] = AutoTilePrototype::INDEX_BOTTOM;
            uiIndex[1] = AutoTilePrototype::INDEX_TOP;
            size.setHeight(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pSide[2];
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex[0]), pSide[0]) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex[1]), pSide[1]))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pSide[0].get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(0, 0, size.width(), size.height(), *pSide[1].get(), 0, 0);
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // T tiles
    for (uint32 i = AutoTilePrototype::INDEX_T_TOP; i <= AutoTilePrototype::INDEX_T_RIGHT; ++i)
    {
        AutoTilePrototype::AUTO_TILE_INDEX uiIndex;
        QPoint pos(0,0);
        QSize size(TILE_SIZE, TILE_SIZE);
        switch (i)
        {
        case AutoTilePrototype::INDEX_T_TOP:
            uiIndex = AutoTilePrototype::INDEX_TOP;
            size.setHeight(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_T_BOTTOM:
            uiIndex = AutoTilePrototype::INDEX_BOTTOM;
            size.setHeight(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_T_LEFT:
            uiIndex = AutoTilePrototype::INDEX_LEFT;
            size.setWidth(TILE_SIZE/2);
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_T_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_RIGHT;
            size.setWidth(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pSide;
        QPixmapPtr pInnerCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex), pSide) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pSide.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos.x(), pos.y(), size.width(), size.height(), *pInnerCenter.get(), pos.x(), pos.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // Y tiles
    for (uint32 i = AutoTilePrototype::INDEX_Y_TOP_BOTTOM_LEFT; i <= AutoTilePrototype::INDEX_Y_RIGHT_BOTTOM_LEFT; ++i)
    {
        AutoTilePrototype::AUTO_TILE_INDEX uiIndex;
        QPoint pos(0,0);
        switch (i)
        {
        case AutoTilePrototype::INDEX_Y_TOP_BOTTOM_LEFT:
            uiIndex = AutoTilePrototype::INDEX_TOP;
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_Y_TOP_BOTTOM_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_TOP;
            pos.setY(TILE_SIZE/2);
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_Y_BOTTOM_TOP_LEFT:
            uiIndex = AutoTilePrototype::INDEX_BOTTOM;
            break;
        case AutoTilePrototype::INDEX_Y_BOTTOM_TOP_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_BOTTOM;
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_Y_LEFT_TOP_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_LEFT;
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_Y_LEFT_BOTTOM_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_LEFT;
            pos.setX(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_Y_RIGHT_TOP_LEFT:
            uiIndex = AutoTilePrototype::INDEX_RIGHT;
            break;
        case AutoTilePrototype::INDEX_Y_RIGHT_BOTTOM_LEFT:
            uiIndex = AutoTilePrototype::INDEX_RIGHT;
            pos.setY(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pSide;
        QPixmapPtr pInnerCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex), pSide) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pSide.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos.x(), pos.y(), TILE_SIZE/2, TILE_SIZE/2, *pInnerCenter.get(), pos.x(), pos.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // curves
    for (uint32 i = AutoTilePrototype::INDEX_CURVE_TOP_LEFT; i <= AutoTilePrototype::INDEX_CURVE_BOTTOM_RIGHT; ++i)
    {
        AutoTilePrototype::AUTO_TILE_INDEX uiIndex;
        QPoint pos(0,0);
        switch (i)
        {
        case AutoTilePrototype::INDEX_CURVE_TOP_LEFT:
            uiIndex = AutoTilePrototype::INDEX_TOP_LEFT;
            pos.setX(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_CURVE_TOP_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_TOP_RIGHT;
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_CURVE_BOTTOM_LEFT:
            uiIndex = AutoTilePrototype::INDEX_BOTTOM_LEFT;
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_CURVE_BOTTOM_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_BOTTOM_RIGHT;
            break;
        }
        QPixmapPtr pEdge;
        QPixmapPtr pInnerCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex), pEdge) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pEdge.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos.x(), pos.y(), TILE_SIZE/2, TILE_SIZE/2, *pInnerCenter.get(), pos.x(), pos.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // triple inner edges
    for (uint32 i = AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT; i <= AutoTilePrototype::INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT; ++i)
    {
        QPoint pos(0,0);
        switch (i)
        {
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT:
            pos.setX(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT:
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT:
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT:
            break;
        }
        QPixmapPtr pCenter;
        QPixmapPtr pInnerCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_CENTER), pCenter) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pInnerCenter.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos.x(), pos.y(), TILE_SIZE/2, TILE_SIZE/2, *pCenter.get(), pos.x(), pos.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // side ends
    for (uint32 i = AutoTilePrototype::INDEX_SIDE_END_TOP; i <= AutoTilePrototype::INDEX_SIDE_END_RIGHT; ++i)
    {
        QSize size(TILE_SIZE, TILE_SIZE);
        AutoTilePrototype::AUTO_TILE_INDEX uiIndex[2];
        switch (i)
        {
        case AutoTilePrototype::INDEX_SIDE_END_TOP:
            uiIndex[0] = AutoTilePrototype::INDEX_TOP_RIGHT;
            uiIndex[1] = AutoTilePrototype::INDEX_TOP_LEFT;
            size.setWidth(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_SIDE_END_BOTTOM:
            uiIndex[0] = AutoTilePrototype::INDEX_BOTTOM_RIGHT;
            uiIndex[1] = AutoTilePrototype::INDEX_BOTTOM_LEFT;
            size.setWidth(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_SIDE_END_LEFT:
            uiIndex[0] = AutoTilePrototype::INDEX_BOTTOM_LEFT;
            uiIndex[1] = AutoTilePrototype::INDEX_TOP_LEFT;
            size.setHeight(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_SIDE_END_RIGHT:
            uiIndex[0] = AutoTilePrototype::INDEX_BOTTOM_RIGHT;
            uiIndex[1] = AutoTilePrototype::INDEX_TOP_RIGHT;
            size.setHeight(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pSide[2];
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex[0]), pSide[0]) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex[1]), pSide[1]))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pSide[0].get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(0, 0, size.width(), size.height(), *pSide[1].get(), 0, 0);
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // circle
    QPixmapPtr pSide[4];
    if (!m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_TOP_LEFT), pSide[0]) ||
        !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_TOP_RIGHT), pSide[1]) ||
        !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_BOTTOM_LEFT), pSide[2]) ||
        !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_BOTTOM_RIGHT), pSide[3]))
        return;
    QPixmapPtr pNewPixmap(new QPixmap(*pSide[0].get()));
    QPainter pixmapPainter(pNewPixmap.get());
    pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2, *pSide[1].get(), TILE_SIZE/2, 0);
    pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, *pSide[2].get(), 0, TILE_SIZE/2);
    pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, *pSide[3].get(), TILE_SIZE/2, TILE_SIZE/2);
    _setPixmap(AutoTilePrototype::INDEX_INNER_EDGE_CENTER, pNewPixmap);
}