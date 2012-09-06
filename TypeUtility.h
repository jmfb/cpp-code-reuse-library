////////////////////////////////////////////////////////////////////////////////
// Filename:    TypeUtility.h
// Description: This file defines a bunch of type related utility classes and
//              functions.
//
// Created:     2005-03-19 19:00:26
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_TYPEUTILITY_H)
#define CRL_TYPEUTILITY_H

namespace TYPE
{
    template <typename T> struct signed_version
    {
        typedef T value_t;
    };
    template <> struct signed_version<unsigned char>
    {
        typedef char value_t;
    };
    template <> struct signed_version<unsigned short>
    {
        typedef short value_t;
    };
    template <> struct signed_version<unsigned long>
    {
        typedef long value_t;
    };
    template <> struct signed_version<unsigned long long>
    {
        typedef long long value_t;
    };
    template <> struct signed_version<unsigned int>
    {
        typedef int value_t;
    };
    
    template <typename T> struct unsigned_version
    {
        typedef T value_t;
    };
    template <> struct unsigned_version<char>
    {
        typedef unsigned char value_t;
    };
    template <> struct unsigned_version<short>
    {
        typedef unsigned short value_t;
    };
    template <> struct unsigned_version<long>
    {
        typedef unsigned long value_t;
    };
    template <> struct unsigned_version<long long>
    {
        typedef unsigned long long value_t;
    };
    template <> struct unsigned_version<int>
    {
        typedef unsigned int value_t;
    };
    
    template <typename T> struct integer_type
    {
        typedef T value_t;
        typedef typename signed_version<T>::value_t signed_t;
        typedef typename unsigned_version<T>::value_t unsigned_t;
    };

    template <typename T>
    inline typename integer_type<T>::signed_t signed_cast(T val)
    {
        return(static_cast<typename integer_type<T>::signed_t>(val));
    }
    
    template <typename T>
    inline typename integer_type<T>::unsigned_t unsigned_cast(T val)
    {
        return(static_cast<typename integer_type<T>::unsigned_t>(val));
    }
}

#endif //CRL_TYPEUTILITY_H

