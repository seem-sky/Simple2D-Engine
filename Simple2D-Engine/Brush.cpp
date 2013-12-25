#include "Brush.h"

using namespace MAP;
using namespace BRUSH;

Brush::Brush(MapLayer& mapLayer) : m_MapLayer(mapLayer)
{
}

void Brush::setBrushInfo(const BrushInfo& brushInfo)
{
    m_BrushInfo = brushInfo;
}
