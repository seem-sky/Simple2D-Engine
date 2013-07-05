#ifndef DYNAMIC_OBJECT_H
#define DYNAMIC_OBJECT_H

#include "WorldObject.h"

namespace MAP
{
    namespace OBJECT
    {
        class DynamicObject : public WorldObject
        {
        public:
            DynamicObject(uint32 uiID, const std::string &name);
        };
        typedef std::shared_ptr<DynamicObject> DynamicObjectPtr;
    }
}
#endif