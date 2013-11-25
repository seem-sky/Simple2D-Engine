#include "DynamicObject.h"

using namespace MAP;
using namespace OBJECT;

DynamicObject::DynamicObject(const DATABASE::DatabaseMgr &pDBMgr, uint32 GUID, uint32 ID) : WorldObject(pDBMgr, GUID, ID)
{
}

void DynamicObject::move(const Int32Point &range, uint32 time)
{
    m_TransformationHolder.addTransformation(new TRANSFORMATION::Move(time, range, m_Position));
}