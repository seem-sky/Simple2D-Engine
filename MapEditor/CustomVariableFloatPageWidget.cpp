#include "CustomVariableFloatPageWidget.h"
#include "moc_CustomVariableFloatPageWidget.h"
#include "DatabaseWindow.h"

using namespace DATABASE;

CustomVariableFloatPageWidget::CustomVariableFloatPageWidget(QWidget *p_pParent) : CustomVariablePageTemplateWidget(p_pParent), Ui_CustomVariableFloatPage()
{
    Ui_CustomVariableFloatPage::setupUi(this);
}

CustomVariableFloatPageWidget::~CustomVariableFloatPageWidget(void)
{
}

void CustomVariableFloatPageWidget::SelectItem(uint32 p_uiID)
{
    ClearWidgets();
    DisconnectWidgets();
    if (DatabaseWindow *t_pDBWindow = (DatabaseWindow*)window())
    {
        const ObjectPrototype *t_Proto = ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID);
        if (!t_Proto)
            return;

        ObjectFloatList::const_iterator t_Itr = t_Proto->m_ObjectFloatList.find(p_uiID);
        if (t_Itr == t_Proto->m_ObjectFloatList.end())
            return;

        m_pName->setText(QString(t_Itr->second.m_sName.c_str()));
        m_pID->setValue(t_Itr->second.m_uiID);
        m_pDefault->setValue(t_Itr->second.m_Value);
    }
    ConnectWidgets();
}

void CustomVariableFloatPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    ObjectPrototype t_Proto;
    if (ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID))
        t_Proto = *ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID);
    else
        return;

    TObjectVariable<float> t_Variable;
    t_Variable.m_sName = m_pName->text().toStdString();
    t_Variable.m_uiID = m_pID->value();
    t_Variable.m_Value = m_pDefault->value();

    ObjectFloatList::iterator t_Itr = t_Proto.m_ObjectFloatList.find(t_Variable.m_uiID);
    if (t_Itr != t_Proto.m_ObjectFloatList.end())
        t_Itr->second = t_Variable;
    else
        t_Proto.m_ObjectFloatList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
        t_pDB->ChangeObjectPrototype(t_Proto);
}

void CustomVariableFloatPageWidget::ConnectWidgets()
{
    CustomVariablePageTemplateWidget::ConnectWidgets();
    connect(m_pDefault, SIGNAL(currentIndexChanged(int)), this, SLOT(DefaultValueChanged(int)));
}

void CustomVariableFloatPageWidget::DisconnectWidgets()
{
    CustomVariablePageTemplateWidget::DisconnectWidgets();
    disconnect(m_pDefault, SIGNAL(currentIndexChanged(int)), this, SLOT(DefaultValueChanged(int)));
}

void CustomVariableFloatPageWidget::ClearWidgets()
{
    DisconnectWidgets();
    CustomVariablePageTemplateWidget::ClearWidgets();
    m_pDefault->clear();
    ConnectWidgets();
}

void CustomVariableFloatPageWidget::ClickNew()
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

void CustomVariableFloatPageWidget::LoadItems()
{
    const ObjectPrototype *t_pProto = ObjectDatabaseWindow::GetLatestPrototype(m_uiOwnerID);
    if (!t_pProto)
        return;

    m_pStoreBox->clear();
    for (ObjectFloatList::const_iterator t_Itr = t_pProto->m_ObjectFloatList.begin(); t_Itr != t_pProto->m_ObjectFloatList.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}