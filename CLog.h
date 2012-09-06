////////////////////////////////////////////////////////////////////////////////
// Filename:    CLog.h
// Description: ...
//
// Created:     2006-04-08 17:08:11
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CLOG_H)
#define CRL_CLOG_H

#include <fstream>
#include <sstream>

#include <CRL/CSingleton.h>
#include <CRL/FileUtility.h>
#include <CRL/TimeUtility.h>

namespace ERR
{
    
class CLog : public PTR::CSingleton<CLog>
{
public:
    void Log(const std::string& message)
    {
        mOut << TIME::Format(TIME::GetTime(), "[{HH}:{mm}:{ss}.{nnn}]: ") << message << std::endl;
    }

private:
    CLog()
    {
        std::string app = FSYS::GetModuleFileName();
        std::string ts = TIME::Format(TIME::GetDate(), "{yyyy}{MM}{dd}");
        std::string log = FSYS::FormatPath(FSYS::GetFilePath(app), FSYS::GetFileTitle(app) + "_" + ts + ".log");
        mOut.open(log.c_str(), std::ios::app);
    }
    ~CLog()
    {
    }
    
    std::ofstream mOut;
    
    friend class PTR::CSingleton<CLog>;
    
private:
    CLog(const CLog& rhs);
    CLog& operator=(const CLog& rhs);
};    

class log
{
public:
    log()
    {
    }
    ~log()
    {
        CLog::Get().Log(mOut.str());
    }
    
    template <typename T>
    log& operator<<(const T& t)
    {
        mOut << t;
        return(*this);
    }

private:
    std::ostringstream mOut;
};

}

namespace std
{
    typedef ERR::log log;
}

#endif //CRL_CLOG_H

