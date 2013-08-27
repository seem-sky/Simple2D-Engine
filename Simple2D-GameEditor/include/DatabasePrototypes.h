#ifndef DATABASE_PROTOTYPES_H
#define DATABASE_PROTOTYPES_H

#include "Color.h"
#include <boost/multi_array.hpp>
#include <QtCore/QString>
#include "MapLayer.h"
#include "Container.h"

namespace MAP
{
    class MapMgr;
}

namespace DATABASE
{
    /*#####
    # Prototype superclass
    #####*/
    class Prototype
    {
    public:
        Prototype(uint32 uiID) : m_uiID(uiID) {}

        inline void setName(const QString &sName) { m_sName = sName; }
        inline QString getName() const { return m_sName; }
        inline uint32 getID() const { return m_uiID; }
        inline void setID(uint32 uiID) { m_uiID = uiID; }

    private:
        uint32 m_uiID;
        QString m_sName;
    };
    typedef std::shared_ptr<Prototype> PrototypePtr;

    /*#####
    # TexturePrototype
    #####*/
    class TexturePrototype : public Prototype
    {
    public:
        TexturePrototype(uint32 uiID = 0) : Prototype(uiID) {}
        inline void setFileName(const QString &sFileName) { m_sFileName = sFileName; }
        inline QString getFileName() const { return m_sFileName; }
        inline void setPath(const QString &sPath) { m_sPath = sPath; }
        inline QString getPath() const { return m_sPath; }
        inline QString getPathName() const { return getPath() + getFileName(); }

        inline Color getTransparencyColor() const { return m_TransparencyColor; }
        inline void setTransparencyColor(const Color &color) { m_TransparencyColor = color; }

    private:
        Color m_TransparencyColor;
        QString m_sPath;        // path from ~/textures
        QString m_sFileName;    // filename
    };
    typedef std::shared_ptr<TexturePrototype> TexturePrototypePtr;
    typedef std::shared_ptr<const TexturePrototype> ConstTexturePrototypePtr;

    /*#####
    # TilePrototype
    #####*/
    class TilePrototype : public TexturePrototype
    {
    public:
        TilePrototype(uint32 uiID = 0) : TexturePrototype(uiID), m_uiPassable(0), m_uiTerrainType(0) {}

        // passability flag, if nothing set, its unpassable
        enum PassabilityFlag
        {
            PASSABLE_NONE           = 0x00,
            PASSABLE_UP             = 0x01,
            PASSABLE_DOWN           = 0x02,
            PASSABLE_RIGHT          = 0x04,
            PASSABLE_LEFT           = 0x08,
            PASSABLE_UP_TO_DOWN     = PASSABLE_UP|PASSABLE_DOWN,
            PASSABLE_LEFT_TO_RIGHT  = PASSABLE_LEFT|PASSABLE_RIGHT,
            PASSABLE_ALL            = PASSABLE_LEFT|PASSABLE_RIGHT|PASSABLE_DOWN|PASSABLE_UP,
        };
        inline bool hasPassabilityFlag(const PassabilityFlag &flag) const { return (m_uiPassable & flag) != 0; }
        inline void addPassabilityFlag(const uint8 &flag) { m_uiPassable |= flag; }
        inline void removePassabilityFlag(const uint8 &flag) { m_uiPassable ^= flag; }
        inline uint8 getPassability() const  { return m_uiPassable; }

        inline void setTerrainType(uint32 uiTerrainType) { m_uiTerrainType = uiTerrainType; }
        inline uint32 getTerrainType() const { return m_uiTerrainType; }

    private:
        uint8 m_uiPassable;
        uint32 m_uiTerrainType;
    };
    typedef std::shared_ptr<TilePrototype> TilePrototypePtr;
    typedef std::shared_ptr<const TilePrototype> ConstTilePrototypePtr;
    typedef std::vector<ConstTilePrototypePtr> ConstTilePrototypePtrVector;

    /*#####
    # TilePrototype
    #####*/
    namespace TILE_SET
    {
        typedef boost::multi_array<uint32, 2> UInt32Multiarray2D;
        const uint32 DEFAULT_COLUMN_COUNT = 2;
        const uint32 DEFAULT_ROW_COUNT = 2;
        class TileSetPrototype : public Prototype
        {
        private:
            void _resizeIfNeeded(UInt32Point size);

        public:
            TileSetPrototype(uint32 uiID = 0);

            void resizeTiles(UInt32Point size);
            inline UInt32Point getTileCount() const { return m_Size; }
            void clear();

            void setTileID(UInt32Point pos, uint32 ID);
            uint32 getTileID(UInt32Point pos) const;

        private:
            UInt32Point m_Size;
            UInt32Multiarray2D m_Tiles;
        };
    }
    typedef std::shared_ptr<TILE_SET::TileSetPrototype> TileSetPrototypePtr;
    typedef std::shared_ptr<const TILE_SET::TileSetPrototype> ConstTileSetPrototypePtr;
    typedef std::vector<ConstTileSetPrototypePtr> ConstTileSetPrototypePtrVector;

    /*#####
    # AutoTilePrototype
    #####*/
    namespace AUTO_TILE
    {
        enum TILE_CHECK
        {
            SAME_AROUND         = 0x00,
            OTHER_TOP_LEFT      = 0x01,
            OTHER_TOP           = 0x02,
            OTHER_TOP_RIGHT     = 0x04,
            OTHER_LEFT          = 0x08,
            OTHER_RIGHT         = 0x10,
            OTHER_BOTTOM_LEFT   = 0x20,
            OTHER_BOTTOM        = 0x40,
            OTHER_BOTTOM_RIGHT  = 0x80,
        };

        enum AUTO_TILE_INDEX
        {
            // auto tile set
            INDEX_TOP_LEFT,
            INDEX_TOP,
            INDEX_TOP_RIGHT,
            INDEX_LEFT,
            INDEX_CENTER,
            INDEX_RIGHT,
            INDEX_BOTTOM_LEFT,
            INDEX_BOTTOM,
            INDEX_BOTTOM_RIGHT,
            INDEX_INNER_CENTER,                     // 9

            // side ends
            INDEX_SIDE_END_TOP,                     // 10
            INDEX_SIDE_END_BOTTOM,
            INDEX_SIDE_END_LEFT,
            INDEX_SIDE_END_RIGHT,                   // 13

            // double sides
            INDEX_SIDE_VERTICAL,
            INDEX_SIDE_HORIZONTAL,                  // 15

            // T tiles
            INDEX_T_TOP,
            INDEX_T_BOTTOM,
            INDEX_T_LEFT,
            INDEX_T_RIGHT,                          // 19

            // curves
            INDEX_CURVE_TOP_LEFT,
            INDEX_CURVE_TOP_RIGHT,
            INDEX_CURVE_BOTTOM_LEFT,
            INDEX_CURVE_BOTTOM_RIGHT,               // 23

            // inner edges
            INDEX_INNER_EDGE_TOP_LEFT,
            INDEX_INNER_EDGE_TOP_RIGHT,
            INDEX_INNER_EDGE_BOTTOM_LEFT,
            INDEX_INNER_EDGE_BOTTOM_RIGHT,          // 27

            // diagonal inner edges
            INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT,
            INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT, // 29

            // multi inner edges
            INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT,
            INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT,
            INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT,
            INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT,// 33

            // circle
            INDEX_CIRCLE,                           // 34

            // Y tiles
            INDEX_Y_TOP_BOTTOM_LEFT,
            INDEX_Y_TOP_BOTTOM_RIGHT,
            INDEX_Y_LEFT_TOP_RIGHT,
            INDEX_Y_LEFT_BOTTOM_RIGHT,              // 38
            INDEX_Y_BOTTOM_TOP_LEFT,
            INDEX_Y_BOTTOM_TOP_RIGHT,
            INDEX_Y_RIGHT_TOP_LEFT,
            INDEX_Y_RIGHT_BOTTOM_LEFT,              // 42

            INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT,
            INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT,
            INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT,
            INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT,    // 46

            INDEX_NONE
        };

        enum AUTO_TILE_TILE_REQUIREMENTS
        {
            CENTER                              = SAME_AROUND,

            SIDE_LEFT                           = OTHER_LEFT,
            SIDE_RIGHT                          = OTHER_RIGHT,
            SIDE_TOP                            = OTHER_TOP,
            SIDE_BOTTOM                         = OTHER_BOTTOM,

            SIDE_VERTICAL                       = SIDE_LEFT | SIDE_RIGHT,
            SIDE_HORIZONTAL                     = SIDE_TOP | SIDE_BOTTOM,

            EDGE_TOP_LEFT                       = SIDE_TOP | SIDE_LEFT,
            EDGE_TOP_RIGHT                      = SIDE_TOP | SIDE_RIGHT,
            EDGE_BOTTOM_LEFT                    = SIDE_BOTTOM | SIDE_LEFT,
            EDGE_BOTTOM_RIGHT                   = SIDE_BOTTOM | SIDE_RIGHT,

            LEFT_END                            = EDGE_TOP_LEFT | EDGE_BOTTOM_LEFT,
            RIGHT_END                           = EDGE_TOP_RIGHT | EDGE_BOTTOM_RIGHT,
            TOP_END                             = EDGE_TOP_LEFT | EDGE_TOP_RIGHT,
            BOTTOM_END                          = EDGE_BOTTOM_LEFT | EDGE_BOTTOM_RIGHT,

            CIRCLE                              = EDGE_TOP_LEFT | EDGE_BOTTOM_RIGHT,

            INNER_EDGE_TOP_LEFT                 = OTHER_TOP_LEFT,
            INNER_EDGE_TOP_RIGHT                = OTHER_TOP_RIGHT,
            INNER_EDGE_BOTTOM_LEFT              = OTHER_BOTTOM_LEFT,
            INNER_EDGE_BOTTOM_RIGHT             = OTHER_BOTTOM_RIGHT,

            INNER_EDGE_TOP_LEFT_TOP_RIGHT       = INNER_EDGE_TOP_LEFT | INNER_EDGE_TOP_RIGHT,
            INNER_EDGE_TOP_LEFT_BOTTOM_LEFT     = INNER_EDGE_TOP_LEFT | INNER_EDGE_BOTTOM_LEFT,
            INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT   = INNER_EDGE_TOP_RIGHT | INNER_EDGE_BOTTOM_RIGHT,
            INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT = INNER_EDGE_BOTTOM_LEFT | INNER_EDGE_BOTTOM_RIGHT,

            INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT    = INNER_EDGE_TOP_LEFT | INNER_EDGE_BOTTOM_RIGHT,
            INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT    = INNER_EDGE_TOP_RIGHT | INNER_EDGE_BOTTOM_LEFT,

            INNER_EDGE_CENTER                   = INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT | INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT,

            CURVE_TOP_LEFT                      = EDGE_TOP_LEFT | INNER_EDGE_BOTTOM_RIGHT,
            CURVE_TOP_RIGHT                     = EDGE_TOP_RIGHT | INNER_EDGE_BOTTOM_LEFT,
            CURVE_BOTTOM_LEFT                   = EDGE_BOTTOM_LEFT | INNER_EDGE_TOP_RIGHT,
            CURVE_BOTTOM_RIGHT                  = EDGE_BOTTOM_RIGHT | INNER_EDGE_TOP_LEFT,

            T_TOP                               = SIDE_TOP | INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT,
            T_BOTTOM                            = SIDE_BOTTOM | INNER_EDGE_TOP_LEFT_TOP_RIGHT,
            T_LEFT                              = SIDE_LEFT | INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT,
            T_RIGHT                             = SIDE_RIGHT | INNER_EDGE_TOP_LEFT_BOTTOM_LEFT,

            Y_TOP_BOTTOM_LEFT                   = SIDE_TOP | INNER_EDGE_BOTTOM_LEFT,
            Y_TOP_BOTTOM_RIGHT                  = SIDE_TOP | INNER_EDGE_BOTTOM_RIGHT,
            Y_LEFT_TOP_RIGHT                    = SIDE_LEFT | INNER_EDGE_TOP_RIGHT,
            Y_LEFT_BOTTOM_RIGHT                 = SIDE_LEFT | INNER_EDGE_BOTTOM_RIGHT,
            Y_BOTTOM_TOP_LEFT                   = SIDE_BOTTOM | INNER_EDGE_TOP_LEFT,
            Y_BOTTOM_TOP_RIGHT                  = SIDE_BOTTOM | INNER_EDGE_TOP_RIGHT,
            Y_RIGHT_TOP_LEFT                    = SIDE_RIGHT | INNER_EDGE_TOP_LEFT,
            Y_RIGHT_BOTTOM_LEFT                 = SIDE_RIGHT | INNER_EDGE_BOTTOM_LEFT,

            INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT       = INNER_EDGE_TOP_LEFT_TOP_RIGHT | INNER_EDGE_BOTTOM_LEFT,
            INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT      = INNER_EDGE_TOP_LEFT_TOP_RIGHT | INNER_EDGE_BOTTOM_RIGHT,
            INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT    = INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT | INNER_EDGE_TOP_LEFT,
            INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT   = INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT | INNER_EDGE_TOP_RIGHT
        };

        const uint32 MAX_AUTO_TILE_COLUMNS      = 3;
        const uint32 AUTO_TILE_SET_COUNT        = 10;
        class AutoTilePrototype : public Prototype
        {
        public:
            AutoTilePrototype(uint32 uiID = 0) : Prototype(uiID)
            {
                memset(&m_uiTileID, NULL, sizeof(m_uiTileID));
            }

            inline void setTileID(AUTO_TILE_INDEX index, uint32 uiID) { m_uiTileID[index] = uiID; }
            inline uint32 getTileID(AUTO_TILE_INDEX index) const { return m_uiTileID[index]; }

        private:
            uint32 m_uiTileID[AUTO_TILE_SET_COUNT];
        };

        AUTO_TILE_INDEX getAutoTileIndexForTileCheck(uint32 uiTileCheck);
        bool hasTileCheck(uint32 uiTileCheck, AUTO_TILE_TILE_REQUIREMENTS tiles);
    };

    typedef std::shared_ptr<AUTO_TILE::AutoTilePrototype> AutoTilePrototypePtr;
    typedef std::shared_ptr<const AUTO_TILE::AutoTilePrototype> ConstAutoTilePrototypePtr;
    typedef std::vector<ConstAutoTilePrototypePtr> ConstAutoTilePrototypePtrVector;

    /*#####
    # SpritePrototype
    #####*/
    class SpritePrototype : public TexturePrototype
    {
    public:
        SpritePrototype(uint32 uiID = 0) : TexturePrototype(uiID) {}
    };
    typedef std::shared_ptr<SpritePrototype> SpritePrototypePtr;
    typedef std::shared_ptr<const SpritePrototype> ConstSpritePrototypePtr;
    typedef std::vector<ConstSpritePrototypePtr> ConstSpritePrototypePtrVector;

    /*#####
    # AnimationPrototype
    #####*/
    class AnimationPrototype : public Prototype
    {
    public:
        class Sprite
        {
        public:
            Sprite() : m_uiSpriteID(0), m_uiRotation(0), m_uiScale(100), m_uiOpacity(100) {}
            Int32Point m_Pos;
            uint32 m_uiSpriteID;
            uint16 m_uiRotation;
            uint32 m_uiScale;
            uint8 m_uiOpacity;

            inline bool isEmpty() const { return !m_uiSpriteID; }
        };
        typedef std::vector<Sprite> SpriteVector;

        class Frame
        {
        public:
            Frame() : m_uiMsecTime(0) {}

            Sprite getSprite(uint32 index) const;
            void setSprite(uint32 index, Sprite sprite);
            void addSprite(Sprite sprite);
            void removeSprite(uint32 index, Sprite sprite);

            inline uint32 getSpriteCount() const { return m_Sprites.size(); }
            inline void setSpriteCount(uint32 size) { m_Sprites.resize(size); calculateOffset(); }
            inline const SpriteVector& getSprites() const { return m_Sprites; }

            inline const Int32Point& getOffset() const { return m_FrameOffset; }
            void calculateOffset();
            void setOffsetIfNeeded(const Int32Point &offset);

            inline uint32 getTimeInMsec() const { return m_uiMsecTime; }
            inline void setTimeInMsec(uint32 time) { m_uiMsecTime = time; }

            inline bool isEmpty() const { return m_Sprites.empty() && !m_uiMsecTime; }

        private:
            Int32Point m_FrameOffset;
            SpriteVector m_Sprites;
            uint32 m_uiMsecTime;
        };
        typedef std::vector<Frame> FrameVector;

    public:
        AnimationPrototype(uint32 uiID = 0) : Prototype(uiID) {}

        bool getFrame(uint32 uiIndex, Frame &frame) const;
        void setFrame(uint32 uiIndex, Frame frame);
        void removeFrame(uint32 uiIndex);
        inline uint32 getFrameCount() const { return m_Frames.size(); }

    private:
        FrameVector m_Frames;
    };
    typedef std::shared_ptr<AnimationPrototype> AnimationPrototypePtr;
    typedef std::shared_ptr<const AnimationPrototype> ConstAnimationPrototypePtr;
    typedef std::vector<ConstAnimationPrototypePtr> ConstAnimationPrototypePtrVector;

    /*#####
    # WorldObjectPrototype
    #####*/
    namespace MAP_OBJECT
    {
        enum ObjectType
        {
            TYPE_WORLDOBJECT,
            TYPE_DYNAMIC_OBJECT
        };

        // animation stuff
        const uint32 MIN_WORLD_OBJECT_POSE = 4;
        struct AnimationInfo
        {
            AnimationInfo(uint32 uiAnimationID = 0, uint32 uiAnimationTypeID = 0) : m_uiAnimationID(uiAnimationID), m_uiObjectAnimationTypeID(uiAnimationTypeID)
            {}

            uint32 m_uiAnimationID;
            uint32 m_uiObjectAnimationTypeID;
        };
        typedef std::shared_ptr<AnimationInfo> AnimationInfoPtr;
        typedef std::shared_ptr<const AnimationInfo> ConstAnimationInfoPtr;
        typedef Container<AnimationInfo> AnimationInfoContainer;

        class WorldObjectPrototype : public Prototype
        {
        public:
            WorldObjectPrototype(uint32 uiID = 0);

            inline int32 getBoundingX() const { return m_BoundingRect.getPositionX(); }
            inline void setBoundingX(int32 x) { m_BoundingRect.setPositionX(x); }
            inline int32 getBoundingY() const { return m_BoundingRect.getPositionY(); }
            inline void setBoundingY(int32 y) { m_BoundingRect.setPositionY(y); }
            inline uint32 getBoundingWidth() const { return m_BoundingRect.getWidth(); }
            inline void setBoundingWidth(uint32 width) { m_BoundingRect.setWidth(width); }
            inline uint32 getBoundingHeight() const { return m_BoundingRect.getHeight(); }
            inline void setBoundingHeight(uint32 height) { m_BoundingRect.setHeight(height); }
            inline Int32Rect getBoundingRect() const { return m_BoundingRect; }
            inline void setBoundingRect(Int32Rect rect) { m_BoundingRect = std::move(rect); }

            inline void setAnimationSpeed(uint16 uiSpeed) { m_uiAnimationSpeed = uiSpeed; }
            inline uint16 getAnimationSpeed() const { return m_uiAnimationSpeed; }

            inline void setScriptName(const QString &sScriptName) { m_ScriptName = sScriptName; }
            inline QString getScriptName() const { return m_ScriptName; }

            AnimationInfo getAnimationInfo(uint32 uiIndex) const;
            void setAnimationInfo(uint32 uiIndex, AnimationInfo animationInfo);

            inline uint32 getAnimationCount() const { return m_AnimationInfos.getSize(); }
            void setAnimationCount(uint32 uiCount);

            virtual uint32 getMinimumAnimationCount() const { return MIN_WORLD_OBJECT_POSE; }

        private:
            Int32Rect m_BoundingRect;
            AnimationInfoContainer m_AnimationInfos;
            uint16 m_uiAnimationSpeed;
            QString m_ScriptName;
        };

        /*#####
        # DynamicObjectPrototype
        #####*/
        const uint32 MIN_DYNAMIC_OBJECT_POSE = 8;
        class DynamicObjectPrototype : public WorldObjectPrototype
        {
        public:
            DynamicObjectPrototype(uint32 uiID = 0) : WorldObjectPrototype(uiID), m_uiMovementSpeed(0) {}

            inline void setMovementSpeed(uint16 uiSpeed) { m_uiMovementSpeed = uiSpeed; }
            inline uint16 getMovementSpeed() const { return m_uiMovementSpeed; }

            uint32 getMinimumAnimationCount() const { return MIN_DYNAMIC_OBJECT_POSE; }

        private:
            uint16 m_uiMovementSpeed;
        };

        QString getTypeString(ObjectType type);
    }
    typedef std::shared_ptr<MAP_OBJECT::WorldObjectPrototype> WorldObjectPrototypePtr;
    typedef std::shared_ptr<const MAP_OBJECT::WorldObjectPrototype> ConstWorldObjectPrototypePtr;
    typedef std::vector<ConstWorldObjectPrototypePtr> ConstWorldObjectPrototypePtrVector;

    typedef std::shared_ptr<MAP_OBJECT::DynamicObjectPrototype> DynamicObjectPrototypePtr;
    typedef std::shared_ptr<const MAP_OBJECT::DynamicObjectPrototype> ConstDynamicObjectPrototypePtr;
    typedef std::vector<ConstDynamicObjectPrototypePtr> ConstDynamicObjectPrototypePtrVector;

    /*#####
    # ObjectAnimationPrototype
    #####*/
    class ObjectAnimationTypePrototype : public Prototype
    {
    public:
        ObjectAnimationTypePrototype(uint32 uiID = 0, QString sName = "") : Prototype(uiID)
        {
            setName(sName);
        }
    };
    typedef std::shared_ptr<ObjectAnimationTypePrototype> ObjectAnimationTypePrototypePtr;
    typedef std::shared_ptr<const ObjectAnimationTypePrototype> ConstObjectAnimationTypePrototypePtr;
    typedef std::vector<ConstObjectAnimationTypePrototypePtr> ConstObjectAnimationPrototypePtrVector;

    /*#####
    # LocalisationPrototype
    #####*/
    typedef std::vector<QString> StringVector;
    class LocalisationPrototype : public Prototype
    {
    public:
        LocalisationPrototype(uint32 uiID = 0, uint32 uiLocalCount = 1) : Prototype(uiID)
        {
            setLocalsCount(uiLocalCount);
        }

        inline void setLocalsCount(uint32 uiCount) { m_Locals.resize(uiCount); }
        inline uint32 getLocalsCount() const { return m_Locals.size(); }
        inline void setLocalisation(uint32 uiIndex, const QString &sLocal)
        {
            if (uiIndex >= getLocalsCount())
                setLocalsCount(uiIndex+1);
            m_Locals.at(uiIndex) = sLocal;
        }

        inline QString getLocalisation(uint32 uiIndex)
        {
            if (uiIndex < getLocalsCount())
                return m_Locals.at(uiIndex);
            return "";
        }

    private:
        StringVector m_Locals;
    };
    typedef std::shared_ptr<LocalisationPrototype> LocalisationPrototypePtr;
    typedef std::shared_ptr<const LocalisationPrototype> ConstLocalisationPrototypePtr;
    typedef std::vector<ConstLocalisationPrototypePtr> ConstLocalisationPrototypePtrVector;

    /*#####
    # MapPrototype
    #####*/
    class MapDatabaseXMLReader;
    namespace MAP_STRUCTURE
    {
        enum MapDirection
        {
            DIRECTION_UP,
            DIRECTION_RIGHT,
            DIRECTION_DOWN,
            DIRECTION_LEFT
        };

        enum MapObjectLayer
        {
            LAYER_LOWER,
            LAYER_MIDDLE,
            LAYER_UPPER
        };

        // map objects
        struct MapObject
        {
            MapObject() : m_Type(DATABASE::MAP_OBJECT::TYPE_WORLDOBJECT), m_ObjectID(0), m_GUID(0), m_Direction(DIRECTION_DOWN), m_Layer(LAYER_MIDDLE) {}

            bool isEmpty() const { return !m_GUID && !m_ObjectID; }

            DATABASE::MAP_OBJECT::ObjectType m_Type;
            uint32 m_ObjectID;
            uint32 m_GUID;
            Int32Point m_Position;
            MapDirection m_Direction;
            MapObjectLayer m_Layer;
        };
        typedef std::shared_ptr<MapObject> MapObjectPtr;
        typedef std::shared_ptr<const MapObject> ConstMapObjectPtr;
        typedef Container<MapObject> MapObjectContainer;
        typedef std::vector<MapObjectPtr> MapObjectPtrVector;
        typedef boost::multi_array<MapObjectPtrVector, 3> MapObjectPtrVectorMultiarray3D;

        // map infos
        typedef MAP::MapTile MapTile;
        typedef MAP::Layer Layer;
        class MapPrototype : public DATABASE::Prototype
        {
            friend class MapReader;
            friend class MapWriter;
            friend class MapDatabase;
            friend class MAP::MapMgr;
            friend class MapDatabaseXMLReader;

        private:
            void _clearTiles();

        public:
            MapPrototype(uint32 uiID = 0, const QString &sFileName = "");

            inline bool hasMapDataStored() const { return m_DataLoaded; }

            inline QString getFileName() const { return m_FileName; }
            inline void setFileName(const QString &sFileName) { m_FileName = sFileName; }
            inline void setScriptName(const QString &sScriptName) { m_ScriptName = sScriptName; }
            inline QString getScriptName() const { return m_ScriptName; }

            void setSizeX(uint32 x);
            void setSizeY(uint32 y);
            void setLayerSize(uint32 size, Layer layer);
            void setSize(UInt32Point size, uint32 uiForegroundLayerSize, uint32 uiBackgroundLayerSize);
            inline UInt32Point getSize() const { return m_Layer.getSize(); }
            inline uint32 getLayerSize(Layer layer) const { return m_Layer.getLayerSize(layer); };

            inline uint32 getParentID() const { return m_uiParentID; }
            inline void setParentID(uint32 uiParentID) { m_uiParentID = uiParentID; }

            uint32 getTile(UInt32Point3D at, Layer layer) const;
            void setTile(UInt32Point3D at, uint32 uiID, Layer layer);

            uint32 getAutoTile(UInt32Point3D at, Layer layer) const;
            void setAutoTile(UInt32Point3D at, uint32 uiID, Layer layer);

            void setMapTile(UInt32Point3D at, MapTile mapTile, Layer layer);
            MapTile getMapTile(UInt32Point3D at, Layer layer) const;

            void addMapObject(MapObjectPtr pObject);
            MapObjectPtr addMapObject(DATABASE::MAP_OBJECT::ObjectType type, uint32 uiID, Int32Point pos);
            inline uint32 getMapObjectCount() const { return m_Objects.getSize(); }
            inline bool getMapObject(uint32 GUID, ConstMapObjectPtr &pResult) const { return m_Objects.getItem(GUID, pResult); }
            inline bool getMapObject(uint32 GUID, MapObjectPtr &pResult)  { return m_Objects.getItem(GUID, pResult); }
            void removeMapObject(uint32 GUID);
            inline const MapObjectContainer& getMapObjects() const { return m_Objects; }

            enum RESULT_FLAG
            {
                FLAG_NOTHING    = 0x0,
                FLAG_SAME       = 0x1,
                FLAG_OTHER      = 0x2,
                FLAG_ALL        = FLAG_SAME | FLAG_OTHER
            };
            uint32 checkAutoTiles(const uint32 &uiID, const UInt32Point3D &pos, UInt32PointSet &result, Layer layer, uint32 resultFlag = FLAG_ALL);

        private:
            bool m_DataLoaded;

            uint32 m_uiParentID;
            QString m_FileName;
            QString m_ScriptName;

            MapObjectContainer m_Objects;
            MAP::MapLayer m_Layer;
        };
        typedef std::shared_ptr<MapPrototype> MapPrototypePtr;
        typedef std::shared_ptr<const MapPrototype> ConstMapPrototypePtr;
        typedef std::vector<MapPrototypePtr> MapPrototypePtrVector;
        typedef std::map<uint32, MapPrototypePtr> MapPrototypePtrMap;
    }
}
#endif