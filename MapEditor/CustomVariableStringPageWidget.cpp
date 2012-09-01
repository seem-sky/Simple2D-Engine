#include "CustomVariableStringPageWidget.h"
#include "moc_CustomVariableStringPageWidget.h"
#include "DatabaseWindow.h"

using namespace DATABASE;

CustomVariableStringPageWidget::CustomVariableStringPageWidget(QWidget *p_pParent) : CustomVariablePageTemplateWidget(p_pParent), Ui_CustomVariableStringPage()
{
    Ui_CustomVariableStringPage::setupUi(this);
}

CustomVariableStringPageWidget::~CustomVariableStringPageWidget(void)
{
}

void CustomVariableStringPageWidget::SelectItem(uint32 p_uiID)
{
    ClearWidgets();
    DisconnectWidgets();
    if (DatabaseWindow *t_pDBWindow = (DatabaseWindow*)window())
    {
        const ObjectPrototype *t_Proto = ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID);
        if (!t_Proto)
            return;

        ObjectStringList::const_iterator t_Itr = t_Proto->m_ObjectStringList.find(p_uiID);
        if (t_Itr == t_Proto->m_ObjectStringList.end())
            return;

        m_pName->setText(QString(t_Itr->second.m_sName.c_str()));
        m_pID->setValue(t_Itr->second.m_uiID);
        m_pDefault->setText(t_Itr->second.m_Value.c_str());
    }
    ConnectWidgets();
}

void CustomVariableStringPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    ObjectPrototype t_Proto;
    if (ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID))
        t_Proto = *ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID);
    else
        return;

    TObjectVariable<std::string> t_Variable;
    t_Variable.m_sName = m_pName->text().toStdString();
    t_Variable.m_uiID = m_pID->value();
    t_Variable.m_Value = m_pDefault->text().toStdString();

    ObjectStringList::iterator t_Itr = t_Proto.m_ObjectStringList.find(t_Variable.m_uiID);
    if (t_Itr != t_Proto.m_ObjectStringList.end())
        t_Itr->second = t_Variable;
    else
        t_Proto.m_ObjectStringList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
        t_pDB->ChangeObjectPrototype(t_Proto);
}

void CustomVariableStringPageWidget::ConnectWidgets()
{
    CustomVariablePageTemplateWidget::ConnectWidgets();
    connect(m_pDefault, SIGNAL(currentIndexChanged(int)), this, SLOT(DefaultValueChanged(int)));
}

void CustomVariableStringPageWidget::DisconnectWidgets()
{
    CustomVariablePageTemplateWidget::DisconnectWidgets();
    disconnect(m_pDefault, SIGNAL(currentIndexChanged(int)), this, SLOT(DefaultValueChanged(int)));
}

void CustomVariableStringPageWidget::ClearWidgets()
{
    DisconnectWidgets();
    CustomVariablePageTemplateWidget::ClearWidgets();
    m_pDefault->clear();
    ConnectWidgets();
}

void CustomVariableStringPageWidget::ClickNew()
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

void CustomVariableStringPageWidget::LoadItems()
{
    const ObjectPrototype *t_pProto = ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID);
    if (!t_pProto)
        return;

    m_pStoreBox->clear();
    for (ObjectStringList::const_iterator t_Itr = t_pProto->m_ObjectStringList.begin(); t_Itr != t_pProto->m_ObjectStringList.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}