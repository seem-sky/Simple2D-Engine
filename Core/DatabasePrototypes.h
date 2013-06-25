#ifndef DATABASE_PROTOTYPES_H
#define DATABASE_PROTOTYPES_H

#include "Database.h"
#include "Color.h"

namespace DATABASE
{
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
    typedef boost::shared_ptr<TexturePrototype> TexturePrototypePtr;
    typedef boost::shared_ptr<const TexturePrototype> ConstTexturePrototypePtr;

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
    typedef boost::shared_ptr<TilePrototype> TilePrototypePtr;
    typedef boost::shared_ptr<const TilePrototype> ConstTilePrototypePtr;
    typedef std::vector<ConstTilePrototypePtr> ConstTilePrototypePtrVector;

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

    typedef boost::shared_ptr<AUTO_TILE::AutoTilePrototype> AutoTilePrototypePtr;
    typedef boost::shared_ptr<const AUTO_TILE::AutoTilePrototype> ConstAutoTilePrototypePtr;
    typedef std::vector<ConstAutoTilePrototypePtr> ConstAutoTilePrototypePtrVector;

    /*#####
    # SpritePrototype
    #####*/
    class SpritePrototype : public TexturePrototype
    {
    public:
        SpritePrototype(uint32 uiID = 0) : TexturePrototype(uiID) {}
    };
    typedef boost::shared_ptr<SpritePrototype> SpritePrototypePtr;
    typedef boost::shared_ptr<const SpritePrototype> ConstSpritePrototypePtr;
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
            Point<int32> m_Pos;
            uint32 m_uiSpriteID;
            uint16 m_uiRotation;
            uint32 m_uiScale;
            uint8 m_uiOpacity;
        };
        typedef std::vector<Sprite> SpriteVector;

        class Frame
        {
        public:
            Frame() : m_uiMsecTime(0) {}
            uint32 m_uiMsecTime;
            SpriteVector m_Sprites;
        };
        typedef std::vector<Frame> FrameVector;

    public:
        AnimationPrototype(uint32 uiID = 0) : Prototype(uiID) {}

        bool getFrame(uint32 uiIndex, Frame &frame) const;
        void setFrame(uint32 uiIndex, const Frame &frame);
        void removeFrame(uint32 uiIndex);
        inline uint32 getFrameCount() const { return m_Frames.size(); }

    private:
        FrameVector m_Frames;
    };
    typedef boost::shared_ptr<AnimationPrototype> AnimationPrototypePtr;
    typedef boost::shared_ptr<const AnimationPrototype> ConstAnimationPrototypePtr;
    typedef std::vector<ConstAnimationPrototypePtr> ConstAnimationPrototypePtrVector;

    /*#####
    # WorldObjectPrototype
    #####*/
    enum ObjectType
    {
        TYPE_WORLDOBJECT,
        TYPE_DYNAMIC_OBJECT
    };

    const uint32 MIN_WORLD_OBJECT_POSE = 4;
    class WorldObjectPrototype : public Prototype
    {
    public:
        WorldObjectPrototype(uint32 uiID = 0) : Prototype(uiID), m_uiAnimationSpeed(100)
        {
            // set minimum poses, so we have stand pose for all directions
            m_AnimationInfos.resize(MIN_WORLD_OBJECT_POSE, 0);
            for (uint32 i = 1; i <= MIN_WORLD_OBJECT_POSE; ++i)
                m_AnimationInfos.at(i-1).m_uiObjectAnimationTypeID = i;
        }

        inline uint32 getBoundingX() const { return m_BoundingRect.getPositionX(); }
        inline void setBoundingX(uint32 x) { m_BoundingRect.setPositionX(x); }
        inline uint32 getBoundingY() const { return m_BoundingRect.getPositionY(); }
        inline void setBoundingY(uint32 y) { m_BoundingRect.setPositionY(y); }
        inline uint32 getBoundingWidth() const { return m_BoundingRect.getWidth(); }
        inline void setBoundingWidth(uint32 width) { m_BoundingRect.setWidth(width); }
        inline uint32 getBoundingHeight() const { return m_BoundingRect.getHeight(); }
        inline void setBoundingHeight(uint32 height) { m_BoundingRect.setHeight(height); }
        inline Rect<uint32> getBoundingRect() const { return m_BoundingRect; }
        inline void setBoundingRect(const Rect<uint32> rect) { m_BoundingRect = rect; }

        inline void setAnimationSpeed(uint16 uiSpeed) { m_uiAnimationSpeed = uiSpeed; }
        inline uint16 getAnimationSpeed() const { return m_uiAnimationSpeed; }

        inline void setScriptName(const QString &sScriptName) { m_ScriptName = sScriptName; }
        inline QString getScriptName() const { return m_ScriptName; }

        static QString getTypeString(ObjectType type)
        {
            switch (type)
            {
            case TYPE_WORLDOBJECT: return "WorldObject";
            case TYPE_DYNAMIC_OBJECT: return "DynamicObject";
            }
            return "";
        }

        // animation stuff
        struct AnimationInfo
        {
            AnimationInfo(uint32 uiAnimationID = 0, uint32 uiAnimationTypeID = 0) : m_uiAnimationID(uiAnimationID), m_uiObjectAnimationTypeID(uiAnimationTypeID)
            {}

            uint32 m_uiAnimationID;
            uint32 m_uiObjectAnimationTypeID;
        };
        typedef std::vector<AnimationInfo> AnimationInfoVector;

        inline AnimationInfo getAnimationInfo(uint32 uiIndex) const
        {
            if (uiIndex < m_AnimationInfos.size())
                return m_AnimationInfos.at(uiIndex);
            return 0;
        }
        inline void setAnimationInfo(uint32 uiIndex, AnimationInfo animationInfo)
        {
            if (uiIndex >= m_AnimationInfos.size())
                m_AnimationInfos.resize(uiIndex+1);
            // do not change animation type id if its an standard entry
            if (uiIndex < MIN_WORLD_OBJECT_POSE)
                m_AnimationInfos.at(uiIndex).m_uiAnimationID = animationInfo.m_uiAnimationID;
            else
                m_AnimationInfos.at(uiIndex) = animationInfo;
        }
        inline uint32 getAnimationCount() const { return m_AnimationInfos.size(); }
        virtual void setAnimationCount(uint32 uiCount)
        {
            if (uiCount < MIN_WORLD_OBJECT_POSE || m_AnimationInfos.size() == uiCount)
                return;
            m_AnimationInfos.resize(uiCount);
        }

    private:
        Rect<uint32> m_BoundingRect;
        AnimationInfoVector m_AnimationInfos;
        uint16 m_uiAnimationSpeed;
        QString m_ScriptName;
    };
    typedef boost::shared_ptr<WorldObjectPrototype> WorldObjectPrototypePtr;
    typedef boost::shared_ptr<const WorldObjectPrototype> ConstWorldObjectPrototypePtr;
    typedef std::vector<ConstWorldObjectPrototypePtr> ConstWorldObjectPrototypePtrVector;

    /*#####
    # DynamicObjectPrototype
    #####*/
    const uint32 MIN_DYNAMIC_OBJECT_POSE = 8;
    class DynamicObjectPrototype : public WorldObjectPrototype
    {
    public:
        DynamicObjectPrototype(uint32 uiID = 0) : WorldObjectPrototype(uiID), m_uiSpeed(0) {}
        inline void setSpeed(uint16 uiSpeed) { m_uiSpeed = uiSpeed; }
        inline uint16 getSpeed() const { return m_uiSpeed; }

        void setAnimationCount(uint32 uiCount)
        {
            if (uiCount >= MIN_WORLD_OBJECT_POSE)
                WorldObjectPrototype::setAnimationCount(uiCount);
        }

    private:
        uint16 m_uiSpeed;
    };
    typedef boost::shared_ptr<DynamicObjectPrototype> DynamicObjectPrototypePtr;
    typedef boost::shared_ptr<const DynamicObjectPrototype> ConstDynamicObjectPrototypePtr;
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
    typedef boost::shared_ptr<ObjectAnimationTypePrototype> ObjectAnimationTypePrototypePtr;
    typedef boost::shared_ptr<const ObjectAnimationTypePrototype> ConstObjectAnimationTypePrototypePtr;
    typedef std::vector<ConstObjectAnimationTypePrototypePtr> ConstObjectAnimationPrototypePtrVector;

    /*#####
    # LocalisationPrototype
    #####*/
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
    typedef boost::shared_ptr<LocalisationPrototype> LocalisationPrototypePtr;
    typedef boost::shared_ptr<const LocalisationPrototype> ConstLocalisationPrototypePtr;
    typedef std::vector<ConstLocalisationPrototypePtr> ConstLocalisationPrototypePtrVector;
}
#endif