#ifndef POINT_H
#define POINT_H

template<typename T>
struct Point3D;

template<typename T>
struct Point
{
    T x;
    T y;

    inline Point(const Point3D<T> &p);
    inline Point(T X = 0, T Y = 0) : x(X), y(Y) { }
    inline Point<T>& operator =(Point3D<T> const& p);
    inline Point<T>& operator +=(Point<T> const& p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    inline Point<T>& operator -=(Point<T> const& p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    inline Point<T>& operator *=(const T &t)
    {
        x *= t;
        y *= t;
        return *this;
    }

    inline Point<T>& operator /=(const T &t)
    {
        x /= t;
        y /= t;
        return *this;
    }
};

template<typename T>
struct Point3D
{
    T x;
    T y;
    T z;

    inline Point3D(const Point<T> &p, T z = 0) : z(z) { *this = p; }
    inline Point3D(T X = 0, T Y = 0, T Z = 0) : x(X), y(Y), z(Z) { }

    inline Point3D<T>& operator =(Point<T> const& p)
    {
        *this = Point3D<T>(p.x, p.y, this->z);
        return *this;
    }

    inline Point3D<T>& operator +=(Point3D<T> const& p)
    {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }

    inline Point3D<T>& operator -=(Point3D<T> const& p)
    {
        x -= p.x;
        y -= p.y;
        z += p.z;
        return *this;
    }
};

/*#####
# Point
#####*/
template <typename T>
inline Point<T>& Point<T>::operator =(Point3D<T> const& p)
{
    *this = Point<T>(p.x, p.y);
    return *this;
}

template <typename T>
inline Point<T>::Point(const Point3D<T> &p)
{
    x = p.x;
    y = p.y;
}

template <typename T>
inline const Point<T> operator +(Point<T> const& lhs, Point<T> const& rhs)
{
    Point<T> tmp(lhs);
    tmp += rhs;
    return tmp;
}

template <typename T>
inline const Point<T> operator -(Point<T> const& lhs, Point<T> const& rhs)
{
    Point<T> tmp(lhs);
    tmp -= rhs;
    return tmp;
}

template <typename T>
inline const Point<T> operator *(Point<T> const& lhs, T const& rhs)
{
    Point<T> tmp(lhs);
    tmp *= rhs;
    return tmp;
}

template <typename T>
inline const Point<T> operator /(Point<T> const& lhs, T const& rhs)
{
    Point<T> tmp(lhs);
    tmp /= rhs;
    return tmp;
}

template <typename T>
inline bool operator ==(Point<T> const& lhs, Point<T> const& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename T>
inline bool operator !=(Point<T> const& lhs, Point<T> const& rhs)
{
    return !(lhs == rhs);
}

template <typename T>
inline bool operator <(Point<T> const& lhs, Point<T> const& rhs)
{
    return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
}

template <typename T>
inline bool operator >(Point<T> const& lhs, Point<T> const& rhs)
{
    return (lhs.x > rhs.x) || (lhs.x == rhs.x && lhs.y > rhs.y);
}

template <typename T>
inline bool operator <=(Point<T> const& lhs, Point<T> const& rhs)
{
    return !(lhs > rhs);
}

template <typename T>
inline bool operator >=(Point<T> const& lhs, Point<T> const& rhs)
{
    return !(lhs < rhs);
}

/*#####
# Point3D
#####*/
template <typename T>
inline const Point3D<T> operator +(Point3D<T> const& lhs, Point3D<T> const& rhs)
{
    Point3D<T> tmp(lhs);
    tmp += rhs;
    return tmp;
}

template <typename T>
inline const Point3D<T> operator -(Point3D<T> const& lhs, Point3D<T> const& rhs)
{
    Point3D<T> tmp(lhs);
    tmp -= rhs;
    return tmp;
}

template <typename T>
inline bool operator ==(Point3D<T> const& lhs, Point3D<T> const& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template <typename T>
inline bool operator !=(Point3D<T> const& lhs, Point3D<T> const& rhs)
{
    return !(lhs == rhs);
}

template <typename T>
inline bool operator <(Point3D<T> const& lhs, Point3D<T> const& rhs)
{
    return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y) || (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z < rhs.z);
}

template <typename T>
inline bool operator >(Point3D<T> const& lhs, Point3D<T> const& rhs)
{
    return (lhs.x > rhs.x) || (lhs.x == rhs.x && lhs.y > rhs.y) || (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z > rhs.z);
}

template <typename T>
inline bool operator <=(Point3D<T> const& lhs, Point3D<T> const& rhs)
{
    return !(lhs > rhs);
}

template <typename T>
inline bool operator >=(Point3D<T> const& lhs, Point3D<T> const& rhs)
{
    return !(lhs < rhs);
}
#endif