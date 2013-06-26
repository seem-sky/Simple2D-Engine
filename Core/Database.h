#ifndef DATABASE_H
#define DATABASE_H

#include "Container.h"
#include "Logfile.h"

namespace DATABASE
{
    /*#####
    # Prototype superclass
    #####*/
    class Prototype
    {
    public:
        Prototype(uint32 uiID) : m_uiID(uiID) {}

        inline void setName(const QString &sName) { m_sName = sName; }
        inline QString getName() const { return m_sName; }
        inline uint32 getID() const { return m_uiID; }
        inline void setID(uint32 uiID) { m_uiID = uiID; }

    private:
        uint32 m_uiID;
        QString m_sName;
    };
    typedef std::shared_ptr<Prototype> PrototypePtr;
    typedef std::vector<PrototypePtr> PrototypePtrVector;

    /*#####
    # Database
    #####*/
    template <class T>
    class DatabaseChanger;
    template <class T>
    class Database : public Container<T>
    {
        friend class DatabaseMgr;
        friend class DatabaseChanger<T>;

    public:
        void getItemShortInfos(UInt32StringMap &result) const
        {
            for (uint32 i = 0; i < getSize(); ++i)
            {
                if (m_Items.at(i) && m_Items.at(i)->getID() != 0)
                    result.insert(std::make_pair(m_Items.at(i)->getID(), m_Items.at(i)->getName()));
            }
        }

        void resize(uint32 uiSize, bool fillNew = true)
        {
            uint32 uiOldSize = getSize();
            m_Items.resize(uiSize);
            BASIC_LOG("Resize database to " + QString::number(uiSize));
            if (fillNew && uiSize > uiOldSize)
            {
                BASIC_LOG("Fill database entrys from " + QString::number(uiOldSize) + " to " + QString::number(uiSize) + " with data.");
                for (uint32 i = uiOldSize; i < uiSize; ++i)                 // first ID == 1, so use i+1 for ID at position i
                    m_Items.at(i) = std::shared_ptr<T>(new T(i+1));
            }
            else if (uiSize < uiOldSize)
                BASIC_LOG("Erase database entrys from " + QString::number(uiOldSize) + " to " + QString::number(uiSize) + ".");
        }
    };
};
#endif