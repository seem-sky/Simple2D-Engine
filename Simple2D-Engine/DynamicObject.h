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
            DynamicObject(const DATABASE::DatabaseMgr &pDBMgr, uint32 GUID, uint32 ID);

            void move(const Int32Point &range, uint32 time);
        };
    }
}
#endif