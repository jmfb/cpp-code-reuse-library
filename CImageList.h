////////////////////////////////////////////////////////////////////////////////
// Filename:    CImageList.h
// Description: This file declares the image list wrapper class.
//
// Created:     2005-03-26 02:28:19
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CIMAGELIST_H)
#define CRL_CIMAGELIST_H

#include <map>
#include <CRL/CScope.h>
#include <CRL/WinUtility.h>

namespace WIN
{

class CImageListPolicy
{
public:
    typedef HIMAGELIST data_t;
    static HIMAGELIST GetDefault()
    {
        return(NULL);
    }
    static void Destroy(HIMAGELIST h)
    {
        ::ImageList_Destroy(h);
    }
};

class CImageList : public PTR::CScope<CImageListPolicy>
{
public:
    CImageList() : mCX(0), mCY(0)
    {
    }
    ~CImageList()
    {
    }
    
    operator HIMAGELIST() const
    {
        return(Get());
    }
    
    void CreateSmallIconList()
    {
        mCX = ::GetSystemMetrics(SM_CXSMICON);
        mCY = ::GetSystemMetrics(SM_CYSMICON);
        Attach(::ImageList_Create(mCX, mCY, ILC_COLORDDB|ILC_MASK, 0, 64));
    }
    
    int operator[](int id)
    {
        map_t::iterator iter = mIDtoIndex.find(id);
        if (iter == mIDtoIndex.end())
        {
            iter = mIDtoIndex.insert(std::make_pair(id, ::ImageList_AddIcon(Get(), WIN::LoadIcon(id, mCX, mCY)))).first;
        }
        return(iter->second);
    }
    
    void Draw(HDC dc, int index, int x, int y, unsigned int style = ILD_NORMAL)
    {
        ::ImageList_Draw(Get(), index, dc, x, y, style);
    }
    
private:
    int mCX;
    int mCY;
    
    typedef std::map<int, int> map_t;
    map_t mIDtoIndex;
};

}

#endif //CRL_CIMAGELIST_H

