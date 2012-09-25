#include "CustomVariableStringPageWidget.h"
#include "moc_CustomVariableStringPageWidget.h"
#include "DatabaseOutput.h"

using namespace DATABASE;

CustomVariableStringPageWidget::CustomVariableStringPageWidget(QWidget *p_pParent) : CustomVariablePageTemplateWidget(p_pParent), Ui_CustomVariableStringPage()
{
    Ui_CustomVariableStringPage::setupUi(this);
}

CustomVariableStringPageWidget::~CustomVariableStringPageWidget(void)
{
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

TVariable<std::string> CustomVariableStringPageWidget::GetVariableFromData()
{
    TVariable<std::string> t_Var;
    t_Var.m_sName  = m_pName->text().toStdString();
    t_Var.m_uiID   = m_pID->value();
    t_Var.m_Value  = m_pDefault->text().toStdString();
    return t_Var;
}

void CustomObjectVariableStringPageWidget::ClickNew()
{
    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
    {
        uint32 t_uiID = t_pDB->AddNewCustomObjectVariable(m_uiOwnerID, VARIABLE_STRING);
        if (t_uiID)
        {
            int t_Index = InsertItem(t_uiID, QString((ToString(t_uiID)+":").c_str()));
            if (t_Index != -1)
                m_pStoreBox->setCurrentIndex(t_Index);
        }
    }
}

void CustomObjectVariableStringPageWidget::LoadItems()
{
    m_pStoreBox->clear();
    const ObjectPrototype *t_pProto = DatabaseOutput::GetLatestObjectPrototype(m_uiOwnerID);
    if (!t_pProto)
        return;

    IDList t_uiParentIDList = DatabaseOutput::GetAllParents(t_pProto);
    for (IDList::iterator t_Itr = t_uiParentIDList.begin(); t_Itr != t_uiParentIDList.end(); ++t_Itr)
    {
        if (const ObjectPrototype *t_pParentProto = DatabaseOutput::GetLatestObjectPrototype(*t_Itr))
        {
            for (VariableStringList::const_iterator t_VarItr = t_pParentProto->m_ObjectStringList.begin(); t_VarItr != t_pParentProto->m_ObjectStringList.end(); ++t_VarItr)
            {
                std::string t_sText = "P:" + ToString(t_pParentProto->m_uiID) + ":" + ToString(t_VarItr->first)+":" + t_VarItr->second.m_sName;
                m_pStoreBox->addItem(QString(t_sText.c_str()));
            }
        }
    }

    for (VariableStringList::const_iterator t_Itr = t_pProto->m_ObjectStringList.begin(); t_Itr != t_pProto->m_ObjectStringList.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}

void CustomObjectVariableStringPageWidget::SelectItem(uint32 p_uiID, uint32 p_uiParentID)
{
    ClearWidgets();
    DisconnectWidgets();
    uint32 t_uiID = 0;
    if (p_uiParentID)
        t_uiID = p_uiParentID;
    else
        t_uiID = m_uiOwnerID;

    const ObjectPrototype *t_pProto = DatabaseOutput::GetLatestObjectPrototype(t_uiID);
    if (!t_pProto)
        return;

    VariableStringList::const_iterator t_Itr = t_pProto->m_ObjectStringList.find(p_uiID);
    if (t_Itr == t_pProto->m_ObjectStringList.end())
        return;

    SetWidgets(t_Itr->second.m_uiID, QString(t_Itr->second.m_sName.c_str()), p_uiParentID ? 0 : 1);

    m_pDefault->setText(QString(t_Itr->second.m_Value.c_str()));
    if (p_uiParentID)
        m_pDefault->setEnabled(false);
    else
        m_pDefault->setEnabled(true);
    ConnectWidgets();
}

void CustomObjectVariableStringPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    ObjectPrototype t_Proto;
    if (const ObjectPrototype *t_pProto = DatabaseOutput::GetLatestObjectPrototype(m_uiOwnerID))
        t_Proto = *t_pProto;
    else
        return;

    TVariable<std::string> t_Variable = GetVariableFromData();

    VariableStringList::iterator t_Itr = t_Proto.m_ObjectStringList.find(t_Variable.m_uiID);
    if (t_Itr != t_Proto.m_ObjectStringList.end())
        t_Itr->second = t_Variable;
    else
        t_Proto.m_ObjectStringList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
        t_pDB->ChangeObjectPrototype(t_Proto);
}

void CustomGlobalVariableStringPageWidget::SelectItem(uint32 p_uiID, uint32 p_uiParentID)
{
    QString t_sText = m_pStoreBox->currentText();
    QString t_sID = t_sText;
    t_sID.truncate(t_sID.indexOf(":"));
    GlobalVariableOutput *t_pGlobalVariables = GlobalVariableOutput::Get();
    if (!t_pGlobalVariables)
        return;

    const TVariable<std::string> *t_Var = t_pGlobalVariables->GetLatestString(t_sID.toUInt());
    if (!t_Var)
        return;

    m_pID->setValue(t_Var->m_uiID);
    m_pName->setText(QString(t_Var->m_sName.c_str()));
    m_pDefault->setText(QString(t_Var->m_Value.c_str()));
}

void CustomGlobalVariableStringPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    TVariable<std::string> t_Var = GetVariableFromData();
    if (GlobalVariableOutput *t_pGVOut = GlobalVariableOutput::Get())
        t_pGVOut->SetVariable(t_Var, p_bDelete);
}

void CustomGlobalVariableStringPageWidget::LoadItems()
{
    GlobalVariables *t_pGlobalVariables = GlobalVariables::Get();
    if (!t_pGlobalVariables)
        return;

    std::map<uint32, std::string> t_VarNames;
    t_pGlobalVariables->GetStringNames(t_VarNames);

    for (std::map<uint32, std::string>::iterator t_Itr = t_VarNames.begin(); t_Itr != t_VarNames.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}

void CustomGlobalVariableStringPageWidget::ClickNew()
{
    GlobalVariableOutput *t_pGVOut = GlobalVariableOutput::Get();
    if (!t_pGVOut)
        return;

    uint32 t_uiNewID = t_pGVOut->GetFreeStringID();

    if (!t_uiNewID)
        return;

    TVariable<std::string> t_NewVar;
    t_NewVar.m_uiID = t_uiNewID;
    t_pGVOut->SetVariable(t_NewVar);
    int t_Index = InsertItem(t_uiNewID, QString((ToString(t_uiNewID) + ":").c_str()));
    if (t_Index == -1)
        return;

    m_pStoreBox->setCurrentIndex(t_Index);
}