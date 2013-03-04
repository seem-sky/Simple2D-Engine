#include "VariableHolder.h"
#include "StringAdditions.h"
#include <XML_Reader.h>

/*#####
# VariableSettings
#####*/
bool VariableSettings::LoadDataFromXML(const XML::XML_ReadData &p_Data)
{
    CComVariant t_Value;
    if (!p_Data.GetAttributeValue("Localisation", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)) || t_Value.uintVal > VARIABLE_LOCALISATION_MAX)
        return false;
    m_VarLocalisation = (VariableLocalisation)t_Value.uintVal;

    if (!p_Data.GetAttributeValue("VariableType", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)) || t_Value.uintVal > VARIABLE_TYPE_MAX)
        return false;
    m_VariableType = (VariableType)t_Value.uintVal;
    switch (m_VarLocalisation)
    {
    case LOCALISATION_VALUE:
        if (!p_Data.GetAttributeValue("Value", t_Value))
            return false;
        switch (m_VariableType)
        {
        case VARIABLE_BOOL:
            if (FAILED(t_Value.ChangeType(VT_BOOL, &t_Value)))
                return false;
            m_Type.m_Value.m_VariableType.m_BoolValue = t_Value.boolVal ? true : false;
            break;
        case VARIABLE_INT:
            if (FAILED(t_Value.ChangeType(VT_INT)))
                return false;
            m_Type.m_Value.m_VariableType.m_IntValue = t_Value.intVal;
            break;
        case VARIABLE_FLOAT:
            if (FAILED(t_Value.ChangeType(VT_R4)))
                return false;
            m_Type.m_Value.m_VariableType.m_FloatValue = t_Value.fltVal;
            break;
        case VARIABLE_STRING:
            m_Type.m_Value.m_VariableType.m_StringValue = bstr_t(t_Value);
            break;
        }
        break;
    default:
        if (!p_Data.GetAttributeValue("ID", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)))
            return false;

        switch (m_VarLocalisation)
        {
        case LOCALISATION_GLOBAL:
            m_Type.m_GlobalVariable.m_uiVariableID = t_Value.uintVal;
            break;
        case LOCALISATION_LOCAL:
            m_Type.m_LocalVariable.m_uiVariableID = t_Value.uintVal;
            break;
        }
        break;
    }

    return true;
}

bool VariableSettings::GetVariableXMLData(XML::XML_WriteData &p_Data) const
{
    switch (m_VarLocalisation)
    {
    case LOCALISATION_VALUE:
        switch (m_VariableType)
        {
        case VARIABLE_BOOL: p_Data.AddAttribute("Value", m_Type.m_Value.m_VariableType.m_BoolValue); break;
        case VARIABLE_INT: p_Data.AddAttribute("Value", m_Type.m_Value.m_VariableType.m_IntValue); break;
        case VARIABLE_FLOAT: p_Data.AddAttribute("Value", m_Type.m_Value.m_VariableType.m_FloatValue); break;
        case VARIABLE_STRING: p_Data.AddAttribute("Value", m_Type.m_Value.m_VariableType.m_StringValue); break;
        default:
            return false;
        }
        break;
    case LOCALISATION_GLOBAL: p_Data.AddAttribute("ID", m_Type.m_GlobalVariable.m_uiVariableID); break;
    case LOCALISATION_LOCAL: p_Data.AddAttribute("ID", m_Type.m_LocalVariable.m_uiVariableID); break;
    default:
        return false;
    }

    p_Data.AddAttribute("VariableType", (uint32)m_VariableType);
    p_Data.AddAttribute("Localisation", (uint32)m_VarLocalisation);
    return true;
}

std::string VariableSettings::GetVariableTypeText() const
{
    switch(m_VariableType)
    {
    case VARIABLE_BOOL:
        return "Bool";
    case VARIABLE_INT:
        return "Integer";
    case VARIABLE_FLOAT:
        return "Float";
    case VARIABLE_STRING:
        return "String";
    }

    return "";
}

std::string VariableSettings::GetVariableSettingText() const
{
    // variable localisation
    switch (m_VarLocalisation)
    {
    case LOCALISATION_LOCAL:
        return "Local " + GetVariableTypeText() + "[" + ToString(m_Type.m_LocalVariable.m_uiVariableID) + "] ";
    case LOCALISATION_GLOBAL:
        return "Global " + GetVariableTypeText() + "[" + ToString(m_Type.m_GlobalVariable.m_uiVariableID) + "] ";
    case LOCALISATION_VALUE:
        {
            std::string t_sText = "Value " + GetVariableTypeText() + " ";
            // variable type
            switch (m_VariableType)
            {
            case VARIABLE_BOOL:
                t_sText += m_Type.m_Value.m_VariableType.m_BoolValue ? "true " : "false ";
                break;
            case VARIABLE_INT:
                t_sText += ToString(m_Type.m_Value.m_VariableType.m_IntValue) + " ";
                break;
            case VARIABLE_FLOAT:
                t_sText += ToString(m_Type.m_Value.m_VariableType.m_FloatValue) + " ";
                break;
            case VARIABLE_STRING:
                t_sText += "\"";
                t_sText += bstr_t(m_Type.m_Value.m_VariableType.m_StringValue);
                t_sText += "\" ";
                break;
            }
            return t_sText;
        }
    }

    return "";
}

/*#####
# VariableHolder
#####*/
void VariableHolder::Clear()
{
    m_Bools.Clear();
    m_Integers.Clear();
    m_Floats.Clear();
    m_Strings.Clear();
}

void VariableHolder::LoadDataFromXML(const XML::XML_ReadData &p_Data)
{
    const XML::ReadChildList *t_pVariableList = p_Data.GetChildList();
    if (!t_pVariableList)
        return;

    for (XML::ReadChildList::const_iterator t_VTypeItr = t_pVariableList->begin(); t_VTypeItr != t_pVariableList->end(); ++t_VTypeItr)
    {
        // bool value
        if (t_VTypeItr->first == "bool")
            AddBoolFromXML(t_VTypeItr->second);
        // int32 value
        else if (t_VTypeItr->first == "integer")
            AddIntegerFromXML(t_VTypeItr->second);
        // float value
        else if (t_VTypeItr->first == "float")
            AddFloatFromXML(t_VTypeItr->second);
        // string value
        else if (t_VTypeItr->first == "string")
            AddStringFromXML(t_VTypeItr->second);
    }
}

int32 VariableHolder::AddBoolFromXML(const XML::XML_ReadData &p_Data)
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

int32 VariableHolder::AddIntegerFromXML(const XML::XML_ReadData &p_Data)
{
    if (p_Data.HasAttributes())
    {
        TVariable<int32> t_Var;
        CComVariant t_Value;
        if (!p_Data.GetAttributeValue("name", t_Value))
            return -1;

        t_Var.m_sName = bstr_t(t_Value);
        if (!p_Data.GetAttributeValue("value", t_Value) || FAILED(t_Value.ChangeType(VT_INT)))
            return -1;

        t_Var.m_Value = t_Value.uintVal;
        return AddInteger(t_Var);
    }

    return -1;
}

int32 VariableHolder::AddFloatFromXML(const XML::XML_ReadData &p_Data)
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

int32 VariableHolder::AddStringFromXML(const XML::XML_ReadData &p_Data)
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
            t_Data.AddAttribute("name", (LPCSTR)t_pVar->m_sName.c_str());
            t_Data.AddAttribute("value", t_pVar->m_Value);
            p_XMLData.AddChild("bool", t_Data);
        }
    }
}

void VariableHolder::ParseIntegersToXML(XML::XML_WriteData &p_XMLData) const
{
    for (uint32 i = 0; i < m_Integers.Size(); ++i)
    {
        if (const TVariable<int32> *t_pVar = m_Integers.GetVariable(i))
        {
            XML::XML_WriteData t_Data(XML::XML_WRITE_APPEND);
            t_Data.AddAttribute("name", (LPCSTR)t_pVar->m_sName.c_str());
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
            t_Data.AddAttribute("name", (LPCSTR)t_pVar->m_sName.c_str());
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
            t_Data.AddAttribute("name", (LPCSTR)t_pVar->m_sName.c_str());
            t_Data.AddAttribute("value", (LPCSTR)t_pVar->m_sName.c_str());
            p_XMLData.AddChild("string", t_Data);
        }
    }
}

void VariableHolder::GetXMLData(XML::XML_WriteData &p_XMLData) const
{
    ParseBoolsToXML(p_XMLData);
    ParseIntegersToXML(p_XMLData);
    ParseFloatsToXML(p_XMLData);
    ParseStringsToXML(p_XMLData);
}