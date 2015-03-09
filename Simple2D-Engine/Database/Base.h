#pragma once

#include <vector>
#include <memory>
#include "Interface.h"

namespace database
{
    template <class T, uint32 MAXIMUM_SIZE>
    class Base : public Interface
    {
    private:
        std::vector<std::unique_ptr<T>> m_pPrototypes;

    public:
        void setPrototype(T* pPrototype)
        {
            if (pPrototype && !pPrototype->isEmpty())
                m_pPrototypes.at(pPrototype->getID() - 1) = std::unique_ptr<T>(pPrototype);
        }

        const T* getPrototype(uint32 ID) const override
        {
            if (ID && getSize() >= ID && m_pPrototypes.at(ID - 1) && !m_pPrototypes.at(ID - 1)->isEmpty())
                return m_pPrototypes.at(--ID).get();
            return nullptr;
        }

        T* getPrototype(uint32 ID)
        {
            return const_cast<T*>(const_cast<const Base<T, MAXIMUM_SIZE>&>(*this).getPrototype(ID));
        }

        T* takePrototype(uint32 ID)
        {
            if (auto ptr = const_cast<T*>(const_cast<const Base<T, MAXIMUM_SIZE>&>(*this).getPrototype(ID)))
                return m_pPrototypes.at(ID - 1).release();
            return nullptr;
        }

        void removePrototype(uint32 ID)
        {
            if (ID && getSize() >= ID &&)
                m_pPrototypes.at(ID - 1).reset();
        }

        void setSize(uint32 size)
        {
            if (size > MAXIMUM_SIZE)
                throw std::length_error("Invalid size.");
            m_pPrototypes.resize(size);
        }

        uint32 getSize() const override
        {
            return uint32(m_pPrototypes.size());
        }

        uint32 getMaximumSize() const
        {
            return MAXIMUM_SIZE;
        }

        bool isEmpty() const override
        {
            return m_pPrototypes.empty();
        }
    };
}
