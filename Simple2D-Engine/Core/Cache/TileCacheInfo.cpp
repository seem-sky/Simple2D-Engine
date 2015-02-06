#include "TileCacheInfo.h"
#include <math_extensions.h>

using namespace CACHE;

TileCacheInfo::TileCacheInfo(const QPixmap* pPixmap, const GEOMETRY::Point<uint32>& pos)
    : m_pPixmap(pPixmap), m_Pos(pos)
{}

const GEOMETRY::Point<uint32>& TileCacheInfo::getPosition() const
{
    return m_Pos;
}

const QPixmap* TileCacheInfo::getPixmap() const
{
    return m_pPixmap;
}

bool TileCacheInfo::isValid() const
{
    return m_pPixmap && m_Pos != GEOMETRY::Point<uint32>(MATH::maximum<uint32>(), MATH::maximum<uint32>());
}
