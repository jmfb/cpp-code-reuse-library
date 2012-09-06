#if !defined(CRL_CDATETIME_H)
#define CRL_CDATETIME_H

#include <CRL/CDate.h>
#include <CRL/CTime.h>

namespace TIME
{

class CDateTime : public CTime, public CDate
{
public:
    CDateTime() : CTime(), CDate()
    {
    }
    CDateTime(
        unsigned long year,
        unsigned long month,
        unsigned long day,
        unsigned long hour,
        unsigned long minute,
        unsigned long second,
        unsigned long milliseconds)
    : CTime(hour, minute, second, milliseconds), CDate(year, month, day)
    {
    }
    CDateTime(const CDate& date) : CTime(), CDate(date)
    {
    }
    CDateTime(const CTime& time) : CTime(time), CDate()
    {
    }
    CDateTime(const CDate& date, const CTime& time) : CTime(time), CDate(date)
    {
    }
    CDateTime(const CDateTime& rhs) : CTime(rhs), CDate(rhs)
    {
    }
    ~CDateTime()
    {
    }
    
    CDateTime& operator=(const CDateTime& rhs)
    {
        CTime::operator=(rhs);
        CDate::operator=(rhs);
        return(*this);
    }
    CDateTime& operator=(const CDate& date)
    {
        CTime::Clear();
        CDate::operator=(date);
        return(*this);
    }
    CDateTime& operator=(const CTime& time)
    {
        CTime::operator=(time);
        CDate::Clear();
        return(*this);
    }
    void Clear()
    {
        CTime::Clear();
        CDate::Clear();
    }
    void ClearDate()
    {
        CDate::Clear();
    }
    void ClearTime()
    {
        CTime::Clear();
    }
    
    void SetDateTime(
        unsigned long year,
        unsigned long month,
        unsigned long day,
        unsigned long hour,
        unsigned long minute,
        unsigned long second,
        unsigned long milliseconds)
    {
        CTime::SetTime(hour, minute, second, milliseconds);
        CDate::SetDate(year, month, day);
    }
    void SetDatePart(const CDate& date)
    {
        CDate::operator=(date);
    }
    void SetTimePart(const CTime& time)
    {
        CTime::operator=(time);
    }

    CDate& GetDate()
    {
        return(*this);
    }
    const CDate& GetDate() const
    {
        return(*this);
    }
    CTime& GetTime()
    {
        return(*this);
    }
    const CTime& GetTime() const
    {
        return(*this);
    }
    
    bool operator==(const CDateTime& rhs) const
    {
        return(CDate::operator==(rhs) && CTime::operator==(rhs));
    }
    bool operator!=(const CDateTime& rhs) const
    {
        return(CDate::operator!=(rhs) || CTime::operator!=(rhs));
    }
    bool operator<(const CDateTime& rhs) const
    {
        return(CDate::operator<(rhs) || (CDate::operator==(rhs) && CTime::operator<(rhs)));
    }
    bool operator<=(const CDateTime& rhs) const
    {
        return(CDate::operator<(rhs) || (CDate::operator==(rhs) && CTime::operator<=(rhs)));
    }
    bool operator>(const CDateTime& rhs) const
    {
        return(CDate::operator>(rhs) || (CDate::operator==(rhs) && CTime::operator>(rhs)));
    }
    bool operator>=(const CDateTime& rhs) const
    {
        return(CDate::operator>(rhs) || (CDate::operator==(rhs) && CTime::operator>=(rhs)));
    }
};

}

#endif //CRL_CDATETIME_H

