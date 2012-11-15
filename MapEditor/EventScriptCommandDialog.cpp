#include "EventScriptCommandDialog.h"
#include "moc_EventScriptCommandDialog.h"
#include "DatabaseOutput.h"
#include "EventEditorWidget.h"

using namespace EVENT_SCRIPT;

/*#####
# EventScriptCommandDialog -> superclass
#####*/
EventScriptCommandDialog::EventScriptCommandDialog(EventScriptCommand *p_pCommand, QWidget *p_pParent) : m_pCommand(p_pCommand), QDialog(p_pParent)
{
    connect(this, SIGNAL(accepted()), this, SLOT(Accept()));
}

/*#####
# EventScriptCommandDialog
#####*/
EventScriptCommentDialog::EventScriptCommentDialog(EventScriptComment *p_pCommand, QWidget *p_pParent) :
EventScriptCommandDialog(p_pCommand, p_pParent), Ui_CommandComment()
{
    setupUi(this);
    m_ResizeObj.AddResizeWidget(m_pText, QPoint(10, 10+m_pButtons->height()));
    m_ResizeObj.AddMoveWidget(m_pButtons, QPoint(10,10));

    connect(this, SIGNAL(accepted()), this, SLOT(Accept()));
}

void EventScriptCommentDialog::LoadValuesFromEventScriptCommand()
{
    if (!m_pCommand)
        return;

    m_pText->setText(QString::fromStdString(((EventScriptComment*)m_pCommand)->GetCommentText()));
}

void EventScriptCommentDialog::Accept()
{
    if (!m_pCommand)
        return;

    ((EventScriptComment*)m_pCommand)->SetCommentText(m_pText->toPlainText().toStdString());
}

/*#####
# EventScriptChangeVariableDialog
#####*/
EventScriptChangeVariableDialog::EventScriptChangeVariableDialog(EventScriptChangeVariable *p_pCommand, QWidget *p_pParent) :
EventScriptCommandDialog(p_pCommand, p_pParent), Ui_CommandChangeVariable()
{
    setupUi(this);
    connect(m_pBool, SIGNAL(toggled(bool)), this, SLOT(ChangeVariableType(bool)));
    connect(m_pInteger, SIGNAL(toggled(bool)), this, SLOT(ChangeVariableType(bool)));
    connect(m_pFloat, SIGNAL(toggled(bool)), this, SLOT(ChangeVariableType(bool)));
    connect(m_pString, SIGNAL(toggled(bool)), this, SLOT(ChangeVariableType(bool)));
    ChangeVariableType(true);

    connect(m_pSet, SIGNAL(toggled(bool)), this, SLOT(ChangeOperator(bool)));
    connect(m_pAdd, SIGNAL(toggled(bool)), this, SLOT(ChangeOperator(bool)));
    connect(m_pSubtract, SIGNAL(toggled(bool)), this, SLOT(ChangeOperator(bool)));
    connect(m_pMultiplicate, SIGNAL(toggled(bool)), this, SLOT(ChangeOperator(bool)));
    connect(m_pDivide, SIGNAL(toggled(bool)), this, SLOT(ChangeOperator(bool)));
    connect(m_pModulo, SIGNAL(toggled(bool)), this, SLOT(ChangeOperator(bool)));
    connect(m_pTrigger, SIGNAL(toggled(bool)), this, SLOT(ChangeOperator(bool)));
}

void EventScriptChangeVariableDialog::ChangeVariableType(bool p_bChecked)
{
    if (!p_bChecked)
        return;

    m_pSet->setEnabled(false);
    m_pAdd->setEnabled(false);
    m_pSubtract->setEnabled(false);
    m_pMultiplicate->setEnabled(false);
    m_pDivide->setEnabled(false);
    m_pModulo->setEnabled(false);
    m_pTrigger->setEnabled(false);

    m_pValueBool->setEnabled(false);
    m_pValueInteger->setEnabled(false);
    m_pValueFloat->setEnabled(false);
    m_pValueString->setEnabled(false);

    const VariableHolder *t_pGlobalVariableHolder = NULL;
    if (DATABASE::DatabaseOutput *t_pDBOut = DATABASE::DatabaseOutput::Get())
        t_pGlobalVariableHolder = t_pDBOut->GetGlobalVariables();

    const VariableHolder *t_pLocalVariableHolder = ((ScriptPage*)parent())->GetVariableHolder();
    if (!t_pGlobalVariableHolder || !t_pLocalVariableHolder)
        return;

    QComboBox *t_pGlobalSrcBox = NULL;
    QComboBox *t_pLocalSrcBox = NULL;
    std::vector<std::string> t_GlobalVarNames;
    std::vector<std::string> t_LocalVarNames;
    if (m_pBool->isChecked())
    {
        t_pGlobalVariableHolder->GetBoolNames(t_GlobalVarNames);
        t_pLocalVariableHolder->GetBoolNames(t_LocalVarNames);
        m_pSet->setEnabled(true);
        m_pTrigger->setEnabled(true);
        m_pSet->setChecked(true);

        m_pValueBool->setEnabled(true);

        t_pGlobalSrcBox = m_pSrcGlobalBoolBox;
        t_pLocalSrcBox = m_pSrcLocalBoolBox;
    }
    else if (m_pInteger->isChecked())
    {
        t_pGlobalVariableHolder->GetIntegerNames(t_GlobalVarNames);
        t_pLocalVariableHolder->GetIntegerNames(t_LocalVarNames);
        m_pSet->setEnabled(true);
        m_pAdd->setEnabled(true);
        m_pSubtract->setEnabled(true);
        m_pMultiplicate->setEnabled(true);
        m_pDivide->setEnabled(true);
        m_pModulo->setEnabled(true);
        m_pSet->setChecked(true);

        m_pValueInteger->setEnabled(true);

        t_pGlobalSrcBox = m_pSrcGlobalIntegerBox;
        t_pLocalSrcBox = m_pSrcLocalIntegerBox;
    }
    else if (m_pFloat->isChecked())
    {
        t_pGlobalVariableHolder->GetFloatNames(t_GlobalVarNames);
        t_pLocalVariableHolder->GetFloatNames(t_LocalVarNames);
        m_pSet->setEnabled(true);
        m_pAdd->setEnabled(true);
        m_pSubtract->setEnabled(true);
        m_pMultiplicate->setEnabled(true);
        m_pDivide->setEnabled(true);
        m_pSet->setChecked(true);

        m_pValueFloat->setEnabled(true);

        t_pGlobalSrcBox = m_pSrcGlobalFloatBox;
        t_pLocalSrcBox = m_pSrcLocalFloatBox;
    }
    else if (m_pString->isChecked())
    {
        t_pGlobalVariableHolder->GetStringNames(t_GlobalVarNames);
        t_pLocalVariableHolder->GetStringNames(t_LocalVarNames);
        m_pSet->setEnabled(true);
        m_pAdd->setEnabled(true);
        m_pSet->setChecked(true);

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

void EventScriptChangeVariableDialog::ChangeOperator(bool p_bChecked)
{
    if (!p_bChecked)
        return;

    m_pSrcTab->setEnabled(!m_pTrigger->isChecked());
}

void EventScriptChangeVariableDialog::Accept()
{
    if (!m_pCommand)
    {
        reject();
        return;
    }

    EventScriptChangeVariable *t_pCommand = (EventScriptChangeVariable*)m_pCommand;

    // set variable type
    if (m_pBool->isChecked())
        t_pCommand->SetVariableType(VARIABLE_BOOL);
    else if (m_pInteger->isChecked())
        t_pCommand->SetVariableType(VARIABLE_INT);
    else if (m_pFloat->isChecked())
        t_pCommand->SetVariableType(VARIABLE_FLOAT);
    else if (m_pString->isChecked())
        t_pCommand->SetVariableType(VARIABLE_STRING);

    // set dest variable
    QString t_sDestTab = m_pDestTab->tabText(m_pDestTab->currentIndex());
    VariableSettings t_DestVar;
    if (t_sDestTab == "Global Variable")
    {
        t_DestVar.m_VarLocalisation = LOCALISATION_GLOBAL;
        if (!m_pGlobalVarBox->count())
        {
            reject();
            return;
        }

        t_DestVar.m_Type.m_GlobalVariable.m_uiVariableID = m_pGlobalVarBox->currentIndex();
    }
    else if (t_sDestTab == "Local Variable")
    {
        t_DestVar.m_VarLocalisation = LOCALISATION_LOCAL;
        if (!m_pLocalVarBox->count())
        {
            reject();
            return;
        }

        t_DestVar.m_Type.m_LocalVariable.m_uiVariableID = m_pLocalVarBox->currentIndex();
    }
    else if (t_sDestTab == "Object Variable")
        t_DestVar.m_VarLocalisation = LOCALISATION_OBJECT;

    t_DestVar.m_VariableType = t_pCommand->GetVariableType();
    t_pCommand->SetDestVariable(t_DestVar);

    // set operator
    if (m_pSet->isChecked())
        t_pCommand->SetOperator(VARIABLE_OPERATOR_SET);
    else if (m_pAdd->isChecked())
        t_pCommand->SetOperator(VARIABLE_OPERATOR_ADD);
    else if (m_pSubtract->isChecked())
        t_pCommand->SetOperator(VARIABLE_OPERATOR_SUBTRACT);
    else if (m_pMultiplicate->isChecked())
        t_pCommand->SetOperator(VARIABLE_OPERATOR_MULTIPLICATE);
    else if (m_pDivide->isChecked())
        t_pCommand->SetOperator(VARIABLE_OPERATOR_DIVIDE);
    else if (m_pModulo->isChecked())
        t_pCommand->SetOperator(VARIABLE_OPERATOR_MODULO);
    else if (m_pTrigger->isChecked())
    {
        t_pCommand->SetOperator(VARIABLE_OPERATOR_TRIGGER);
        return;
    }

    // set src variable
    QString t_sSrcTab = m_pSrcTab->tabText(m_pSrcTab->currentIndex());
    VariableSettings t_SrcVar;
    if (t_sSrcTab == "Value")
    {
        t_SrcVar.m_VarLocalisation = LOCALISATION_VALUE;
        if (m_pBool->isChecked())
            t_SrcVar.m_Type.m_Value.m_VariableType.m_BoolValue = m_pValueBool->currentIndex() == 0 ? false : true;
        else if (m_pInteger->isChecked())
            t_SrcVar.m_Type.m_Value.m_VariableType.m_IntValue = m_pValueInteger->value();
        else if (m_pFloat->isChecked())
            t_SrcVar.m_Type.m_Value.m_VariableType.m_FloatValue = m_pValueFloat->value();
        else if (m_pString->isChecked())
            t_SrcVar.m_Type.m_Value.m_VariableType.m_StringValue = _bstr_t(m_pValueString->text().toStdString().c_str());
    }
    else if (t_sSrcTab == "Global Variable")
    {
        t_SrcVar.m_VarLocalisation = LOCALISATION_GLOBAL;
        switch(t_pCommand->GetVariableType())
        {
        case VARIABLE_BOOL:
            if (!m_pSrcGlobalBoolBox->count())
            {
                reject();
                return;
            }

            t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalBoolBox->currentIndex();
            break;
        case VARIABLE_INT:
            if (!m_pSrcGlobalIntegerBox->count())
            {
                reject();
                return;
            }

            t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalIntegerBox->currentIndex();
            break;
        case VARIABLE_FLOAT:
            if (!m_pSrcGlobalFloatBox->count())
            {
                reject();
                return;
            }

            t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalFloatBox->currentIndex();
            break;
        case VARIABLE_STRING:
            if (!m_pSrcGlobalStringBox->count())
            {
                reject();
                return;
            }

            t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalStringBox->currentIndex();
            break;
        }
    }
    else if (t_sSrcTab == "Local Variable")
    {
        t_SrcVar.m_VarLocalisation = LOCALISATION_LOCAL;
        switch(t_pCommand->GetVariableType())
        {
        case VARIABLE_BOOL:
            if (!m_pSrcLocalBoolBox->count())
            {
                reject();
                return;
            }

            t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalBoolBox->currentIndex();
            break;
        case VARIABLE_INT:
            if (!m_pSrcLocalIntegerBox->count())
            {
                reject();
                return;
            }

            t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalIntegerBox->currentIndex();
            break;
        case VARIABLE_FLOAT:
            if (!m_pSrcLocalFloatBox->count())
            {
                reject();
                return;
            }

            t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalFloatBox->currentIndex();
            break;
        case VARIABLE_STRING:
            if (!m_pSrcLocalStringBox->count())
            {
                reject();
                return;
            }

            t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalStringBox->currentIndex();
            break;
        }
    }
    else if (t_sSrcTab == "Object Variable")
        t_SrcVar.m_VarLocalisation = LOCALISATION_OBJECT;

    t_SrcVar.m_VariableType = t_pCommand->GetVariableType();
    t_pCommand->SetSrcVariable(t_SrcVar);
}

void EventScriptChangeVariableDialog::LoadValuesFromEventScriptCommand()
{
    if (!m_pCommand)
        return;

    EventScriptChangeVariable *t_pCommand = (EventScriptChangeVariable*)m_pCommand;
    // set variable type
    switch(t_pCommand->GetVariableType())
    {
    case VARIABLE_BOOL: m_pBool->setChecked(true); break;
    case VARIABLE_INT: m_pInteger->setChecked(true); break;
    case VARIABLE_FLOAT: m_pFloat->setChecked(true); break;
    case VARIABLE_STRING: m_pString->setChecked(true); break;
    default: return;
    }

    // set operator
    switch(t_pCommand->GetOperator())
    {
    case VARIABLE_OPERATOR_SET: m_pSet->setChecked(true); break;
    case VARIABLE_OPERATOR_ADD: m_pAdd->setChecked(true); break;
    case VARIABLE_OPERATOR_SUBTRACT: m_pSubtract->setChecked(true); break;
    case VARIABLE_OPERATOR_MULTIPLICATE: m_pMultiplicate->setChecked(true); break;
    case VARIABLE_OPERATOR_DIVIDE: m_pDivide->setChecked(true); break;
    case VARIABLE_OPERATOR_MODULO: m_pModulo->setChecked(true); break;
    case VARIABLE_OPERATOR_TRIGGER: m_pTrigger->setChecked(true); break;
    }

    // dest variable
    VariableSettings t_DestVar = t_pCommand->GetDestVariable();
    m_pDestTab->setCurrentIndex(t_DestVar.m_VarLocalisation-2);
    switch(t_DestVar.m_VarLocalisation)
    {
    case LOCALISATION_GLOBAL:
        m_pGlobalVarBox->setCurrentIndex(t_DestVar.m_Type.m_GlobalVariable.m_uiVariableID);
        break;
    case LOCALISATION_LOCAL:
        m_pGlobalVarBox->setCurrentIndex(t_DestVar.m_Type.m_LocalVariable.m_uiVariableID);
        break;
    }

    // set src variable
    VariableSettings t_SrcVar = t_pCommand->GetSrcVariable();
    m_pSrcTab->setCurrentIndex(t_SrcVar.m_VarLocalisation-1);
    switch(t_pCommand->GetVariableType())
    {
    case VARIABLE_BOOL:
        switch(t_SrcVar.m_VarLocalisation)
        {
        case LOCALISATION_GLOBAL: m_pSrcGlobalBoolBox->setCurrentIndex(t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID); break;
        case LOCALISATION_LOCAL: m_pSrcLocalBoolBox->setCurrentIndex(t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID); break;
        case LOCALISATION_VALUE: m_pValueBool->setCurrentIndex(t_SrcVar.m_Type.m_Value.m_VariableType.m_BoolValue ? 1 : 0); break;
        }
        break;
    case VARIABLE_INT:
        switch(t_SrcVar.m_VarLocalisation)
        {
        case LOCALISATION_GLOBAL: m_pSrcGlobalIntegerBox->setCurrentIndex(t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID); break;
        case LOCALISATION_LOCAL: m_pSrcLocalIntegerBox->setCurrentIndex(t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID); break;
        case LOCALISATION_VALUE: m_pValueInteger->setValue(t_SrcVar.m_Type.m_Value.m_VariableType.m_IntValue); break;
        }
        break;
    case VARIABLE_FLOAT:
        switch(t_SrcVar.m_VarLocalisation)
        {
        case LOCALISATION_GLOBAL: m_pSrcGlobalFloatBox->setCurrentIndex(t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID); break;
        case LOCALISATION_LOCAL: m_pSrcLocalFloatBox->setCurrentIndex(t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID); break;
        case LOCALISATION_VALUE: m_pValueFloat->setValue(t_SrcVar.m_Type.m_Value.m_VariableType.m_FloatValue); break;
        }
        break;
    case VARIABLE_STRING:
        switch(t_SrcVar.m_VarLocalisation)
        {
        case LOCALISATION_GLOBAL: m_pSrcGlobalStringBox->setCurrentIndex(t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID); break;
        case LOCALISATION_LOCAL: m_pSrcLocalStringBox->setCurrentIndex(t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID); break;
        case LOCALISATION_VALUE:
            std::string t_sTemp = bstr_t(t_SrcVar.m_Type.m_Value.m_VariableType.m_StringValue);
            m_pValueString->setText(QString::fromStdString(t_sTemp));
            break;
        }
        break;
    }
}

/*#####
# EventScriptIfConditionDialog
#####*/
EventScriptIfConditionDialog::EventScriptIfConditionDialog(EventScriptIfCondition *p_pCommand, QWidget *p_pParent) : EventScriptCommandDialog(p_pCommand, p_pParent),
Ui_CommandIfCondition()
{
    setupUi(this);
    connect(m_pBool, SIGNAL(toggled(bool)), this, SLOT(VariableTypeChanged(bool)));
    connect(m_pInteger, SIGNAL(toggled(bool)), this, SLOT(VariableTypeChanged(bool)));
    connect(m_pFloat, SIGNAL(toggled(bool)), this, SLOT(VariableTypeChanged(bool)));
    connect(m_pString, SIGNAL(toggled(bool)), this, SLOT(VariableTypeChanged(bool)));
    VariableTypeChanged(true);

    connect(m_pType, SIGNAL(currentChanged(int)), this, SLOT(ConditionTypeChanged(int)));
}

void EventScriptIfConditionDialog::LoadValuesFromEventScriptCommand()
{

}

void EventScriptIfConditionDialog::Accept()
{
    if (!m_pCommand)
        return;

    EventScriptIfCondition *t_pCommand = (EventScriptIfCondition*)m_pCommand;
    // set operator
    if (m_pEqual->isChecked())
        t_pCommand->SetOperator(CONDITION_OPERATOR_EQUAL);
    else if (m_pGreaterEqual->isChecked())
        t_pCommand->SetOperator(CONDITION_OPERATOR_GREATER_EQUAL);
    else if (m_pLessEqual->isChecked())
        t_pCommand->SetOperator(CONDITION_OPERATOR_LESS_EQUAL);
    else if (m_pGreater->isChecked())
        t_pCommand->SetOperator(CONDITION_OPERATOR_GREATER);
    else if (m_pLess->isChecked())
        t_pCommand->SetOperator(CONDITION_OPERATOR_LESS);

    QString t_sTabText = m_pType->tabText(m_pType->currentIndex());
    if (t_sTabText == "Variables")
    {
        // variable type
        VariableType t_VarType;
        if (m_pBool)
            t_VarType = VARIABLE_BOOL;
        else if (m_pInteger)
            t_VarType = VARIABLE_INT;
        else if (m_pFloat)
            t_VarType = VARIABLE_FLOAT;
        else if (m_pString)
            t_VarType = VARIABLE_STRING;
        else
        {
            reject();
            return;
        }

        t_pCommand->SetConditionType(CONDITION_TYPE_VARIABLE);
        // set destination variable settings
        VariableSettings t_DestVar;
        t_DestVar.m_VariableType = t_VarType;
        QString t_sDestTabText = m_pDestTab->tabText(m_pDestTab->currentIndex());
        if (t_sDestTabText == "Global Variable")
        {
            t_DestVar.m_VarLocalisation = LOCALISATION_GLOBAL;
            if (!m_pGlobalVarBox->count())
            {
                reject();
                return;
            }
            t_DestVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pGlobalVarBox->currentIndex();
        }
        else if (t_sDestTabText == "Local Variable")
        {
            t_DestVar.m_VarLocalisation = LOCALISATION_LOCAL;
            if (!m_pLocalVarBox->count())
            {
                reject();
                return;
            }
            t_DestVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pLocalVarBox->currentIndex();
        }
        else
        {
            reject();
            return;
        }
        t_pCommand->SetDestinationVariable(t_DestVar);

        // set src variable settings
        VariableSettings t_SrcVar;
        t_SrcVar.m_VariableType = t_VarType;
        QString t_sSrcTabText = m_pSrcTab->tabText(m_pDestTab->currentIndex());
        if (t_sSrcTabText == "Global Variable")
        {
            t_SrcVar.m_VarLocalisation = LOCALISATION_GLOBAL;
            switch(t_VarType)
            {
            case VARIABLE_BOOL:
                if (!m_pSrcGlobalBoolBox->count())
                {
                    reject();
                    return;
                }

                t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalBoolBox->currentIndex();
                break;
            case VARIABLE_INT:
                if (!m_pSrcGlobalIntegerBox->count())
                {
                    reject();
                    return;
                }

                t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalIntegerBox->currentIndex();
                break;
            case VARIABLE_FLOAT:
                if (!m_pSrcGlobalFloatBox->count())
                {
                    reject();
                    return;
                }

                t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalFloatBox->currentIndex();
                break;
            case VARIABLE_STRING:
                if (!m_pSrcGlobalStringBox->count())
                {
                    reject();
                    return;
                }

                t_SrcVar.m_Type.m_GlobalVariable.m_uiVariableID = (uint32)m_pSrcGlobalStringBox->currentIndex();
                break;

            default: reject(); return;
            }
        }
        else if (t_sSrcTabText == "Local Variable")
        {
            t_SrcVar.m_VarLocalisation = LOCALISATION_LOCAL;
            switch(t_VarType)
            {
            case VARIABLE_BOOL:
                if (!m_pSrcLocalBoolBox->count())
                {
                    reject();
                    return;
                }

                t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalBoolBox->currentIndex();
                break;
            case VARIABLE_INT:
                if (!m_pSrcLocalIntegerBox->count())
                {
                    reject();
                    return;
                }

                t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalIntegerBox->currentIndex();
                break;
            case VARIABLE_FLOAT:
                if (!m_pSrcLocalFloatBox->count())
                {
                    reject();
                    return;
                }

                t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalFloatBox->currentIndex();
                break;
            case VARIABLE_STRING:
                if (!m_pSrcLocalStringBox->count())
                {
                    reject();
                    return;
                }

                t_SrcVar.m_Type.m_LocalVariable.m_uiVariableID = (uint32)m_pSrcLocalStringBox->currentIndex();
                break;

            default: reject(); return;
            }
        }
        else if (t_sSrcTabText == "Value")
        {
            t_SrcVar.m_VarLocalisation = LOCALISATION_VALUE;
            if (m_pBool->isChecked())
                t_SrcVar.m_Type.m_Value.m_VariableType.m_BoolValue = m_pValueBool->currentIndex() ? false : true;
            else if (m_pInteger->isChecked())
                t_SrcVar.m_Type.m_Value.m_VariableType.m_IntValue = m_pValueInteger->value();
            else if (m_pFloat->isChecked())
                t_SrcVar.m_Type.m_Value.m_VariableType.m_FloatValue = m_pValueFloat->value();
            else if (m_pString->isChecked())
                t_SrcVar.m_Type.m_Value.m_VariableType.m_StringValue = _bstr_t(m_pValueString->text().toStdString().c_str());
        }
        else
        {
            reject();
            return;
        }
        t_pCommand->SetSourceVariable(t_SrcVar);
    }
}

void EventScriptIfConditionDialog::VariableTypeChanged(bool p_bChecked)
{
    if (!p_bChecked)
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

    const VariableHolder *t_pLocalVariableHolder = ((ScriptPage*)parent())->GetVariableHolder();
    if (!t_pGlobalVariableHolder || !t_pLocalVariableHolder)
        return;

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