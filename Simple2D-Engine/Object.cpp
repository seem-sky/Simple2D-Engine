#include "Object.h"

using namespace MAP;
using namespace OBJECT;

Object::Object(uint32 GUID, uint32 ID, const std::string &name, const std::string &script) : Entity(GUID, ID, name, script)
{}

void Object::move(const Int32Point &range, uint32 time)
{
    m_TransformationHolder.addTransformation(new TRANSFORMATION::Move(time, range, m_Position));
}

void Object::update(uint32 uiDiff)
{
    m_TransformationHolder.updateTransformations(uiDiff);
}