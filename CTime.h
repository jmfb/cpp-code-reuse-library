#if !defined(CRL_CTIME_H)
#define CRL_CTIME_H

#include <CRL/MathUtility.h>

namespace TIME
{

class CTime
{
public:
    CTime() : mData(0)
    {
    }
    CTime(
        unsigned long hour,
        unsigned long minute,
        unsigned long second,
        unsigned long milliseconds) : mData(0)
    {
        SetTime(hour, minute, second, milliseconds);
    }
    CTime(const CTime& rhs) : mData(rhs.mData)
    {
    }
    ~CTime()
    {
    }
    
    CTime& operator=(const CTime& rhs)
    {
        mData = rhs.mData;
        return(*this);
    }
    void Clear()
    {
        mData = 0;
    }
    
    void SetTime(
        unsigned long hour,
        unsigned long minute,
        unsigned long second,
        unsigned long milliseconds)
    {
        SetHour(hour);
        SetMinute(minute);
        SetSecond(second);
        SetMilliseconds(milliseconds);
    }
    void SetHour(unsigned long hour)
    {
        mHour = MATH::Bound(0ul, 23ul, hour);
    }
    void SetMinute(unsigned long minute)
    {
        mMinute = MATH::Bound(0ul, 59ul, minute);
    }
    void SetSecond(unsigned long second)
    {
        mSecond = MATH::Bound(0ul, 59ul, second);
    }
    void SetMilliseconds(unsigned long milliseconds)
    {
        mMilliseconds = MATH::Bound(0ul, 999ul, milliseconds);
    }
    
    unsigned long GetHour() const
    {
        return(mHour);
    }
    unsigned long GetMinute() const
    {
        return(mMinute);
    }
    unsigned long GetSecond() const
    {
        return(mSecond);
    }
    unsigned long GetMilliseconds() const
    {
        return(mMilliseconds);
    }
    
    void SetTimeData(unsigned long data)
    {
        mData = data;
        mUnused = 0;
        SetTime(mHour, mMinute, mSecond, mMilliseconds);
    }
    unsigned long GetTimeData() const
    {
        return(mData);
    }

    bool operator==(const CTime& rhs) const
    {
        return(mData == rhs.mData);
    }
    bool operator!=(const CTime& rhs) const
    {
        return(mData != rhs.mData);
    }
    bool operator<(const CTime& rhs) const
    {
        return(mData < rhs.mData);
    }
    bool operator<=(const CTime& rhs) const
    {
        return(mData <= rhs.mData);
    }
    bool operator>(const CTime& rhs) const
    {
        return(mData > rhs.mData);
    }
    bool operator>=(const CTime& rhs) const
    {
        return(mData >= rhs.mData);
    }
    
private:
    union
    {
        struct
        {
            unsigned mMilliseconds : 10;    //0 - 1023 (need 0 - 999)
            unsigned mSecond : 6;           //0 - 63 (need 0 - 59)
            unsigned mMinute : 6;           //0 - 63 (need 0 - 59)
            unsigned mHour : 5;             //0 - 31 (need 0 - 23)
            unsigned mUnused : 5;           //Upper 5 unused bits, must be zero
        };
        unsigned long mData;    //32 bit time value in order of significance
    };
};

}

#endif //CRL_CTIME_H

