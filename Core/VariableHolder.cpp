#include "VariableHolder.h"
#include "StringAdditions.h"

void VariableHolder::Clear()
{
    m_Bools.Clear();
    m_Integers.Clear();
    m_Floats.Clear();
    m_Strings.Clear();
}

int VariableHolder::AddBoolFromXML(const XML::XML_ReadData &p_Data)
{
    if (p_Data.HasAttributes())
    {
        TVariable<bool> t_Var;
        CComVariant t_Value;
        if (!p_Data.GetAttributeValue("name", t_Value))
            return -1;

        t_Var.m_sName = bstr_t(t_Value);
        if (!p_Data.GetAttributeValue("value", t_Value) || FAILED(t_Value.ChangeType(VT_BOOL)))
            return -1;

        t_Var.m_Value = t_Value.boolVal ? true : false;
        return AddBool(t_Var);
    }

    return -1;
}

int VariableHolder::AddIntegerFromXML(const XML::XML_ReadData &p_Data)
{
    if (p_Data.HasAttributes())
    {
        TVariable<int> t_Var;
        CComVariant t_Value;
        if (!p_Data.GetAttributeValue("name", t_Value))
            return -1;

        t_Var.m_sName = bstr_t(t_Value);
        if (!p_Data.GetAttributeValue("value", t_Value) || FAILED(t_Value.ChangeType(VT_INT)))
            return -1;

        t_Var.m_Value = t_Value.intVal;
        return AddInteger(t_Var);
    }

    return -1;
}

int VariableHolder::AddFloatFromXML(const XML::XML_ReadData &p_Data)
{
    if (p_Data.HasAttributes())
    {
        TVariable<float> t_Var;
        CComVariant t_Value;
        if (!p_Data.GetAttributeValue("name", t_Value))
            return -1;

        t_Var.m_sName = bstr_t(t_Value);
        if (!p_Data.GetAttributeValue("value", t_Value) || FAILED(t_Value.ChangeType(VT_R4)))
            return -1;

        t_Var.m_Value = t_Value.fltVal;
        return AddFloat(t_Var);
    }

    return -1;
}

int VariableHolder::AddStringFromXML(const XML::XML_ReadData &p_Data)
{
    if (p_Data.HasAttributes())
    {
        TVariable<std::string> t_Var;
        CComVariant t_Value;
        if (!p_Data.GetAttributeValue("name", t_Value))
            return -1;

        t_Var.m_sName = bstr_t(t_Value);
        if (!p_Data.GetAttributeValue("value", t_Value))
            return -1;

        t_Var.m_Value = bstr_t(t_Value);
        return AddString(t_Var);
    }

    return -1;
}

void VariableHolder::ParseBoolsToXML(XML::XML_WriteData &p_XMLData) const
{
    for (uint32 i = 0; i < m_Bools.Size(); ++i)
    {
        if (const TVariable<bool> *t_pVar = m_Bools.GetVariable(i))
        {
            XML::XML_WriteData t_Data(XML::XML_WRITE_APPEND);
            t_Data.AddAttribute("name", (LPCOLESTR)_bstr_t(t_pVar->m_sName.c_str()));
            t_Data.AddAttribute("value", t_pVar->m_Value);
            p_XMLData.AddChild("bool", t_Data);
        }
    }
}

void VariableHolder::ParseIntegersToXML(XML::XML_WriteData &p_XMLData) const
{
    for (uint32 i = 0; i < m_Integers.Size(); ++i)
    {
        if (const TVariable<int> *t_pVar = m_Integers.GetVariable(i))
        {
            XML::XML_WriteData t_Data(XML::XML_WRITE_APPEND);
            t_Data.AddAttribute("name", (LPCOLESTR)_bstr_t(t_pVar->m_sName.c_str()));
            t_Data.AddAttribute("value", t_pVar->m_Value);
            p_XMLData.AddChild("integer", t_Data);
        }
    }
}

void VariableHolder::ParseFloatsToXML(XML::XML_WriteData &p_XMLData) const
{
    for (uint32 i = 0; i < m_Floats.Size(); ++i)
    {
        if (const TVariable<float> *t_pVar = m_Floats.GetVariable(i))
        {
            XML::XML_WriteData t_Data(XML::XML_WRITE_APPEND);
            t_Data.AddAttribute("name", (LPCOLESTR)_bstr_t(t_pVar->m_sName.c_str()));
            t_Data.AddAttribute("value", t_pVar->m_Value);
            p_XMLData.AddChild("float", t_Data);
        }
    }
}

void VariableHolder::ParseStringsToXML(XML::XML_WriteData &p_XMLData) const
{
    for (uint32 i = 0; i < m_Strings.Size(); ++i)
    {
        if (const TVariable<std::string> *t_pVar = m_Strings.GetVariable(i))
        {
            XML::XML_WriteData t_Data(XML::XML_WRITE_APPEND);
            t_Data.AddAttribute("name", (LPCOLESTR)_bstr_t(t_pVar->m_sName.c_str()));
            t_Data.AddAttribute("value", (LPCOLESTR)_bstr_t(t_pVar->m_Value.c_str()));
            p_XMLData.AddChild("string", t_Data);
        }
    }
}

void VariableHolder::ParseVariables(XML::XML_WriteData &p_XMLData) const
{
    ParseBoolsToXML(p_XMLData);
    ParseIntegersToXML(p_XMLData);
    ParseFloatsToXML(p_XMLData);
    ParseStringsToXML(p_XMLData);
    p_XMLData.SetWriteState(XML::XML_WRITE_OVERWRITE);
}