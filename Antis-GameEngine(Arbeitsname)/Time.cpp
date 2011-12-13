#include "Time.h"

CTime::CTime() : m_CurCount(0), m_NextFrame(0), m_FrameRate(0), m_Frames(0), m_TimeDif(0), TSingleton()
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "CTime : ";
    if(!QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency))
        ERROR_LOG(m_sLogLocationName + "Unable to find performance counter.");

    m_Offset            = m_Frequency;
    QueryPerformanceCounter((LARGE_INTEGER*)&m_LastCount);
    m_LastCount        += m_Offset;
}

CTime::~CTime()
{

}

// returnes true if next frame should be shown
bool CTime::TimeForNextFrame()
{
    if(m_CurCount > m_NextFrame)
        return true;
    else 
        return false;
}

// Update time
void CTime::UpdateTime()
{
    m_TimeDif = m_CurCount;
    m_CurCount = GetCurTime();
    QueryPerformanceCounter((LARGE_INTEGER*)&m_CurCount);
    if(m_CurCount - m_LastCount > m_Frequency)
    {
        m_LastCount = m_CurCount;
        m_FrameRate = m_Frames;
        m_Frames = 0;
    }
    else
        m_Frames++;
}

LONGLONG CTime::GetCurTime()
{
    LONGLONG CurTime = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&CurTime);
    return CurTime;
};

LONGLONG CTime::GetTimeElapsed()
{
    return (m_CurCount - m_TimeDif)* 1000 / m_Frequency;
}