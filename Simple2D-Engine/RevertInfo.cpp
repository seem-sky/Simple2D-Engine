#include "RevertInfo.h"

using namespace MAP::BRUSH;
using namespace REVERT;

void RevertInfo::revert(MAP::MapLayer &mapLayer)
{
    for (auto& pair : m_Tiles)
    {
        try
        {
            mapLayer.setMapTile(UInt32Point3D(pair.first, m_LayerIndex), m_LayerType, pair.second);
        }
        catch (std::exception& e)
        {
        }
    }
}
