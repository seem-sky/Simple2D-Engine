#include "BrushArea.h"
#include "MapException.h"

using namespace MAP;
using namespace BRUSH;
using namespace EXCEPTION;

/*#####
# BrushArea
#####*/
BrushArea::BrushArea(const Layer &layer, const UInt32Point& center) : m_MapLayer(layer), m_Current(center)
{
}

UInt32Point BrushArea::next()
{
    throw TileException("There are no more tiles");
}

/*#####
# BrushAreaFill
#####*/
BrushAreaFill::BrushAreaFill(const MapTile& destTile, const Layer &layer, const UInt32Point& center) : BrushArea(layer, center), m_DestTile(destTile)
{
    m_BitCheck.set(current());
    m_OpenPoints.push_back(current());
}

UInt32Point BrushAreaFill::next()
{
    if (m_OpenPoints.empty())
        throw TileException("There are no more tiles");

    m_Current = m_OpenPoints.front();
    m_OpenPoints.pop_front();

    // store positions around
    for (uint32 i = 0; i < 4; ++i)
    {
        // set position check
        UInt32Point3D checkPos = m_Current;
        switch (i)
        {
        case 0: // top
            if (!checkPos.y)
                continue;
            --checkPos.y;
            break;

        case 1: // left
            if (!checkPos.x)
                continue;
            --checkPos.x;
            break;

        case 2: // right
            if (checkPos.x+1 >= m_MapLayer.getSize().x)
                continue;
            ++checkPos.x;
            break;

        case 3: // bottom
            if (checkPos.y+1 >= m_MapLayer.getSize().y)
                continue;
            ++checkPos.y;
            break;
        }

        if (m_BitCheck.get(checkPos))
            continue;

        m_BitCheck.set(checkPos);
        const auto& checkTile = m_MapLayer.getMapTile(checkPos);
        if (m_DestTile == checkTile.getMapTile())
            m_OpenPoints.push_back(checkPos);
    }
    return current();
}