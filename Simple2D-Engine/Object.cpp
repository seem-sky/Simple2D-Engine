#include "Object.h"

using namespace ENTITY;

Object::Object(uint32 uiID, const std::string &name) : m_uiID(uiID), m_Name(name)
{}

void Object::move(const Int32Point &range, uint32 time)
{
    m_TransformationHolder.addTransformation(TRANSFORMATION::MovePtr(new TRANSFORMATION::Move(time, this, range)));
}

void Object::update(uint32 uiDiff)
{
    m_TransformationHolder.updateTransformations(uiDiff);
}