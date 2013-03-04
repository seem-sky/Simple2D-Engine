#ifndef THREAD_H
#define THREAD_H

#include <windows.h>

class Thread
{
public:
    Thread ( DWORD (WINAPI * pFun) (void* arg), void* pArg);
    ~Thread () { CloseHandle (_handle); }
    void Resume () { ResumeThread (_handle); }
    void WaitForDeath () { WaitForSingleObject (_handle, 500);}

private:
    HANDLE _handle;
    DWORD  _tid;     // thread id
};

enum ThreadState
{
    THREAD_NONE,
    THREAD_DONE,
    THREAD_FAILED,
    THREAD_IN_PROGRESS,
};

template <class T>
class ActiveObject
{
public:
    // The constructor of the derived class
    // should call
    //    _thread.Resume ();
    // at the end of construction
    ActiveObject(T *p_pReceiver) : m_ThreadState(THREAD_NONE), m_pReceiver(p_pReceiver),
#pragma warning(disable: 4355) // 'this' used before initialized
        _thread (ThreadEntry, this)
#pragma warning(default: 4355)
    {}

    void Kill ()
    {
        // Let's make sure it's gone
        _thread.WaitForDeath ();
        delete this;
    }

    inline ThreadState GetThreadState() const { return m_ThreadState; }

    inline T* GetReceiver() const { return m_pReceiver; }

protected:
    virtual void Run () = 0;
    static DWORD WINAPI ThreadEntry (void* pArg)
    {
        ActiveObject * pActive = (ActiveObject *) pArg;
        pActive->Run ();
        return 0;
    }

    virtual void SetThreadState(ThreadState p_State)
    {
        m_ThreadState = p_State;
        //if (m_pReceiver)
        //    m_pReceiver->ThreadDone(this, m_ThreadState);
    }
    Thread _thread;

private:
    ThreadState m_ThreadState;
    T *m_pReceiver;
};
#endif
