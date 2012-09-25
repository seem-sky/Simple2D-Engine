#include "ChangeVariableWindow.h"
#include "moc_ChangeVariableWindow.h"

using namespace EVENT_SCRIPT;

ChangeVariableWindow::ChangeVariableWindow(CommandChangeVariable *p_pCommand, QWidget *p_pParent) : m_pCommand(p_pCommand), QDialog(p_pParent), Ui_CommandChangeVariable()
{
    setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(AcceptVariable()));
    connect(m_pBool, SIGNAL(clicked()), this, SLOT(ChangeVariableType()));
    connect(m_pInteger, SIGNAL(clicked()), this, SLOT(ChangeVariableType()));
    connect(m_pFloat, SIGNAL(clicked()), this, SLOT(ChangeVariableType()));
    connect(m_pString, SIGNAL(clicked()), this, SLOT(ChangeVariableType()));
    ChangeVariableType();

    connect(m_pSet, SIGNAL(clicked()), this, SLOT(ChangeOperator()));
    connect(m_pAdd, SIGNAL(clicked()), this, SLOT(ChangeOperator()));
    connect(m_pSubtract, SIGNAL(clicked()), this, SLOT(ChangeOperator()));
    connect(m_pMultiplicate, SIGNAL(clicked()), this, SLOT(ChangeOperator()));
    connect(m_pDivide, SIGNAL(clicked()), this, SLOT(ChangeOperator()));
    connect(m_pModulo, SIGNAL(clicked()), this, SLOT(ChangeOperator()));
    connect(m_pTrigger, SIGNAL(clicked()), this, SLOT(ChangeOperator()));
}

ChangeVariableWindow::~ChangeVariableWindow(void)
{
}

void ChangeVariableWindow::ChangeVariableType()
{
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

    if (m_pBool->isChecked())
    {
        m_pSet->setEnabled(true);
        m_pTrigger->setEnabled(true);
        m_pSet->setChecked(true);

        m_pValueBool->setEnabled(true);
    }
    else if (m_pInteger->isChecked())
    {
        m_pSet->setEnabled(true);
        m_pAdd->setEnabled(true);
        m_pSubtract->setEnabled(true);
        m_pMultiplicate->setEnabled(true);
        m_pDivide->setEnabled(true);
        m_pModulo->setEnabled(true);
        m_pSet->setChecked(true);

        m_pValueInteger->setEnabled(true);
    }
    else if (m_pFloat->isChecked())
    {
        m_pSet->setEnabled(true);
        m_pAdd->setEnabled(true);
        m_pSubtract->setEnabled(true);
        m_pMultiplicate->setEnabled(true);
        m_pDivide->setEnabled(true);
        m_pSet->setChecked(true);

        m_pValueFloat->setEnabled(true);
    }
    else if (m_pString->isChecked())
    {
        m_pSet->setEnabled(true);
        m_pAdd->setEnabled(true);
        m_pSet->setChecked(true);

        m_pValueString->setEnabled(true);
    }
}

void ChangeVariableWindow::ChangeOperator()
{
    if (m_pTrigger->isChecked())
        m_pSrcTab->setEnabled(false);
    else
        m_pSrcTab->setEnabled(true);
}

void ChangeVariableWindow::AcceptVariable()
{
    if (!m_pCommand)
        return;

    // set variable type
    if (m_pBool->isChecked())
        m_pCommand->SetVariableType(VARIABLE_BOOL);
    else if (m_pInteger->isChecked())
        m_pCommand->SetVariableType(VARIABLE_INT);
    else if (m_pFloat->isChecked())
        m_pCommand->SetVariableType(VARIABLE_FLOAT);
    else if (m_pString->isChecked())
        m_pCommand->SetVariableType(VARIABLE_STRING);

    // set operator
    if (m_pSet->isChecked())
        m_pCommand->SetOperator(OPERATOR_SET);
    else if (m_pAdd->isChecked())
        m_pCommand->SetOperator(OPERATOR_ADD);
    else if (m_pSubtract->isChecked())
        m_pCommand->SetOperator(OPERATOR_SUBTRACT);
    else if (m_pMultiplicate->isChecked())
        m_pCommand->SetOperator(OPERATOR_MULTIPLICATE);
    else if (m_pDivide->isChecked())
        m_pCommand->SetOperator(OPERATOR_DIVIDE);
    else if (m_pModulo->isChecked())
        m_pCommand->SetOperator(OPERATOR_MODULO);
    else if (m_pTrigger->isChecked())
        m_pCommand->SetOperator(OPERATOR_TRIGGER);

    // set dest variable
    QString t_sDestTab = m_pDestTab->tabText(m_pDestTab->currentIndex());
    ChangeConditions t_DestVar;
    if (t_sDestTab == "Global Variable")
    {
        t_DestVar.m_VarLocalisation = LOCALISATION_GLOBAL;
    }
    else if (t_sDestTab == "Local Variable")
    {
        t_DestVar.m_VarLocalisation = LOCALISATION_LOCAL;
    }
    else if (t_sDestTab == "Object Variable")
    {
        t_DestVar.m_VarLocalisation = LOCALISATION_OBJECT;
    }
    m_pCommand->SetDestVariable(t_DestVar);

    // set src variable
    QString t_sSrcTab = m_pSrcTab->tabText(m_pSrcTab->currentIndex());
    ChangeConditions t_SrcVar;
    if (t_sSrcTab == "Value")
    {
        t_SrcVar.m_VarLocalisation = LOCALISATION_VALUE;
        if (m_pBool->isChecked())
            t_SrcVar.Type.m_Value.m_VariableType.m_BoolValue = m_pValueBool->currentIndex() ? false : true;
        else if (m_pInteger->isChecked())
            t_SrcVar.Type.m_Value.m_VariableType.m_IntValue = m_pValueInteger->value();
        else if (m_pFloat->isChecked())
            t_SrcVar.Type.m_Value.m_VariableType.m_FloatValue = m_pValueFloat->value();
        else if (m_pString->isChecked())
            t_SrcVar.Type.m_Value.m_VariableType.m_StringValue = _bstr_t(m_pValueString->text().toStdString().c_str());
    }
    else if (t_sSrcTab == "Global Variable")
    {
        t_SrcVar.m_VarLocalisation = LOCALISATION_GLOBAL;
    }
    else if (t_sSrcTab == "Local Variable")
    {
        t_SrcVar.m_VarLocalisation = LOCALISATION_LOCAL;
    }
    else if (t_sSrcTab == "Object Variable")
    {
        t_SrcVar.m_VarLocalisation = LOCALISATION_OBJECT;
    }
    m_pCommand->SetSrcVariable(t_SrcVar);
}