#ifndef DATABASE_PROTOTYPES_H
#define DATABASE_PROTOTYPES_H

#include "Color.h"
#include <boost/multi_array.hpp>
#include <QtCore/QString>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>
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
        Prototype(uint32 ID = 0) : m_ID(ID) {}

        inline void setName(const QString &sName) { m_Name = sName; }
        inline QString getName() const { return m_Name; }
        inline uint32 getID() const { return m_ID; }
        inline void setID(uint32 ID) { m_ID = ID; }

        // IO
        virtual void toXML(QXmlStreamWriter &writer) const;
        virtual void fromXML(const QXmlStreamAttributes &attributes);
        virtual void insertChildren(const QXmlStreamReader &reader) {}

    private:
        uint32 m_ID;
        QString m_Name;
    };

    /*#####
    # ResourcePrototype
    #####*/
    class RessourcePrototype : public Prototype
    {
    public:
        RessourcePrototype(uint32 ID = 0) : Prototype(ID) {}

        inline void setFileName(const QString &sFileName) { m_FileName = sFileName; }
        inline QString getFileName() const { return m_FileName; }
        inline void setPath(const QString &sPath) { m_Path = sPath; }
        inline QString getPath() const { return m_Path; }
        inline QString getPathName() const { return getPath() + getFileName(); }

        // IO
        virtual void toXML(QXmlStreamWriter &writer) const;
        virtual void fromXML(const QXmlStreamAttributes &attributes);

    private:
        QString m_Path;        // path from ~/textures
        QString m_FileName;    // filename
    };

    /*#####
    # TexturePrototype
    #####*/
    class TexturePrototype : public RessourcePrototype
    {
    public:
        TexturePrototype(uint32 ID = 0) : RessourcePrototype(ID) {}

        inline Color getTransparencyColor() const { return m_TransparencyColor; }
        inline void setTransparencyColor(const Color &color) { m_TransparencyColor = color; }

        // IO
        virtual void toXML(QXmlStreamWriter &writer) const;
        virtual void fromXML(const QXmlStreamAttributes &attributes);

    private:
        Color m_TransparencyColor;
    };

    /*#####
    # TilePrototype
    #####*/
    class TilePrototype : public TexturePrototype
    {
    public:
        TilePrototype(uint32 ID = 0) : TexturePrototype(ID), m_uiPassable(0), m_uiTerrainType(0) {}

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
        inline bool hasPassabilityFlag(const PassabilityFlag flag) const { return (m_uiPassable & flag) != 0; }
        inline void addPassabilityFlag(const uint8 flag) { m_uiPassable |= flag; }
        inline void removePassabilityFlag(const uint8 flag) { m_uiPassable ^= flag; }
        inline uint8 getPassability() const  { return m_uiPassable; }

        inline void setTerrainType(uint32 uiTerrainType) { m_uiTerrainType = uiTerrainType; }
        inline uint32 getTerrainType() const { return m_uiTerrainType; }

        // IO
        void toXML(QXmlStreamWriter &writer) const;
        void fromXML(const QXmlStreamAttributes &attributes);

    private:
        uint8 m_uiPassable;
        uint32 m_uiTerrainType;
    };

    /*#####
    # TileSetPrototype
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
            TileSetPrototype(uint32 ID = 0);

            void resizeTiles(UInt32Point size);
            inline UInt32Point getTileCount() const { return m_Size; }
            void clear();

            void setTileID(UInt32Point pos, uint32 ID);
            uint32 getTileID(UInt32Point pos) const;

            // IO
            void toXML(QXmlStreamWriter &writer) const;
            void fromXML(const QXmlStreamAttributes &attributes);

        private:
            UInt32Point m_Size;
            UInt32Multiarray2D m_Tiles;
        };
    }

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
            AutoTilePrototype(uint32 ID = 0) : Prototype(ID) {}

            inline void setTileID(AUTO_TILE_INDEX index, uint32 ID) { m_uiTileID.at(index) = ID; }
            inline uint32 getTileID(AUTO_TILE_INDEX index) const { return m_uiTileID.at(index); }

            // IO
            virtual void toXML(QXmlStreamWriter &writer) const;
            virtual void fromXML(const QXmlStreamAttributes &attributes);

        private:
            std::array<TILE_INDEX, AUTO_TILE_SET_COUNT> m_uiTileID;
        };

        AUTO_TILE_INDEX getAutoTileIndexForTileCheck(uint32 uiTileCheck);
        bool hasTileCheck(uint32 uiTileCheck, AUTO_TILE_TILE_REQUIREMENTS tiles);
    };

    /*#####
    # SpritePrototype
    #####*/
    class SpritePrototype : public TexturePrototype
    {
    public:
        SpritePrototype(uint32 ID = 0) : TexturePrototype(ID) {}
    };

    /*#####
    # Animation
    #####*/
    namespace ANIMATION
    {
        /*#####
        # AnimationTypePrototype
        #####*/
        class AnimationTypePrototype : public Prototype
        {
        public:
            AnimationTypePrototype(uint32 ID = 0, QString name = "") : Prototype(ID)
            {
                setName(name);
            }
        };

        /*#####
        # AnimationPrototype
        #####*/
        class Sprite
        {
        public:
            Sprite() : m_uiSpriteID(0), m_uiRotation(0), m_Scale(100), m_Opacity(100) {}
            Int32Point m_Pos;
            uint32 m_uiSpriteID;
            uint16 m_uiRotation;
            float m_Scale;
            float m_Opacity;

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

        class AnimationPrototype : public Prototype
        {
        public:
            AnimationPrototype(uint32 ID = 0) : Prototype(ID) {}

            bool getFrame(uint32 uiIndex, Frame &frame) const;
            void setFrame(uint32 uiIndex, Frame frame);
            void removeFrame(uint32 uiIndex);
            inline uint32 getFrameCount() const { return m_Frames.size(); }
            inline const FrameVector& getAnimation() const { return m_Frames; }
            inline void setAnimation(const FrameVector& animation) { m_Frames = animation; }

            // IO
            void toXML(QXmlStreamWriter &writer) const;
            void insertChildren(const QXmlStreamReader &reader);

        private:
            FrameVector m_Frames;
        };
    }

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
        const uint8 MIN_WORLD_OBJECT_POSE = 4;
        struct AnimationInfo
        {
            AnimationInfo(uint32 uiAnimationID = 0, uint32 uiAnimationTypeID = 0) : m_uiAnimationID(uiAnimationID), m_uiAnimationTypeID(uiAnimationTypeID)
            {}

            uint32 m_uiAnimationID;
            uint32 m_uiAnimationTypeID;
        };
        typedef std::vector<AnimationInfo> AnimationInfoVector;

        class WorldObjectPrototype : public Prototype
        {
        private:
            void initAnimationPoses();

        public:
            WorldObjectPrototype(uint32 ID = 0);

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

            inline const AnimationInfo& getAnimationInfo(uint32 uiIndex) const { return m_AnimationInfos.at(uiIndex); }
            inline AnimationInfo& getAnimationInfo(uint32 uiIndex) { return m_AnimationInfos.at(uiIndex); }
            void setAnimationInfo(uint32 uiIndex, AnimationInfo animationInfo);

            inline uint32 getAnimationCount() const { return m_AnimationInfos.size(); }
            void setAnimationCount(uint32 uiCount);

            virtual uint8 getMinimumAnimationCount() const { return MIN_WORLD_OBJECT_POSE; }

            // IO
            virtual void toXML(QXmlStreamWriter &writer) const;
            virtual void fromXML(const QXmlStreamAttributes &attributes);
            virtual void insertChildren(const QXmlStreamReader &reader);

        private:
            Int32Rect m_BoundingRect;
            AnimationInfoVector m_AnimationInfos;
            uint16 m_uiAnimationSpeed;
            QString m_ScriptName;
        };

        /*#####
        # DynamicObjectPrototype
        #####*/
        const uint8 MIN_DYNAMIC_OBJECT_POSE = 8;
        class DynamicObjectPrototype : public WorldObjectPrototype
        {
        public:
            DynamicObjectPrototype(uint32 ID = 0) : WorldObjectPrototype(ID), m_uiMovementSpeed(TILE_SIZE) {}

            inline void setMovementSpeed(uint16 uiSpeed) { m_uiMovementSpeed = uiSpeed; }
            inline uint16 getMovementSpeed() const { return m_uiMovementSpeed; }

            uint8 getMinimumAnimationCount() const { return MIN_DYNAMIC_OBJECT_POSE; }

        private:
            uint16 m_uiMovementSpeed;
        };

        QString getTypeString(ObjectType type);
    }

    /*#####
    # LocalisationPrototype
    #####*/
    typedef std::vector<QString> StringVector;
    class LocalisationPrototype : public Prototype
    {
    public:
        LocalisationPrototype(uint32 ID = 0, uint32 uiLocalCount = 1) : Prototype(ID)
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

        inline QString getLocalisation(uint32 uiIndex) const
        {
            if (uiIndex < getLocalsCount())
                return m_Locals.at(uiIndex);
            return "";
        }

        // IO
        void toXML(QXmlStreamWriter &writer) const;
        void insertChildren(const QXmlStreamReader &reader);

    private:
        StringVector m_Locals;
    };

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
        typedef Container<MapObject> MapObjectContainer;
        typedef boost::multi_array<MapObjectContainer, 3> MapObjectPtrVectorMultiarray3D;

        // map infos
        typedef MAP::MapTile MapTile;
        typedef MAP::Layer Layer;
        class MapPrototype : public Prototype
        {
            friend class MapDatabase;
            friend class MAP::MapMgr;
            friend class MapDatabaseXMLReader;

        private:
            void _clearTiles();

        public:
            MapPrototype(uint32 ID = 0, const QString &fileName = "") : Prototype(ID), m_FileName(fileName), m_uiParentID(0), m_DataLoaded(false)
            {}

            inline bool hasMapDataStored() const { return m_DataLoaded; }

            inline QString getFileName() const { return m_FileName; }
            inline void setFileName(const QString &sFileName) { m_FileName = sFileName; }
            inline void setScriptName(const QString &sScriptName) { m_ScriptName = sScriptName; }
            inline QString getScriptName() const { return m_ScriptName; }

            void setSizeX(uint32 x);
            void setSizeY(uint32 y);
            void setLayerSize(uint8 size, Layer layer);
            void setSize(UInt32Point size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize);
            inline UInt32Point getSize() const { return m_Layer.getSize(); }
            inline uint8 getLayerSize(Layer layer) const { return m_Layer.getLayerSize(layer); };

            inline uint32 getParentID() const { return m_uiParentID; }
            inline void setParentID(uint32 uiParentID) { m_uiParentID = uiParentID; }

            TILE_INDEX getTile(UInt32Point3D at, Layer layer) const;
            void setTile(UInt32Point3D at, TILE_INDEX ID, Layer layer);

            AUTO_TILE_INDEX getAutoTile(UInt32Point3D at, Layer layer) const;
            void setAutoTile(UInt32Point3D at, AUTO_TILE_INDEX ID, Layer layer);

            void setMapTile(UInt32Point3D at, MapTile mapTile, Layer layer);
            MapTile getMapTile(UInt32Point3D at, Layer layer) const;

            void addMapObject(MapObject *pObject);
            MapObject* addMapObject(DATABASE::MAP_OBJECT::ObjectType type, uint32 ID, Int32Point pos);
            inline uint32 getMapObjectCount() const { return m_Objects.getSize(); }
            inline const MapObject* getMapObject(uint32 GUID) const { return m_Objects.getItem(GUID); }
            inline MapObject* getMapObject(uint32 GUID)  { return m_Objects.getItem(GUID); }
            void removeMapObject(uint32 GUID);
            inline const MapObjectContainer& getMapObjects() const { return m_Objects; }

            enum RESULT_FLAG
            {
                FLAG_NOTHING    = 0x0,
                FLAG_SAME       = 0x1,
                FLAG_OTHER      = 0x2,
                FLAG_ALL        = FLAG_SAME | FLAG_OTHER
            };
            uint32 checkAutoTiles(uint32 ID, UInt32Point3D pos, UInt32PointSet &result, Layer layer, uint32 resultFlag = FLAG_ALL);

            // IO
            void toXML(QXmlStreamWriter &writer) const;
            void fromXML(const QXmlStreamAttributes &attributes);

        private:
            bool m_DataLoaded;

            uint32 m_uiParentID;
            QString m_FileName;
            QString m_ScriptName;

            MapObjectContainer m_Objects;
            MAP::MapLayer m_Layer;
        };
    }
}
#endif