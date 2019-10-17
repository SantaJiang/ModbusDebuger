#include "highprecisiontimer.h"
#include <Windows.h>



HighPrecisionTimer::HighPrecisionTimer()
{
    m_hTimer = ::CreateWaitableTimer(NULL, TRUE, NULL);
    m_frequency = readFrequency();

    m_performanceCount = readPerformanceCount();
}

HighPrecisionTimer::HighPrecisionTimer(const HighPrecisionTimer &Other)
{
    m_hTimer = ::CreateWaitableTimer(NULL, TRUE, NULL);
    *this = Other;
}

HighPrecisionTimer::~HighPrecisionTimer()
{
    if (m_hTimer != NULL)
    {
        ::CloseHandle(m_hTimer);
        m_hTimer = NULL;
    }
}

HighPrecisionTimer &HighPrecisionTimer::operator =(const HighPrecisionTimer &Other)
{
    m_frequency = Other.m_frequency;
    m_performanceCount = Other.m_performanceCount;

    return (*this);
}

bool HighPrecisionTimer::msleep(int msecs)
{
    LARGE_INTEGER liDueTime;
    __int64 usecs = msecs*1000;
    liDueTime.QuadPart = -10*(usecs);
    ::SetWaitableTimer(m_hTimer, &liDueTime, 0, NULL, NULL, 0);
    return (WaitForSingleObject(m_hTimer, INFINITE) == WAIT_OBJECT_0);
}

bool HighPrecisionTimer::usleep(int usecs)
{
    __int64 curCount = readPerformanceCount();
    __int64 dstCount = curCount + m_frequency*usecs/1000000;
    do {
        curCount = readPerformanceCount();
    } while (curCount < dstCount);
    return true;
}

bool HighPrecisionTimer::msleepEx(int msecs)
{
    __int64 nCurCount = readPerformanceCount();
    __int64 usedTime = (nCurCount-m_performanceCount)*1000000/m_frequency;
    __int64 usecs = msecs*1000;

    bool bResult = false;
    if (usecs > usedTime)
    {
        LARGE_INTEGER liDueTime;
        liDueTime.QuadPart = -10 * (usecs - usedTime);
        ::SetWaitableTimer(m_hTimer, &liDueTime, 0, NULL, NULL, 0);
        if (WaitForSingleObject(m_hTimer, INFINITE) == WAIT_OBJECT_0)
        {
            bResult = true;
        }
    }

    m_performanceCount = readPerformanceCount();
    return bResult;
}

__int64 HighPrecisionTimer::secsTo(const HighPrecisionTimer &Other)
{
    __int64 nCountOffset = Other.m_performanceCount - m_performanceCount;
    return (nCountOffset / m_frequency);
}

__int64 HighPrecisionTimer::msecsTo(const HighPrecisionTimer &Other)
{
    __int64 nCountOffset = Other.m_performanceCount - m_performanceCount;
    return ((nCountOffset * 1000) / m_frequency);
}

__int64 HighPrecisionTimer::usecsTo(const HighPrecisionTimer &Other)
{
    __int64 nCountOffset = Other.m_performanceCount - m_performanceCount;
    return ((nCountOffset * 1000000) / m_frequency);
}

__int64 HighPrecisionTimer::msecsUsed(bool bRefresh)
{
    return (usecsUsed(bRefresh)/1000);
}

__int64 HighPrecisionTimer::usecsUsed(bool bRefresh)
{
    __int64 nCurCount = readPerformanceCount();
    __int64 nCountOffset = nCurCount - m_performanceCount;
    if (bRefresh)
    {
        m_performanceCount = nCurCount;
    }
    return ((nCountOffset * 1000000) / m_frequency);
}

__int64 HighPrecisionTimer::readFrequency()
{
    LARGE_INTEGER frequency;
    ::QueryPerformanceFrequency(&frequency);
    return frequency.QuadPart;
}

__int64 HighPrecisionTimer::readPerformanceCount()
{
    LARGE_INTEGER performanceCount;
    ::QueryPerformanceCounter(&performanceCount);
    return performanceCount.QuadPart;
}
