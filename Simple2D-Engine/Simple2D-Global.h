#ifndef SIMPLE_2D_GLOBAL_H
#define SIMPLE_2D_GLOBAL_H

#include <Typedefs.h>

namespace DATABASE
{
    namespace PROTOTYPE
    {
        // index typedefs
        typedef uint16 TILE_INDEX;
        typedef uint8 AUTO_TILE_INDEX;

        namespace AUTO_TILE
        {
            enum TILE_CHECK
            {
                SAME_AROUND = 0x00,
                OTHER_TOP_LEFT = 0x01,
                OTHER_TOP = 0x02,
                OTHER_TOP_RIGHT = 0x04,
                OTHER_LEFT = 0x08,
                OTHER_RIGHT = 0x10,
                OTHER_BOTTOM_LEFT = 0x20,
                OTHER_BOTTOM = 0x40,
                OTHER_BOTTOM_RIGHT = 0x80,
            };
        }
    }
}

namespace MAP
{
    enum class LayerType
    {
        LAYER_BACKGROUND,
        LAYER_FOREGROUND
    };
}
#endif
