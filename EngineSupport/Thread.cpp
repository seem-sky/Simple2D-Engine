#include "Thread.h"

Thread::Thread ( DWORD (WINAPI * pFun) (void* arg), void* pArg)
{
    _handle = CreateThread (
        0, // Security attributes
        0, // Stack size
        pFun,
        pArg,
        CREATE_SUSPENDED,
        &_tid);
}

// The constructor of the derived class
// should call
//    _thread.Resume ();
// at the end of construction

ActiveObject::ActiveObject () :
#pragma warning(disable: 4355) // 'this' used before initialized
  _thread (ThreadEntry, this)
#pragma warning(default: 4355)
{
}

void ActiveObject::Kill ()
{
    // Let's make sure it's gone
    _thread.WaitForDeath ();
    delete this;
}

DWORD WINAPI ActiveObject::ThreadEntry (void* pArg)
{
    ActiveObject * pActive = (ActiveObject *) pArg;
    pActive->Run ();
    return 0;
}
