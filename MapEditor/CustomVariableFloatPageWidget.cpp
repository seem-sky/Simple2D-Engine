#include "CustomVariableFloatPageWidget.h"
#include "moc_CustomVariableFloatPageWidget.h"
#include "DatabaseOutput.h"
#include <GlobalVariables.h>

using namespace DATABASE;

CustomVariableFloatPageWidget::CustomVariableFloatPageWidget(QWidget *p_pParent) : CustomVariablePageTemplateWidget(p_pParent), Ui_CustomVariableFloatPage()
{
    Ui_CustomVariableFloatPage::setupUi(this);
}

CustomVariableFloatPageWidget::~CustomVariableFloatPageWidget(void)
{
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

TVariable<float> CustomVariableFloatPageWidget::GetVariableFromData()
{
    TVariable<float> t_Var;
    t_Var.m_sName  = m_pName->text().toStdString();
    t_Var.m_uiID   = m_pID->value();
    t_Var.m_Value  = m_pDefault->value();
    return t_Var;
}

void CustomObjectVariableFloatPageWidget::ClickNew()
{
    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
    {
        uint32 t_uiID = t_pDB->AddNewCustomObjectVariable(m_uiOwnerID, VARIABLE_FLOAT);
        if (t_uiID)
        {
            int t_Index = InsertItem(t_uiID, QString((ToString(t_uiID)+":").c_str()));
            if (t_Index != -1)
                m_pStoreBox->setCurrentIndex(t_Index);
        }
    }
}

void CustomObjectVariableFloatPageWidget::LoadItems()
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
            for (VariableFloatList::const_iterator t_VarItr = t_pParentProto->m_ObjectFloatList.begin(); t_VarItr != t_pParentProto->m_ObjectFloatList.end(); ++t_VarItr)
            {
                std::string t_sText = "P:" + ToString(t_pParentProto->m_uiID) + ":" + ToString(t_VarItr->first)+":" + t_VarItr->second.m_sName;
                m_pStoreBox->addItem(QString(t_sText.c_str()));
            }
        }
    }

    for (VariableFloatList::const_iterator t_Itr = t_pProto->m_ObjectFloatList.begin(); t_Itr != t_pProto->m_ObjectFloatList.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}

void CustomObjectVariableFloatPageWidget::SelectItem(uint32 p_uiID, uint32 p_uiParentID)
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

    VariableFloatList::const_iterator t_Itr = t_pProto->m_ObjectFloatList.find(p_uiID);
    if (t_Itr == t_pProto->m_ObjectFloatList.end())
        return;

    SetWidgets(t_Itr->second.m_uiID, QString(t_Itr->second.m_sName.c_str()), p_uiParentID ? 0 : 1);

    m_pDefault->setValue(t_Itr->second.m_Value);
    if (p_uiParentID)
        m_pDefault->setEnabled(false);
    else
        m_pDefault->setEnabled(true);
    ConnectWidgets();
}

void CustomObjectVariableFloatPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    ObjectPrototype t_Proto;
    if (const ObjectPrototype *t_pProto = DatabaseOutput::GetLatestObjectPrototype(m_uiOwnerID))
        t_Proto = *t_pProto;
    else
        return;

    TVariable<float> t_Variable = GetVariableFromData();

    VariableFloatList::iterator t_Itr = t_Proto.m_ObjectFloatList.find(t_Variable.m_uiID);
    if (t_Itr != t_Proto.m_ObjectFloatList.end())
        t_Itr->second = t_Variable;
    else
        t_Proto.m_ObjectFloatList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
        t_pDB->ChangeObjectPrototype(t_Proto);
}

void CustomGlobalVariableFloatPageWidget::SelectItem(uint32 p_uiID, uint32 p_uiParentID)
{
    QString t_sText = m_pStoreBox->currentText();
    QString t_sID = t_sText;
    t_sID.truncate(t_sID.indexOf(":"));
    GlobalVariableOutput *t_pGlobalVariables = GlobalVariableOutput::Get();
    if (!t_pGlobalVariables)
        return;

    const TVariable<float> *t_Var = t_pGlobalVariables->GetLatestFloat(t_sID.toUInt());
    if (!t_Var)
        return;

    SetWidgets(t_Var->m_uiID, t_Var->m_sName.c_str());
    m_pDefault->setValue(t_Var->m_Value);
}

void CustomGlobalVariableFloatPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    TVariable<float> t_Var = GetVariableFromData();
    if (GlobalVariableOutput *t_pGVOut = GlobalVariableOutput::Get())
        t_pGVOut->SetVariable(t_Var, p_bDelete);
}

void CustomGlobalVariableFloatPageWidget::LoadItems()
{
    GlobalVariables *t_pGlobalVariables = GlobalVariables::Get();
    if (!t_pGlobalVariables)
        return;

    std::map<uint32, std::string> t_VarNames;
    t_pGlobalVariables->GetFloatNames(t_VarNames);

    for (std::map<uint32, std::string>::iterator t_Itr = t_VarNames.begin(); t_Itr != t_VarNames.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}

void CustomGlobalVariableFloatPageWidget::ClickNew()
{
    GlobalVariableOutput *t_pGVOut = GlobalVariableOutput::Get();
    if (!t_pGVOut)
        return;

    uint32 t_uiNewID = t_pGVOut->GetFreeFloatID();

    if (!t_uiNewID)
        return;

    TVariable<float> t_NewVar;
    t_NewVar.m_uiID = t_uiNewID;
    t_pGVOut->SetVariable(t_NewVar);
    int t_Index = InsertItem(t_uiNewID, QString((ToString(t_uiNewID) + ":").c_str()));
    if (t_Index == -1)
        return;

    m_pStoreBox->setCurrentIndex(t_Index);
}