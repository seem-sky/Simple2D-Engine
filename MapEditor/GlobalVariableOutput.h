#ifndef GLOBAL_VARIABLE_OUTPUT_H
#define GLOBAL_VARIABLE_OUTPUT_H

#include <Singleton.h>
#include <GlobalVariables.h>
#include <XML_Writer.h>

class GlobalVariableOutput : public TSingleton<GlobalVariableOutput>
{
public:
    GlobalVariableOutput(void);
    ~GlobalVariableOutput(void);

    uint32 GetFreeBoolID();
    uint32 GetFreeIntegerID();
    uint32 GetFreeFloatID();
    uint32 GetFreeStringID();

    static XML::XML_WriteData ParseVariable(uint32 t_uiID, TVariable<bool> p_Var);
    static XML::XML_WriteData ParseVariable(uint32 t_uiID, TVariable<int> p_Var);
    static XML::XML_WriteData ParseVariable(uint32 t_uiID, TVariable<float> p_Var);
    static XML::XML_WriteData ParseVariable(uint32 t_uiID, TVariable<std::string> p_Var);

    void SetVariable(TVariable<bool> p_Var, bool p_bDelete = false);
    void SetVariable(TVariable<int> p_Var, bool p_bDelete = false);
    void SetVariable(TVariable<float> p_Var, bool p_bDelete = false);
    void SetVariable(TVariable<std::string> p_Var, bool p_bDelete = false);

    const VariableBoolList* GetBools() const { return &m_Bools; }
    const VariableIntegerList* GetIntegers() const { return &m_Integers; }
    const VariableFloatList* GetFloats() const { return &m_Floats; }
    const VariableStringList* GetStrings() const { return &m_Strings; }

    const TVariable<bool>* GetLatestBool(uint32 p_uiID) const;
    const TVariable<int>* GetLatestInteger(uint32 p_uiID) const;
    const TVariable<float>* GetLatestFloat(uint32 p_uiID) const;
    const TVariable<std::string>* GetLatestString(uint32 p_uiID) const;

    XML::XML_WriteData GetXMLData() const;

    void ClearVariables();

private:
    VariableBoolList m_Bools;
    VariableIntegerList m_Integers;
    VariableFloatList m_Floats;
    VariableStringList m_Strings; 
};
#endif