#if !defined(CRL_TIMEUTILITY_H)
#define CRL_TIMEUTILITY_H

#include <windows.h>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <cstring>
#include <CRL/CDateTime.h>
#include <CRL/StringUtility.h>

namespace TIME
{
    //Template function to convert any time value to a CDateTime via a convert function
    template <typename T>
    inline CDateTime ToDateTime(const T& from)
    {
        CDateTime to;
        Convert(from, to);
        return(to);
    }

    //Current date and time functions
    inline CDateTime GetDateTime()
    {
        SYSTEMTIME st;
        ::GetLocalTime(&st);
        return(ToDateTime(st));
    }
    inline CDate GetDate()
    {
        return(GetDateTime());
    }
    inline CTime GetTime()
    {
        return(GetDateTime());
    }
    
    //Format functions
    inline const char* FormatMonth(const CDate& date, bool full)
    {
        static const char* MONTH_STRING[13][2] = {
            {NULL, NULL},
            {"Jan", "January"},
            {"Feb", "February"},
            {"Mar", "March"},
            {"Apr", "April"},
            {"May", "May"},
            {"Jun", "June"},
            {"Jul", "July"},
            {"Aug", "August"},
            {"Sep", "September"},
            {"Oct", "October"},
            {"Nov", "November"},
            {"Dec", "December"}
        };
        return(MONTH_STRING[date.GetMonth()][full ? 1 : 0]);
    }
    
    inline std::string Format(const CDate& date, const std::string& fmt)
    {
        std::ostringstream d, dd, M, MM, yyyy;
        
        d << date.GetDay();
        dd << std::setw(2) << std::setfill('0') << date.GetDay();
        M << date.GetMonth();
        MM << std::setw(2) << std::setfill('0') << date.GetMonth();
        yyyy << date.GetYear();
        
        std::string retval = fmt;
        retval = STRING::replace(retval, "{d}", d.str());
        retval = STRING::replace(retval, "{dd}", dd.str());
        retval = STRING::replace(retval, "{M}", M.str());
        retval = STRING::replace(retval, "{MM}", MM.str());
        retval = STRING::replace(retval, "{MMM}", FormatMonth(date, false));
        retval = STRING::replace(retval, "{MMMM}", FormatMonth(date, true));
        retval = STRING::replace(retval, "{yy}", yyyy.str().substr(2, 2));
        retval = STRING::replace(retval, "{yyyy}", yyyy.str());
        return(retval);
    }
    inline std::string Format(const CTime& time, const std::string& fmt)
    {
        std::ostringstream h, hh, H, HH, m, mm, s, ss, n, nnn;
        
        unsigned long hour = time.GetHour();
        unsigned long hour12 = hour % 12 == 0 ? 12 : hour % 12;
        
        h << hour12;
        hh << std::setw(2) << std::setfill('0') << hour12;
        H << hour;
        HH << std::setw(2) << std::setfill('0') << hour;
        m << time.GetMinute();
        mm << std::setw(2) << std::setfill('0') << time.GetMinute();
        s << time.GetSecond();
        ss << std::setw(2) << std::setfill('0') << time.GetSecond();
        n << time.GetMilliseconds();
        nnn << std::setw(3) << std::setfill('0') << time.GetMilliseconds();
        
        std::string retval = fmt;
        retval = STRING::replace(retval, "{h}", h.str());
        retval = STRING::replace(retval, "{hh}", hh.str());
        retval = STRING::replace(retval, "{H}", H.str());
        retval = STRING::replace(retval, "{HH}", HH.str());
        retval = STRING::replace(retval, "{m}", m.str());
        retval = STRING::replace(retval, "{mm}", mm.str());
        retval = STRING::replace(retval, "{s}", s.str());
        retval = STRING::replace(retval, "{ss}", ss.str());
        retval = STRING::replace(retval, "{n}", n.str());
        retval = STRING::replace(retval, "{nnn}", nnn.str());
        retval = STRING::replace(retval, "{t}", hour < 12 ? "A" : "P");
        retval = STRING::replace(retval, "{tt}", hour < 12 ? "AM" : "PM");
        return(retval);
    }
    inline std::string Format(const CDateTime& datetime, const std::string& fmt)
    {
        std::string retval = fmt;
        retval = Format(datetime.GetDate(), retval);
        retval = Format(datetime.GetTime(), retval);
        return(retval);
    }
    
    //conversion functions
    inline void Convert(const SYSTEMTIME& from, CDateTime& to)
    {
        to.SetDateTime(
            from.wYear,
            from.wMonth,
            from.wDay,
            from.wHour,
            from.wMinute,
            from.wSecond,
            from.wMilliseconds);
    }
    inline void Convert(const CDateTime& from, SYSTEMTIME& to)
    {
        to.wYear = static_cast<unsigned short>(from.GetYear());
        to.wMonth = static_cast<unsigned short>(from.GetMonth());
        to.wDay = static_cast<unsigned short>(from.GetDay());
        to.wHour = static_cast<unsigned short>(from.GetHour());
        to.wMinute = static_cast<unsigned short>(from.GetMinute());
        to.wSecond = static_cast<unsigned short>(from.GetSecond());
        to.wMilliseconds = static_cast<unsigned short>(from.GetMilliseconds());
        
        //Convert to/from a file time to validate wWeekday member
        FILETIME ft;
        ::SystemTimeToFileTime(&to, &ft);
        ::FileTimeToSystemTime(&ft, &to);
    }
    
    inline FILETIME ConvertFileTimeUtcToLocalFileTime(const FILETIME& fileTimeUtc)
    {
        FILETIME fileTimeLocal = {0};
        std::memset(&fileTimeLocal, 0, sizeof(fileTimeLocal));
        ::FileTimeToLocalFileTime(&fileTimeUtc, &fileTimeLocal);
        return fileTimeLocal;
    }
    inline SYSTEMTIME ConvertFileTimeToSystemTime(const FILETIME& fileTime)
    {
        SYSTEMTIME systemTime = {0};
        std::memset(&systemTime, 0, sizeof(systemTime));
        ::FileTimeToSystemTime(&fileTime, &systemTime);
        return systemTime;
    }
    inline std::tm ConvertSystemTimeToTm(const SYSTEMTIME& systemTime)
    {
        std::tm tm;
        tm.tm_sec = systemTime.wSecond;
        tm.tm_min = systemTime.wMinute;
        tm.tm_hour = systemTime.wHour;
        tm.tm_mday = systemTime.wDay;
        tm.tm_mon = systemTime.wMonth - 1;
        tm.tm_year = systemTime.wYear - 1900;
        return tm;
    }
    inline std::time_t ConvertTmToTimeT(std::tm tm)
    {
        return std::mktime(&tm);
    }
    inline std::chrono::system_clock::time_point ConvertTimeTToTimePoint(std::time_t timeT)
    {
        return std::chrono::system_clock::from_time_t(timeT);
    }
    inline std::chrono::system_clock::time_point ConvertFileTimeUtcToTimePoint(const FILETIME& fileTimeUtc)
    {
        return ConvertTimeTToTimePoint(
            ConvertTmToTimeT(
            ConvertSystemTimeToTm(
            ConvertFileTimeToSystemTime(
            ConvertFileTimeUtcToLocalFileTime(
                fileTimeUtc)))));
    }
}

#endif //CRL_TIMEUTILITY_H

