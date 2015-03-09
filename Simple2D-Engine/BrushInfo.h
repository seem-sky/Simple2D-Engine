#ifndef BRUSH_INFO_H
#define BRUSH_INFO_H

#include <Global.h>

namespace MAP
{
    namespace BRUSH
    {
        class BrushInfo
        {
        public:
            enum class Type
            {
                TILE,
                AUTO_TILE,
                TILE_SET
            };

            enum class Mode
            {
                PEN,
                FILL
            };

            BrushInfo(uint32 ID = 0, Type type = Type::TILE, Mode mode = Mode::PEN) : m_Type(type), m_Mode(mode), m_ID(ID) 
            {}

            inline Type getType() const { return m_Type; }
            inline void setType(Type type) { m_Type = type; }
            inline Mode getMode() const { return m_Mode; }
            inline void setMode(Mode mode) { m_Mode = mode; }
            inline uint32 getID() const { return m_ID; }
            inline void setID(uint32 ID) { m_ID = ID; }

        private:
            Type m_Type = Type::TILE;
            Mode m_Mode = Mode::PEN;
            uint32 m_ID = 0;
        };
    }
}
#endif
