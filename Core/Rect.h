#ifndef RECT_H
#define RECT_H

#include "Point.h"

template<typename T>
class Rect
{
public:
    Rect(Point<T> pos = Point<T>(), Point<T> size = Point<T>()) : m_Pos(pos), m_Size(size) {}

    inline Point<T> getPosition() const { return m_Pos; }
    inline void setPosition(const Point<T> &pos) { m_Pos = pos; }
    inline T getPositionX() const { return m_Pos.x; }
    inline void setPositionX(const T &x) { m_Pos.x = x; }
    inline T getPositionY() const { return m_Pos.y; }
    inline void setPositionY(const T &y) { m_Pos.y = y; }

    inline Point<T> getSize() const { return m_Size; }
    inline void setSize(const Point<T> &size) { m_Size = size; }
    inline T getWidth() const { return m_Size.x; }
    inline void setWidth(const T &width) { m_Size.x = width; }
    inline T getHeight() const { return m_Size.y; }
    inline void setHeight(const T &height) { m_Size.y = height; }

private:
    Point<T> m_Pos;
    Point<T> m_Size;
};
#endif