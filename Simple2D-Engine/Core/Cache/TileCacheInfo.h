#ifndef CACHE_TILE_CACHE_INFO_H
#define CACHE_TILE_CACHE_INFO_H

#include <geometry/Point.h>
#include <Global.h>
#include <math_extensions.h>

class QPixmap;

namespace CACHE
{
    class TileCacheInfo
    {
    public:
        TileCacheInfo(const QPixmap* pPixmap = nullptr, const GEOMETRY::Point<uint32>& pos = GEOMETRY::Point<uint32>(MATH::maximum<uint32>(), MATH::maximum<uint32>()));

        const GEOMETRY::Point<uint32>& getPosition() const;
        const QPixmap* getPixmap() const;

        bool isValid() const;

    private:
        const QPixmap* m_pPixmap;
        const GEOMETRY::Point<uint32> m_Pos;
    };
}
#endif
