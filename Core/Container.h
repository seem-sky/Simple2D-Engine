#ifndef CONTAINER_H
#define CONTAINER_H

#include "Global.h"
#include <boost/smart_ptr.hpp>

template <class T>
class Container
{
public:
    virtual void clear() { m_Items.clear(); }

    virtual void setItem(uint32 uiID, boost::shared_ptr<T> &item)
    {
        if (uiID)
        {
            if (--uiID >= m_Items.size())
                m_Items.resize(uiID+1);
            m_Items.at(uiID) = item;
        }
    }

    virtual bool getItem(uint32 uiID, boost::shared_ptr<const T> &result) const
    {
        if (uiID && --uiID < m_Items.size() && m_Items.at(uiID))
        {
            result = m_Items.at(uiID);
            return true;
        }
        return false;
    }

    bool getItem(uint32 uiID, boost::shared_ptr<T> &result)
    {
        boost::shared_ptr<const T> temp;
        if (const_cast<const Container<T>&>(*this).getItem(uiID, temp))
        {
            result = boost::const_pointer_cast<T>(temp);
            return true;
        }
        return false;
    }

    inline uint32 getSize() const { return m_Items.size(); }
    virtual void resize(uint32 uiSize, bool fillNew = true)
    {
        if (fillNew)
            m_Items.resize(uiSize, boost::shared_ptr<T>(new T()));
        else
            m_Items.resize(uiSize);
    }

protected:
    std::vector<boost::shared_ptr<T>> m_Items;
};
#endif