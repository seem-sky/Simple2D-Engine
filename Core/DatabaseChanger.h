#ifndef DATABASE_CHANGER_H
#define DATABASE_CHANGER_H

#include "DatabaseMgr.h"

namespace DATABASE
{
    template <class T>
    class DatabaseChanger
    {
    private:
        inline void setNewDBSize()
        {
            m_NewDB.clear();
            if (m_pTargetDB)
                m_NewDB.resizeDB(m_pTargetDB->getDBSize(), false);
        }

    public:
        DatabaseChanger(boost::shared_ptr<Database<T>> pDB)
        {
            setDB(pDB);
        }
        DatabaseChanger() {}
        inline void setDB(boost::shared_ptr<Database<T>> pDB)
        {
            m_pTargetDB = pDB;
            setNewDBSize();
        }

        inline bool getPrototype(uint32 uiID, boost::shared_ptr<const T> &result) const
        {
            if (!m_pTargetDB)
                return false;
            if (m_NewDB.getPrototype(uiID, result))
                return true;
            if (m_pTargetDB->getPrototype(uiID, result))
            {
                result = boost::shared_ptr<const T>(new T(*result));
                return true;
            }
            return false;
        }

        inline bool getPrototype(uint32 uiID, boost::shared_ptr<T> &result)
        {
            boost::shared_ptr<const T> temp;
            if (const_cast<const DatabaseChanger<T>&>(*this).getPrototype(uiID, temp))
            {
                result = boost::const_pointer_cast<T>(temp);
                return true;
            }
            return false;
        }

        inline void getPrototypeShortInfos(UInt32StdStringMap &result) const
        {
            if (!m_pTargetDB)
                return;
            m_NewDB.getPrototypeShortInfos(result);
            m_pTargetDB->getPrototypeShortInfos(result);
            UInt32StdStringMap::iterator itr = result.begin();
            std::advance(itr, m_NewDB.getDBSize());
            result.erase(itr, result.end());
        }

        inline void setPrototype(uint32 uiID, boost::shared_ptr<T> &prototype)
        {
            m_NewDB.setPrototype(uiID, prototype);
        }

        inline uint32 getDBSize()
        {
            return m_NewDB.getDBSize();
        }

        inline void resizeDB(uint32 newSize)
        {
            m_NewDB.resizeDB(newSize);
        }

        inline void storeChanges()
        {
            if (!m_pTargetDB)
                return;
            for (uint32 i = 1; i <= m_NewDB.getDBSize(); ++i)
            {
                boost::shared_ptr<T> proto;
                if (m_NewDB.getPrototype(i, proto))
                    m_pTargetDB->setPrototype(i, proto);
            }
            m_pTargetDB->resizeDB(m_NewDB.getDBSize());
            setNewDBSize();
        }

    private:
        boost::shared_ptr<Database<T>> m_pTargetDB;
        Database<T> m_NewDB;
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