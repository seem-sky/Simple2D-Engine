#ifndef THREAD_H
#define THREAD_H

#include <windows.h>

class Thread
{
public:
    Thread ( DWORD (WINAPI * pFun) (void* arg), void* pArg)
    {
        _handle = CreateThread (
            0, // Security attributes
            0, // Stack size
            pFun,
            pArg,
            CREATE_SUSPENDED,
            &_tid);
    }
    ~Thread () { CloseHandle (_handle); }
    void Resume () { ResumeThread (_handle); }
    void WaitForDeath ()
    {
        WaitForSingleObject (_handle, 500);
    }
private:
    HANDLE _handle;
    DWORD  _tid;     // thread id
};

class ActiveObject
{
public:
    ActiveObject ();
    virtual ~ActiveObject () {}
    void Kill ();

protected:
    virtual void Run () = 0;

    static DWORD WINAPI ThreadEntry (void *pArg);

    Thread _thread;
};
#endif
