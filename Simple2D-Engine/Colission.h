#ifndef COLISSION_H
#define COLISSION_H

#include <geometry/Rectangle.h>
#include <geometry/Polygon.h>
#include <geometry/algorithm.h>
#include <Typedefs.h>

namespace COLISSION
{
    template <typename T1>
    class Interface
    {
    private:
        virtual void _calculatePolygon() = 0;

    public:
        Interface()
        {
            _calculatePolygon();
        }

        virtual void checkColission(const GEOMETRY::Rectangle<T>& rect) const = 0;
    };

    template <typename T1, typename T2>
    class Vertical : public Interface
    {
    private:
        void _calculatePolygon()
        {
            m_ColissionRect.setLeft(m_From.getLeft());
            m_ColissionRect.setRight(m_From.getRight());
            if (m_Move > 0)
            {
                m_ColissionRect.setTop(m_From.getTop());
                m_ColissionRect.setBottom(m_From.getBottom() + m_Move);
            }
            else
            {
                m_ColissionRect.setTop(m_From.getTop() + m_Move);
                m_ColissionRect.setBottom(m_From.getBottom());
            }
        }

    public:
        Vertical(const GEOMETRY::Rectangle<T1>& rect, T2 move) : m_From(rect), m_Move(move), Interface();
        {
        }

        void checkColission(const GEOMETRY::Rectangle<T>& rect) const
        {
            if (GEOMETRY::contains(m_ColissionRect, rect))
            {
                if (m_Move > 0) // move right
                {
                    m_ColissionRect.setRight(rect.getLeft() - 1);
                    m_Move = m_ColissionRect.getSize() - m_From.getSize();
                }
                else
                {
                    m_ColissionRect.setLeft(rect.getRight() + 1);
                    m_Move = -(m_ColissionRect.getSize() - m_From.getSize());
                }
            }
            else
            {
                auto inters = GEOMETRY::intersections(m_ColissionRect, rect);
                if (m_Move > 0)
                {
                    m_ColissionRect.setRight(std::min(inters.begin(), inters.end(), [](const GEOMETRY::Point<double>& p1, const GEOMETRY::Point<double>& p2){
                        return p1.getY() < p2.getY();
                    }) -1);
                    m_Move = m_ColissionRect.getSize() - m_From.getSize();
                }
                else
                {
                    m_ColissionRect.setRight(std::max(inters.begin(), inters.end(), [](const GEOMETRY::Point<double>& p1, const GEOMETRY::Point<double>& p2){
                        return p1.getY() < p2.getY();
                    })+ 1);
                    m_Move = -(m_ColissionRect.getSize() - m_From.getSize());
                }
            }
        }

    private:
        GEOMETRY::Rectangle<T1> m_From;
        GEOMETRY::Rectangle<T1> m_ColissionRect;
        T2 m_Move;
    };
    /*template <typename T1, typename T2>
    class ColissionDiagonal
    {
    private:
    PointVec<T> _checkRect(T1 xMin, T1 xMax, const GEOMETRY::Rectangle<T1>& rect) const
    {
    PointVec<T> result;
    if (_checkPoint(xMin, xMax, rect.getTopLeft()))
    result.push_back(rect.getTopLeft());
    if (_checkPoint(xMin, xMax, rect.getBottomLeft()))
    result.push_back(rect.getBottomLeft());
    if (_checkPoint(xMin, xMax, rect.getBottomRight()))
    result.push_back(rect.getBottomRight());
    if (_checkPoint(xMin, xMax, rect.getTopRight()))
    result.push_back(rect.getTopRight());
    return result;
    }

    bool _checkPoint(T1 xMin, T1 xMax, const Point<T1>& p) const
    {
    return p.getX() > xMin && p.getX() < xMax;
    }

    void _calculatePolygon()
    {
    GEOMETRY::Rectangle<T1> destinationRect(GEOMETRY::Point<T1>(m_From.getLeft() + m_Move.getX(), m_From.getTop() + m_Move.getY()),
    GEOMETRY::Point<T1>(m_From.getRight() + m_Move.getX(), m_From.getBottom() + m_Move.getY()));
    auto& topRect = m_From.getTop() <= destinationRect.getTop() ? m_From : destinationRect;
    auto& bottomRect = m_From.getBottom() >= destinationRect.getBottom() ? m_From : destinationRect;
    T1 xMin(std::min(m_From.getLeft(), destinationRect.getLeft()));
    T1 xMax(std::max(m_From.getRight(), destinationRect.getRight()));
    auto polyVec = _checkRect(xMin, xMax, bottomRect);
    auto vec2 = _checkRect(xMin, xMax, topRect);
    polyVec.insert(polyVec.end(), vec2.begin(), vec2.end());
    m_Polygon.insertPoints(0, polyVec);
    }

    public:
    Colission(const GEOMETRY::Rectangle<T1>& rect, const GEOMETRY::Point<T2>& move) : m_From(rect), m_Move(move)
    {
    _calculatePolygon(rect, move);
    }

    GEOMETRY::Point<T> checkColission(const GEOMETRY::Rectangle<T>& rect) const
    {
    if (GEOMETRY::contains(m_Polygon, rect))
    {

    }
    else if (GEOMETRY::intersections(m_Polygon, rect))
    {

    }
    return m_Move;
    }

    private:
    GEOMETRY::Rectangle<T> m_From;
    GEOMETRY::Point<T> m_Move;
    GEOMETRY::Polygon<T> m_Polygon;
    };*/
}
#endif