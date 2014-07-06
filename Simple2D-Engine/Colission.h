#ifndef COLISSION_H
#define COLISSION_H

#include <Global.h>

template <class T>
class Colission
{
public:
    Colission(const Rect<T>& from, const Rect<T>& to) : m_From(from), m_To(to)
    {}

    bool checkColissionWith(const Rect<T>& other, Point<T>& pos) const
    {
        // setup movement rect
        QRect rect;
        if (m_From.x < m_To.x)
        {
            rect.setX(x() + getWorldObjectBoundingRect().getPositionX());
            rect.setWidth(moveTo.x() + getWorldObjectBoundingRect().getPositionX());
        }
        else
        {
            rect.setWidth(x() + getWorldObjectBoundingRect().getPositionX());
            rect.setX(moveTo.x() + getWorldObjectBoundingRect().getPositionX());
        }

        if (m_From.y < m_To.y)
        {
            rect.setY(x() + getWorldObjectBoundingRect().getPositionY());
            rect.setHeight(moveTo.x() + getWorldObjectBoundingRect().getPositionY());
        }
        else
        {
            rect.setHeight(x() + getWorldObjectBoundingRect().getPositionY());
            rect.setY(moveTo.x() + getWorldObjectBoundingRect().getPositionY());
        }

        //if (_contains(rect, other))
        //    return false;
    }

private:
    Rect<T> m_From, m_To;
};
#endif