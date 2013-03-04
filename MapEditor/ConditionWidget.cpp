#include "ConditionWidget.h"
#include "moc_ConditionWidget.h"
#include "DatabaseOutput.h"
#include "EventEditorWidget.h"

using namespace EVENT_SCRIPT;

ConditionWidget::ConditionWidget(QWidget *p_pParent) : QWidget(p_pParent), Ui_ConditionWidget()
{
    setupUi(this);
    connect(m_pBool, SIGNAL(toggled(bool)), this, SLOT(VariableTypeChanged(bool)));
    connect(m_pInteger, SIGNAL(toggled(bool)), this, SLOT(VariableTypeChanged(bool)));
    connect(m_pFloat, SIGNAL(toggled(bool)), this, SLOT(VariableTypeChanged(bool)));
    connect(m_pString, SIGNAL(toggled(bool)), this, SLOT(VariableTypeChanged(bool)));
    VariableTypeChanged(true);

    connect(m_pType, SIGNAL(currentChanged(int)), this, SLOT(ConditionTypeChanged(int)));
    m_pDestTab->setCurrentIndex(0);
    m_pSrcTab->setCurrentIndex(0);
}

bool ConditionWidget::GetCondition(EVENT_SCRIPT::ConditionSettingsPtr &p_pResult) const
{
    if (!p_pResult)
        return false;

    // set operator
    if (m_pEqual->isChecked())
        p_pResult->m_Operator = ConditionSettings::OPERATOR_EQUAL;
    else if (m_pNotEqual->isChecked())
        p_pResult->m_Operator = ConditionSettings::OPERATOR_NOT_EQUAL;
    else if (m_pGreaterEqual->isChecked())
        p_pResult->m_Operator = ConditionSettings::OPERATOR_GREATER_EQUAL;
    else if (m_pLessEqual->isChecked())
        p_pResult->m_Operator = ConditionSettings::OPERATOR_LESS_EQUAL;
    else if (m_pGreater->isChecked())
        p_pResult->m_Operator = ConditionSettings::OPERATOR_GREATER;
    else if (m_pLess->isChecked())
        p_pResult->m_Operator = ConditionSettings::OPERATOR_LESS;
    else
        return false;

    QString t_sTabText = m_pType->tabText(m_pType->currentIndex());
    if (t_sTabText == "Variables")
    {
        // variable type
        VariableType t_VarType;
        if (m_pBool->isChecked())
            t_VarType = VARIABLE_BOOL;
        else if (m_pInteger->isChecked())
            t_VarType = VARIABLE_INT;
        else if (m_pFloat->isChecked())
            t_VarType = VARIABLE_FLOAT;
        else if (m_pString->isChecked())
            t_VarType = VARIABLE_STRING;
        else
            return false;

        p_pResult->m_ConditionType = ConditionSettings::TYPE_VARIABLE;
        // set destination variable settings
        VariableSettings t_DestVar;
        t_DestVar.m_VariableType = t_VarType;
        QString t_sDestTabText = m_pDestTab->tabText(m_pDestTab->currentIndex());
        if (t_sDestTabText == "Global Variable")
        {
            t_DestVar.m_VarLocalisation = LOCALISATION_GLOBAL;
            if (!m_pGlobalVarBox->count())
                return false;

            t_DestVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pGlobalVarBox->currentIndex();
        }
        else if (t_sDestTabText == "Local Variable")
        {
            t_DestVar.m_VarLocalisation = LOCALISATION_LOCAL;
            if (!m_pLocalVarBox->count())
                return false;

            t_DestVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pLocalVarBox->currentIndex();
        }
        else
            return false;

        p_pResult->SetDestinationVariable(t_DestVar);

        // set src variable settings
        VariableSettings t_SrcVar;
        t_SrcVar.m_VariableType = t_VarType;
        QString t_sSrcTabText = m_pSrcTab->tabText(m_pSrcTab->currentIndex());
        if (t_sSrcTabText == "Global Variable")
        {
            t_SrcVar.m_VarLocalisation = LOCALISATION_GLOBAL;
            switch(t_VarType)
            {
            case VARIABLE_BOOL:
                if (!m_pSrcGlobalBoolBox->count())
                    return false;

                t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalBoolBox->currentIndex();
                break;
            case VARIABLE_INT:
                if (!m_pSrcGlobalIntegerBox->count())
                    return false;

                t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalIntegerBox->currentIndex();
                break;
            case VARIABLE_FLOAT:
                if (!m_pSrcGlobalFloatBox->count())
                    return false;

                t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalFloatBox->currentIndex();
                break;
            case VARIABLE_STRING:
                if (!m_pSrcGlobalStringBox->count())
                    return false;

                t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalStringBox->currentIndex();
                break;

            default: return false;
            }
        }
        else if (t_sSrcTabText == "Local Variable")
        {
            t_SrcVar.m_VarLocalisation = LOCALISATION_LOCAL;
            switch(t_VarType)
            {
            case VARIABLE_BOOL:
                if (!m_pSrcLocalBoolBox->count())
                    return false;

                t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalBoolBox->currentIndex();
                break;
            case VARIABLE_INT:
                if (!m_pSrcLocalIntegerBox->count())
                    return false;

                t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalIntegerBox->currentIndex();
                break;
            case VARIABLE_FLOAT:
                if (!m_pSrcLocalFloatBox->count())
                    return false;

                t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalFloatBox->currentIndex();
                break;
            case VARIABLE_STRING:
                if (!m_pSrcLocalStringBox->count())
                    return false;

                t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalStringBox->currentIndex();
                break;

            default: return false;
            }
        }
        else if (t_sSrcTabText == "Value")
        {
            t_SrcVar.m_VarLocalisation = LOCALISATION_VALUE;
            if (m_pBool->isChecked())
                t_SrcVar.m_Type.m_Value.m_VariableType.m_BoolValue = m_pValueBool->currentIndex() ? true : false;
            else if (m_pInteger->isChecked())
                t_SrcVar.m_Type.m_Value.m_VariableType.m_IntValue = m_pValueInteger->value();
            else if (m_pFloat->isChecked())
                t_SrcVar.m_Type.m_Value.m_VariableType.m_FloatValue = m_pValueFloat->value();
            else if (m_pString->isChecked())
                t_SrcVar.m_Type.m_Value.m_VariableType.m_StringValue = _bstr_t(m_pValueString->text().toStdString().c_str());
        }
        else
            return false;
        p_pResult->SetSourceVariable(t_SrcVar);
    }
    else
        return false;

    return true;
}

void ConditionWidget::LoadValuesFromEventScriptCommand(const EVENT_SCRIPT::ConditionSettingsPtr &p_pCondition)
{
    if (!p_pCondition)
        return;
    // set operator
    switch(p_pCondition->m_Operator)
    {
    case ConditionSettings::OPERATOR_EQUAL: m_pEqual->setChecked(true); break;
    case ConditionSettings::OPERATOR_NOT_EQUAL: m_pNotEqual->setChecked(true); break;
    case ConditionSettings::OPERATOR_GREATER_EQUAL: m_pGreaterEqual->setChecked(true); break;
    case ConditionSettings::OPERATOR_LESS_EQUAL: m_pLessEqual->setChecked(true); break;
    case ConditionSettings::OPERATOR_GREATER: m_pGreater->setChecked(true); break;
    case ConditionSettings::OPERATOR_LESS: m_pLess->setChecked(true); break;
    }

    // condition type
    m_pType->setCurrentIndex(((int32)ConditionSettings::TYPE_VARIABLE)-1);
    switch(p_pCondition->m_ConditionType)
    {
    case ConditionSettings::TYPE_VARIABLE:
        if (!p_pCondition->GetDestinationVariable())
            return;

        // variable type
        switch (p_pCondition->GetDestinationVariable()->m_VariableType)
        {
        case VARIABLE_BOOL: m_pBool->setChecked(true); break;
        case VARIABLE_INT: m_pInteger->setChecked(true); break;
        case VARIABLE_FLOAT: m_pFloat->setChecked(true); break;
        case VARIABLE_STRING: m_pString->setChecked(true); break;
        }

        // dest variable
        m_pDestTab->setCurrentIndex(((int32)p_pCondition->GetDestinationVariable()->m_VarLocalisation)-2);
        switch (p_pCondition->GetDestinationVariable()->m_VarLocalisation)
        {
        case LOCALISATION_GLOBAL:
            m_pGlobalVarBox->setCurrentIndex(p_pCondition->GetDestinationVariable()->m_Type.m_GlobalVariable.m_uiVariableID);
            break;
        case LOCALISATION_LOCAL:
            m_pLocalVarBox->setCurrentIndex(p_pCondition->GetDestinationVariable()->m_Type.m_LocalVariable.m_uiVariableID);
            break;
        }

        // src variable
        if (!p_pCondition->GetSourceVariable())
            return;

        m_pSrcTab->setCurrentIndex(((int32)p_pCondition->GetSourceVariable()->m_VarLocalisation)-1);
        switch (p_pCondition->GetSourceVariable()->m_VarLocalisation)
        {
        case LOCALISATION_VALUE:
            switch (p_pCondition->GetSourceVariable()->m_VariableType)
            {
            case VARIABLE_BOOL: m_pValueBool->setCurrentIndex(p_pCondition->GetSourceVariable()->m_Type.m_Value.m_VariableType.m_BoolValue ? 1 : 0); break;
            case VARIABLE_INT: m_pValueInteger->setValue(p_pCondition->GetSourceVariable()->m_Type.m_Value.m_VariableType.m_IntValue); break;
            case VARIABLE_FLOAT: m_pValueFloat->setValue(p_pCondition->GetSourceVariable()->m_Type.m_Value.m_VariableType.m_FloatValue); break;
            case VARIABLE_STRING:
                std::string t_sTemp = bstr_t(p_pCondition->GetSourceVariable()->m_Type.m_Value.m_VariableType.m_StringValue);
                m_pValueString->setText(QString::fromStdString(t_sTemp));
                break;
            }
            break;

        case LOCALISATION_GLOBAL:
            switch (p_pCondition->GetSourceVariable()->m_VariableType)
            {
            case VARIABLE_BOOL: m_pSrcGlobalBoolBox->setCurrentIndex(p_pCondition->GetSourceVariable()->m_Type.m_GlobalVariable.m_uiVariableID); break;
            case VARIABLE_INT: m_pSrcGlobalIntegerBox->setCurrentIndex(p_pCondition->GetSourceVariable()->m_Type.m_GlobalVariable.m_uiVariableID); break;
            case VARIABLE_FLOAT: m_pSrcGlobalFloatBox->setCurrentIndex(p_pCondition->GetSourceVariable()->m_Type.m_GlobalVariable.m_uiVariableID); break;
            case VARIABLE_STRING: m_pSrcGlobalStringBox->setCurrentIndex(p_pCondition->GetSourceVariable()->m_Type.m_GlobalVariable.m_uiVariableID); break;
            }
            break;

        case LOCALISATION_LOCAL:
            switch (p_pCondition->GetSourceVariable()->m_VariableType)
            {
            case VARIABLE_BOOL: m_pSrcLocalBoolBox->setCurrentIndex(p_pCondition->GetSourceVariable()->m_Type.m_LocalVariable.m_uiVariableID); break;
            case VARIABLE_INT: m_pSrcLocalIntegerBox->setCurrentIndex(p_pCondition->GetSourceVariable()->m_Type.m_LocalVariable.m_uiVariableID); break;
            case VARIABLE_FLOAT: m_pSrcLocalFloatBox->setCurrentIndex(p_pCondition->GetSourceVariable()->m_Type.m_LocalVariable.m_uiVariableID); break;
            case VARIABLE_STRING: m_pSrcLocalStringBox->setCurrentIndex(p_pCondition->GetSourceVariable()->m_Type.m_LocalVariable.m_uiVariableID); break;
            }
            break;
        }
    }
}

void ConditionWidget::ConditionTypeChanged(int p_Index)
{
    if (p_Index == -1)
        return;

    QString t_sTabText = m_pType->tabText(p_Index);
    if (t_sTabText == "Variables")
        VariableTypeChanged(true);
}

void ConditionWidget::ReInitialize()
{
    ConditionTypeChanged(m_pType->currentIndex());
}

void ConditionWidget::VariableTypeChanged(bool p_bChecked)
{
    if (!p_bChecked || !parent())
        return;

    if (m_pType->tabText(m_pType->currentIndex()) != "Variables")
        return;

    m_pGreater->setEnabled(!m_pBool->isChecked());
    m_pGreaterEqual->setEnabled(!m_pBool->isChecked());
    m_pLess->setEnabled(!m_pBool->isChecked());
    m_pLessEqual->setEnabled(!m_pBool->isChecked());

    const VariableHolder *t_pGlobalVariableHolder = NULL;
    if (DATABASE::DatabaseOutput *t_pDBOut = DATABASE::DatabaseOutput::Get())
        t_pGlobalVariableHolder = t_pDBOut->GetGlobalVariables();

    const VariableHolder *t_pLocalVariableHolder = ((ScriptPage*)parent()->parent()->parent()->parent())->GetVariableHolder();
    if (!t_pGlobalVariableHolder || !t_pLocalVariableHolder)
        return;

    m_pValueBool->setEnabled(false);
    m_pValueInteger->setEnabled(false);
    m_pValueFloat->setEnabled(false);
    m_pValueString->setEnabled(false);

    QComboBox *t_pGlobalSrcBox = NULL;
    QComboBox *t_pLocalSrcBox = NULL;
    std::vector<std::string> t_GlobalVarNames;
    std::vector<std::string> t_LocalVarNames;
    if (m_pBool->isChecked())
    {
        t_pGlobalVariableHolder->GetBoolNames(t_GlobalVarNames);
        t_pLocalVariableHolder->GetBoolNames(t_LocalVarNames);
        m_pValueBool->setEnabled(true);
        t_pGlobalSrcBox = m_pSrcGlobalBoolBox;
        t_pLocalSrcBox = m_pSrcLocalBoolBox;
    }
    else if (m_pInteger->isChecked())
    {
        t_pGlobalVariableHolder->GetIntegerNames(t_GlobalVarNames);
        t_pLocalVariableHolder->GetIntegerNames(t_LocalVarNames);
        m_pValueInteger->setEnabled(true);
        t_pGlobalSrcBox = m_pSrcGlobalIntegerBox;
        t_pLocalSrcBox = m_pSrcLocalIntegerBox;
    }
    else if (m_pFloat->isChecked())
    {
        t_pGlobalVariableHolder->GetFloatNames(t_GlobalVarNames);
        t_pLocalVariableHolder->GetFloatNames(t_LocalVarNames);
        m_pValueFloat->setEnabled(true);
        t_pGlobalSrcBox = m_pSrcGlobalFloatBox;
        t_pLocalSrcBox = m_pSrcLocalFloatBox;
    }
    else if (m_pString->isChecked())
    {
        t_pGlobalVariableHolder->GetStringNames(t_GlobalVarNames);
        t_pLocalVariableHolder->GetStringNames(t_LocalVarNames);
        m_pValueString->setEnabled(true);
        t_pGlobalSrcBox = m_pSrcGlobalStringBox;
        t_pLocalSrcBox = m_pSrcLocalStringBox;
    }

    // global variables
    // modify src global boxes
    m_pSrcGlobalBoolBox->clear();
    m_pSrcGlobalIntegerBox->clear();
    m_pSrcGlobalFloatBox->clear();
    m_pSrcGlobalStringBox->clear();

    m_pSrcGlobalBoolBox->setEnabled(false);
    m_pSrcGlobalIntegerBox->setEnabled(false);
    m_pSrcGlobalFloatBox->setEnabled(false);
    m_pSrcGlobalStringBox->setEnabled(false);
    if (t_pGlobalSrcBox)
    {
        t_pGlobalSrcBox->setEnabled(true);
        for (uint32 i = 0; i < t_GlobalVarNames.size(); ++i)
            t_pGlobalSrcBox->addItem(QString::number(i) + ":" + QString::fromStdString(t_GlobalVarNames.at(i)));
    }

    // add variables to dest global variable box
    m_pGlobalVarBox->clear();
    for (uint32 i = 0; i < t_GlobalVarNames.size(); ++i)
        m_pGlobalVarBox->addItem(QString::number(i) + ":" + QString::fromStdString(t_GlobalVarNames.at(i)));

    // local variables
    // modify src local boxes
    m_pSrcLocalBoolBox->clear();
    m_pSrcLocalIntegerBox->clear();
    m_pSrcLocalFloatBox->clear();
    m_pSrcLocalStringBox->clear();

    m_pSrcLocalBoolBox->setEnabled(false);
    m_pSrcLocalIntegerBox->setEnabled(false);
    m_pSrcLocalFloatBox->setEnabled(false);
    m_pSrcLocalStringBox->setEnabled(false);
    if (t_pLocalSrcBox)
    {
        t_pLocalSrcBox->setEnabled(true);
        for (uint32 i = 0; i < t_LocalVarNames.size(); ++i)
            t_pLocalSrcBox->addItem(QString::number(i) + ":" + QString::fromStdString(t_LocalVarNames.at(i)));
    }

    // add variables to dest global variable box
    m_pLocalVarBox->clear();
    for (uint32 i = 0; i < t_LocalVarNames.size(); ++i)
        m_pLocalVarBox->addItem(QString::number(i) + ":" + QString::fromStdString(t_LocalVarNames.at(i)));
}