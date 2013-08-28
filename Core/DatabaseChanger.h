#ifndef DATABASE_CHANGER_H
#define DATABASE_CHANGER_H

#include "DatabaseMgr.h"

namespace DATABASE
{
    template <class T, typename TIndex>
    class DatabaseChanger
    {
    private:
        inline void _setNewDBSize()
        {
            m_pNewDB->clear();
            if (m_pTargetDB)
                m_pNewDB->resize(m_pTargetDB->getSize(), false);
        }

    public:
        DatabaseChanger(std::shared_ptr<Database<T, TIndex>> pDB) : m_pNewDB(new Database<T, TIndex>())
        {
            setDB(pDB);
        }
        DatabaseChanger() : m_pNewDB(new Database<T, TIndex>()) {}
        virtual void setDB(std::shared_ptr<Database<T, TIndex>> pDB)
        {
            m_pTargetDB = pDB;
            _setNewDBSize();
        }

        inline bool getItem(uint32 uiID, std::shared_ptr<const T> &result) const
        {
            if (!m_pTargetDB)
                return false;
            if (m_pNewDB->getItem(uiID, result))
                return true;
            if (m_pTargetDB->getItem(uiID, result))
            {
                result = std::shared_ptr<const T>(new T(*result));
                return true;
            }
            return false;
        }

        inline bool getItem(uint32 uiID, std::shared_ptr<T> &result)
        {
            std::shared_ptr<const T> temp;
            if (const_cast<const DatabaseChanger<T, TIndex>&>(*this).getItem(uiID, temp))
            {
                result = std::const_pointer_cast<T>(temp);
                return true;
            }
            return false;
        }

        inline void getItemShortInfos(UInt32StringMap &result) const
        {
            if (!m_pTargetDB)
                return;
            m_pNewDB->getItemShortInfos(result);
            m_pTargetDB->getItemShortInfos(result);
            UInt32StringMap::iterator itr = result.begin();
            std::advance(itr, m_pNewDB->getSize());
            result.erase(itr, result.end());
        }

        inline void setItem(uint32 uiID, std::shared_ptr<T> &prototype)
        {
            m_pNewDB->setItem(uiID, prototype);
        }

        inline uint32 getSize()
        {
            return m_pNewDB->getSize();
        }

        inline void resize(uint32 newSize)
        {
            m_pNewDB->resize(newSize);
        }

        virtual void storeChanges()
        {
            if (!m_pTargetDB)
                return;
            for (uint32 i = 1; i <= m_pNewDB->getSize(); ++i)
            {
                std::shared_ptr<T> proto;
                if (m_pNewDB->getItem(i, proto))
                    m_pTargetDB->setItem(i, proto);
            }
            m_pTargetDB->resize(m_pNewDB->getSize());
            m_pNewDB->clear();
            _setNewDBSize();
        }

    protected:
        std::shared_ptr<Database<T, TIndex>> m_pTargetDB;
        std::shared_ptr<Database<T, TIndex>> m_pNewDB;
    };

    typedef std::shared_ptr<const DatabaseChanger<TilePrototype, TILE_INDEX>> ConstTileDatabaseChangerPtr;
    typedef std::shared_ptr<const DatabaseChanger<SpritePrototype, SPRITE_INDEX>> ConstSpriteDatabaseChangerPtr;
    typedef std::shared_ptr<DatabaseChanger<MAP_OBJECT::WorldObjectPrototype, WORLD_OBJECT_INDEX>> WorldObjectDatabaseChangerPtr;
    typedef std::shared_ptr<DatabaseChanger<AnimationPrototype, ANIMATION_INDEX>> AnimationDatabaseChangerPtr;
    typedef std::shared_ptr<const DatabaseChanger<AnimationPrototype, ANIMATION_INDEX>> ConstAnimationDatabaseChangerPtr;
    typedef std::shared_ptr<const DatabaseChanger<ObjectAnimationTypePrototype, ANIMATION_TYPE_INDEX>> ConstObjectAnimationTypeDatabaseChangerPtr;
}
#endif
