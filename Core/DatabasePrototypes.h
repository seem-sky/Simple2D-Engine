#ifndef DATABASE_PROTOTYPES_H
#define DATABASE_PROTOTYPES_H

#include "Global.h"
#include <boost/smart_ptr.hpp>
#include "Color.h"

namespace DATABASE
{
    /*#####
    # Prototype superclass
    #####*/
    class Prototype
    {
    public:
        Prototype(uint32 uiID) : m_uiID(uiID) {}

        inline void setName(const std::string &sName) { m_sName = sName; }
        inline std::string getName() const { return m_sName; }
        inline uint32 getID() const { return m_uiID; }
        inline void setID(uint32 uiID) { m_uiID = uiID; }

    private:
        uint32 m_uiID;
        std::string m_sName;
    };
    typedef boost::shared_ptr<Prototype> PrototypePtr;
    typedef std::vector<PrototypePtr> PrototypePtrVector;

    /*#####
    # Database
    #####*/
    template <class T>
    class DatabaseChanger;
    template <class T>
    class Database
    {
        friend class DatabaseMgr;
        friend class DatabaseChanger<T>;
    protected:
        void clear() { m_Prototypes.clear(); }

    public:
        void setPrototype(uint32 uiID, boost::shared_ptr<T> &prototype)
        {
            if (uiID)
            {
                if (--uiID >= m_Prototypes.size())
                    m_Prototypes.resize(uiID+1);
                m_Prototypes.at(uiID) = prototype;
            }
        }

        bool getPrototype(uint32 uiID, boost::shared_ptr<const T> &result) const
        {
            if (uiID && --uiID < m_Prototypes.size() && m_Prototypes.at(uiID))
            {
                result = m_Prototypes.at(uiID);
                return true;
            }
            return false;
        }

        bool getPrototype(uint32 uiID, boost::shared_ptr<T> &result)
        {
            boost::shared_ptr<const T> temp;
            if (const_cast<const Database<T>&>(*this).getPrototype(uiID, temp))
            {
                result = boost::const_pointer_cast<T>(temp);
                return true;
            }
            return false;
        }

        void getPrototypeShortInfos(UInt32StdStringMap &result) const
        {
            for (uint32 i = 0; i < getDBSize(); ++i)
            {
                if (m_Prototypes.at(i) && m_Prototypes.at(i)->getID() != 0)
                    result.insert(std::make_pair(m_Prototypes.at(i)->getID(), m_Prototypes.at(i)->getName()));
            }
        }

        inline uint32 getDBSize() const { return m_Prototypes.size(); }
        inline void resizeDB(uint32 uiSize, bool fillNew = true)
        {
            uint32 uiOldSize = getDBSize();
            m_Prototypes.resize(uiSize);
            if (fillNew && uiSize > uiOldSize)
            {
                for (uint32 i = uiOldSize; i < uiSize; ++i)                 // first ID == 1, so use i+1 for ID at position i
                    m_Prototypes.at(i) = boost::shared_ptr<T>(new T(i+1));
            }
        }

    private:
        std::vector<boost::shared_ptr<T>> m_Prototypes;
    };

    /*#####
    # TexturePrototype
    #####*/
    class TexturePrototype : public Prototype
    {
    public:
        TexturePrototype(uint32 uiID = 0) : Prototype(uiID) {}
        inline void setFileName(const std::string &sFileName) { m_sFileName = sFileName; }
        inline std::string getFileName() const { return m_sFileName; }
        inline void setPath(const std::string &sPath) { m_sPath = sPath; }
        inline std::string getPath() const { return m_sPath; }
        inline std::string getPathName() const { return getPath() + getFileName(); }

        inline Color getTransparencyColor() const { return m_TransparencyColor; }
        inline void setTransparencyColor(const Color &color) { m_TransparencyColor = color; }

    private:
        Color m_TransparencyColor;
        std::string m_sPath;        // path from ~/textures
        std::string m_sFileName;    // filename
    };
    typedef boost::shared_ptr<TexturePrototype> TexturePrototypePtr;

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
    const uint32 MAX_AUTO_TILE_COLUMNS      = 3;
    const uint32 AUTO_TILE_SET_COUNT        = 10;
    class AutoTilePrototype : public Prototype
    {
    public:
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

            // inner edge center
            INDEX_INNER_EDGE_CENTER,                // 34

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

    private:
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

    public:
        AutoTilePrototype(uint32 uiID = 0) : Prototype(uiID)
        {
            memset(&m_uiTileID, NULL, sizeof(m_uiTileID));
        }

        inline void setTileID(AUTO_TILE_INDEX index, uint32 uiID) { m_uiTileID[index] = uiID; }
        inline uint32 getTileID(AUTO_TILE_INDEX index) const { return m_uiTileID[index]; }

        inline static bool hasTileCheck(const uint32 &uiTileCheck, AUTO_TILE_TILE_REQUIREMENTS tiles)
        {
            if ((uiTileCheck & tiles) == tiles)
                return true;
            return false;
        }

        inline static AUTO_TILE_INDEX getAutoTileIndexForTileCheck(uint32 uiTileCheck)
        {
            // lonely tile
            if (hasTileCheck(uiTileCheck, CIRCLE))
                return INDEX_INNER_EDGE_CENTER;

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
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT))
                return INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT))
                return INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT;
            if (hasTileCheck(uiTileCheck, INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT))
                return INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT;

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

    private:
        uint32 m_uiTileID[AUTO_TILE_SET_COUNT];
    };
    typedef boost::shared_ptr<AutoTilePrototype> AutoTilePrototypePtr;
    typedef boost::shared_ptr<const AutoTilePrototype> ConstAutoTilePrototypePtr;
    typedef std::vector<ConstAutoTilePrototypePtr> ConstAutoTilePrototypePtrVector;

    /*#####
    # SpritePrototype
    #####*/
    class SpritePrototype : public TexturePrototype
    {
    public:
        SpritePrototype(uint32 uiID = 0) : TexturePrototype(uiID) {}

        inline uint8 getColumns() const { return m_Frames.x; }
        inline void setColumns(uint8 columns) { m_Frames.x = columns; }
        inline uint8 getRows() const { return m_Frames.y; }
        inline void setRows(uint8 rows) { m_Frames.y = rows; }

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

    private:
        Point<uint8> m_Frames;
        Rect<uint32> m_BoundingRect;
    };
    typedef boost::shared_ptr<SpritePrototype> SpritePrototypePtr;
    typedef boost::shared_ptr<const SpritePrototype> ConstSpritePrototypePtr;
    typedef std::vector<ConstSpritePrototypePtr> ConstSpritePrototypePtrVector;

    /*#####
    # WorldObjectPrototype
    #####*/
    class WorldObjectPrototype : public Prototype
    {
    public:
        WorldObjectPrototype(uint32 uiID = 0) : Prototype(uiID), m_uiTextureID(0), m_uiFrames(0) {}
        inline void setTextureID(uint32 uiTextureID) { m_uiTextureID = uiTextureID; }
        inline uint32 getTextureID() const { return m_uiTextureID; }

        inline void setFramesPerSecond(uint16 uiFrames) { m_uiFrames = uiFrames; }
        inline uint16 getFramesPerSecond() const { return m_uiFrames; }

    private:
        uint32 m_uiTextureID;
        uint16 m_uiFrames;
    };
    typedef boost::shared_ptr<WorldObjectPrototype> WorldObjectPrototypePtr;
    typedef boost::shared_ptr<const WorldObjectPrototype> ConstWorldObjectPrototypePtr;
    typedef std::vector<ConstWorldObjectPrototypePtr> ConstWorldObjectPrototypePtrVector;

    /*#####
    # DynamicObjectPrototype
    #####*/
    class DynamicObjectPrototype : public WorldObjectPrototype
    {
    public:
        DynamicObjectPrototype(uint32 uiID = 0) : WorldObjectPrototype(uiID), m_uiSpeed(0) {}
        inline void setSpeed(uint16 uiSpeed) { m_uiSpeed = uiSpeed; }
        inline uint16 getSpeed() const { return m_uiSpeed; }

    private:
        uint16 m_uiSpeed;
    };
    typedef boost::shared_ptr<DynamicObjectPrototype> DynamicObjectPrototypePtr;
    typedef boost::shared_ptr<const DynamicObjectPrototype> ConstDynamicObjectPrototypePtr;
    typedef std::vector<ConstDynamicObjectPrototypePtr> ConstDynamicObjectPrototypePtrVector;

    /*#####
    # TextPrototype
    #####*/
    class TextPrototype : public Prototype
    {
    public:
        TextPrototype(uint32 uiID = 0, uint32 uiLocalCount = 1) : Prototype(uiID)
        {
            setLocalCount(uiLocalCount);
        }

        inline void setLocalCount(uint32 uiCount) { m_Locals.resize(uiCount); }
        inline uint32 getLocalCount() const { return m_Locals.size(); }
        inline void setLocal(uint32 uiIndex, const std::string &sLocal) { if (uiIndex < getLocalCount()) m_Locals.at(uiIndex) = sLocal; }
        inline std::string getLocal(uint32 uiIndex)
        {
            if (uiIndex < getLocalCount())
                return m_Locals.at(uiIndex);
            return "";
        }

    private:
        StdStringVector m_Locals;
    };
    typedef boost::shared_ptr<TextPrototype> TextPrototypePtr;
    typedef boost::shared_ptr<const TextPrototype> ConstTextPrototypePtr;
    typedef std::vector<ConstTextPrototypePtr> ConstTextPrototypePtrVector;
}
#endif