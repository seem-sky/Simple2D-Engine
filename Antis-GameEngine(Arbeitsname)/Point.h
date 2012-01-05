#ifndef POINT_H
#define POINT_H

template<class T> 
struct Point
{
    T x;
    T y;

    Point(T X = 0, T Y = 0) : x(X), y(Y) { }

    Point operator +(Point p)
    {
        return Point(x + p.x, y + p.y);
    }

    Point operator -(Point p)
    {
        return Point(x - p.x, y - p.y);
    }

    void operator +=(Point p)
    {
        x += p.x;
        y += p.y;
    }

    void operator -=(Point p)
    {
        x -= p.x;
        y -= p.y;
    }
};
#endif;