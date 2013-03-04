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