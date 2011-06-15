#ifndef TIME_H
#define TIME_H

#include "Logfile.h"
#include "Global.h"
#include <windows.h>

class CTime : public TSingleton<CTime>
{
public:
    CTime();
    ~CTime();
    bool TimeForNextFrame();
    void NextFrame();
    int GetCurFrameRate();
    void UpdateTime();
    float GetTimeElapsed();

private:
    //Frequenz des Performance Counters
    LONGLONG m_Frequency;
    //Abstand zwischen den Frames
    LONGLONG m_Offset;
    //Zeit bis zum nächsten Frame
    LONGLONG m_NextFrame;
    //aktueller Zählerstand
    LONGLONG m_CurCount;
    //Letzte Zeitmessung
    LONGLONG m_LastCount;
    //Gibt die Zeit von m_CurCount vor der letzten Zeit Aktualisierung an
    LONGLONG m_TimeDif;
    //Momentane Frames
    int m_Frames;
    //Berechnete Frames der letzten Sekunde
    int m_FrameRate;

    //Zeiger auf Logfile
    CLogfile *m_pLogfile;
};
#endif