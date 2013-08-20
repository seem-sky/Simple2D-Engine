#include "Object.h"
#include "Map.h"

using namespace MAP;
using namespace OBJECT;

Object::Object(uint32 GUID, uint32 ID, const std::string &name, const std::string &script) : Entity(GUID, ID, name, script)
{}

void Object::update(uint32 uiDiff)
{
    m_TransformationHolder.updateTransformations(uiDiff);
}

uint32 Object::getMapGUID() const
{
    if (!m_pMap)
        return 0;
    return m_pMap->getGUID();
}