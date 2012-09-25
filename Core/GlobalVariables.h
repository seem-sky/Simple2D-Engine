#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <vector>
#include <map>
#include "Global.h"
#include "Singleton.h"

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
    TVariable() : m_uiID(0), m_Value() {}

    uint32 m_uiID;
    std::string m_sName;
    T m_Value;
};

typedef std::map<uint32, TVariable<bool>> VariableBoolList;
typedef std::map<uint32, TVariable<int>> VariableIntegerList;
typedef std::map<uint32, TVariable<float>> VariableFloatList;
typedef std::map<uint32, TVariable<std::string>> VariableStringList;

class GlobalVariables : public TSingleton<GlobalVariables>
{
public:
    GlobalVariables(void);
    ~GlobalVariables(void);

    inline const TVariable<bool>* GetBool(uint32 p_uiID) const
    {
        VariableBoolList::const_iterator t_Itr = m_Bools.find(p_uiID);
        if (t_Itr == m_Bools.end())
            return NULL;

        return &t_Itr->second;
    }

    inline const TVariable<int>* GetInteger(uint32 p_uiID) const
    {
        VariableIntegerList::const_iterator t_Itr = m_Integers.find(p_uiID);
        if (t_Itr == m_Integers.end())
            return NULL;

        return &t_Itr->second;
    }

    inline const TVariable<float>* GetFloat(uint32 p_uiID) const
    {
        VariableFloatList::const_iterator t_Itr = m_Floats.find(p_uiID);
        if (t_Itr == m_Floats.end())
            return NULL;

        return &t_Itr->second;
    }

    inline const TVariable<std::string>* GetString(uint32 p_uiID) const
    {
        VariableStringList::const_iterator t_Itr = m_Strings.find(p_uiID);
        if (t_Itr == m_Strings.end())
            return NULL;

        return &t_Itr->second;
    }

    inline void AddBool(TVariable<bool> p_Variable) { m_Bools.insert(std::make_pair(p_Variable.m_uiID, p_Variable)); }
    inline void AddInteger(TVariable<int> p_Variable) { m_Integers.insert(std::make_pair(p_Variable.m_uiID, p_Variable)); }
    inline void AddFloat(TVariable<float> p_Variable) { m_Floats.insert(std::make_pair(p_Variable.m_uiID, p_Variable)); }
    inline void AddString(TVariable<std::string> p_Variable) { m_Strings.insert(std::make_pair(p_Variable.m_uiID, p_Variable)); }

    void GetBoolNames(std::map<uint32, std::string> &p_lNames) const;
    void GetIntegerNames(std::map<uint32, std::string> &p_lNames) const;
    void GetFloatNames(std::map<uint32, std::string> &p_lNames) const;
    void GetStringNames(std::map<uint32, std::string> &p_lNames) const;

private:
    VariableBoolList m_Bools;
    VariableIntegerList m_Integers;
    VariableFloatList m_Floats;
    VariableStringList m_Strings;
};
#endif