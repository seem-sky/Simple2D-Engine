#ifndef DATABASE_PROTOTYPES_H
#define DATABASE_PROTOTYPES_H

#include "Color.h"
#include <boost/multi_array.hpp>
#include <QtCore/QString>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>
#include <array>
#include "Container.h"
#include <FlagManager.h>
#include <geometry/Rectangle.h>
#include <geometry/Point.h>
#include "PrototypeAnimationModule.h"
#include "Simple2D-Global.h"
#include "WorldObjectInfo.h"

namespace CACHE
{
    class Tiles;
}

namespace DATABASE
{
    namespace PROTOTYPE
    {
        /*#####
        # Prototype superclass
        #####*/
        class Prototype
        {
        public:
			Prototype(uint32 ID = 0) : m_ID(ID) {}

            inline void setName(const QString& sName) { m_Name = sName; }
            inline QString getName() const { return m_Name; }
            inline uint32 getID() const { return m_ID; }
            inline void setID(uint32 ID) { m_ID = ID; }

            // IO
            virtual void toXML(QXmlStreamWriter& writer) const;
            virtual void fromXML(const QXmlStreamAttributes& attributes);
            virtual void insertChildren(const QXmlStreamReader& reader) {}

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

            inline void setFileName(const QString& sFileName) { m_FileName = sFileName; }
            inline QString getFileName() const { return m_FileName; }
            inline void setPath(const QString& sPath) { m_Path = sPath; }
            inline QString getPath() const { return m_Path; }
            inline QString getPathName() const { return getPath() + getFileName(); }

            // IO
            virtual void toXML(QXmlStreamWriter& writer) const;
            virtual void fromXML(const QXmlStreamAttributes& attributes);

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
            inline void setTransparencyColor(const Color& color) { m_TransparencyColor = color; }

            // IO
            virtual void toXML(QXmlStreamWriter& writer) const;
            virtual void fromXML(const QXmlStreamAttributes& attributes);

            inline QString getTextureString() const { return getPathName() + QString::fromStdString(getTransparencyColor().getColorString()); }

        private:
            Color m_TransparencyColor;
        };

        /*#####
        # TilePrototype
        #####*/
        class TilePrototype : public TexturePrototype
        {
        public:
            TilePrototype(uint32 ID = 0) : TexturePrototype(ID) {}

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
            inline bool hasPassabilityFlag(const PassabilityFlag flag) const { return (m_uiPassable&  flag) != 0; }
            inline void addPassabilityFlag(const uint8 flag) { m_uiPassable |= flag; }
            inline void removePassabilityFlag(const uint8 flag) { m_uiPassable ^= flag; }
            inline uint8 getPassability() const  { return m_uiPassable; }

            inline void setTerrainType(uint32 uiTerrainType) { m_uiTerrainType = uiTerrainType; }
            inline uint32 getTerrainType() const { return m_uiTerrainType; }

            // IO
            void toXML(QXmlStreamWriter& writer) const;
            void fromXML(const QXmlStreamAttributes& attributes);

        private:
            uint8 m_uiPassable = 0;
            uint32 m_uiTerrainType = 0;
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
                void _resizeIfNeeded(GEOMETRY::Point<uint32> size);

            public:
                TileSetPrototype(uint32 ID = 0);

                void resizeTileSet(GEOMETRY::Point<uint32> size);
                inline GEOMETRY::Point<uint32> getTileSetSize() const { return m_Size; }
                void clear();

                void setTileID(GEOMETRY::Point<uint32> pos, uint32 ID);
                uint32 getTileID(GEOMETRY::Point<uint32> pos) const;

                // IO
                void toXML(QXmlStreamWriter& writer) const;
                void fromXML(const QXmlStreamAttributes& attributes);

            private:
                GEOMETRY::Point<uint32> m_Size;
                UInt32Multiarray2D m_Tiles;
            };
        
            QPixmap createPixmap(const TileSetPrototype& tileSet, CACHE::Tiles& tileCache);
        }

        /*#####
        # AutoTilePrototype
        #####*/
        namespace AUTO_TILE
        {
            typedef std::array<TILE_INDEX, AUTO_TILE_SET_COUNT> TileArray;
            class AutoTilePrototype : public Prototype
            {
            public:
                AutoTilePrototype(uint32 ID = 0) : Prototype(ID) {}

                inline void setTileID(AUTO_TILE_INDEX index, uint32 ID) { m_uiTileID.at(index) = ID; }
                inline uint32 getTileID(AUTO_TILE_INDEX index) const { return m_uiTileID.at(index); }

                inline void setTiles(const TileArray& tiles) { m_uiTileID = tiles; }
                inline const TileArray& getTiles() const { return m_uiTileID; }

                // IO
                virtual void toXML(QXmlStreamWriter& writer) const;
                virtual void fromXML(const QXmlStreamAttributes& attributes);

            private:
                TileArray m_uiTileID;
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
            const uint32 STANDARD_ANIMATION_TYPES = 8;
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
                GEOMETRY::Point<int32> m_Pos;
                uint32 m_uiSpriteID = 0;
                uint16 m_uiRotation = 0;
                float m_Scale = 100;
                float m_Opacity = 100;

                inline bool isEmpty() const { return !m_uiSpriteID; }
            };
            typedef std::vector<Sprite> SpriteVector;

            class Frame
            {
            public:
                Sprite getSprite(uint32 index) const;
                void setSprite(uint32 index, Sprite sprite);
                void addSprite(Sprite sprite);
                void removeSprite(uint32 index, Sprite sprite);

                inline uint32 getSpriteCount() const { return static_cast<uint32>(m_Sprites.size()); }
                inline void setSpriteCount(uint32 size) { m_Sprites.resize(size); calculateOffset(); }
                inline const SpriteVector& getSprites() const { return m_Sprites; }

                inline const GEOMETRY::Point<int32>& getOffset() const { return m_FrameOffset; }
                void calculateOffset();
                void setOffsetIfNeeded(const GEOMETRY::Point<int32>& offset);

                inline uint32 getTimeInMsec() const { return m_uiMsecTime; }
                inline void setTimeInMsec(uint32 time) { m_uiMsecTime = time; }

                inline bool isEmpty() const { return m_Sprites.empty() && !m_uiMsecTime; }

            private:
                GEOMETRY::Point<int32> m_FrameOffset;
                SpriteVector m_Sprites;
                uint32 m_uiMsecTime = 0;
            };
            typedef std::vector<Frame> FrameVector;

            class AnimationPrototype : public Prototype
            {
            public:
                AnimationPrototype(uint32 ID = 0) : Prototype(ID) {}

                bool getFrame(uint32 uiIndex, Frame& frame) const;
                void setFrame(uint32 uiIndex, Frame frame);
                void removeFrame(uint32 uiIndex);
                inline uint32 getFrameCount() const { return static_cast<uint32>(m_Frames.size()); }
                inline const FrameVector& getAnimation() const { return m_Frames; }
                inline void setAnimation(const FrameVector& animation) { m_Frames = animation; }

                // IO
                void toXML(QXmlStreamWriter& writer) const;
                void insertChildren(const QXmlStreamReader& reader);

            private:
                FrameVector m_Frames;
            };
        }

        /*#####
        # WorldObjectPrototype
        #####*/
        namespace WORLD_OBJECT
        {
            // animation stuff
            const uint8 MIN_WORLD_OBJECT_POSE = 4;

			enum class Flags
			{
				FLAG_NONE = 0x00,
				FLAG_ANIMATION = 0x01,
			};

            class WorldObjectPrototype : public Prototype
            {
            public:
				WorldObjectPrototype(uint32 ID = 0) : Prototype(ID) {}

				inline const FlagManager<Flags>& getFlagManager() const { return m_Flags; }
				inline FlagManager<Flags>& getFlagManager() { return m_Flags; }

                inline GEOMETRY::Rectangle<int32> getBoundingRect() { return m_BoundingRect; }
                inline const GEOMETRY::Rectangle<int32>& getBoundingRect() const { return m_BoundingRect; }
                inline void setBoundingRect(GEOMETRY::Rectangle<int32> rect) { m_BoundingRect = rect; }

                inline void setAnimationSpeed(uint16 uiSpeed) { m_uiAnimationSpeed = uiSpeed; }
                inline uint16 getAnimationSpeed() const { return m_uiAnimationSpeed; }

                inline void setScriptName(const QString& sScriptName) { m_ScriptName = sScriptName; }
                inline QString getScriptName() const { return m_ScriptName; }

				inline uint32 getAnimationCount() const { return m_AnimationModule.getAnimationCount(); }
				uint8 getMinimumAnimationCount() const { return MIN_WORLD_OBJECT_POSE; }

				inline const MODULE::ANIMATION::AnimationModule& getAnimationModule() const { return m_AnimationModule; }
				inline MODULE::ANIMATION::AnimationModule& getAnimationModule() { return m_AnimationModule; }

                // IO
                virtual void toXML(QXmlStreamWriter& writer) const;
                virtual void fromXML(const QXmlStreamAttributes& attributes);
                virtual void insertChildren(const QXmlStreamReader& reader);

            private:
                GEOMETRY::Rectangle<int32> m_BoundingRect;
				uint16 m_uiAnimationSpeed = 100;
                QString m_ScriptName;

				FlagManager<Flags> m_Flags;
				MODULE::ANIMATION::AnimationModule m_AnimationModule;
            };
        }

        /*#####
        # LocalisationPrototype
        #####*/
        typedef std::vector<QString> StringVector;
        namespace LOCALISATION
        {
            const uint32 LOCALISATION_COUNT = 2;
            class LocalisationPrototype : public Prototype
            {
            public:
                LocalisationPrototype(uint32 ID = 0, uint32 uiLocalCount = 1) : Prototype(ID)
                {
                    setLocalsCount(uiLocalCount);
                }

                inline void setLocalsCount(uint32 uiCount) { m_Locals.resize(uiCount); }
                inline uint32 getLocalsCount() const { return static_cast<uint32>(m_Locals.size()); }
                inline void setLocalisation(uint32 uiIndex, const QString& sLocal)
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
                void toXML(QXmlStreamWriter& writer) const;
                void insertChildren(const QXmlStreamReader& reader);

            private:
                StringVector m_Locals;
            };
        }

        /*#####
        # MapPrototype
        #####*/
        class MapDatabaseXMLReader;
        namespace MAP_STRUCTURE
        {
            const char MAP_FILE_ENDING[] = ".map";
            const char MAP_FOLDER[] = "/Maps/";

            enum MapObjectLayer
            {
                LAYER_LOWER,
                LAYER_MIDDLE,
                LAYER_UPPER
            };

            // map objects
            struct MapObject
            {
                MapObject() : m_ObjectID(0), m_GUID(0), m_Direction(MAP::MAP_DATA::MapDirection::DOWN), m_Layer(LAYER_MIDDLE) {}

                bool isEmpty() const { return !m_GUID && !m_ObjectID; }

                uint32 m_ObjectID;
                uint32 m_GUID;
                GEOMETRY::Point<int32> m_Position;
                MAP::MAP_DATA::MapDirection m_Direction;
                MapObjectLayer m_Layer;
            };
            typedef Container<MapObject> MapObjectContainer;
            typedef boost::multi_array<MapObjectContainer, 3> MapObjectPtrVectorMultiarray3D;

            class MapPrototype : public Prototype
            {
            private:
                void _clearTiles();

            public:
                MapPrototype::MapPrototype(uint32 ID = 0, const QString& fileName = "");

                bool isValid();

                QString getFilePath() const;
                inline QString getFileName() const { return m_FileName; }
                inline void setFileName(const QString& sFileName) { m_FileName = sFileName; }
                inline void setScriptName(const QString& sScriptName) { m_ScriptName = sScriptName; }
                inline QString getScriptName() const { return m_ScriptName; }

                void setSizeX(uint32 x);
                void setSizeY(uint32 y);
                void setLayerSize(uint8 size, MAP::LayerType layer);
                void setSize(const GEOMETRY::Point<uint32>& size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize);
                inline GEOMETRY::Point<uint32> getSize() const { return m_Size; }
                inline uint8 getLayerSize(MAP::LayerType layer) const { return m_Layer.at(static_cast<uint32>(layer)); };

                inline uint32 getParentID() const { return m_uiParentID; }
                inline void setParentID(uint32 uiParentID) { m_uiParentID = uiParentID; }

                // XML IO
                void toXML(QXmlStreamWriter& writer) const;
                void fromXML(const QXmlStreamAttributes& attributes);

            private:
                uint32 m_uiParentID = 0;
                QString m_FileName;
                QString m_ScriptName;

                GEOMETRY::Point<uint32> m_Size;
                std::array<uint8, 2> m_Layer;
            };
        }
    }
}

#endif
