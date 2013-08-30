#ifndef DATABASE_CHANGER_H
#define DATABASE_CHANGER_H

#include "DatabaseMgr.h"

namespace DATABASE
{
    template <class T>
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
        DatabaseChanger(std::shared_ptr<Database<T>> pDB) : m_pNewDB(new Database<T>())
        {
            setDB(pDB);
        }
        DatabaseChanger() : m_pNewDB(new Database<T>()) {}
        virtual void setDB(std::shared_ptr<Database<T>> pDB)
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
            if (const_cast<const DatabaseChanger<T>&>(*this).getItem(uiID, temp))
            {
                result = std::const_pointer_cast<T>(temp);
                return true;
            }
            return false;
        }

        inline void getItemShortInfos(UInt32StringPairVector &result) const
        {
            if (!m_pTargetDB)
                return;
            UInt32StringPairVector secondResult;
            m_pNewDB->getItemShortInfos(secondResult);
            m_pTargetDB->getItemShortInfos(result);
            // compare results
            for (auto &obj : secondResult)
            {
                bool success = false;
                for (auto itr = result.begin(); itr != result.end(); ++itr)
                {
                    if (obj.first == itr->first)
                    {
                        std::swap(obj.second, itr->second);
                        success = true;
                        break;
                    }
                    else if (obj.first < itr->first)
                    {
                        result.insert(itr, std::move(obj));
                        success = true;
                        break;
                    }
                }
                if (!success)
                    result.push_back(std::move(obj));
            }
            // resize result if greater than m_pNewDB
            if (m_pNewDB->getSize() < result.size())
                result.resize(m_pNewDB->getSize());
        }

        inline void setItem(uint32 uiID, std::shared_ptr<T> &prototype)
        {
            m_pNewDB->setItem(uiID, prototype);
            m_ChangedIDs.insert(uiID);
        }

        inline uint32 getSize()
        {
            return m_pNewDB->getSize();
        }

        inline void resize(uint32 newSize)
        {
            m_pNewDB->resize(newSize, true);
        }

        virtual void storeChanges()
        {
            if (!m_pTargetDB)
                return;

            // compare changed items
            for (auto i : m_ChangedIDs)
            {
                std::shared_ptr<T> proto;
                if (m_pNewDB->getItem(i, proto))
                    m_pTargetDB->setItem(i, proto);
            }

            m_pTargetDB->resize(m_pNewDB->getSize());
            _setNewDBSize();
            m_ChangedIDs.clear();
        }

    protected:
        std::shared_ptr<Database<T>> m_pTargetDB;
        std::shared_ptr<Database<T>> m_pNewDB;
        UInt32UnorderedSet m_ChangedIDs;
    };

    typedef std::shared_ptr<const DatabaseChanger<TilePrototype>> ConstTileDatabaseChangerPtr;
    typedef std::shared_ptr<const DatabaseChanger<SpritePrototype>> ConstSpriteDatabaseChangerPtr;
    typedef std::shared_ptr<DatabaseChanger<MAP_OBJECT::WorldObjectPrototype>> WorldObjectDatabaseChangerPtr;
    typedef std::shared_ptr<DatabaseChanger<AnimationPrototype>> AnimationDatabaseChangerPtr;
    typedef std::shared_ptr<const DatabaseChanger<AnimationPrototype>> ConstAnimationDatabaseChangerPtr;
    typedef std::shared_ptr<const DatabaseChanger<ObjectAnimationTypePrototype>> ConstObjectAnimationTypeDatabaseChangerPtr;
}
#endif
