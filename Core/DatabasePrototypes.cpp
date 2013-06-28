#include "DatabasePrototypes.h"

namespace DATABASE
{
    /*#####
    # AutoTilePrototype
    #####*/
    namespace AUTO_TILE
    {
        bool hasTileCheck(uint32 uiTileCheck, AUTO_TILE_TILE_REQUIREMENTS tiles)
        {
            if ((uiTileCheck & tiles) == tiles)
                return true;
            return false;
        }

        AUTO_TILE_INDEX getAutoTileIndexForTileCheck(uint32 uiTileCheck)
        {
            // lonely tile
            if (hasTileCheck(uiTileCheck, CIRCLE))
                return INDEX_CIRCLE;

            // end sides
            if (hasTileCheck(uiTileCheck, LEFT_END))
                return INDEX_SIDE_END_LEFT;
            if (hasTileCheck(uiTileCheck, RIGHT_END))
                return INDEX_SIDE_END_RIGHT;
            if (hasTileCheck(uiTileCheck, TOP_END))
                return INDEX_SIDE_END_TOP;
            if (hasTileCheck(uiTileCheck, BOTTOM_END))
                return INDEX_SIDE_END_BOTTOM;

            // double sides
            if (hasTileCheck(uiTileCheck, SIDE_VERTICAL))
                return INDEX_SIDE_VERTICAL;
            if (hasTileCheck(uiTileCheck, SIDE_HORIZONTAL))
                return INDEX_SIDE_HORIZONTAL;

            // curves
            if (hasTileCheck(uiTileCheck, CURVE_TOP_LEFT))
                return INDEX_CURVE_TOP_LEFT;
            if (hasTileCheck(uiTileCheck, CURVE_TOP_RIGHT))
                return INDEX_CURVE_TOP_RIGHT;
            if (hasTileCheck(uiTileCheck, CURVE_BOTTOM_LEFT))
                return INDEX_CURVE_BOTTOM_LEFT;
            if (hasTileCheck(uiTileCheck, CURVE_BOTTOM_RIGHT))
                return INDEX_CURVE_BOTTOM_RIGHT;

            // T tiles
            if (hasTileCheck(uiTileCheck, T_TOP))
                return INDEX_T_TOP;
            if (hasTileCheck(uiTileCheck, T_LEFT))
                return INDEX_T_LEFT;
            if (hasTileCheck(uiTileCheck, T_RIGHT))
                return INDEX_T_RIGHT;
            if (hasTileCheck(uiTileCheck, T_BOTTOM))
                return INDEX_T_BOTTOM;

            // edges
            if (hasTileCheck(uiTileCheck, EDGE_TOP_LEFT))
                return INDEX_TOP_LEFT;
            if (hasTileCheck(uiTileCheck, EDGE_TOP_RIGHT))
                return INDEX_TOP_RIGHT;
            if (hasTileCheck(uiTileCheck, EDGE_BOTTOM_LEFT))
                return INDEX_BOTTOM_LEFT;
            if (hasTileCheck(uiTileCheck, EDGE_BOTTOM_RIGHT))
                return INDEX_BOTTOM_RIGHT;

            // Y tiles
            if (hasTileCheck(uiTileCheck, Y_TOP_BOTTOM_LEFT))
                return INDEX_Y_TOP_BOTTOM_LEFT;
            if (hasTileCheck(uiTileCheck, Y_TOP_BOTTOM_RIGHT))
                return INDEX_Y_TOP_BOTTOM_RIGHT;
            if (hasTileCheck(uiTileCheck, Y_LEFT_TOP_RIGHT))
                return INDEX_Y_LEFT_TOP_RIGHT;
            if (hasTileCheck(uiTileCheck, Y_LEFT_BOTTOM_RIGHT))
                return INDEX_Y_LEFT_BOTTOM_RIGHT;
            if (hasTileCheck(uiTileCheck, Y_BOTTOM_TOP_LEFT))
                return INDEX_Y_BOTTOM_TOP_LEFT;
            if (hasTileCheck(uiTileCheck, Y_BOTTOM_TOP_RIGHT))
                return INDEX_Y_BOTTOM_TOP_RIGHT;
            if (hasTileCheck(uiTileCheck, Y_RIGHT_TOP_LEFT))
                return INDEX_Y_RIGHT_TOP_LEFT;
            if (hasTileCheck(uiTileCheck, Y_RIGHT_BOTTOM_LEFT))
                return INDEX_Y_RIGHT_BOTTOM_LEFT;

            // like a star
            if (hasTileCheck(uiTileCheck, INNER_EDGE_CENTER))
                return INDEX_INNER_CENTER;

            // triple inner edges
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT))
                return INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT))
                return INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT))
                return INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT))
                return INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT;

            // sides
            if (hasTileCheck(uiTileCheck, SIDE_TOP))
                return INDEX_TOP;
            if (hasTileCheck(uiTileCheck, SIDE_LEFT))
                return INDEX_LEFT;
            if (hasTileCheck(uiTileCheck, SIDE_RIGHT))
                return INDEX_RIGHT;
            if (hasTileCheck(uiTileCheck, SIDE_BOTTOM))
                return INDEX_BOTTOM;

            // diagonal inner edges
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT))
                return INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT))
                return INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT;

            // double inner edges
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_BOTTOM_LEFT))
                return INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT))
                return INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT))
                return INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT))
                return INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT;

            // single inner edges
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT))
                return INDEX_INNER_EDGE_TOP_LEFT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_RIGHT))
                return INDEX_INNER_EDGE_TOP_RIGHT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_BOTTOM_LEFT))
                return INDEX_INNER_EDGE_BOTTOM_LEFT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_BOTTOM_RIGHT))
                return INDEX_INNER_EDGE_BOTTOM_RIGHT;

            // if nothing other matches return center
            return INDEX_CENTER;
        }
    };

    /*#####
    # TileSetPrototype
    #####*/
    TILE_SET::TileSetPrototype::TileSetPrototype(uint32 uiID) : Prototype(uiID)
    {
        resizeTiles(UInt32Point(DEFAULT_COLUMN_COUNT, DEFAULT_ROW_COUNT));
    }

    void TILE_SET::TileSetPrototype::resizeTiles(UInt32Point size)
    {
        m_Tiles.resize(boost::extents[size.x][size.y]);
        m_Size = size;
    }

    void TILE_SET::TileSetPrototype::clear()
    {
        m_Tiles.resize(boost::extents[0][0]);
        m_Size = UInt32Point(0, 0);
    }

    void TILE_SET::TileSetPrototype::setTileID(UInt32Point pos, uint32 ID)
    {
        _resizeIfNeeded(UInt32Point(pos.x+1, pos.y+1));
        m_Tiles[pos.x][pos.y] = ID;
    }

    uint32 TILE_SET::TileSetPrototype::getTileID(UInt32Point pos) const
    {
        if (pos.x < m_Size.x && pos.y < m_Size.y)
            return m_Tiles[pos.x][pos.y];
        return 0;
    }

    void TILE_SET::TileSetPrototype::_resizeIfNeeded(UInt32Point size)
    {
        UInt32Point newSize(m_Size);
        if (size.x > newSize.x)
            newSize.x = size.x;
        if (size.y > newSize.y)
            newSize.y = size.y;
        if (newSize != m_Size)
            resizeTiles(newSize);
    }

    /*#####
    # AnimationPrototype
    #####*/
    bool AnimationPrototype::getFrame(uint32 uiIndex, Frame &frame) const
    {
        if (uiIndex < m_Frames.size())
        {
            frame = m_Frames.at(uiIndex);
            return true;
        }
        return false;
    }

    void AnimationPrototype::setFrame(uint32 uiIndex, const Frame &frame)
    {
        if (uiIndex >= m_Frames.size())
            m_Frames.resize(uiIndex+1);
        m_Frames.at(uiIndex) = frame;
    }

    void AnimationPrototype::removeFrame(uint32 uiIndex)
    {
        if (uiIndex >= m_Frames.size())
            return;
        if (uiIndex == m_Frames.size()-1)
            m_Frames.resize(uiIndex);
        else
            m_Frames.at(uiIndex) = Frame();
    }
};