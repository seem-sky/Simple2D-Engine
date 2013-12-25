#include "Entity.h"

using namespace ENTITY;

Entity::Entity(uint32 GUID, uint32 ID, const std::string& name, const std::string& script) : m_uiGUID(GUID), m_uiID(ID), m_Name(name), m_Script(script)
{}