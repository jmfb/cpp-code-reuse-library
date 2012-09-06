#if !defined(CRL_MATHUTILITY_H)
#define CRL_MATHUTILITY_H

namespace MATH
{
    //return the greater of {x,y} (uses less than comparison)
    template <typename TX, typename TY>
    inline TX Max(const TX& x, const TY& y)
    {
        return(y < x ? x : y);
    }

    //return the lesser of {x,y}
    template <typename TX, typename TY>
    inline TX Min(const TX& x, const TY& y)
    {
        return(x < y ? x : y);
    }

    //determine if z is in the range of [x-y]
    template <typename TX, typename TY, typename TZ>
    inline bool Between(const TX& x, const TY& y, const TZ& z)
    {
        return(z >= x && z <= y);
    }

    //return z within the bounds of [x-y]
    template <typename TX, typename TY, typename TZ>
    inline TX Bound(const TX& x, const TY& y, const TZ& z)
    {
        return(z < x ? x : z > y ? y : z);
    }

    //return z instead of x when x is equal to y, otherwise return x
    template <typename TX, typename TY, typename TZ>
    inline TX Instead(const TX& x, const TY& y, const TZ& z)
    {
        return(x == y ? z : x);
    }
    
    template <typename TX, typename TY>
    inline bool TestFlag(const TX& value, const TY& flag)
    {
        return (value & static_cast<TX>(flag)) == static_cast<TX>(flag);
    }
}

#endif //CRL_MATHUTILITY_H

