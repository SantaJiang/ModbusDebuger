#ifndef HIGHPRECISIONTIMER_H
#define HIGHPRECISIONTIMER_H


class HighPrecisionTimer
{
public:
    HighPrecisionTimer();
    HighPrecisionTimer(const HighPrecisionTimer& Other);
    virtual ~HighPrecisionTimer();
    HighPrecisionTimer& operator =(const HighPrecisionTimer& Other);

    bool msleep(int msecs);
    bool usleep(int usecs);
    bool msleepEx(int msecs);

    __int64 secsTo(const HighPrecisionTimer& Other);
    __int64 msecsTo(const HighPrecisionTimer& Other);
    __int64 usecsTo(const HighPrecisionTimer& Other);

    __int64 msecsUsed(bool bRefresh = false);
    __int64 usecsUsed(bool bRefresh = false);

protected:
    inline __int64 readFrequency();
    inline __int64 readPerformanceCount();
private:
    void*   m_hTimer;
    __int64 m_frequency;
    __int64 m_performanceCount;
};

#endif // HIGHPRECISIONTIMER_H
