#ifndef RECT_H
#define RECT_H

#include "Point.h"

template<typename T1, typename T2>
class Rect
{
public:
    Rect(Point<T1> pos = Point<T1>(), Point<T2> size = Point<T2>()) : m_Pos(pos), m_Size(size) {}

    inline Point<T1> getPosition() const { return m_Pos; }
    inline Point<T1>& getPosition() { return m_Pos; }
    inline void setPosition(const Point<T1> &pos) { m_Pos = pos; }
    inline T1 getPositionX() const { return m_Pos.x; }
    inline void setPositionX(const T1 &x) { m_Pos.x = x; }
    inline T1 getPositionY() const { return m_Pos.y; }
    inline void setPositionY(const T1 &y) { m_Pos.y = y; }

    inline Point<T2> getSize() const { return m_Size; }
    inline Point<T2>& getSize() { return m_Size; }
    inline void setSize(const Point<T2> &size) { m_Size = size; }
    inline T2 getWidth() const { return m_Size.x; }
    inline void setWidth(const T2 &width) { m_Size.x = width; }
    inline T2 getHeight() const { return m_Size.y; }
    inline void setHeight(const T2 &height) { m_Size.y = height; }

private:
    Point<T1> m_Pos;
    Point<T2> m_Size;
};
#endif