#pragma once

#include <map>
#include <Global.h>
#include <memory>
#include <functional>

namespace database
{
    template <class DB, class PROTOTYPE, bool COMPARE(const PROTOTYPE&, const PROTOTYPE&)>
    class Merger : public Interface
    {
    private:
        DB& m_DB;
        std::map<uint32, std::unique_ptr<PROTOTYPE>> m_New;
        uint32 m_Size;

    public:
        Merger(DB& db)
            : Interface(), m_Size(db.getSize()), m_DB(db)
        {}

        void setPrototype(PROTOTYPE* pPrototype)
        {
            if (!pPrototype)
                return;

            bool hasDifferences = true;
            if (auto pStored = m_DB.getPrototype(pPrototype->getID()))
                hasDifferences = !COMPARE(*pPrototype, *pStored);

            auto itr = m_New.find(pPrototype->getID());
            if (itr == m_New.end())
            {
                if (hasDifferences)
                    m_New.insert(std::make_pair(pPrototype->getID(), std::unique_ptr<PROTOTYPE>(pPrototype)));
            }
            else
            {
                if (hasDifferences)
                    itr->second = std::unique_ptr<PROTOTYPE>(pPrototype);
                // when there are no differences to the original stored prototype, delete the changed one
                else
                    m_New.erase(itr);
            }
        }

        const PROTOTYPE* getPrototype(uint32 ID) const override
        {
            auto itr = m_New.find(ID);
            if (itr != m_New.end())
                return itr->second.get();
            return m_DB.getPrototype(ID);
        }

        PROTOTYPE* getPrototype(uint32 ID)
        {
            return const_cast<PROTOTYPE*>(const_cast<const Merger&>(*this).getPrototype(ID));
        }

        void merge()
        {
            m_DB.setSize(m_Size);
            for (auto& itr : m_New)
                m_DB.setPrototype(itr.second.release());
            discard();
        }

        void discard()
        {
            m_Size = m_DB.getSize();
            m_New.clear();
        }

        void setSize(uint32 size) override
        {
            if (size > m_DB.getMaximumSize())
                throw std::length_error("Invalid size.");
            if (m_Size > size)
            {
                // remove stored items with greater IDs
                std::vector<uint32> m_Remove;
                for (auto itr = m_New.rbegin(); itr != m_New.rend() && itr->first > size; ++itr)
                    m_Remove.push_back(itr->first);
                for (auto ID : m_Remove)
                    m_New.erase(ID);
            }
            m_Size = size;
        }

        uint32 getSize() const override
        {
            return m_Size;
        }

        uint32 getMaximumSize() const override
        {
            return m_DB.getMaximumSize();
        }

        bool isEmpty() const override
        {
            return m_Size == 0;
        }
    };
}
