#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include "Object.h"

namespace MAP
{
    namespace OBJECT
    {
        class WorldObject : public ENTITY::Object
        {
        public:
            WorldObject(uint32 uiID, const std::string &name);
        };
    }
}
#endif