#include "Time.h"

CTime::CTime()
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "CTime : ";
    m_pLogfile = CLogfile::Get();
    if(!QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency))
        ERROR_LOG(m_sLogLocationName + "Unable to find performance counter.");

    m_Offset            = m_Frequency;
    m_CurCount          = NULL;
    m_NextFrame         = NULL;
    m_FrameRate         = NULL;
    m_Frames            = NULL;
    m_TimeDif           = NULL;
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

// calculate time for next frame
void CTime::NextFrame()
{
    m_NextFrame = m_CurCount + m_Offset;
}

// Update time
void CTime::UpdateTime()
{
    m_TimeDif = m_CurCount;
    QueryPerformanceCounter((LARGE_INTEGER*)&m_CurCount);
    if(m_CurCount - m_LastCount > m_Frequency)
    {
        m_LastCount = m_CurCount;
        m_FrameRate = m_Frames;
        m_Frames = 0;
    }
    else
    {
        m_Frames++;
    }
}

// returns current frame rate
int CTime::GetCurFrameRate()
{
    return m_FrameRate;
}

// returns diff of the last frame
float CTime::GetTimeElapsed()
{
    return static_cast<float>(m_CurCount - m_TimeDif)* 1000 / m_Frequency;
}