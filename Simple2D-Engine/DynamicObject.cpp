#include "DynamicObject.h"

using namespace MAP;
using namespace OBJECT;

DynamicObject::DynamicObject(uint32 GUID, DATABASE::ConstDynamicObjectPrototypePtr pDynamicObject) : WorldObject(GUID, pDynamicObject)
{
}
