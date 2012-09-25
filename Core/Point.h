#ifndef POINT_H
#define POINT_H

template<class T> 
struct Point
{
    T x;
    T y;

    Point(T X = 0, T Y = 0) : x(X), y(Y) { }

    inline Point operator +(Point<T> &p)
    {
        return Point<T>(x + p.x, y + p.y);
    }

    inline Point operator -(Point<T> &p)
    {
        return Point<T>(x - p.x, y - p.y);
    }

    inline void operator +=(Point<T> &p)
    {
        x += p.x;
        y += p.y;
    }

    inline void operator -=(Point<T> &p)
    {
        x -= p.x;
        y -= p.y;
    }

    inline bool operator <(const Point<T> &p) const
    {
        if (x < p.x)
            return true;
        if (x == p.x && y < p.y)
            return true;

        return false;
    }

    inline bool operator <=(const Point<T> &p) const
    {
        if (x <= p.x)
            return true;
        if (x > p.x && y <= p.y)
            return true;

        return false;
    }

    inline bool operator >(const Point<T> &p) const
    {
        if (x > p.x)
            return true;
        if (x == p.x && y > p.y)
            return true;

        return false;
    }

    inline bool operator >=(const Point<T> &p) const
    {
        if (x >= p.x)
            return true;
        if (x < p.x && y >= p.y)
            return true;

        return false;
    }

    inline bool operator ==(const Point<T> &p) const
    {
        if (x == p.x && y == p.y)
            return true;

        return false;
    }

    inline bool operator !=(const Point<T> &p) const
    {
        if (x != p.x || y != p.y)
            return true;

        return false;
    }
};
#endif;