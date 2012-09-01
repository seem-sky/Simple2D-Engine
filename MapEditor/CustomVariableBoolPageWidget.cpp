#include "CustomVariableBoolPageWidget.h"
#include "moc_CustomVariableBoolPageWidget.h"
#include "DatabaseWindow.h"

using namespace DATABASE;

CustomVariableBoolPageWidget::CustomVariableBoolPageWidget(QWidget *p_pParent) : CustomVariablePageTemplateWidget(p_pParent), Ui_CustomVariableBoolPage()
{
    Ui_CustomVariableBoolPage::setupUi(this);
}

CustomVariableBoolPageWidget::~CustomVariableBoolPageWidget(void)
{
}

void CustomVariableBoolPageWidget::SelectItem(uint32 p_uiID)
{
    ClearWidgets();
    DisconnectWidgets();
    if (DatabaseWindow *t_pDBWindow = (DatabaseWindow*)window())
    {
        const ObjectPrototype *t_Proto = ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID);
        if (!t_Proto)
            return;

        ObjectBoolList::const_iterator t_Itr = t_Proto->m_ObjectBoolList.find(p_uiID);
        if (t_Itr == t_Proto->m_ObjectBoolList.end())
            return;

        m_pName->setText(QString(t_Itr->second.m_sName.c_str()));
        m_pID->setValue(t_Itr->second.m_uiID);
        m_pDefault->setCurrentIndex(t_Itr->second.m_Value ? 1 : 0);
    }
    ConnectWidgets();
}

void CustomVariableBoolPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    ObjectPrototype t_Proto;
    if (ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID))
        t_Proto = *ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID);
    else
        return;

    TObjectVariable<bool> t_Variable;
    t_Variable.m_sName  = m_pName->text().toStdString();
    t_Variable.m_uiID   = m_pID->value();
    if (m_pDefault->currentIndex() != -1)
        t_Variable.m_Value  = m_pDefault->currentIndex();

    ObjectBoolList::iterator t_Itr = t_Proto.m_ObjectBoolList.find(t_Variable.m_uiID);
    if (t_Itr != t_Proto.m_ObjectBoolList.end())
        t_Itr->second = t_Variable;
    else
        t_Proto.m_ObjectBoolList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
        t_pDB->ChangeObjectPrototype(t_Proto);
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

void CustomVariableBoolPageWidget::ClickNew()
{
    if (DatabaseWindow *t_pDBWindow = (DatabaseWindow*)window())
    {
        if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
        {
            uint32 t_uiID = t_pDB->AddNewCustomObjectVariable(m_uiOwnerID, VARIABLE_BOOL);
            if (t_uiID)
            {
                int t_Index = InsertItem(t_uiID, QString((ToString(t_uiID)+":").c_str()));
                if (t_Index != -1)
                    m_pStoreBox->setCurrentIndex(t_Index);
            }
        }
    }
}

void CustomVariableBoolPageWidget::LoadItems()
{
    const ObjectPrototype *t_pProto = ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID);
    if (!t_pProto)
        return;

    m_pStoreBox->clear();
    for (ObjectBoolList::const_iterator t_Itr = t_pProto->m_ObjectBoolList.begin(); t_Itr != t_pProto->m_ObjectBoolList.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}