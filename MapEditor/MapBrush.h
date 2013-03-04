#ifndef MAP_BRUSH
#define MAP_BRUSH

#include "Global.h"
#include "MapView.h"
#include <boost/dynamic_bitset.hpp>

namespace MAP
{
    class MapBrush
    {
    public:
        enum BrushType
        {
            BRUSH_PEN,
            BRUSH_RECT,
            BRUSH_FILL,
        };

        enum MouseKey
        {
            MOUSE_LEFT = 1,
            MOUSE_RIGHT = 2,
        };

    private:
        void _drawPen(MapViewWidget *pWidget, const Point3D<uint32> &center, MouseKey uiButton);
        void _drawFill(MapViewWidget *pWidget, const Point3D<uint32> &center, MouseKey uiButton);
        void _calculateFillArea(MapViewWidget *pWidget, const Point3D<uint32> &center, MouseKey uiButton);
        bool _checkFill(MapPrototypePtr const &map, uint32 const &uiLookUpTileID, uint32 const &uiNewTileID, Point3D<uint32> const &pos,
            std::vector<boost::dynamic_bitset<>> &mapBitset, UInt32PointVector &openPoints);

    public:
        MapBrush();
        inline void setBrushType(BrushType type, MouseKey uiButton) { m_Type[uiButton-1] = type; }
        inline BrushType getBrushType(MouseKey uiButton) const { return m_Type[uiButton-1]; }
        inline void setTile(uint32 uiID, MouseKey uiButton) { m_uiTileID[uiButton-1] = uiID; }
        inline uint32 getTile(MouseKey uiButton) const { return m_uiTileID[uiButton-1]; }

        void drawOnMap(MapViewWidget *pWidget, Point3D<uint32> center, MouseKey uiButton);

        void brushPress(MapViewWidget *pWidget, Point3D<uint32> center, MouseKey uiButton);
        void brushRelease(MapViewWidget *pWidget, Point3D<uint32> center, MouseKey uiButton);
        void brushMove(MapViewWidget *pWidget, Point3D<uint32> center);
        
    private:
        bool m_buttonHold[2];
        MapViewWidget *m_pLastEmitter[2];
        BrushType m_Type[2];
        uint32 m_uiTileID[2];
    };
}
#endif