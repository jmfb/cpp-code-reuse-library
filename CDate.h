#if !defined(CRL_CDATE_H)
#define CRL_CDATE_H

#include <CRL/MathUtility.h>

namespace TIME
{

class CDate
{
public:
    CDate() : mData(0)
    {
        Clear();
    }
    CDate(
        unsigned long year,
        unsigned long month,
        unsigned long day) : mData(0)
    {
        SetDate(year, month, day);
    }
    CDate(const CDate& rhs) : mData(rhs.mData)
    {
    }
    ~CDate()
    {
    }
    
    CDate& operator=(const CDate& rhs)
    {
        mData = rhs.mData;
        return(*this);
    }
    void Clear()
    {
        SetDate(1900, 1, 1);
    }
    
    void SetDate(
        unsigned long year,
        unsigned long month,
        unsigned long day)
    {
        SetYear(year);
        SetMonth(month);
        SetDay(day);
    }
    void SetYear(unsigned long year)
    {
        mYear = MATH::Bound(1900ul, 9999ul, year);
    }
    void SetMonth(unsigned long month)
    {
        mMonth = MATH::Bound(1ul, 12ul, month);
    }
    void SetDay(unsigned long day)
    {
        mDay = MATH::Bound(1ul, 31ul, day);
    }
    
    unsigned long GetYear() const
    {
        return(mYear);
    }
    unsigned long GetMonth() const
    {
        return(mMonth);
    }
    unsigned long GetDay() const
    {
        return(mDay);
    }
    
    void SetDateData(unsigned long data)
    {
        mData = data;
        mUnused = 0;
        SetDate(mYear, mMonth, mDay);
    }
    unsigned long GetDateData() const
    {
        return(mData);
    }
    
    bool operator==(const CDate& rhs) const
    {
        return(mData == rhs.mData);
    }
    bool operator!=(const CDate& rhs) const
    {
        return(mData != rhs.mData);
    }
    bool operator<(const CDate& rhs) const
    {
        return(mData < rhs.mData);
    }
    bool operator<=(const CDate& rhs) const
    {
        return(mData <= rhs.mData);
    }
    bool operator>(const CDate& rhs) const
    {
        return(mData > rhs.mData);
    }
    bool operator>=(const CDate& rhs) const
    {
        return(mData >= rhs.mData);
    }
    
private:
    union
    {
        struct
        {
            unsigned mDay : 5;      //0 - 31 (need 1 - 31)
            unsigned mMonth : 4;    //0 - 15 (need 1 - 12)
            unsigned mYear : 14;    //0 - 16383 (need 1900 - 9999)
            unsigned mUnused : 9;   //Upper 9 unused bits, must be zero
        };
        unsigned long mData;    //32 bit date value in order of significance
    };
};

}

#endif //CRL_CDATE_H

