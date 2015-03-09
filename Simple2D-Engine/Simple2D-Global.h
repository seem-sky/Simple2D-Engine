#ifndef SIMPLE_2D_GLOBAL_H
#define SIMPLE_2D_GLOBAL_H

#include <Global.h>

namespace DATABASE
{
    namespace PROTOTYPE
    {
        // index typedefs
        typedef uint16 TILE_INDEX;
        typedef uint8 AUTO_TILE_INDEX;

        namespace AUTO_TILE
        {
            const uint32 AUTO_TILE_SET_COUNT = 10;

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

            enum AutoTileIndex
            {
                // auto tile set
                INDEX_TOP_LEFT,
                INDEX_TOP,
                INDEX_TOP_RIGHT,
                INDEX_LEFT,
                INDEX_CENTER,
                INDEX_RIGHT,
                INDEX_BOTTOM_LEFT,
                INDEX_BOTTOM,
                INDEX_BOTTOM_RIGHT,
                INDEX_INNER_CENTER,                     // 9

                // side ends
                INDEX_SIDE_END_TOP,                     // 10
                INDEX_SIDE_END_BOTTOM,
                INDEX_SIDE_END_LEFT,
                INDEX_SIDE_END_RIGHT,                   // 13

                // double sides
                INDEX_SIDE_VERTICAL,
                INDEX_SIDE_HORIZONTAL,                  // 15

                // T tiles
                INDEX_T_TOP,
                INDEX_T_BOTTOM,
                INDEX_T_LEFT,
                INDEX_T_RIGHT,                          // 19

                // curves
                INDEX_CURVE_TOP_LEFT,
                INDEX_CURVE_TOP_RIGHT,
                INDEX_CURVE_BOTTOM_LEFT,
                INDEX_CURVE_BOTTOM_RIGHT,               // 23

                // inner edges
                INDEX_INNER_EDGE_TOP_LEFT,
                INDEX_INNER_EDGE_TOP_RIGHT,
                INDEX_INNER_EDGE_BOTTOM_LEFT,
                INDEX_INNER_EDGE_BOTTOM_RIGHT,          // 27

                // diagonal inner edges
                INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT,
                INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT, // 29

                // multi inner edges
                INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT,
                INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT,
                INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT,
                INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT,// 33

                // circle
                INDEX_CIRCLE,                           // 34

                // Y tiles
                INDEX_Y_TOP_BOTTOM_LEFT,
                INDEX_Y_TOP_BOTTOM_RIGHT,
                INDEX_Y_LEFT_TOP_RIGHT,
                INDEX_Y_LEFT_BOTTOM_RIGHT,              // 38
                INDEX_Y_BOTTOM_TOP_LEFT,
                INDEX_Y_BOTTOM_TOP_RIGHT,
                INDEX_Y_RIGHT_TOP_LEFT,
                INDEX_Y_RIGHT_BOTTOM_LEFT,              // 42

                INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT,
                INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT,
                INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT,
                INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT,    // 46

                INDEX_NONE
            };

            enum AUTO_TILE_TILE_REQUIREMENTS
            {
                CENTER = SAME_AROUND,

                SIDE_LEFT = OTHER_LEFT,
                SIDE_RIGHT = OTHER_RIGHT,
                SIDE_TOP = OTHER_TOP,
                SIDE_BOTTOM = OTHER_BOTTOM,

                SIDE_VERTICAL = SIDE_LEFT | SIDE_RIGHT,
                SIDE_HORIZONTAL = SIDE_TOP | SIDE_BOTTOM,

                EDGE_TOP_LEFT = SIDE_TOP | SIDE_LEFT,
                EDGE_TOP_RIGHT = SIDE_TOP | SIDE_RIGHT,
                EDGE_BOTTOM_LEFT = SIDE_BOTTOM | SIDE_LEFT,
                EDGE_BOTTOM_RIGHT = SIDE_BOTTOM | SIDE_RIGHT,

                LEFT_END = EDGE_TOP_LEFT | EDGE_BOTTOM_LEFT,
                RIGHT_END = EDGE_TOP_RIGHT | EDGE_BOTTOM_RIGHT,
                TOP_END = EDGE_TOP_LEFT | EDGE_TOP_RIGHT,
                BOTTOM_END = EDGE_BOTTOM_LEFT | EDGE_BOTTOM_RIGHT,

                CIRCLE = EDGE_TOP_LEFT | EDGE_BOTTOM_RIGHT,

                INNER_EDGE_TOP_LEFT = OTHER_TOP_LEFT,
                INNER_EDGE_TOP_RIGHT = OTHER_TOP_RIGHT,
                INNER_EDGE_BOTTOM_LEFT = OTHER_BOTTOM_LEFT,
                INNER_EDGE_BOTTOM_RIGHT = OTHER_BOTTOM_RIGHT,

                INNER_EDGE_TOP_LEFT_TOP_RIGHT = INNER_EDGE_TOP_LEFT | INNER_EDGE_TOP_RIGHT,
                INNER_EDGE_TOP_LEFT_BOTTOM_LEFT = INNER_EDGE_TOP_LEFT | INNER_EDGE_BOTTOM_LEFT,
                INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT = INNER_EDGE_TOP_RIGHT | INNER_EDGE_BOTTOM_RIGHT,
                INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT = INNER_EDGE_BOTTOM_LEFT | INNER_EDGE_BOTTOM_RIGHT,

                INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT = INNER_EDGE_TOP_LEFT | INNER_EDGE_BOTTOM_RIGHT,
                INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT = INNER_EDGE_TOP_RIGHT | INNER_EDGE_BOTTOM_LEFT,

                INNER_EDGE_CENTER = INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT | INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT,

                CURVE_TOP_LEFT = EDGE_TOP_LEFT | INNER_EDGE_BOTTOM_RIGHT,
                CURVE_TOP_RIGHT = EDGE_TOP_RIGHT | INNER_EDGE_BOTTOM_LEFT,
                CURVE_BOTTOM_LEFT = EDGE_BOTTOM_LEFT | INNER_EDGE_TOP_RIGHT,
                CURVE_BOTTOM_RIGHT = EDGE_BOTTOM_RIGHT | INNER_EDGE_TOP_LEFT,

                T_TOP = SIDE_TOP | INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT,
                T_BOTTOM = SIDE_BOTTOM | INNER_EDGE_TOP_LEFT_TOP_RIGHT,
                T_LEFT = SIDE_LEFT | INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT,
                T_RIGHT = SIDE_RIGHT | INNER_EDGE_TOP_LEFT_BOTTOM_LEFT,

                Y_TOP_BOTTOM_LEFT = SIDE_TOP | INNER_EDGE_BOTTOM_LEFT,
                Y_TOP_BOTTOM_RIGHT = SIDE_TOP | INNER_EDGE_BOTTOM_RIGHT,
                Y_LEFT_TOP_RIGHT = SIDE_LEFT | INNER_EDGE_TOP_RIGHT,
                Y_LEFT_BOTTOM_RIGHT = SIDE_LEFT | INNER_EDGE_BOTTOM_RIGHT,
                Y_BOTTOM_TOP_LEFT = SIDE_BOTTOM | INNER_EDGE_TOP_LEFT,
                Y_BOTTOM_TOP_RIGHT = SIDE_BOTTOM | INNER_EDGE_TOP_RIGHT,
                Y_RIGHT_TOP_LEFT = SIDE_RIGHT | INNER_EDGE_TOP_LEFT,
                Y_RIGHT_BOTTOM_LEFT = SIDE_RIGHT | INNER_EDGE_BOTTOM_LEFT,

                INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT = INNER_EDGE_TOP_LEFT_TOP_RIGHT | INNER_EDGE_BOTTOM_LEFT,
                INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT = INNER_EDGE_TOP_LEFT_TOP_RIGHT | INNER_EDGE_BOTTOM_RIGHT,
                INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT = INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT | INNER_EDGE_TOP_LEFT,
                INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT = INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT | INNER_EDGE_TOP_RIGHT
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

    typedef uint32 GUID;
    const uint32 TILE_SIZE = 32;

    namespace SCRIPT_AREA
    {
        const int32 MINIMUM_SIZE = TILE_SIZE / 2;
    }
}
#endif
