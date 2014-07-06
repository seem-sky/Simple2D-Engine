#ifndef MANAGER_H
#define MANAGER_H

#include <Global.h>
#include <memory>

template <class T>
class Manager
{
protected:
    virtual void setNew(std::unique_ptr<T>& pNew)
    {
        m_pCurrent = std::move(pNew);
    }

public:
    Manager::Manager() : m_isUpdating(false)
    {}

    void change(std::unique_ptr<T>& pNew, bool storeOld = false)
    {
        // do not change current while updating
        if (m_isUpdating)
        {
            m_pNew = std::move(pNew);
            return;
        }
        if (storeOld)
            m_pOld = std::move(m_pCurrent);
        setNew(pNew);
    }

    void changeWithOld(bool storeOld = false)
    {
        if (storeOld)
            changeScene(m_pOld, storeOld);
    }

    void clearOld()
    {
        m_pOld.reset();
    }

    virtual void updateCurrent(uint32 uiDiff)
    {
        m_isUpdating = true;
        if (m_pCurrent)
            m_pCurrent->update(uiDiff);
        m_isUpdating = false;
        if (m_pNew)
            change(m_pNew);
    }

    inline std::unique_ptr<T>& getCurrent() { return m_pCurrent; }
    inline const std::unique_ptr<T>& getCurrent() const { return m_pCurrent; }

private:
    std::unique_ptr<T> m_pCurrent;
    std::unique_ptr<T> m_pNew;
    std::unique_ptr<T> m_pOld;

    bool m_isUpdating;
};
#endif