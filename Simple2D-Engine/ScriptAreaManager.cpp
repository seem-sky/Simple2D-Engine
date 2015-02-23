#include "Map/ScriptArea/Manager.h"

using namespace MAP::SCRIPT_AREA;

bool Manager::isEmpty() const
{
    return m_pScriptAreas.empty();
}

uint32 Manager::count() const
{
    return static_cast<uint32>(m_pScriptAreas.size());
}

ScriptArea* Manager::addScriptArea(ScriptArea* pScript)
{
    if (!pScript || pScript->isEmpty())
        return nullptr;

    if (hasScriptArea(pScript->getGUID()))
        throw std::runtime_error("GUID already exists.");

    m_pScriptAreas.push_back(std::unique_ptr<ScriptArea>(pScript));
    return m_pScriptAreas.back().get();
}

ScriptArea* Manager::addScriptArea(const Data& data)
{
    if (!data.isValid())
        return nullptr;

    if (hasScriptArea(data.getGUID()))
        throw std::runtime_error("GUID already exists.");

    m_pScriptAreas.push_back(std::unique_ptr<ScriptArea>(new ScriptArea(data.getGUID())));
    m_pScriptAreas.back()->setArea(data.getAreaData());
    m_pScriptAreas.back()->setAction(data.getActionData());
    return m_pScriptAreas.back().get();
}

ScriptArea* Manager::addScriptArea(const AREA::Data& area, const ACTION::Data& action)
{
    return addScriptArea(Data(_getNewGUID(), area, action));
}

const ScriptArea* Manager::getScriptArea(MAP::GUID guid) const
{
    for (auto& pScript : m_pScriptAreas)
    {
        if (pScript->getGUID() == guid)
            return pScript.get();
    }
    return nullptr;
}

ScriptArea* Manager::getScriptArea(MAP::GUID guid)
{
    return const_cast<ScriptArea*>(const_cast<const Manager&>(*this).getScriptArea(guid));
}

const std::vector<std::unique_ptr<ScriptArea>>& Manager::getScriptAreas() const
{
    return m_pScriptAreas;
}

ScriptArea* Manager::takeScriptArea(MAP::GUID guid)
{
    for (auto itr = m_pScriptAreas.begin(); itr != m_pScriptAreas.end(); ++itr)
    {
        if ((*itr)->getGUID() == guid)
        {
            auto pScript = std::move(*itr);
            m_pScriptAreas.erase(itr);
            return pScript.release();
        }
    }
    return nullptr;
}

bool Manager::hasScriptArea(MAP::GUID guid) const
{
    for (auto& pScript : m_pScriptAreas)
    {
        if (pScript->getGUID() == guid)
            return true;
    }
    return false;
}

void Manager::removeScriptArea(MAP::GUID guid)
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

MAP::GUID Manager::_getNewGUID() const
{
    if (isEmpty())
        return 1;
    return m_pScriptAreas.back()->getGUID() + 1;
}
