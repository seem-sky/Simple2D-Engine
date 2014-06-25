#include "BrushFactory.h"
#include "Brush.h"

#include "BrushAreaSingle.h"

#include "BrushTypeTile.h"

namespace MAP
{
    namespace BRUSH
    {
        /*#####
        # BrushInfo
        #####*/
        BrushInfo::BrushInfo(uint32 ID, Type type, Mode mode) : m_Type(type), m_Mode(mode), m_ID(ID)
        {}

        /*#####
        # BrushFactory
        #####*/
        Brush2Ptr BrushFactory::createBrush(const BrushInfo& info, Layer& mapLayer, const UInt32Point& pos)
        {
            // create area
            AREA::AreaPtr pArea;
            switch (info.getMode())
            {
            case BrushInfo::Mode::PEN:
                pArea = AREA::AreaPtr(new AREA::Single(mapLayer, pos));
                break;
            case BrushInfo::Mode::FILL:
                break;
            }

            // create type
            TYPE::TypePtr pType;
            switch (info.getType())
            {
            case BrushInfo::Type::TILE:
                pType = TYPE::TypePtr(new TYPE::Tile(info.getID(), mapLayer, pos));
                break;
            case BrushInfo::Type::AUTO_TILE:
                break;
            case BrushInfo::Type::TILE_SET:
                break;
            }
            
            return Brush2Ptr(new Brush(pArea.release(), pType.release()));
        }
    }
}
