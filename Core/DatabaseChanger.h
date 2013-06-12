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
        DatabaseChanger(boost::shared_ptr<Database<T>> pDB) : m_pNewDB(new Database<T>())
        {
            setDB(pDB);
        }
        DatabaseChanger() : m_pNewDB(new Database<T>()) {}
        virtual void setDB(boost::shared_ptr<Database<T>> pDB)
        {
            m_pTargetDB = pDB;
            _setNewDBSize();
        }

        inline bool getItem(uint32 uiID, boost::shared_ptr<const T> &result) const
        {
            if (!m_pTargetDB)
                return false;
            if (m_pNewDB->getItem(uiID, result))
                return true;
            if (m_pTargetDB->getItem(uiID, result))
            {
                result = boost::shared_ptr<const T>(new T(*result));
                return true;
            }
            return false;
        }

        inline bool getItem(uint32 uiID, boost::shared_ptr<T> &result)
        {
            boost::shared_ptr<const T> temp;
            if (const_cast<const DatabaseChanger<T>&>(*this).getItem(uiID, temp))
            {
                result = boost::const_pointer_cast<T>(temp);
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

        inline void setItem(uint32 uiID, boost::shared_ptr<T> &prototype)
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
                boost::shared_ptr<T> proto;
                if (m_pNewDB->getItem(i, proto))
                    m_pTargetDB->setItem(i, proto);
            }
            m_pTargetDB->resize(m_pNewDB->getSize());
            m_pNewDB->clear();
            _setNewDBSize();
        }

    protected:
        boost::shared_ptr<Database<T>> m_pTargetDB;
        boost::shared_ptr<Database<T>> m_pNewDB;
    };

    typedef boost::shared_ptr<const DatabaseChanger<TexturePrototype>> ConstTextureDatabaseChangerPtr;
    typedef boost::shared_ptr<const DatabaseChanger<TilePrototype>> ConstTileDatabaseChangerPtr;
    typedef boost::shared_ptr<const DatabaseChanger<SpritePrototype>> ConstSpriteDatabaseChangerPtr;
    typedef boost::shared_ptr<DatabaseChanger<WorldObjectPrototype>> WorldObjectDatabaseChangerPtr;
    typedef boost::shared_ptr<DatabaseChanger<AnimationPrototype>> AnimationDatabaseChangerPtr;
    typedef boost::shared_ptr<const DatabaseChanger<AnimationPrototype>> ConstAnimationDatabaseChangerPtr;
    typedef boost::shared_ptr<const DatabaseChanger<ObjectAnimationTypePrototype>> ConstObjectAnimationTypeDatabaseChangerPtr;
}
#endif