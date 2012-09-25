#include "GlobalVariableOutput.h"

using namespace XML;

GlobalVariableOutput::GlobalVariableOutput(void) : TSingleton()
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "GlobalVariableOutput: ";
}

GlobalVariableOutput::~GlobalVariableOutput(void)
{
}

void GlobalVariableOutput::SetVariable(TVariable<bool> p_Var, bool p_bDelete)
{
    uint32 t_uiID = p_Var.m_uiID;
    if (p_bDelete)
        p_Var.m_uiID = 0;
    VariableBoolList::iterator t_Itr = m_Bools.find(t_uiID);
    if (t_Itr == m_Bools.end())
        m_Bools.insert(std::make_pair(t_uiID, p_Var));
    else
        t_Itr->second = p_Var;
}

void GlobalVariableOutput::SetVariable(TVariable<int> p_Var, bool p_bDelete)
{
    uint32 t_uiID = p_Var.m_uiID;
    if (p_bDelete)
        p_Var.m_uiID = 0;
    VariableIntegerList::iterator t_Itr = m_Integers.find(t_uiID);
    if (t_Itr == m_Integers.end())
        m_Integers.insert(std::make_pair(t_uiID, p_Var));
    else
        t_Itr->second = p_Var;
}

void GlobalVariableOutput::SetVariable(TVariable<float> p_Var, bool p_bDelete)
{
    uint32 t_uiID = p_Var.m_uiID;
    if (p_bDelete)
        p_Var.m_uiID = 0;
    VariableFloatList::iterator t_Itr = m_Floats.find(t_uiID);
    if (t_Itr == m_Floats.end())
        m_Floats.insert(std::make_pair(t_uiID, p_Var));
    else
        t_Itr->second = p_Var;
}

void GlobalVariableOutput::SetVariable(TVariable<std::string> p_Var, bool p_bDelete)
{
    uint32 t_uiID = p_Var.m_uiID;
    if (p_bDelete)
        p_Var.m_uiID = 0;
    VariableStringList::iterator t_Itr = m_Strings.find(t_uiID);
    if (t_Itr == m_Strings.end())
        m_Strings.insert(std::make_pair(t_uiID, p_Var));
    else
        t_Itr->second = p_Var;
}

XML_WriteData GlobalVariableOutput::ParseVariable(uint32 t_uiID, TVariable<bool> p_Var)
{
    XML_WriteData t_Data;
    t_Data.AddAttribute("ID", p_Var.m_uiID);
    if (p_Var.m_uiID != 0)
    {
        t_Data.AddAttribute("name", (LPCOLESTR)_bstr_t(p_Var.m_sName.c_str()));
        t_Data.AddAttribute("value", p_Var.m_Value);
        t_Data.SetWriteState(XML_WRITE_ADD);
    }
    else
        t_Data.SetWriteState(XML_WRITE_DELETE);

    return t_Data;
}

XML_WriteData GlobalVariableOutput::ParseVariable(uint32 t_uiID, TVariable<int> p_Var)
{
    XML_WriteData t_Data;
    t_Data.AddAttribute("ID", p_Var.m_uiID);
    if (p_Var.m_uiID != 0)
    {
        t_Data.AddAttribute("name", (LPCOLESTR)_bstr_t(p_Var.m_sName.c_str()));
        t_Data.AddAttribute("value", p_Var.m_Value);
        t_Data.SetWriteState(XML_WRITE_ADD);
    }
    else
        t_Data.SetWriteState(XML_WRITE_DELETE);

    return t_Data;
}

XML_WriteData GlobalVariableOutput::ParseVariable(uint32 t_uiID, TVariable<float> p_Var)
{
    XML_WriteData t_Data;
    t_Data.AddAttribute("ID", p_Var.m_uiID);
    if (p_Var.m_uiID != 0)
    {
        t_Data.AddAttribute("name", (LPCOLESTR)_bstr_t(p_Var.m_sName.c_str()));
        t_Data.AddAttribute("value", p_Var.m_Value);
        t_Data.SetWriteState(XML_WRITE_ADD);
    }
    else
        t_Data.SetWriteState(XML_WRITE_DELETE);

    return t_Data;
}

XML_WriteData GlobalVariableOutput::ParseVariable(uint32 t_uiID, TVariable<std::string> p_Var)
{
    XML_WriteData t_Data;
    t_Data.AddAttribute("ID", p_Var.m_uiID);
    if (p_Var.m_uiID != 0)
    {
        t_Data.AddAttribute("name", (LPCOLESTR)_bstr_t(p_Var.m_sName.c_str()));
        t_Data.AddAttribute("value", (LPCOLESTR)_bstr_t(p_Var.m_Value.c_str()));
        t_Data.SetWriteState(XML_WRITE_ADD);
    }
    else
        t_Data.SetWriteState(XML_WRITE_DELETE);

    return t_Data;
}

XML_WriteData GlobalVariableOutput::GetXMLData() const
{
    XML_WriteData t_GlobalVariables(XML_WRITE_ADD);
    XML_WriteData t_Bools(XML_WRITE_ADD);
    for (VariableBoolList::const_iterator t_Itr = m_Bools.begin(); t_Itr != m_Bools.end(); ++t_Itr)
        t_Bools.AddChild("Variable", ParseVariable(t_Itr->first, t_Itr->second));

    XML_WriteData t_Integers(XML_WRITE_ADD);
    for (VariableIntegerList::const_iterator t_Itr = m_Integers.begin(); t_Itr != m_Integers.end(); ++t_Itr)
        t_Integers.AddChild("Variable", ParseVariable(t_Itr->first, t_Itr->second));

    XML_WriteData t_Floats(XML_WRITE_ADD);
    for (VariableFloatList::const_iterator t_Itr = m_Floats.begin(); t_Itr != m_Floats.end(); ++t_Itr)
        t_Floats.AddChild("Variable", ParseVariable(t_Itr->first, t_Itr->second));

    XML_WriteData t_Strings(XML_WRITE_ADD);
    for (VariableStringList::const_iterator t_Itr = m_Strings.begin(); t_Itr != m_Strings.end(); ++t_Itr)
        t_Strings.AddChild("Variable", ParseVariable(t_Itr->first, t_Itr->second));

    t_GlobalVariables.AddChild("GlobalBools", t_Bools);
    t_GlobalVariables.AddChild("GlobalInts", t_Integers);
    t_GlobalVariables.AddChild("GlobalFloats", t_Floats);
    t_GlobalVariables.AddChild("GlobalStrings", t_Strings);
    return t_GlobalVariables;
}

void GlobalVariableOutput::ClearVariables()
{
    m_Bools.clear();
    m_Integers.clear();
    m_Floats.clear();
    m_Strings.clear();
}

const TVariable<bool>* GlobalVariableOutput::GetLatestBool(uint32 p_uiID) const
{
    VariableBoolList::const_iterator t_Itr = m_Bools.find(p_uiID);
    if (t_Itr != m_Bools.end())
        return &t_Itr->second;

    TVariable<bool> *t_pVar = NULL;
    if (GlobalVariables *t_pGV = GlobalVariables::Get())
        return t_pGV->GetBool(p_uiID);
    return NULL;
}

const TVariable<int>* GlobalVariableOutput::GetLatestInteger(uint32 p_uiID) const
{
    VariableIntegerList::const_iterator t_Itr = m_Integers.find(p_uiID);
    if (t_Itr != m_Integers.end())
        return &t_Itr->second;

    TVariable<int> *t_pVar = NULL;
    if (GlobalVariables *t_pGV = GlobalVariables::Get())
        return t_pGV->GetInteger(p_uiID);
    return NULL;
}

const TVariable<float>* GlobalVariableOutput::GetLatestFloat(uint32 p_uiID) const
{
    VariableFloatList::const_iterator t_Itr = m_Floats.find(p_uiID);
    if (t_Itr != m_Floats.end())
        return &t_Itr->second;

    TVariable<float> *t_pVar = NULL;
    if (GlobalVariables *t_pGV = GlobalVariables::Get())
        return t_pGV->GetFloat(p_uiID);
    return NULL;
}

const TVariable<std::string>* GlobalVariableOutput::GetLatestString(uint32 p_uiID) const
{
    VariableStringList::const_iterator t_Itr = m_Strings.find(p_uiID);
    if (t_Itr != m_Strings.end())
        return &t_Itr->second;

    TVariable<std::string> *t_pVar = NULL;
    if (GlobalVariables *t_pGV = GlobalVariables::Get())
        return t_pGV->GetString(p_uiID);
    return NULL;
}

uint32 GlobalVariableOutput::GetFreeBoolID()
{
    GlobalVariables *t_pGV = GlobalVariables::Get();
    if (!t_pGV)
        return 0;

    std::map<uint32, std::string> t_VariableNames;
    t_pGV->GetBoolNames(t_VariableNames);

    for (uint32 t_I = 1; t_I < MAX_UINT; ++t_I)
    {
        const TVariable<bool> *t_Var = GetLatestBool(t_I);
        if (!t_Var || t_Var->m_uiID == 0)
            return t_I;
    }

    return 0;
}

uint32 GlobalVariableOutput::GetFreeIntegerID()
{
    GlobalVariables *t_pGV = GlobalVariables::Get();
    if (!t_pGV)
        return 0;

    std::map<uint32, std::string> t_VariableNames;
    t_pGV->GetIntegerNames(t_VariableNames);

    for (uint32 t_I = 1; t_I < MAX_UINT; ++t_I)
    {
        const TVariable<int> *t_Var = GetLatestInteger(t_I);
        if (!t_Var || t_Var->m_uiID == 0)
            return t_I;
    }

    return 0;
}

uint32 GlobalVariableOutput::GetFreeFloatID()
{
    GlobalVariables *t_pGV = GlobalVariables::Get();
    if (!t_pGV)
        return 0;

    std::map<uint32, std::string> t_VariableNames;
    t_pGV->GetFloatNames(t_VariableNames);

    for (uint32 t_I = 1; t_I < MAX_UINT; ++t_I)
    {
        const TVariable<float> *t_Var = GetLatestFloat(t_I);
        if (!t_Var || t_Var->m_uiID == 0)
            return t_I;
    }

    return 0;
}

uint32 GlobalVariableOutput::GetFreeStringID()
{
    GlobalVariables *t_pGV = GlobalVariables::Get();
    if (!t_pGV)
        return 0;

    std::map<uint32, std::string> t_VariableNames;
    t_pGV->GetStringNames(t_VariableNames);

    for (uint32 t_I = 1; t_I < MAX_UINT; ++t_I)
    {
        const TVariable<std::string> *t_Var = GetLatestString(t_I);
        if (!t_Var || t_Var->m_uiID == 0)
            return t_I;
    }

    return 0;
}