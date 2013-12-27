#include "DatabasePrototypes.h"
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include "TileCache.h"

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

    QPixmap TILE_SET::createPixmap(const TileSetPrototype& tileSet)
    {
        QPixmap pixmap(tileSet.getTileCount().x*TILE_SIZE, tileSet.getTileCount().y*TILE_SIZE);
        pixmap.fill();
        QPainter painter(&pixmap);
        UInt32Point pos;
        for (pos.x = 0; pos.x < tileSet.getTileCount().x; ++pos.x)
        {
            for (pos.y = 0; pos.y < tileSet.getTileCount().y; ++pos.y)
            {
                if (auto pTilePixmap = GTileCache::get()->getItem(tileSet.getTileID(pos)))
                    painter.drawTiledPixmap(pos.x*TILE_SIZE, pos.y*TILE_SIZE, TILE_SIZE, TILE_SIZE,* pTilePixmap);
            }
        }
        return pixmap;
    }

    /*#####
    # AnimationPrototype
    #####*/
    namespace ANIMATION
    {
        bool AnimationPrototype::getFrame(uint32 uiIndex, Frame& frame) const
        {
            if (uiIndex < m_Frames.size())
            {
                frame = m_Frames.at(uiIndex);
                return true;
            }
            return false;
        }

        void AnimationPrototype::setFrame(uint32 uiIndex, Frame frame)
        {
            if (uiIndex >= m_Frames.size())
                m_Frames.resize(uiIndex+1);
            m_Frames.at(uiIndex) = std::move(frame);
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

        void Frame::calculateOffset()
        {
            m_FrameOffset.x = 0;
            m_FrameOffset.y = 0;
            for (auto& sprite : m_Sprites)
            {
                if (sprite.m_Pos.x < m_FrameOffset.x)
                    m_FrameOffset.x = sprite.m_Pos.x;
                if (sprite.m_Pos.y < m_FrameOffset.y)
                    m_FrameOffset.y = sprite.m_Pos.y;
            }
        }

        void Frame::setSprite(uint32 index, Sprite sprite)
        {
            if (index >= m_Sprites.size())
                m_Sprites.resize(index+1);
            m_Sprites.at(index) = std::move(sprite);
            calculateOffset();
        }

        Sprite Frame::getSprite(uint32 index) const
        {
            if (index < m_Sprites.size())
                return m_Sprites.at(index);
            return Sprite();
        }

        void Frame::addSprite(Sprite sprite)
        {
            m_Sprites.push_back(std::move(sprite));
            setOffsetIfNeeded(sprite.m_Pos);
        }

        void Frame::setOffsetIfNeeded(const Int32Point& offset)
        {
            if (m_FrameOffset.x > offset.x)
                m_FrameOffset.x = offset.x;
            if (m_FrameOffset.y > offset.y)
                m_FrameOffset.y = offset.y;
        }
    }

    /*#####
    # WorldObjectPrototype
    #####*/
    namespace MAP_OBJECT
    {
        WorldObjectPrototype::WorldObjectPrototype(uint32 uiID) : Prototype(uiID), m_uiAnimationSpeed(100)
        {
            initAnimationPoses();
        }

        void WorldObjectPrototype::initAnimationPoses()
        {
            // set minimum poses
            m_AnimationInfos.resize(getMinimumAnimationCount());
            for (uint32 i = 1; i <= getMinimumAnimationCount(); ++i)
                m_AnimationInfos.at(i-1).m_uiAnimationTypeID = i;
        }

        void WorldObjectPrototype::setAnimationInfo(uint32 uiIndex, AnimationInfo animationInfo)
        {
            // do not change animation type id if its an standard entry
            if (uiIndex <= getMinimumAnimationCount())
                m_AnimationInfos.at(uiIndex).m_uiAnimationID = animationInfo.m_uiAnimationID;
            else
                m_AnimationInfos.at(uiIndex) = animationInfo;
        }

        void WorldObjectPrototype::setAnimationCount(uint32 uiCount)
        {
            if (uiCount < getMinimumAnimationCount())
                uiCount = getMinimumAnimationCount();
            m_AnimationInfos.resize(uiCount);
        }

        /*#####
        # DynamicObjectPrototype
        #####*/

        /*#####
        # free functions
        #####*/
        QString getTypeString(ObjectType type)
        {
            switch (type)
            {
            case TYPE_WORLDOBJECT: return "WorldObject";
            case TYPE_DYNAMIC_OBJECT: return "DynamicObject";
            }
            return "Unknown";
        }
    }

    /*#####
    # MapPrototype
    #####*/
    namespace MAP_STRUCTURE
    {
        MapPrototype::MapPrototype(uint32 ID, const QString& fileName) : Prototype(ID), m_FileName(fileName), m_uiParentID(0)
        {
            m_Layer.at(MAP::LAYER_BACKGROUND) = 1;
            m_Layer.at(MAP::LAYER_FOREGROUND) = 0;
        }

        void MapPrototype::addMapObject(MapObject* pObject)
        {
            if (pObject)
                m_Objects.setItem(pObject->m_GUID, pObject);
        }

        void MapPrototype::removeMapObject(uint32 GUID)
        {
            if (GUID <= getMapObjectCount())
                m_Objects.setItem(GUID, nullptr);
        }

        MapObject* MapPrototype::addMapObject(DATABASE::MAP_OBJECT::ObjectType type, uint32 uiID, Int32Point pos)
        {
            auto newObject = new MapObject();
            newObject->m_ObjectID = uiID;
            newObject->m_Type = type;
            newObject->m_Position = pos;
            newObject->m_GUID = m_Objects.getSize()+1;
            m_Objects.setItem(newObject->m_GUID, newObject);
            return newObject;
        }

        bool MapPrototype::isValid()
        {
            return !m_FileName.isEmpty();
        }

        void MapPrototype::setSizeX(uint32 x)
        {
            setSize(UInt32Point(x, getSize().y), getLayerSize(MAP::LAYER_FOREGROUND), getLayerSize(MAP::LAYER_BACKGROUND));
        }

        void MapPrototype::setSizeY(uint32 y)
        {
            setSize(UInt32Point(getSize().x, y), getLayerSize(MAP::LAYER_FOREGROUND), getLayerSize(MAP::LAYER_BACKGROUND));
        }

        void MapPrototype::setLayerSize(uint8 size, Layer layer)
        {
            setSize(getSize(), layer == MAP::LAYER_FOREGROUND ? size : getLayerSize(MAP::LAYER_FOREGROUND),
                layer == MAP::LAYER_BACKGROUND ? size : getLayerSize(MAP::LAYER_BACKGROUND));
        }

        void MapPrototype::setSize(const UInt32Point& size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize)
        {
            m_Size = size;
            m_Layer.at(MAP::LAYER_BACKGROUND) = uiBackgroundLayerSize;
            m_Layer.at(MAP::LAYER_FOREGROUND) = uiForegroundLayerSize;
        }
    }
}