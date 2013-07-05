#ifndef TSINGLETON_H
#define TSINGLETON_H

template <class T>
class TSingleton
{
protected:
    static T *m_pSingleton;

public:
    inline static T* get()
    {
        if (!m_pSingleton)
            m_pSingleton = new T;

        return m_pSingleton;
    }

    void del()
    {
        if (m_pSingleton)
        {
            delete (m_pSingleton);
            m_pSingleton = NULL;
        }
    }
};
template <class T>
T* TSingleton<T>::m_pSingleton = 0;
#endif