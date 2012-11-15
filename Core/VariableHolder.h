#ifndef VARIABLE_HOLDER_H
#define VARIABLE_HOLDER_H

#include <vector>
#include "Global.h"
#include "Singleton.h"
#include "XML_Writer.h"

enum VariableType
{
    VARIABLE_NONE,
    VARIABLE_BOOL,
    VARIABLE_INT,
    VARIABLE_FLOAT,
    VARIABLE_STRING,
};

template <class T>
struct TVariable
{
    TVariable() : m_Value() {}

    std::string m_sName;
    T m_Value;
};

template <class T>
class TVariableVector
{
public:
    inline const TVariable<T>* GetVariable(uint32 p_uiID) const
    {
        if (p_uiID > m_Variables.size())
            return NULL;

        return &m_Variables.at(p_uiID);
    }

    inline TVariable<T>* GetVariable(uint32 p_uiID)
    {
        if (p_uiID > m_Variables.size())
            return NULL;

        return &m_Variables.at(p_uiID);
    }

    inline void SetVariable(uint32 p_uiID, TVariable<T> &p_Var) { m_Variables.at(p_uiID) = p_Var; }
    inline void SetVariableValue(uint32 p_uiID, T &p_Value) { m_Variables.at(p_uiID).m_Value = p_Value; }

    inline uint32 AddVariable(TVariable<T> &p_Variable)
    {
        m_Variables.push_back(p_Variable);
        return m_Variables.size()-1;
    }

    inline void Resize(uint32 p_uiNewSize) { m_Variables.resize(p_uiNewSize); }
    inline uint32 Size() const { return m_Variables.size(); }
    inline void Clear() { m_Variables.clear(); }
    inline void GetVariableNames(std::vector<std::string> &p_lNames) const
    {
        for (uint32 i = 0; i < m_Variables.size(); ++i)
            p_lNames.push_back(m_Variables.at(i).m_sName);
    }

private:
    std::vector<TVariable<T>> m_Variables;
};

typedef std::vector<TVariable<bool>> VariableBoolList;
typedef std::vector<TVariable<int>> VariableIntegerList;
typedef std::vector<TVariable<float>> VariableFloatList;
typedef std::vector<TVariable<std::string>> VariableStringList;

class VariableHolder
{
public:
    inline TVariable<bool>* GetBool(uint32 p_uiID) { return m_Bools.GetVariable(p_uiID); }
    inline TVariable<int>* GetInteger(uint32 p_uiID) { return m_Integers.GetVariable(p_uiID); }
    inline TVariable<float>* GetFloat(uint32 p_uiID) { return m_Floats.GetVariable(p_uiID); }
    inline TVariable<std::string>* GetString(uint32 p_uiID) { return m_Strings.GetVariable(p_uiID); }
    inline const TVariable<bool>* GetBool(uint32 p_uiID) const { return m_Bools.GetVariable(p_uiID); }
    inline const TVariable<int>* GetInteger(uint32 p_uiID) const { return m_Integers.GetVariable(p_uiID); }
    inline const TVariable<float>* GetFloat(uint32 p_uiID) const { return m_Floats.GetVariable(p_uiID); }
    inline const TVariable<std::string>* GetString(uint32 p_uiID) const { return m_Strings.GetVariable(p_uiID); }

    inline void SetBool(uint32 p_uiID, TVariable<bool> &p_Variable) { m_Bools.SetVariable(p_uiID, p_Variable); }
    inline void SetInteger(uint32 p_uiID, TVariable<int> &p_Variable) { m_Integers.SetVariable(p_uiID, p_Variable); }
    inline void SetFloat(uint32 p_uiID, TVariable<float> &p_Variable) { m_Floats.SetVariable(p_uiID, p_Variable); }
    inline void SetString(uint32 p_uiID, TVariable<std::string> &p_Variable) { m_Strings.SetVariable(p_uiID, p_Variable); }

    inline void SetBoolValue(uint32 p_uiID, bool &p_Value) { m_Bools.SetVariableValue(p_uiID, p_Value); }
    inline void SetIntegerValue(uint32 p_uiID, int &p_Value) { m_Integers.SetVariableValue(p_uiID, p_Value); }
    inline void SetFloatValue(uint32 p_uiID, float &p_Value) { m_Floats.SetVariableValue(p_uiID, p_Value); }
    inline void SetStringValue(uint32 p_uiID, std::string &p_Value) { m_Strings.SetVariableValue(p_uiID, p_Value); }

    inline uint32 AddBool(TVariable<bool> &p_Variable) { return m_Bools.AddVariable(p_Variable); }
    inline uint32 AddInteger(TVariable<int> &p_Variable) { return m_Integers.AddVariable(p_Variable); }
    inline uint32 AddFloat(TVariable<float> &p_Variable) { return m_Floats.AddVariable(p_Variable); }
    inline uint32 AddString(TVariable<std::string> &p_Variable) { return m_Strings.AddVariable(p_Variable); }

    inline uint32 GetBoolCount() const { return m_Bools.Size(); }
    inline uint32 GetIntegerCount() const { return m_Integers.Size(); }
    inline uint32 GetFloatCount() const { return m_Floats.Size(); }
    inline uint32 GetStringCount() const { return m_Strings.Size(); }

    inline void SetBoolCount(uint32 p_uiNewSize) { m_Bools.Resize(p_uiNewSize); }
    inline void SetIntegerCount(uint32 p_uiNewSize) { m_Integers.Resize(p_uiNewSize); }
    inline void SetFloatCount(uint32 p_uiNewSize) { m_Floats.Resize(p_uiNewSize); }
    inline void SetStringCount(uint32 p_uiNewSize) { m_Strings.Resize(p_uiNewSize); }

    int AddBoolFromXML(const XML::XML_ReadData &p_Data);
    int AddIntegerFromXML(const XML::XML_ReadData &p_Data);
    int AddFloatFromXML(const XML::XML_ReadData &p_Data);
    int AddStringFromXML(const XML::XML_ReadData &p_Data);

    inline void GetBoolNames(std::vector<std::string> &p_lNames) const { m_Bools.GetVariableNames(p_lNames); }
    inline void GetIntegerNames(std::vector<std::string> &p_lNames) const { m_Integers.GetVariableNames(p_lNames); }
    inline void GetFloatNames(std::vector<std::string> &p_lNames) const { m_Floats.GetVariableNames(p_lNames); }
    inline void GetStringNames(std::vector<std::string> &p_lNames) const { m_Strings.GetVariableNames(p_lNames); }

    void ParseVariables(XML::XML_WriteData &p_XMLData) const;
    void ParseBoolsToXML(XML::XML_WriteData &p_XMLData) const;
    void ParseIntegersToXML(XML::XML_WriteData &p_XMLData) const;
    void ParseFloatsToXML(XML::XML_WriteData &p_XMLData) const;
    void ParseStringsToXML(XML::XML_WriteData &p_XMLData) const;

    void Clear();

private:
    TVariableVector<bool> m_Bools;
    TVariableVector<int> m_Integers;
    TVariableVector<float> m_Floats;
    TVariableVector<std::string> m_Strings;
};
#endif