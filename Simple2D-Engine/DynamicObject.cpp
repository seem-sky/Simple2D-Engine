#include "DynamicObject.h"

using namespace MAP;
using namespace OBJECT;

DynamicObject::DynamicObject(DATABASE::ConstDatabaseMgrPtr pDBMgr, uint32 GUID, DATABASE::ConstDynamicObjectPrototypePtr pDynamicObject) :
    WorldObject(pDBMgr, GUID, pDynamicObject)
{
}

void DynamicObject::move(const Int32Point &range, uint32 time)
{
    m_TransformationHolder.addTransformation(new TRANSFORMATION::Move(time, range, m_Position));
}