#include "CustomVariableIntPageWidget.h"
#include "moc_CustomVariableIntPageWidget.h"
#include "DatabaseOutput.h"

using namespace DATABASE;

CustomVariableIntPageWidget::CustomVariableIntPageWidget(QWidget *p_pParent) : CustomVariablePageTemplateWidget(p_pParent), Ui_CustomVariableIntPage()
{
    Ui_CustomVariableIntPage::setupUi(this);
}

CustomVariableIntPageWidget::~CustomVariableIntPageWidget(void)
{
}

void CustomVariableIntPageWidget::ConnectWidgets()
{
    CustomVariablePageTemplateWidget::ConnectWidgets();
    connect(m_pDefault, SIGNAL(currentIndexChanged(int)), this, SLOT(DefaultValueChanged(int)));
}

void CustomVariableIntPageWidget::DisconnectWidgets()
{
    CustomVariablePageTemplateWidget::DisconnectWidgets();
    disconnect(m_pDefault, SIGNAL(currentIndexChanged(int)), this, SLOT(DefaultValueChanged(int)));
}

void CustomVariableIntPageWidget::ClearWidgets()
{
    DisconnectWidgets();
    CustomVariablePageTemplateWidget::ClearWidgets();
    m_pDefault->clear();
    ConnectWidgets();
}

TVariable<int> CustomVariableIntPageWidget::GetVariableFromData()
{
    TVariable<int> t_Var;
    t_Var.m_sName  = m_pName->text().toStdString();
    t_Var.m_uiID   = m_pID->value();
    t_Var.m_Value   = m_pID->value();
    return t_Var;
}

void CustomObjectVariableIntPageWidget::ClickNew()
{
    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
    {
        uint32 t_uiID = t_pDB->AddNewCustomObjectVariable(m_uiOwnerID, VARIABLE_INT);
        if (t_uiID)
        {
            int t_Index = InsertItem(t_uiID, QString((ToString(t_uiID)+":").c_str()));
            if (t_Index != -1)
                m_pStoreBox->setCurrentIndex(t_Index);
        }
    }
}

void CustomObjectVariableIntPageWidget::LoadItems()
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
            for (VariableIntegerList::const_iterator t_VarItr = t_pParentProto->m_ObjectIntegerList.begin(); t_VarItr != t_pParentProto->m_ObjectIntegerList.end(); ++t_VarItr)
            {
                std::string t_sText = "P:" + ToString(t_pParentProto->m_uiID) + ":" + ToString(t_VarItr->first)+":" + t_VarItr->second.m_sName;
                m_pStoreBox->addItem(QString(t_sText.c_str()));
            }
        }
    }

    for (VariableIntegerList::const_iterator t_Itr = t_pProto->m_ObjectIntegerList.begin(); t_Itr != t_pProto->m_ObjectIntegerList.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}

void CustomObjectVariableIntPageWidget::SelectItem(uint32 p_uiID, uint32 p_uiParentID)
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

    VariableIntegerList::const_iterator t_Itr = t_pProto->m_ObjectIntegerList.find(p_uiID);
    if (t_Itr == t_pProto->m_ObjectIntegerList.end())
        return;

    SetWidgets(t_Itr->second.m_uiID, QString(t_Itr->second.m_sName.c_str()), p_uiParentID ? 0 : 1);

    m_pDefault->setValue(t_Itr->second.m_Value);
    if (p_uiParentID)
        m_pDefault->setEnabled(false);
    else
        m_pDefault->setEnabled(true);
    ConnectWidgets();
}

void CustomObjectVariableIntPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    ObjectPrototype t_Proto;
    if (const ObjectPrototype *t_pProto = DatabaseOutput::GetLatestObjectPrototype(m_uiOwnerID))
        t_Proto = *t_pProto;
    else
        return;

    TVariable<int> t_Variable = GetVariableFromData();

    VariableIntegerList::iterator t_Itr = t_Proto.m_ObjectIntegerList.find(t_Variable.m_uiID);
    if (t_Itr != t_Proto.m_ObjectIntegerList.end())
        t_Itr->second = t_Variable;
    else
        t_Proto.m_ObjectIntegerList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
        t_pDB->ChangeObjectPrototype(t_Proto);
}

void CustomGlobalVariableIntPageWidget::SelectItem(uint32 p_uiID, uint32 p_uiParentID)
{
    QString t_sText = m_pStoreBox->currentText();
    QString t_sID = t_sText;
    t_sID.truncate(t_sID.indexOf(":"));
    GlobalVariableOutput *t_pGlobalVariables = GlobalVariableOutput::Get();
    if (!t_pGlobalVariables)
        return;

    const TVariable<int> *t_Var = t_pGlobalVariables->GetLatestInteger(t_sID.toUInt());
    if (!t_Var)
        return;

    SetWidgets(t_Var->m_uiID, t_Var->m_sName.c_str());
    m_pDefault->setValue(t_Var->m_Value);
}

void CustomGlobalVariableIntPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    TVariable<int> t_Var = GetVariableFromData();
    if (GlobalVariableOutput *t_pGVOut = GlobalVariableOutput::Get())
        t_pGVOut->SetVariable(t_Var, p_bDelete);
}

void CustomGlobalVariableIntPageWidget::LoadItems()
{
    GlobalVariables *t_pGlobalVariables = GlobalVariables::Get();
    if (!t_pGlobalVariables)
        return;

    std::map<uint32, std::string> t_VarNames;
    t_pGlobalVariables->GetIntegerNames(t_VarNames);

    for (std::map<uint32, std::string>::iterator t_Itr = t_VarNames.begin(); t_Itr != t_VarNames.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}

void CustomGlobalVariableIntPageWidget::ClickNew()
{
    GlobalVariableOutput *t_pGVOut = GlobalVariableOutput::Get();
    if (!t_pGVOut)
        return;

    uint32 t_uiNewID = t_pGVOut->GetFreeIntegerID();

    if (!t_uiNewID)
        return;

    TVariable<int> t_NewVar;
    t_NewVar.m_uiID = t_uiNewID;
    t_pGVOut->SetVariable(t_NewVar);
    int t_Index = InsertItem(t_uiNewID, QString((ToString(t_uiNewID) + ":").c_str()));
    if (t_Index == -1)
        return;

    m_pStoreBox->setCurrentIndex(t_Index);
}