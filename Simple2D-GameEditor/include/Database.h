#ifndef DATABASE_H
#define DATABASE_H

#include "DatabasePrototypes.h"

typedef std::map<uint32, QString> UInt32StringMap;
typedef std::vector<std::pair<uint32, QString>> UInt32StringPairVector;

namespace DATABASE
{
    /*#####
    # Database
    #####*/
    class IDatabase
    {
    public:
        virtual void clear() = 0;

        virtual void setPrototype(PROTOTYPE::Prototype* pPrototype) = 0;
        virtual PROTOTYPE::Prototype* getNewPrototype(uint32 ID = 0) const = 0;
        virtual PROTOTYPE::Prototype* getPrototype(uint32 ID) = 0;
        virtual const PROTOTYPE::Prototype* getPrototype(uint32 ID) const = 0;

        virtual uint32 getSize() const = 0;
        virtual void resize(uint32 newSize) = 0;
        virtual uint32 getMaximumSize() const = 0;

        virtual void setupStandardEntrys() = 0;
    };
    typedef std::unique_ptr<IDatabase> IDatabasePtr;

    template <class T>
    class Database : public IDatabase, private Container<T>
    {
    public:
        Database() : Container(), IDatabase() {}
        // overwrite copy constructor
        Database(const Database<T>& pOther)
        {
            m_MaxSize = pOther.getMaximumSize();
            uint32 i = 0;
            m_Items.resize(pOther.getSize());
            for (auto& pItem : pOther.getItems())
            {
                if (pItem)
                    m_Items.at(i) = std::unique_ptr<T>(new T(*pItem));
                ++i;
            }
        }

        virtual void setupStandardEntrys() {}
        inline void clear() { Container::clear(); }

        void resize(uint32 uiSize)
        {
            uint32 uiOldSize = getSize();
            Container::resize(uiSize);
            // ToDo:
            //BASIC_LOG("Resize database to " + QString::number(uiSize));
            if (uiSize > uiOldSize)
            {
                //BASIC_LOG("Fill database entries from " + QString::number(uiOldSize) + " to " + QString::number(uiSize) + " with data.");
                for (uint32 i = uiOldSize; i < uiSize; ++i)                 // first ID == 1, so use i+1 for ID at position i
                    m_Items.at(i) = std::unique_ptr<T>(new T(i+1));
            }
            //else if (uiSize < uiOldSize)
            //    BASIC_LOG("Erase database entries from " + QString::number(uiOldSize) + " to " + QString::number(uiSize) + ".");
        }

        inline uint32 getSize() const { return Container::getSize(); }

        inline void setMaximumSize(uint32 size) { Container::setMaximumSize(size); }
        inline uint32 getMaximumSize() const { return Container::getMaximumSize(); }

        inline PROTOTYPE::Prototype* getNewPrototype(uint32 ID = 0) const { return new T(ID); }

        const std::vector<std::unique_ptr<T>>& getPrototypes() const { return getItems(); }
        inline PROTOTYPE::Prototype* getPrototype(uint32 ID) { return Container::getItem(ID); }
        inline const PROTOTYPE::Prototype* getPrototype(uint32 ID) const  { return Container::getItem(ID); }
        inline T* getOriginalPrototype(uint32 ID) { return Container::getItem(ID); }
        inline const T* getOriginalPrototype(uint32 ID) const  { return Container::getItem(ID); }
        inline void setPrototype(PROTOTYPE::Prototype* pPrototype)
        {
            if (auto pCast = dynamic_cast<T*>(pPrototype))
                Container::setItem(pCast->getID(), pCast);
            else
                throw std::bad_typeid("Invalid prototype type.");
        }
        virtual void setPrototype(T* pPrototype)
        {
            if (pPrototype)
                Container::setItem(pPrototype->getID(), pPrototype);
        }
    };

    /*#####
    # Special Databases
    #####*/
    class TileDatabase : public Database<PROTOTYPE::TilePrototype>
    {
    public:
        TileDatabase();
        TileDatabase(const TileDatabase& other);

        //void setPrototype(TilePrototype* pItem);
        //const TilePrototype* getPrototype(uint32 ID) const;
        //TilePrototype* getPrototype(uint32 ID);

        //TilePrototype* getNewPrototype(uint32 uiID = 0) const;
    };

    class TileSetDatabase : public Database<PROTOTYPE::TILE_SET::TileSetPrototype>
    {
    public:
        //void setPrototype(TILE_SET::TileSetPrototype* pItem);
        //const TILE_SET::TileSetPrototype* getPrototype(uint32 ID) const;
        //TILE_SET::TileSetPrototype* getPrototype(uint32 ID);

        //TILE_SET::TileSetPrototype* getNewPrototype(uint32 uiID = 0) const;
    };

    class AutoTileDatabase : public Database<PROTOTYPE::AUTO_TILE::AutoTilePrototype>
    {
    public:
        AutoTileDatabase();
        AutoTileDatabase(const AutoTileDatabase& other);

        //void setPrototype(AUTO_TILE::AutoTilePrototype* pItem);
        //const AUTO_TILE::AutoTilePrototype* getPrototype(uint32 ID) const;
        //AUTO_TILE::AutoTilePrototype* getPrototype(uint32 ID);

        //AUTO_TILE::AutoTilePrototype* getNewPrototype(uint32 uiID = 0) const;
    };

    class SpriteDatabase : public Database<PROTOTYPE::SpritePrototype>
    {
    public:
        //void setPrototype(SpritePrototype* pItem);
        //const SpritePrototype* getPrototype(uint32 ID) const;
        //SpritePrototype* getPrototype(uint32 ID);

        //SpritePrototype* getNewPrototype(uint32 uiID = 0) const;
    };

    class AnimationDatabase : public Database<PROTOTYPE::ANIMATION::AnimationPrototype>
    {
    public:
        //void setPrototype(AnimationPrototype* pItem);
        //const AnimationPrototype* getPrototype(uint32 ID) const;
        //AnimationPrototype* getPrototype(uint32 ID);

        //AnimationPrototype* getNewPrototype(uint32 uiID = 0) const;
    };

    class AnimationTypeDatabase : public Database<PROTOTYPE::ANIMATION::AnimationTypePrototype>
    {
    public:
        AnimationTypeDatabase();

        AnimationTypeDatabase(const AnimationTypeDatabase& other);

        void clear();
        void setupStandardEntrys();

        //void setPrototype(AnimationTypePrototype* pItem);
        //const AnimationTypePrototype* getPrototype(uint32 ID) const;
        //AnimationTypePrototype* getPrototype(uint32 ID);

        //AnimationTypePrototype* getNewPrototype(uint32 uiID = 0) const;
    };

    class WorldObjectDatabase : public Database<PROTOTYPE::WORLD_OBJECT::WorldObjectPrototype>
    {
    public:
        //virtual void setPrototype(MAP_OBJECT::WorldObjectPrototype* pItem);
        //virtual const MAP_OBJECT::WorldObjectPrototype* getPrototype(uint32 ID) const;
        //virtual MAP_OBJECT::WorldObjectPrototype* getPrototype(uint32 ID);

        //virtual MAP_OBJECT::WorldObjectPrototype* getNewPrototype(uint32 uiID = 0) const;
    };

    class LocalisationDatabase : public Database<PROTOTYPE::LOCALISATION::LocalisationPrototype>
    {
    public:
        //void setPrototype(LocalisationPrototype* pItem);
        //const LocalisationPrototype* getPrototype(uint32 ID) const;
        //LocalisationPrototype* getPrototype(uint32 ID);

        //LocalisationPrototype* getNewPrototype(uint32 uiID = 0) const;
    };
}
#endif
