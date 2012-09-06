////////////////////////////////////////////////////////////////////////////////
// Filename:    CStream.h
// Description: This file declares the binary input and output stream classes.
//
// Created:     2005-03-26 03:57:47
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CSTREAM_H)
#define CRL_CSTREAM_H

#include <iostream>
#include <CRL/StringUtility.h>

namespace STREAM
{

class CInputStream
{
public:
    CInputStream(std::istream& in) : mIn(in)
    {
    }
    ~CInputStream()
    {
    }

    unsigned long Read(void* buffer, unsigned long size)
    {
        mIn.read(reinterpret_cast<char*>(buffer), size);
        return(mIn.gcount());
    }
    
    template <typename T>
    CInputStream& operator>>(T& data)
    {
        Read(&data, sizeof(data));
        return(*this);
    }
    
    CInputStream& operator>>(std::string& data)
    {
        unsigned long size = 0;
        operator>>(size);
        STRING::CStringPtr buffer(new char[size + 1]);
        Read(buffer.Get(), size);
        buffer.Get()[size] = '\0';
        data = buffer.Get();
        return(*this);
    }
    
private:
    std::istream& mIn;
    
private:
    CInputStream(const CInputStream& rhs);
    CInputStream& operator=(const CInputStream& rhs);
};

class COutputStream
{
public:
    COutputStream(std::ostream& out) : mOut(out)
    {
    }
    ~COutputStream()
    {
    }
    
    void Write(const void* buffer, unsigned long size)
    {
        mOut.write(reinterpret_cast<const char*>(buffer), size);
    }
    
    template <typename T>
    COutputStream& operator<<(const T& data)
    {
        Write(&data, sizeof(data));
        return(*this);
    }
    
    COutputStream& operator<<(const std::string& data)
    {
        unsigned long size = data.size();
        operator<<(size);
        Write(data.c_str(), size);
        return(*this);
    }
    
private:
    std::ostream& mOut;
    
private:
    COutputStream(const COutputStream& rhs);
    COutputStream& operator=(const COutputStream& rhs);
};

}

#endif //CRL_CSTREAM_H

