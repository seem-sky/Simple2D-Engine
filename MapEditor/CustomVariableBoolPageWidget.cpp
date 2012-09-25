#include "CustomVariableBoolPageWidget.h"
#include "moc_CustomVariableBoolPageWidget.h"
#include "DatabaseOutput.h"

using namespace DATABASE;

CustomVariableBoolPageWidget::CustomVariableBoolPageWidget(QWidget *p_pParent) : CustomVariablePageTemplateWidget(p_pParent), Ui_CustomVariableBoolPage()
{
    Ui_CustomVariableBoolPage::setupUi(this);
}

CustomVariableBoolPageWidget::~CustomVariableBoolPageWidget(void)
{
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
    t_Var.m_uiID   = m_pID->value();
    if (m_pDefault->currentIndex() != -1)
        t_Var.m_Value  = m_pDefault->currentIndex();

    return t_Var;
}

void CustomObjectVariableBoolPageWidget::ClickNew()
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

void CustomObjectVariableBoolPageWidget::LoadItems()
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
            for (VariableBoolList::const_iterator t_VarItr = t_pParentProto->m_ObjectBoolList.begin(); t_VarItr != t_pParentProto->m_ObjectBoolList.end(); ++t_VarItr)
            {
                std::string t_sText = "P:" + ToString(t_pParentProto->m_uiID) + ":" + ToString(t_VarItr->first)+":" + t_VarItr->second.m_sName;
                m_pStoreBox->addItem(QString(t_sText.c_str()));
            }
        }
    }

    for (VariableBoolList::const_iterator t_Itr = t_pProto->m_ObjectBoolList.begin(); t_Itr != t_pProto->m_ObjectBoolList.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}

void CustomObjectVariableBoolPageWidget::SelectItem(uint32 p_uiID, uint32 p_uiParentID)
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

    VariableBoolList::const_iterator t_Itr = t_pProto->m_ObjectBoolList.find(p_uiID);
    if (t_Itr == t_pProto->m_ObjectBoolList.end())
        return;

    SetWidgets(t_Itr->second.m_uiID, QString(t_Itr->second.m_sName.c_str()), p_uiParentID ? 0 : 1);

    m_pDefault->setCurrentIndex(t_Itr->second.m_Value ? 1 : 0);
    if (p_uiParentID)
        m_pDefault->setEnabled(false);
    else
        m_pDefault->setEnabled(true);

    ConnectWidgets();
}

void CustomObjectVariableBoolPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    ObjectPrototype t_Proto;
    if (const ObjectPrototype *t_pProto = DatabaseOutput::GetLatestObjectPrototype(m_uiOwnerID))
        t_Proto = *t_pProto;
    else
        return;

    TVariable<bool> t_Variable = GetVariableFromData();

    VariableBoolList::iterator t_Itr = t_Proto.m_ObjectBoolList.find(t_Variable.m_uiID);
    if (t_Itr != t_Proto.m_ObjectBoolList.end())
        t_Itr->second = t_Variable;
    else
        t_Proto.m_ObjectBoolList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
        t_pDB->ChangeObjectPrototype(t_Proto);
}

void CustomGlobalVariableBoolPageWidget::SelectItem(uint32 p_uiID, uint32 p_uiParentID)
{
    QString t_sText = m_pStoreBox->currentText();
    QString t_sID = t_sText;
    t_sID.truncate(t_sID.indexOf(":"));
    GlobalVariableOutput *t_pGlobalVariables = GlobalVariableOutput::Get();
    if (!t_pGlobalVariables)
        return;

    const TVariable<bool> *t_Var = t_pGlobalVariables->GetLatestBool(t_sID.toUInt());
    if (!t_Var)
        return;

    SetWidgets(t_Var->m_uiID, t_Var->m_sName.c_str());
    m_pDefault->setCurrentIndex(t_Var->m_Value);
}

void CustomGlobalVariableBoolPageWidget::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    TVariable<bool> t_Var = GetVariableFromData();
    if (GlobalVariableOutput *t_pGVOut = GlobalVariableOutput::Get())
        t_pGVOut->SetVariable(t_Var, p_bDelete);
}

void CustomGlobalVariableBoolPageWidget::LoadItems()
{
    GlobalVariables *t_pGlobalVariables = GlobalVariables::Get();
    if (!t_pGlobalVariables)
        return;

    std::map<uint32, std::string> t_VarNames;
    t_pGlobalVariables->GetBoolNames(t_VarNames);

    for (std::map<uint32, std::string>::iterator t_Itr = t_VarNames.begin(); t_Itr != t_VarNames.end(); ++t_Itr)
    {
        std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second;
        m_pStoreBox->addItem(QString(t_sText.c_str()));
    }
}

void CustomGlobalVariableBoolPageWidget::ClickNew()
{
    GlobalVariableOutput *t_pGVOut = GlobalVariableOutput::Get();
    if (!t_pGVOut)
        return;

    uint32 t_uiNewID = t_pGVOut->GetFreeBoolID();

    if (!t_uiNewID)
        return;

    TVariable<bool> t_NewVar;
    t_NewVar.m_uiID = t_uiNewID;
    t_pGVOut->SetVariable(t_NewVar);
    int t_Index = InsertItem(t_uiNewID, QString((ToString(t_uiNewID) + ":").c_str()));
    if (t_Index == -1)
        return;

    m_pStoreBox->setCurrentIndex(t_Index);
}