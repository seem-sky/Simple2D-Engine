#include "ScriptAreaData.h"

using namespace MAP::SCRIPT_AREA;

bool ScriptAreaData::isEmpty() const
{
    return m_pScriptAreas.empty();
}

uint32 ScriptAreaData::count() const
{
    return static_cast<uint32>(m_pScriptAreas.size());
}

ScriptArea* ScriptAreaData::addScriptArea(ScriptArea* pScript)
{
    if (!pScript)
        return nullptr;

    if (hasScriptArea(pScript->getGUID()))
        throw std::runtime_error("GUID already exists.");

    m_pScriptAreas.push_back(std::unique_ptr<ScriptArea>(pScript));
    return m_pScriptAreas.back().get();
}

ScriptArea* ScriptAreaData::addScriptArea(GEOMETRY::ComplexGeometricShape<int32>* pArea)
{
    if (!pArea)
        return nullptr;

    m_pScriptAreas.push_back(std::unique_ptr<ScriptArea>(new ScriptArea(_getNewGUID())));
    m_pScriptAreas.back()->setArea(pArea);
    return m_pScriptAreas.back().get();
}

const ScriptArea* ScriptAreaData::getScriptArea(MAP::GUID guid) const
{
    for (auto& pScript : m_pScriptAreas)
    {
        if (pScript->getGUID() == guid)
            return pScript.get();
    }
    return nullptr;
}

ScriptArea* ScriptAreaData::getScriptArea(MAP::GUID guid)
{
    return const_cast<ScriptArea*>(const_cast<const ScriptAreaData&>(*this).getScriptArea(guid));
}

const std::vector<std::unique_ptr<ScriptArea>>& ScriptAreaData::getScriptAreas() const
{
    return m_pScriptAreas;
}

bool ScriptAreaData::hasScriptArea(MAP::GUID guid) const
{
    for (auto& pScript : m_pScriptAreas)
    {
        if (pScript->getGUID() == guid)
            return true;
    }
    return false;
}

void ScriptAreaData::removeScriptArea(MAP::GUID guid)
{
    for (auto itr = m_pScriptAreas.begin(); itr != m_pScriptAreas.end(); ++itr)
    {
        if ((*itr)->getGUID() == guid)
        {
            m_pScriptAreas.erase(itr);
            break;
        }
    }
}

MAP::GUID ScriptAreaData::_getNewGUID() const
{
    if (isEmpty())
        return 1;
    return m_pScriptAreas.back()->getGUID() + 1;
}
