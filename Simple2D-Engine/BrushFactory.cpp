#include "BrushFactory.h"
#include "Brush.h"
#include "BrushInfo.h"

#include "BrushAreaSingle.h"
#include "BrushAreaFill.h"

#include "BrushTypeTile.h"
#include "BrushTypeAutoTile.h"
#include "BrushTypeTileSet.h"

namespace MAP
{
    namespace BRUSH
    {
        /*#####
        # BrushFactory
        #####*/
        BrushPtr BrushFactory::createBrush(const BrushInfo& info, const DATABASE::DatabaseMgr& DBMgr, Layer& mapLayer, const UInt32Point& pos)
        {
            // create area
            AREA::AreaPtr pArea;
            switch (info.getMode())
            {
            case BrushInfo::Mode::PEN:
                pArea = AREA::AreaPtr(new AREA::Single(mapLayer, pos));
                break;
            case BrushInfo::Mode::FILL:
                pArea = AREA::AreaPtr(new AREA::Fill(mapLayer, pos));
                break;
            }

            // create type
            TYPE::TypePtr pType;
            switch (info.getType())
            {
            case BrushInfo::Type::TILE:
                pType = TYPE::TypePtr(new TYPE::Tile(info.getID(), DBMgr, mapLayer, pos));
                break;
            case BrushInfo::Type::AUTO_TILE:
                pType = TYPE::TypePtr(new TYPE::AutoTile(info.getID(), DBMgr, mapLayer, pos));
                break;
            case BrushInfo::Type::TILE_SET:
                pType = TYPE::TypePtr(new TYPE::TileSet(info.getID(), DBMgr, mapLayer, pos));
                break;
            }
            
            return BrushPtr(new Brush(pArea.release(), pType.release()));
        }
    }
}
