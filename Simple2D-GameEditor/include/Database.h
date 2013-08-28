#ifndef DATABASE_H
#define DATABASE_H

#include "DatabasePrototypes.h"
#include "Logfile.h"

typedef std::map<uint32, QString> UInt32StringMap;

namespace DATABASE
{
    /*#####
    # Database
    #####*/
    template <class T, typename TIndex>
    class DatabaseChanger;
    template <class T, typename TIndex = uint32>
    class Database : public Container<T, TIndex>
    {
        friend class DatabaseMgr;
        friend class DatabaseChanger<T, TIndex>;

    public:
        void getItemShortInfos(UInt32StringMap &result) const
        {
            for (uint32 i = 0; i < getSize(); ++i)
            {
                if (m_Items.at(i) && m_Items.at(i)->getID() != 0)
                    result.insert(std::make_pair(m_Items.at(i)->getID(), m_Items.at(i)->getName()));
            }
        }

        void resize(TIndex uiSize, bool fillNew = true)
        {
            TIndex uiOldSize = getSize();
            m_Items.resize(uiSize);
            BASIC_LOG("Resize database to " + QString::number(uiSize));
            if (fillNew && uiSize > uiOldSize)
            {
                BASIC_LOG("Fill database entrys from " + QString::number(uiOldSize) + " to " + QString::number(uiSize) + " with data.");
                for (TIndex i = uiOldSize; i < uiSize; ++i)                 // first ID == 1, so use i+1 for ID at position i
                    m_Items.at(i) = std::shared_ptr<T>(new T(i+1));
            }
            else if (uiSize < uiOldSize)
                BASIC_LOG("Erase database entrys from " + QString::number(uiOldSize) + " to " + QString::number(uiSize) + ".");
        }
    };

    /*#####
    # Special Databases
    #####*/
    class ObjectAnimationTypeDatabase : public Database<ObjectAnimationTypePrototype, ANIMATION_TYPE_INDEX>
    {
    public:
        ObjectAnimationTypeDatabase() : Database()
        {
            clear();
        }

        void clear()
        {
            Database::clear();
            Database::setItem(MAP_STRUCTURE::DIRECTION_UP+1, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(MAP_STRUCTURE::DIRECTION_UP+1, "STAND_UP")));
            Database::setItem(MAP_STRUCTURE::DIRECTION_RIGHT+1, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(MAP_STRUCTURE::DIRECTION_RIGHT+1, "STAND_RIGHT")));
            Database::setItem(MAP_STRUCTURE::DIRECTION_DOWN+1, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(MAP_STRUCTURE::DIRECTION_DOWN+1, "STAND_DOWN")));
            Database::setItem(MAP_STRUCTURE::DIRECTION_LEFT+1, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(MAP_STRUCTURE::DIRECTION_LEFT+1, "STAND_LEFT")));
            Database::setItem(5, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(5, "WALK_UP")));
            Database::setItem(6, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(6, "WALK_RIGHT")));
            Database::setItem(7, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(7, "WALK_DOWN")));
            Database::setItem(8, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(8, "WALK_LEFT")));
        }

        void setItem(uint32 uiID, ObjectAnimationTypePrototypePtr &prototype)
        {
            if (uiID > MAP_OBJECT::MIN_DYNAMIC_OBJECT_POSE)
                Database::setItem(uiID, prototype);
        }
    };
    // object animation type database typedefs
    typedef std::shared_ptr<ObjectAnimationTypeDatabase> ObjectAnimationTypeDatabasePtr;
    typedef std::shared_ptr<const ObjectAnimationTypeDatabase> ConstObjectAnimationTypeDatabasePtr;

    /*#####
    # DatabaseMgr
    #####*/
    // tile database typedefs
    typedef Database<TilePrototype, TILE_INDEX> TileDatabase;
    typedef std::shared_ptr<TileDatabase> TileDatabasePtr;
    typedef std::shared_ptr<const TileDatabase> ConstTileDatabasePtr;
    // tileset database typedefs
    typedef Database<TILE_SET::TileSetPrototype, TILE_SET_INDEX> TileSetDatabase;
    typedef std::shared_ptr<TileSetDatabase> TileSetDatabasePtr;
    typedef std::shared_ptr<const TileSetDatabase> ConstTileSetDatabasePtr;
    // autotile database typedefs
    typedef Database<AUTO_TILE::AutoTilePrototype, AUTO_TILE_INDEX> AutoTileDatabase;
    typedef std::shared_ptr<AutoTileDatabase> AutoTileDatabasePtr;
    typedef std::shared_ptr<const AutoTileDatabase> ConstAutoTileDatabasePtr;
    // sprite database typedefs
    typedef Database<SpritePrototype, SPRITE_INDEX> SpriteDatabase;
    typedef std::shared_ptr<SpriteDatabase> SpriteDatabasePtr;
    typedef std::shared_ptr<const SpriteDatabase> ConstSpriteDatabasePtr;
    // animation database typedefs
    typedef Database<AnimationPrototype, ANIMATION_INDEX> AnimationDatabase;
    typedef std::shared_ptr<AnimationDatabase> AnimationDatabasePtr;
    typedef std::shared_ptr<const AnimationDatabase> ConstAnimationDatabasePtr;
    // world object database typedefs
    typedef Database<MAP_OBJECT::WorldObjectPrototype, WORLD_OBJECT_INDEX> WorldObjectDatabase;
    typedef std::shared_ptr<WorldObjectDatabase> WorldObjectDatabasePtr;
    typedef std::shared_ptr<const WorldObjectDatabase> ConstWorldObjectDatabasePtr;
    // dynamic object database typedefs
    typedef Database<MAP_OBJECT::DynamicObjectPrototype, WORLD_OBJECT_INDEX> DynamicObjectDatabase;
    typedef std::shared_ptr<DynamicObjectDatabase> DynamicObjectDatabasePtr;
    typedef std::shared_ptr<const DynamicObjectDatabase> ConstDynamicObjectDatabasePtr;
    // text database typedefs
    typedef Database<LocalisationPrototype, LOCALISATION_INDEX> LocalsDatabase;
    typedef std::shared_ptr<LocalsDatabase> LocalsDatabasePtr;
    typedef std::shared_ptr<const LocalsDatabase> ConstLocalsDatabasePtr;
    // map database typedefs
    typedef std::shared_ptr<MAP_STRUCTURE::MapDatabase> MapDatabasePtr;
    typedef std::shared_ptr<const MAP_STRUCTURE::MapDatabase> ConstMapDatabasePtr;
}
#endif