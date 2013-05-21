#ifndef TSINGLETON
#define TSINGLETON

#include "Global.h"

template <class T>
class TSingleton
{
protected:
    static T *m_pSingleton;
    QString m_logLocationName;

public:
    TSingleton()
    {
        m_logLocationName = LOGFILE_ENGINE_LOG_NAME + "TSingleton : ";
    }

    virtual ~TSingleton()
    {

    }
    inline static T* Get()
    {
        if (!m_pSingleton)
            m_pSingleton = new T;

        return m_pSingleton;
    }

    void Del()
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