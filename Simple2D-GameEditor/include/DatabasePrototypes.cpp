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

    /*#####
    # WorldObjectPrototype
    #####*/
    namespace MAP_OBJECT
    {
        WorldObjectPrototype::WorldObjectPrototype(uint32 uiID) : Prototype(uiID), m_uiAnimationSpeed(100)
        {
            // set minimum poses, so we have stand pose for all directions
            m_AnimationInfos.resize(getMinimumAnimationCount());
            for (uint32 i = 1; i <= getMinimumAnimationCount(); ++i)
                m_AnimationInfos.setItem(i, AnimationInfoPtr(new AnimationInfo(0, i)));
        }

        AnimationInfo WorldObjectPrototype::getAnimationInfo(uint32 uiIndex) const
        {
            ConstAnimationInfoPtr info;
            if (m_AnimationInfos.getItem(uiIndex, info))
                return *info;
            return AnimationInfo();
        }

        void WorldObjectPrototype::setAnimationInfo(uint32 uiIndex, AnimationInfo animationInfo)
        {
            AnimationInfoPtr info;
            // do not change animation type id if its an standard entry
            if (uiIndex <= getMinimumAnimationCount())
                info = AnimationInfoPtr(new AnimationInfo(animationInfo.m_uiAnimationID, uiIndex));
            else
                AnimationInfoPtr(new AnimationInfo(animationInfo));
            m_AnimationInfos.setItem(uiIndex, info);
        }

        void WorldObjectPrototype::setAnimationCount(uint32 uiCount)
        {
            if (uiCount < getMinimumAnimationCount())
                uiCount = getMinimumAnimationCount();
            m_AnimationInfos.resize(uiCount);
        }

        /*#####
        # free functions
        #####*/
        QString getTypeString(ObjectType type)
        {
            QString result;
            switch (type)
            {
            case TYPE_WORLDOBJECT: result = "WorldObject";
            case TYPE_DYNAMIC_OBJECT: result ="DynamicObject";
            }
            return std::move(result);
        }
    }

    /*#####
    # MapPrototype
    #####*/
    namespace MAP_STRUCTURE
    {
        MapPrototype::MapPrototype(uint32 uiID, const QString &fileName) : DATABASE::Prototype(uiID), m_FileName(fileName), m_uiParentID(0),
            m_DataLoaded(false)
        {}

        void MapPrototype::addMapObject(MapObjectPtr pObject)
        {
            if (pObject)
                m_Objects.setItem(pObject->m_GUID, pObject);
        }

        MapObjectPtr MapPrototype::addMapObject(DATABASE::MAP_OBJECT::ObjectType type, uint32 uiID, Int32Point pos)
        {
            MapObjectPtr newObject(new MapObject());
            newObject->m_ObjectID = uiID;
            newObject->m_Type = type;
            newObject->m_Position = pos;
            newObject->m_GUID = m_Objects.getSize()+1;
            m_Objects.setItem(newObject->m_GUID, newObject);
            return newObject;
        }

        uint32 MapPrototype::checkAutoTiles(const uint32 &uiID, const UInt32Point3D &pos, UInt32PointSet &result, Layer layer, uint32 resultFlag)
        {
            uint32 uiBorderCheck = 0;
            MapTile centerTile = getMapTile(pos, layer);
            for (uint32 i = 0; i < 8; ++i)
            {
                DATABASE::AUTO_TILE::TILE_CHECK curTileCheck = DATABASE::AUTO_TILE::SAME_AROUND;
                // set position check
                UInt32Point3D checkPos = pos;
                switch (i)
                {
                case 0: // top-left
                    if (!checkPos.x || !checkPos.y)
                        continue;
                    --checkPos.x;
                    --checkPos.y;
                    curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP_LEFT;
                    break;

                case 1: // top
                    if (!checkPos.y)
                        continue;
                    --checkPos.y;
                    curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP;
                    break;

                case 2: // top-right
                    if (!checkPos.y || checkPos.x+1 >= getSize().x)
                        continue;
                    ++checkPos.x;
                    --checkPos.y;
                    curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP_RIGHT;
                    break;

                case 3: // left
                    if (!checkPos.x)
                        continue;
                    --checkPos.x;
                    curTileCheck = DATABASE::AUTO_TILE::OTHER_LEFT;
                    break;

                case 4: // right
                    if (checkPos.x+1 >= getSize().x)
                        continue;
                    ++checkPos.x;
                    curTileCheck = DATABASE::AUTO_TILE::OTHER_RIGHT;
                    break;

                case 5: // bottom-left
                    if (!checkPos.x || checkPos.y+1 >= getSize().y)
                        continue;
                    --checkPos.x;
                    ++checkPos.y;
                    curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM_LEFT;
                    break;

                case 6: // bottom
                    if (checkPos.y+1 >= getSize().y)
                        continue;
                    ++checkPos.y;
                    curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM;
                    break;

                case 7: // bottom-right
                    if (checkPos.x+1 >= getSize().x || checkPos.y+1 >= getSize().y)
                        continue;
                    ++checkPos.x;
                    ++checkPos.y;
                    curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM_RIGHT;
                    break;
                }
                MapTile mapTile = getMapTile(checkPos, layer);
                // if bad object, continue
                if (mapTile.m_uiAutoTileSetID == MAX_UINT32 || mapTile.m_uiTileID == MAX_UINT32)
                    continue;

                // if not same
                if (mapTile.m_uiAutoTileSetID != uiID)
                {
                    uiBorderCheck += curTileCheck;
                    if (resultFlag & FLAG_OTHER)
                        result.insert(checkPos);
                }
                // if same
                else if (resultFlag & FLAG_SAME)
                    result.insert(checkPos);
            }
            return uiBorderCheck;
        }

        void MapPrototype::_clearTiles()
        {
            m_DataLoaded = false;
            m_Layer.resizeMap(UInt32Point(0,0), 0, 0);
        }

        void MapPrototype::setSizeX(uint32 x)
        {
            setSize(UInt32Point(x, getSize().y), getLayerSize(MAP::LAYER_FOREGROUND), getLayerSize(MAP::LAYER_BACKGROUND));
        }

        void MapPrototype::setSizeY(uint32 y)
        {
            setSize(UInt32Point(getSize().x, y), getLayerSize(MAP::LAYER_FOREGROUND), getLayerSize(MAP::LAYER_BACKGROUND));
        }

        void MapPrototype::setLayerSize(uint32 size, Layer layer)
        {
            setSize(getSize(), layer == MAP::LAYER_FOREGROUND ? size : getLayerSize(MAP::LAYER_FOREGROUND),
                layer == MAP::LAYER_BACKGROUND ? size : getLayerSize(MAP::LAYER_BACKGROUND));
        }

        void MapPrototype::setSize(UInt32Point size, uint32 uiForegroundLayerSize, uint32 uiBackgroundLayerSize)
        {
            m_Layer.setSize(size, uiForegroundLayerSize, uiBackgroundLayerSize);
            if (hasMapDataStored())
                m_Layer.resizeMap(size, uiForegroundLayerSize, uiBackgroundLayerSize);
        }

        uint32 MapPrototype::getTile(UInt32Point3D at, Layer layer) const
        {
            if (hasMapDataStored())
                return m_Layer.getMapTile(at, layer).m_uiTileID;
            return MAX_UINT32;
        }

        void MapPrototype::setTile(UInt32Point3D at, uint32 uiID, Layer layer)
        {
            if (hasMapDataStored())
                m_Layer.getMapTile(at, layer).m_uiTileID = uiID;
        }

        uint32 MapPrototype::getAutoTile(UInt32Point3D at, Layer layer) const
        {
            if (hasMapDataStored())
                return m_Layer.getMapTile(at, layer).m_uiAutoTileSetID;
            return MAX_UINT32;
        }

        void MapPrototype::setAutoTile(UInt32Point3D at, uint32 uiID, Layer layer)
        {
            if (hasMapDataStored())
                m_Layer.getMapTile(at, layer).m_uiAutoTileSetID = uiID;
        }

        void MapPrototype::setMapTile(UInt32Point3D at, MapTile mapTile, Layer layer)
        {
            if (hasMapDataStored())
                m_Layer.getMapTile(at, layer) = mapTile;
        }

        MapTile MapPrototype::getMapTile(UInt32Point3D at, Layer layer) const
        {
            if (hasMapDataStored())
                return m_Layer.getMapTile(at, layer);
            return MapTile(MAX_UINT32, MAX_UINT32);
        }
    }
}