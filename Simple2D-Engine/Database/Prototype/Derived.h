#pragma once

#include "Base.h"
#include "Color.h"
#include "Simple2D-Global.h"
#include <FlagManager.h>
#include <array>
#include <Vector2D.h>
#include <geometry/Rectangle.h>
#include <Animation/FrameData.h>

namespace database
{
    namespace prototype
    {
        /*#####
        # Resource
        #####*/
        class Resource : public Base
        {
        public:
            Resource() = default;
            Resource(uint32 ID);

            virtual bool isEmpty() const;

            void setFileName(const QString& fileName);
            QString getFileName() const;
            void setPath(const QString& path);
            QString getPath() const;
            QString getPathName() const;

            virtual QString getBaseDirectory() const = 0;

        private:
            QString m_Path;
            QString m_FileName;    // filename
        };

        /*#####
        # Texture
        #####*/
        class Texture : public Resource
        {
        public:
            Texture() = default;
            Texture(uint32 ID);

            Color getTransparencyColor() const;
            void setTransparencyColor(const Color& color);

        private:
            Color m_TransparencyColor;
        };

        /*#####
        # Tile
        #####*/
        class Tile : public Texture
        {
        public:
            Tile() = default;
            Tile(uint32 ID);

            // passability flag, if nothing set, its not accessible
            enum class PassabilityFlag
            {
                PASSABLE_NONE = 0x00,
                PASSABLE_UP = 0x01,
                PASSABLE_DOWN = 0x02,
                PASSABLE_RIGHT = 0x04,
                PASSABLE_LEFT = 0x08,
                PASSABLE_UP_TO_DOWN = PASSABLE_UP | PASSABLE_DOWN,
                PASSABLE_LEFT_TO_RIGHT = PASSABLE_LEFT | PASSABLE_RIGHT,
                PASSABLE_ALL = PASSABLE_LEFT | PASSABLE_RIGHT | PASSABLE_DOWN | PASSABLE_UP,
            };

            typedef FlagManager<PassabilityFlag, uint8> PassabilityFlagMgr;
            PassabilityFlagMgr getPassability() const;
            void setPassability(const PassabilityFlagMgr& flags);

            QString getBaseDirectory() const override;

        private:
            PassabilityFlagMgr m_Passability;
        };

        /*#####
        # TileSet
        #####*/
        class TileSet;
        bool compare(const TileSet&, const TileSet&);

        class TileSet : public Base
        {
            friend bool prototype::compare(const TileSet&, const TileSet&);
        public:
            TileSet() = default;
            TileSet(uint32 ID);

            void resize(const GEOMETRY::Size<uint32>& size);
            void resize(uint32 width, uint32 height);
            GEOMETRY::Size<uint32> getSize() const;
            void clear();

            void setTileID(const GEOMETRY::Point<uint32>& pos, uint32 ID);
            uint32 getTileID(const GEOMETRY::Point<uint32>& pos) const;

        private:
            sl::Vector2D<uint32> m_Tiles;
        };

        /*#####
        # AutoTile
        #####*/
        class AutoTile;
        bool compare(const AutoTile&, const AutoTile&);

        typedef std::array<DATABASE::PROTOTYPE::AUTO_TILE_INDEX, DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_SET_COUNT> TileArray;
        class AutoTile : public Base
        {
            friend bool compare(const AutoTile&, const AutoTile&);
        public:
            AutoTile() = default;
            AutoTile(uint32 ID);

            enum class Index
            {
                // auto tile set
                TopLeft,
                Top,
                TopRight,
                Left,
                Center,
                Right,
                BottomLeft,
                Bottom,
                BottomRight,
                InnerCenter,                            // 9

                // side ends
                SideEndTop,                             // 10
                SideEndBottom,
                SideEndLeft,
                SideEndRight,                           // 13

                // double sides
                SideVertical,
                SideHorizontal,                         // 15

                // T tiles
                T_Top,
                T_Bottom,
                T_Left,
                T_Right,                                // 19

                // curves
                CurveTopLeft,
                CurveTopRight,
                CurveBottomLeft,
                CurveBottomRight,                       // 23

                // inner edges
                InnerEdgeTopLeft,
                InnerEdgeTopRight,
                InnerEdgeBottomLeft,
                InnerEdgeBottomRight,                   // 27

                // diagonal inner edges
                InnerEdgeTopLeftBottomRight,
                InnerEdgeTopRightBottomLeft,            // 29

                // multi inner edges
                InnerEdgeTopLeftTopRight,
                InnerEdgeTopLeftBottomLeft,
                InnerEdgeTopRightBottomRight,
                InnerEdgeBottomLeftBottomRight,         // 33

                // circle
                Circle,                                 // 34

                // Y tiles
                Y_TopBottomLeft,
                Y_TopBottomRight,
                Y_LeftTopRight,
                Y_LeftBottomRight,                      // 38
                Y_BottomTopLeft,
                Y_BottomTopRight,
                Y_RightTopLeft,
                Y_RightBottomLeft,                      // 42

                InnerEdgeTopLeftTopRightBottomLeft,
                InnerEdgeTopLeftTopRightBottomRight,
                InnerEdgeTopLeftBottomLeftBottomRight,
                InnerEdgeTopRightBottomLeftBottomRight, // 46

                none
            };

            enum NeighborCheck
            {
                SAME_AROUND = 0x00,
                OTHER_TOP_LEFT = 0x01,
                OTHER_TOP = 0x02,
                OTHER_TOP_RIGHT = 0x04,
                OTHER_LEFT = 0x08,
                OTHER_RIGHT = 0x10,
                OTHER_BOTTOM_LEFT = 0x20,
                OTHER_BOTTOM = 0x40,
                OTHER_BOTTOM_RIGHT = 0x80,
            };

            void setTileID(Index index, DATABASE::PROTOTYPE::AUTO_TILE_INDEX ID);
            DATABASE::PROTOTYPE::AUTO_TILE_INDEX getTileID(Index index) const;

            void setTiles(const TileArray& tiles);
            TileArray getTiles() const;

            static Index getAutoTileIndexForTileCheck(uint32 tileCheck);

        private:
            TileArray m_Tiles;
        };

        /*#####
        # Sprite
        #####*/
        class Sprite : public Texture
        {
        public:
            Sprite() = default;
            Sprite(uint32 ID);

            QString getBaseDirectory() const;
        };

        /*#####
        # AnimationAdditionType
        #####*/
        class AnimationAdditionType : public Base 
        {
        public:
            AnimationAdditionType() = default;
            AnimationAdditionType(uint32 ID);
        };

        /*#####
        # AnimationAddition
        #####*/
        class AnimationAddition : public Base
        {
            friend bool compare(const AnimationAddition&, const AnimationAddition&);
        private:
            std::vector<animation::SpriteData> m_Sprites;

        public:
            AnimationAddition() = default;
            AnimationAddition(uint32 ID);

            uint32 getAdditionType() const;
            void setAdditionType(uint32 type);

            uint32 getSize() const;
            void addSprite(const animation::SpriteData& sprite);
            animation::SpriteData getSprite(uint32 index) const;

        private:
            uint32 m_AdditionTypeID = 1;
        };

        /*#####
        # AnimationAdditionPoint
        #####*/
        class AnimationAdditionPoint : public AnimationAddition
        {
        public:
            AnimationAdditionPoint() = default;
            AnimationAdditionPoint(uint32 ID);

            GEOMETRY::Point<int32> getPosition() const;
            void setPosition(const GEOMETRY::Point<int32>& pos);
            int32 getX() const;
            void setX(int32 x);
            int32 getY() const;
            void setY(int32 y);

        private:
            GEOMETRY::Point<int32> m_Pos;
        };

        /*#####
        # AnimationAdditionRect
        #####*/
        class AnimationAdditionRect : public AnimationAddition
        {
        public:
            AnimationAdditionRect() = default;
            AnimationAdditionRect(uint32 ID);

            GEOMETRY::Rectangle<int32> getRectangle() const;
            void setRectangle(const GEOMETRY::Rectangle<int32>& rect);

        private:
            GEOMETRY::Rectangle<int32> m_Rect;
        };

        /*#####
        # Animation
        #####*/
        class Animation : public Base
        {
        public:
            Animation() = default;
            Animation(uint32 ID);

            const animation::FrameData& getFrame(uint32 index) const;
            void setFrame(uint32 index, const animation::FrameData& frame);
            void removeFrame(uint32 index);
            void setFrameCount(uint32 count);
            uint32 getFrameCount() const;

            void setFrames(const animation::FrameVector& frames);
            const animation::FrameVector& getFrames() const;

        private:
            animation::FrameVector m_Frames;
        };

        /*#####
        # WorldObject
        #####*/
        class WorldObject : public Base
        {
        public:
            enum class VisualType
            {
                Sprite,
                Animation
            };

            class AnimationInfo
            {
            public:
                AnimationInfo(uint32 ID = 0, VisualType visualType = VisualType::Sprite, uint32 animationTypeID = 0);

                uint32 getAnimationID() const;
                void setAnimationID(uint32 ID);

                uint32 getAnimationTypeID() const;
                void setAnimationTypeID(uint32 ID);

                VisualType getVisualType() const;
                void setVisualType(VisualType type);

                bool isEmpty() const;

            private:
                uint32 m_ID;
                VisualType m_VisualType;
                uint32 m_AnimationTypeID;
            };
            typedef std::vector<AnimationInfo> AnimationInfoVector;

            WorldObject() = default;
            WorldObject(uint32 ID);

            GEOMETRY::Rectangle<int32> getBoundingRect() const;
            void setBoundingRect(const GEOMETRY::Rectangle<int32>& rect);

            void setAnimationSpeed(uint16 speed);
            uint16 getAnimationSpeed() const;

            void setScriptName(const QString& scriptName);
            QString getScriptName() const;

            // animations
            AnimationInfoVector& getAnimations();
            const AnimationInfoVector& getAnimations() const;

        private:
            GEOMETRY::Rectangle<int32> m_BoundingRect;
            QString m_ScriptName;

            uint16 m_AnimationSpeed = 100;
            AnimationInfoVector m_AnimationInfos;
        };

        /*#####
        # MapPrototype
        #####*/
            //const char MAP_FILE_ENDING[] = ".map";
            //const char MAP_FOLDER[] = "/Maps/";

            //enum MapObjectLayer
            //{
            //    LAYER_LOWER,
            //    LAYER_MIDDLE,
            //    LAYER_UPPER
            //};

            //// map objects
            //struct MapObject
            //{
            //    MapObject() : m_ObjectID(0), m_GUID(0), m_Direction(MAP::MAP_DATA::MapDirection::down), m_Layer(LAYER_MIDDLE) {}

            //    bool isEmpty() const { return !m_GUID && !m_ObjectID; }

            //    uint32 m_ObjectID;
            //    uint32 m_GUID;
            //    GEOMETRY::Point<int32> m_Position;
            //    MAP::MAP_DATA::MapDirection m_Direction;
            //    MapObjectLayer m_Layer;
            //};
            //typedef Container<MapObject> MapObjectContainer;
            //typedef boost::multi_array<MapObjectContainer, 3> MapObjectPtrVectorMultiarray3D;

        class Map : public Resource
        {
        private:
            void _clearTiles();

        public:
            Map() = default;
            Map(uint32 ID);

            QString getBaseDirectory() const;

            void setScriptName(const QString& sScriptName);
            QString getScriptName() const;

            void resize(uint32 width, uint32 height);
            void resize(const GEOMETRY::Size<uint32>& size);
            GEOMETRY::Size<uint32> getSize() const;

            void setLayerCount(uint8 number, MAP::LayerType layer);
            uint8 getLayerCount(MAP::LayerType layer) const;

            uint32 getParentID() const;
            void setParentID(uint32 parentID);

        private:
            uint32 m_ParentID = 0;
            QString m_ScriptName;

            GEOMETRY::Size<uint32> m_Size;
            std::array<uint8, 2> m_Layer;
        };

        /*#####
        # Localization
        #####*/
        //class Localization : public Base
        //{
        //public:
        //    LocalisationPrototype(uint32 ID = 0, uint32 uiLocalCount = 1) : Prototype(ID)
        //    {
        //        setLocalsCount(uiLocalCount);
        //    }

        //    inline void setLocalsCount(uint32 uiCount) { m_Locals.resize(uiCount); }
        //    inline uint32 getLocalsCount() const { return static_cast<uint32>(m_Locals.size()); }
        //    inline void setLocalisation(uint32 uiIndex, const QString& sLocal)
        //    {
        //        if (uiIndex >= getLocalsCount())
        //            setLocalsCount(uiIndex + 1);
        //        m_Locals.at(uiIndex) = sLocal;
        //    }

        //    inline QString getLocalisation(uint32 uiIndex) const
        //    {
        //        if (uiIndex < getLocalsCount())
        //            return m_Locals.at(uiIndex);
        //        return "";
        //    }

        //    // IO
        //    void toXML(QXmlStreamWriter& writer) const;
        //    void insertChildren(const QXmlStreamReader& reader);

        //private:
        //    StringVector m_Locals;
        //};
    }
}

