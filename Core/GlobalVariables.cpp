#include "GlobalVariables.h"

GlobalVariables::GlobalVariables(void) : TSingleton()
{
}

GlobalVariables::~GlobalVariables(void)
{
}

void GlobalVariables::GetBoolNames(std::map<uint32,std::string> &p_lNames) const
{
    for (VariableBoolList::const_iterator t_Itr = m_Bools.begin(); t_Itr != m_Bools.end(); ++t_Itr)
        p_lNames.insert(std::make_pair(t_Itr->first, t_Itr->second.m_sName));
}

void GlobalVariables::GetIntegerNames(std::map<uint32,std::string> &p_lNames) const
{
    for (VariableIntegerList::const_iterator t_Itr = m_Integers.begin(); t_Itr != m_Integers.end(); ++t_Itr)
        p_lNames.insert(std::make_pair(t_Itr->first, t_Itr->second.m_sName));
}

void GlobalVariables::GetFloatNames(std::map<uint32,std::string> &p_lNames) const
{
    for (VariableFloatList::const_iterator t_Itr = m_Floats.begin(); t_Itr != m_Floats.end(); ++t_Itr)
        p_lNames.insert(std::make_pair(t_Itr->first, t_Itr->second.m_sName));
}

void GlobalVariables::GetStringNames(std::map<uint32,std::string> &p_lNames) const
{
    for (VariableStringList::const_iterator t_Itr = m_Strings.begin(); t_Itr != m_Strings.end(); ++t_Itr)
        p_lNames.insert(std::make_pair(t_Itr->first, t_Itr->second.m_sName));
}