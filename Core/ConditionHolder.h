#ifndef CONDITION_HOLDER_H
#define CONDITION_HOLDER_H

#include "VariableHolder.h"
#include <boost/smart_ptr.hpp>

namespace EVENT_SCRIPT
{
    /*#####
    # ConditionSettings
    #####*/
    struct ConditionSettings
    {
        ConditionSettings() : m_ConditionType(TYPE_NONE), m_Operator(OPERATOR_EQUAL)
        {
            memset(&m_Type, NULL, sizeof(m_Type));
        }

        enum ConditionType
        {
            TYPE_NONE,
            TYPE_VARIABLE,
        };
        static const uint32 TYPE_MAX = 1;

        enum Operator
        {
            OPERATOR_NONE,
            OPERATOR_EQUAL,
            OPERATOR_NOT_EQUAL,
            OPERATOR_GREATER_EQUAL,
            OPERATOR_LESS_EQUAL,
            OPERATOR_GREATER,
            OPERATOR_LESS,
        };
        static const uint32 OPERATOR_MAX = 6;

        void SetConditionType(ConditionType p_Type);

        const VariableSettings* GetDestinationVariable() const;
        const VariableSettings* GetSourceVariable() const;
        void SetDestinationVariable(VariableSettings &p_Variable);
        void SetSourceVariable(VariableSettings &p_Variable);

        bool GetXMLData(XML::XML_WriteData &p_Data) const;
        bool LoadDataFromXML(const XML::XML_ReadData &p_Data);

        bool GetConditionText(std::string &p_Text) const;

        union Type
        {
            struct ConditionVariable
            {
                VariableSettings *m_pDestVar;
                VariableSettings *m_pSrcVar;
            } m_ConditionVariable;
        } m_Type;

        ConditionType m_ConditionType;
        Operator m_Operator;
    };

    typedef boost::shared_ptr<ConditionSettings> ConditionSettingsPtr;
    typedef std::vector<ConditionSettingsPtr> ConditionSettingsPtrList;

    /*#####
    # ConditionHolder
    #####*/
    class ConditionHolder
    {
    public:
        ConditionHolder() : m_Comparison(COMPARISON_AND) {}

        enum Comparison
        {
            COMPARISON_AND,
            COMPARISON_OR,
        };
        static const uint32 COMPARISON_MAX = 1;

        inline ConditionSettingsPtr& operator[](std::size_t &p_Index) { return m_Conditions.at(p_Index); }
        inline const ConditionSettingsPtr& operator[](std::size_t &p_Index) const { return m_Conditions.at(p_Index); }

        inline void AddCondition(const ConditionSettingsPtr &p_Condition) { m_Conditions.push_back(p_Condition); }
        void RemoveCondition(const std::size_t &p_Index);
        void SetCondition(const std::size_t &p_Index, const ConditionSettingsPtr &p_pCondition);
        inline uint32 GetConditionCount() const { return m_Conditions.size(); }

        bool GetConditionText(std::string &p_Text) const;

        inline void SetComparison(Comparison p_Comparison) { m_Comparison = p_Comparison; }
        inline Comparison GetComparison() const { return m_Comparison; }

    private:
        ConditionSettingsPtrList m_Conditions;
        Comparison m_Comparison;
    };
};
#endif