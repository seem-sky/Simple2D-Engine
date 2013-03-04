#include "ConditionHolder.h"

using namespace EVENT_SCRIPT;

/*#####
# ConditionSettings
#####*/
void ConditionSettings::SetConditionType(ConditionType p_Type)
{
    if (m_ConditionType != p_Type)
    {
        m_ConditionType = p_Type;
        switch(m_ConditionType)
        {
        case TYPE_VARIABLE:
            if (m_Type.m_ConditionVariable.m_pDestVar)
                delete m_Type.m_ConditionVariable.m_pDestVar;
            if (m_Type.m_ConditionVariable.m_pSrcVar)
                delete m_Type.m_ConditionVariable.m_pSrcVar;
            break;
        }
        memset(&m_Type, NULL, sizeof(m_Type));
    }
}

const VariableSettings* ConditionSettings::GetDestinationVariable() const
{
    if (m_ConditionType != TYPE_VARIABLE)
        return NULL;

    return m_Type.m_ConditionVariable.m_pDestVar;
}

const VariableSettings* ConditionSettings::GetSourceVariable() const
{
    if (m_ConditionType != TYPE_VARIABLE)
        return NULL;

    return m_Type.m_ConditionVariable.m_pSrcVar;
}

void ConditionSettings::SetDestinationVariable(VariableSettings &p_Variable)
{
    if (m_ConditionType != TYPE_VARIABLE)
        return;

    if (m_Type.m_ConditionVariable.m_pDestVar)
        *m_Type.m_ConditionVariable.m_pDestVar = p_Variable;
    else
        m_Type.m_ConditionVariable.m_pDestVar = new VariableSettings(p_Variable);
}

void ConditionSettings::SetSourceVariable(VariableSettings &p_Variable)
{
    if (m_ConditionType != TYPE_VARIABLE)
        return;

    if (m_Type.m_ConditionVariable.m_pSrcVar)
        *m_Type.m_ConditionVariable.m_pSrcVar = p_Variable;
    else
        m_Type.m_ConditionVariable.m_pSrcVar = new VariableSettings(p_Variable);
}

bool ConditionSettings::GetXMLData(XML::XML_WriteData &p_Data) const
{
    switch (m_ConditionType)
    {
    case TYPE_VARIABLE:
        {
            XML::XML_WriteData t_DestVar(XML::XML_WRITE_CHANGE);
            if (!GetDestinationVariable() || !GetDestinationVariable()->GetVariableXMLData(t_DestVar))
                return false;
            p_Data.AddChild("DestVar", t_DestVar);

            XML::XML_WriteData t_SrcVar(XML::XML_WRITE_CHANGE);
            if (!GetSourceVariable() || !GetSourceVariable()->GetVariableXMLData(t_SrcVar))
                return false;
            p_Data.AddChild("SrcVar", t_SrcVar);
            break;
        }
    default: return false;
    }

    p_Data.AddAttribute("Operator", (uint32)m_Operator);
    p_Data.AddAttribute("ConditionType", (uint32)m_ConditionType);
    return true;
}

bool ConditionSettings::LoadDataFromXML(const XML::XML_ReadData &p_Data)
{
    CComVariant t_Value;
    // operator
    if (!p_Data.GetAttributeValue("Operator", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)) ||
        t_Value.uintVal > OPERATOR_MAX)
        return false;
    m_Operator = (Operator)t_Value.uintVal;

    // condition type
    if (!p_Data.GetAttributeValue("ConditionType", t_Value) || FAILED(t_Value.ChangeType(VT_UINT, &t_Value)) ||
        t_Value.uintVal > TYPE_MAX)
        return false;
    SetConditionType((ConditionType)t_Value.uintVal);

    switch (m_ConditionType)
    {
    case TYPE_VARIABLE:
        {
            // dest var
            const XML::XML_ReadData *t_DestData = p_Data.GetChild("DestVar");
            VariableSettings t_DestVar;
            if (!t_DestData || !t_DestVar.LoadDataFromXML(*t_DestData))
                return false;
            SetDestinationVariable(t_DestVar);

            // src var
            const XML::XML_ReadData *t_SrcData = p_Data.GetChild("SrcVar");
            VariableSettings t_SrcVar;
            if (!t_SrcData || !t_SrcVar.LoadDataFromXML(*t_SrcData))
                return false;
            SetSourceVariable(t_SrcVar);
            break;
        }
    }
    return true;
}

bool ConditionSettings::GetConditionText(std::string &p_sText) const
{
    std::string t_sType;
    std::string t_sOperator;
    switch(m_Operator)
    {
    case OPERATOR_EQUAL: t_sOperator = "== "; break;
    case OPERATOR_NOT_EQUAL: t_sOperator = "!= "; break;
    case OPERATOR_GREATER_EQUAL: t_sOperator = ">= "; break;
    case OPERATOR_LESS_EQUAL: t_sOperator = "<= "; break;
    case OPERATOR_GREATER: t_sOperator = "> "; break;
    case OPERATOR_LESS: t_sOperator = "< "; break;
    default: return false;
    }

    switch (m_ConditionType)
    {
    case TYPE_VARIABLE:
        t_sType = "Variable ";
        std::string t_sDestText;
        std::string t_sSrcText;
        if (m_Type.m_ConditionVariable.m_pDestVar)
            t_sDestText = m_Type.m_ConditionVariable.m_pDestVar->GetVariableSettingText();
        if (m_Type.m_ConditionVariable.m_pSrcVar)
            t_sSrcText = m_Type.m_ConditionVariable.m_pSrcVar->GetVariableSettingText();

        p_sText = t_sType + t_sDestText + t_sOperator + t_sSrcText;
        return true;
    }
    return false;
}

/*#####
# ConditionHolder
#####*/
void ConditionHolder::SetCondition(const std::size_t &p_Index, const ConditionSettingsPtr &p_pCondition)
{
    if (p_Index < m_Conditions.size())
        m_Conditions[p_Index] = p_pCondition;
    else
        AddCondition(p_pCondition);
}

void ConditionHolder::RemoveCondition(const std::size_t &p_Index)
{
    if (p_Index >= m_Conditions.size())
        return;

    ConditionSettingsPtrList::iterator t_Itr = m_Conditions.begin();
    t_Itr += p_Index;
    m_Conditions.erase(t_Itr);
}

bool ConditionHolder::GetConditionText(std::string &p_sText) const
{
    for (std::size_t i = 0; i < m_Conditions.size(); ++i)
    {
        std::string t_sText;
        if (m_Conditions.at(i)->GetConditionText(t_sText))
        {
            if (i+1 < m_Conditions.size())
                t_sText += m_Comparison == COMPARISON_AND ? " &&\n" : " ||\n";
            p_sText.append(t_sText);
        }
    }
    return true;
}