#include "CustomVariableBoolPageWidget.h"
#include "moc_CustomVariableBoolPageWidget.h"
#include "DatabaseOutput.h"
#include <QtGui/QMessageBox>

using namespace DATABASE;

CustomVariableBoolPageWidget::CustomVariableBoolPageWidget(QWidget *p_pParent) : CustomVariablePageTemplateWidget(p_pParent), Ui_CustomVariableBoolPage()
{
    Ui_CustomVariableBoolPage::setupUi(this);
}

void CustomVariableBoolPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete /* = false */)
{
    if (!m_pVariableHolder)
        return;

    m_pVariableHolder->SetBool(p_uiID, GetVariableFromData());
}

void CustomVariableBoolPageWidget::SelectItem(uint32 p_uiID, uint32 p_uiParentID /* = 0 */)
{
    ClearWidgets();
    DisconnectWidgets();
    uint32 t_uiID = 0;
    const VariableHolder *t_pVarHolder = NULL;
    if (p_uiParentID)
    {
        if (!m_pPrototype)
            return;

        if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
            t_pVarHolder = t_pDBOut->GetVariableHolderFromParent(m_pPrototype->m_PrototypeType, p_uiParentID);
    }
    else
        t_pVarHolder = m_pVariableHolder;

    if (!t_pVarHolder)
        return;

    const TVariable<bool> *t_pVar = t_pVarHolder->GetBool(p_uiID);
    if (!t_pVar)
        return;

    SetWidgets(p_uiID, QString::fromStdString(t_pVar->m_sName), p_uiParentID ? false : true);

    m_pDefault->setCurrentIndex(t_pVar->m_Value ? 1 : 0);
    m_pDefault->setEnabled(p_uiParentID ? false : true);
    ConnectWidgets();
}

void CustomVariableBoolPageWidget::LoadItems()
{
    m_pDefault->setEnabled(false);
    m_pName->setEnabled(false);
    m_pID->setEnabled(false);
    m_pStoreBox->clear();
    if (!m_pVariableHolder)
        return;

    if (m_pPrototype)
    {
        if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
        {
            IDList t_uiParentIDList = t_pDBOut->GetAllParents(m_pPrototype);
            for (IDList::iterator t_Itr = t_uiParentIDList.begin(); t_Itr != t_uiParentIDList.end(); ++t_Itr)
            {
                if (const VariableHolder *t_pVarHolder = t_pDBOut->GetVariableHolderFromParent(m_pPrototype->m_PrototypeType, *t_Itr))
                {
                    std::vector<std::string> t_VarNames;
                    t_pVarHolder->GetBoolNames(t_VarNames);
                    for (uint32 i = 0; i < t_VarNames.size(); ++i)
                        m_pStoreBox->addItem("P:" + QString::number(*t_Itr) + ":" + QString::number(i)+":" + QString::fromStdString(t_VarNames.at(i)));
                }
            }
        }
    }

    std::vector<std::string> t_VarNames;
    m_pVariableHolder->GetBoolNames(t_VarNames);
    for (uint32 i = 0; i < t_VarNames.size(); ++i)
        m_pStoreBox->addItem(QString::number(i)+":" + QString::fromStdString(t_VarNames.at(i)));
}

void CustomVariableBoolPageWidget::ConnectWidgets()
{
    CustomVariablePageTemplateWidget::ConnectWidgets();
    connect(m_pDefault, SIGNAL(currentIndexChanged(int)), this, SLOT(DefaultValueChanged(int)));
}

void CustomVariableBoolPageWidget::DisconnectWidgets()
{
    CustomVariablePageTemplateWidget::DisconnectWidgets();
    disconnect(m_pDefault, SIGNAL(currentIndexChanged(int)), this, SLOT(DefaultValueChanged(int)));
}

void CustomVariableBoolPageWidget::ClearWidgets()
{
    DisconnectWidgets();
    CustomVariablePageTemplateWidget::ClearWidgets();
    m_pDefault->setCurrentIndex(0);
    ConnectWidgets();
}

TVariable<bool> CustomVariableBoolPageWidget::GetVariableFromData()
{
    TVariable<bool> t_Var;
    t_Var.m_sName  = m_pName->text().toStdString();
    if (m_pDefault->currentIndex() != -1)
        t_Var.m_Value  = m_pDefault->currentIndex();

    return t_Var;
}

void CustomVariableBoolPageWidget::ResizeVariableCount(uint32 p_uiCount)
{
    if (!m_pVariableHolder)
        return;

    uint32 t_uiCurCount = m_pVariableHolder->GetBoolCount();
    if (t_uiCurCount < p_uiCount)
        m_pVariableHolder->SetBoolCount(p_uiCount);
    else if (t_uiCurCount > p_uiCount)
    {
        if (QMessageBox::Yes == QMessageBox::question(this, "Are you sure?", "The new count is smaller than the old one. Some variables are going to be deleted.", QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
            m_pVariableHolder->SetBoolCount(p_uiCount);
    }

    LoadItems();
}

uint32 CustomVariableBoolPageWidget::GetVariableCount()
{
    if (!m_pVariableHolder)
        return 0;

    return m_pVariableHolder->GetBoolCount();
}