#include "ObjectDatabaseWindow.h"
#include "moc_ObjectDatabaseWindow.h"
#include "TextureDatabaseWindow.h"
#include "EditorConfig.h"
#include "EventEditorWindow.h"
#include "CustomVariableBoolPageWidget.h"
#include <QtGui/QMessageBox>

using namespace DATABASE;

ObjectDatabaseWindow::ObjectDatabaseWindow(QWidget *p_pParent) : DatabasePageTemplate(p_pParent), Ui_ObjectDatabase()
{
    Ui_ObjectDatabase::setupUi(this);
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "ObjectDatabaseWindow : ";
    m_ResizeObj.AddResizeWidget(m_pType);

    connect(m_pEventEditorButton, SIGNAL(clicked()), this, SLOT(ClickOpenEventEditor()));
    connect(m_pParentAddButton, SIGNAL(clicked()), this, SLOT(ParentObjectAdded()));
    connect(m_pParentRemoveButton, SIGNAL(clicked()), this, SLOT(ParentObjectRemoved()));

    LoadObjects();
}

ObjectDatabaseWindow::~ObjectDatabaseWindow(void)
{
}

void ObjectDatabaseWindow::LoadObjects()
{
    m_pStoreBox->clear();
    m_uilIDCache.clear();
    std::map<uint32, std::string> t_sObjectNames;
    if (Database *t_pDB = Database::Get())
        t_pDB->GetObjectNames(t_sObjectNames);

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        t_pDBOut->GetObjectNames(t_sObjectNames);

        for (std::map<uint32, std::string>::iterator t_itr = t_sObjectNames.begin(); t_itr != t_sObjectNames.end(); ++t_itr)
        {
            // if object is deleted, do not show it in list
            if (t_pDBOut->IsObjectPrototypeDeleted(t_itr->first))
                continue;

            InsertItem(t_itr->first, QString((ToString(t_itr->first) + ":" + t_itr->second).c_str()));
        }
    }
}

void ObjectDatabaseWindow::ClickOpenEventEditor()
{
    if (QWidget *t_pWindow = window())
    {
        if (EventEditorWindow *t_pEE = new EventEditorWindow(t_pWindow))
        {
            t_pEE->show();
            t_pWindow->setEnabled(false);
            t_pEE->setEnabled(true);
        }
    }
}

void ObjectDatabaseWindow::ClickButtonNew()
{
    uint32 t_uiID = GetNewID();
    if (t_uiID == (uint32)(0-1))
    {
        ERROR_LOG(m_sLogLocationName + "Unable to insert new Object. Object ID maximum reached.");
        return;
    }

    BASIC_LOG(m_sLogLocationName + "Insert new Object with ID \"" + ToString(t_uiID) + "\".");

    ClearWidgets();
    ChangeItem(t_uiID);
    InsertItem(t_uiID, QString((ToString(t_uiID) + ":").c_str()));
    m_pStoreBox->setCurrentIndex(m_pStoreBox->findText(QString((ToString(t_uiID) + ":").c_str())));
}

void ObjectDatabaseWindow::ClearWidgets()
{
    DisconnectWidgets();
    DatabasePageTemplate::ClearWidgets();

    m_pTextureBox->clear();
    m_pTextureView->clear();
    m_pNPCAniFrequency->setCurrentIndex(2);
    m_pNPCSpeed->setCurrentIndex(2);
    m_pObjectList->clear();
    m_pParentList->clear();
}

bool ObjectDatabaseWindow::SelectItem(uint32 p_uiID)
{
    ClearWidgets();
    DisconnectWidgets();
    bool t_bSuccess = false;
    if (const ObjectPrototype *t_pProto = DatabaseOutput::GetLatestObjectPrototype(p_uiID))
    {
        // ID
        m_pID->setValue(p_uiID);

        // Object Name
        m_pName->setText(QString(t_pProto->m_sName.c_str()));

        // type
        m_pType->setCurrentIndex(t_pProto->m_uiType);

        // parent object
        FillObjectAndParentBox();

        // textures
        std::string t_sTextureType = t_pProto->GetRightTextureType();
        SetSelectableTextures(t_sTextureType);
        bool t_bTextureSuccess = false;
        if (const SpritePrototype *t_pTextureProto = TextureDatabaseWindow::GetLatestPrototype(t_sTextureType, t_pProto->m_uiTextureID))
        {
            int t_Index = m_pTextureBox->findText(QString((ToString(t_pTextureProto->m_uiID) + ":" + t_pTextureProto->m_sFileName).c_str()));
            if (t_Index != -1)
            {
                m_pTextureBox->setCurrentIndex(t_Index);
                ChangeTextureView(t_sTextureType);
                t_bTextureSuccess = true;
            }
        }

        if (!t_bTextureSuccess)
            m_pTextureBox->setCurrentIndex(0);

        switch (t_pProto->m_uiType)
        {
        case OBJECT_TYPE_NPC:
            // animation frequency
            m_pNPCAniFrequency->setCurrentIndex(t_pProto->ObjectType.NPC.m_uiAnimationFrequency);

            // movement speed
            m_pNPCSpeed->setCurrentIndex(t_pProto->ObjectType.NPC.m_uiMoveSpeed);
            break;
        case OBJECT_TYPE_MAP_OBJECT:
            break;
        default:
            break;
        }

        for (int i = 0; i < m_pCustomVariables->count(); ++i)
        {
            CustomVariablePageTemplateWidget *t_pPage = (CustomVariablePageTemplateWidget *)m_pCustomVariables->widget(i);
            if (!t_pPage)
                continue;

            t_pPage->SetNewData(p_uiID);
        }
        m_pCustomVariables->setCurrentIndex(0);
    }

    ConnectWidgets();
    return t_bSuccess;
}

void ObjectDatabaseWindow::ConnectWidgets()
{
    DatabasePageTemplate::ConnectWidgets();
    connect(m_pType, SIGNAL(currentChanged(int)), this, SLOT(TypeChanged(int)));
    connect(m_pNPCAniFrequency, SIGNAL(currentIndexChanged(int)), this, SLOT(NPCAttributeChanged(int)));
    connect(m_pNPCSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(NPCAttributeChanged(int)));
    connect(m_pTextureBox, SIGNAL(currentIndexChanged(int)), this, SLOT(TextureIndexChanged(int)));
}

void ObjectDatabaseWindow::DisconnectWidgets()
{
    DatabasePageTemplate::DisconnectWidgets();
    disconnect(m_pType, SIGNAL(currentChanged(int)), this, SLOT(TypeChanged(int)));
    disconnect(m_pNPCAniFrequency, SIGNAL(currentIndexChanged(int)), this, SLOT(NPCAttributeChanged(int)));
    disconnect(m_pNPCSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(NPCAttributeChanged(int)));
    disconnect(m_pTextureBox, SIGNAL(currentIndexChanged(int)), this, SLOT(TextureIndexChanged(int)));
}

void ObjectDatabaseWindow::ChangeItem(uint32 p_uiID, bool p_Delete)
{
    ObjectPrototype t_NewProto;
    if (const ObjectPrototype *t_pProto = DatabaseOutput::GetLatestObjectPrototype(p_uiID))
        t_NewProto = *t_pProto;

    // ID
    t_NewProto.m_uiID = p_uiID;

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        if (p_Delete)
        {
            t_pDBOut->DeleteObjectPrototype(t_NewProto);
            IDList::iterator t_Itr = m_uilIDCache.find(p_uiID);
            if (t_Itr != m_uilIDCache.end())
                m_uilIDCache.erase(t_Itr);
        }
        else
        {
            // Type
            t_NewProto.m_uiType     = m_pType->currentIndex();

            // object name
            t_NewProto.m_sName      = m_pName->text().toStdString();

            // add parent objects
            t_NewProto.m_uilParentList.clear();
            for (int t_I = 0; t_I < m_pParentList->count(); ++t_I)
            {
                QListWidgetItem *t_pItem = m_pParentList->item(t_I);
                if (!t_pItem)
                    continue;

                QString t_sID = t_pItem->text();
                t_sID.truncate(t_sID.indexOf(":"));
                t_NewProto.m_uilParentList.insert(t_sID.toUInt());
            }

            // texture ID
            t_NewProto.m_uiTextureID= GetCurrentTextureID();
            switch (t_NewProto.m_uiType)
            {
            case OBJECT_TYPE_NPC:
                // animation frequency
                t_NewProto.ObjectType.NPC.m_uiAnimationFrequency = m_pNPCAniFrequency->currentIndex();

                // movement speed
                t_NewProto.ObjectType.NPC.m_uiMoveSpeed = m_pNPCSpeed->currentIndex();
            case OBJECT_TYPE_MAP_OBJECT:
                break;
            default:
                break;
            }
            t_pDBOut->ChangeObjectPrototype(t_NewProto);
        }
    }
}

uint32 ObjectDatabaseWindow::GetCurrentTextureID()
{
    QString t_sTextureID = m_pTextureBox->itemText(m_pTextureBox->currentIndex());
    t_sTextureID.truncate(t_sTextureID.indexOf(":"));
    return t_sTextureID.toUInt();
}

void ObjectDatabaseWindow::ChangeTextureView(std::string p_sType)
{
    m_pTextureView->clear();
    if (const SpritePrototype *t_pProto = TextureDatabaseWindow::GetLatestPrototype(p_sType, GetCurrentTextureID()))
    {
        if (EditorConfig *t_pConf = EditorConfig::Get())
        {
            if (Database* t_pDB = Database::Get())
                m_pTextureView->setPixmap(QString((t_pConf->GetProjectDirectory()+"/" + t_pDB->GetSpritePath(t_pProto->m_uiSpriteType)+t_pProto->m_sFileName).c_str()));
        }
    }
}

void ObjectDatabaseWindow::SetSelectableTextures(std::string p_sType)
{
    if (p_sType == "")
        return;

    m_pTextureBox->clear();
    std::map<uint32, std::string> t_pTextureNames;
    if (Database *t_pDB = Database::Get())
        t_pDB->GetTextureNames(p_sType, t_pTextureNames);

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        t_pDBOut->GetTextureNames(p_sType, t_pTextureNames);

        QString t_sName;
        m_pTextureBox->addItem(QString("0:None"));
        for (std::map<uint32, std::string>::iterator t_itr = t_pTextureNames.begin(); t_itr != t_pTextureNames.end(); ++t_itr)
        {
            // if texture is deleted, do not show it in list
            if (t_pDBOut->IsSpritePrototypeDeleted(p_sType, t_itr->first))
                continue;

            t_sName = (ToString(t_itr->first) + ":" + t_itr->second).c_str();
            m_pTextureBox->addItem(t_sName);
        }
    }
}

void ObjectDatabaseWindow::TypeChanged(int p_Index)
{
    SetSelectableTextures(ObjectPrototype::GetRightTextureType((OBJECT_TYPE)p_Index));
    ChangeItem(GetCurrentItemID());
}

void ObjectDatabaseWindow::TextureIndexChanged(int p_Index)
{
    ChangeTextureView(ObjectPrototype::GetRightTextureType((OBJECT_TYPE)m_pType->currentIndex()));
    ChangeItem(GetCurrentItemID());
}

void ObjectDatabaseWindow::ParentObjectAdded()
{
    if (!m_pObjectList || !m_pParentList)
        return;

    QListWidgetItem *t_pItem = m_pObjectList->currentItem();
    if (!t_pItem)
        return;

    QString t_sID = t_pItem->text();
    t_sID.truncate(t_sID.indexOf(":"));

    // if inheritor is a parent of the new parent, it results in a loop
    if (IsParentOf(t_sID.toUInt(), GetCurrentItemID()))
    {
        QMessageBox::critical(window(), "RECURSIVE HEREDITY", "This heredity results in a recursive heredity (inheritor is the parent of the new parent)!");
        return;
    }

    m_pParentList->addItem(t_pItem->text());
    delete t_pItem;

    ChangeItem(GetCurrentItemID());
    SelectItem(GetCurrentItemID());
}

void ObjectDatabaseWindow::ParentObjectRemoved()
{
    if (!m_pObjectList || !m_pParentList)
        return;

    QListWidgetItem *t_pItem = m_pParentList->currentItem();
    if (!t_pItem)
        return;

    m_pObjectList->addItem(t_pItem->text());
    delete t_pItem;

    ChangeItem(GetCurrentItemID());
    SelectItem(GetCurrentItemID());
}

bool ObjectDatabaseWindow::IsParentOf(uint32 p_uiInheritorID, uint32 p_uiParentID)
{
    if (!p_uiInheritorID || !p_uiParentID)
        return false;

    if (const ObjectPrototype *t_pProto = DatabaseOutput::GetLatestObjectPrototype(p_uiInheritorID))
    {
        if (t_pProto->IsChildrenOf(p_uiParentID))
            return true;

        for (IDList::const_iterator t_Itr = t_pProto->GetParentList()->begin(); t_Itr != t_pProto->GetParentList()->end(); ++t_Itr)
        {
            if (IsParentOf(*t_Itr, p_uiParentID))
                return true;
        }
    }

    return false;
}

void ObjectDatabaseWindow::FillObjectAndParentBox()
{
    if (!m_pObjectList || !m_pParentList)
        return;

    m_pObjectList->clear();
    m_pParentList->clear();

    std::map<uint32, std::string> t_sObjectNames;
    if (Database *t_pDB = Database::Get())
        t_pDB->GetObjectNames(t_sObjectNames);

    uint32 t_uiCurID = GetCurrentItemID();
    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        t_pDBOut->GetObjectNames(t_sObjectNames);
        const ObjectPrototype *t_pProto = DatabaseOutput::GetLatestObjectPrototype(t_uiCurID);
        if (!t_pProto)
            return;

        // add items in m_pObjectList
        for (std::map<uint32, std::string>::iterator t_Itr = t_sObjectNames.begin(); t_Itr != t_sObjectNames.end(); ++t_Itr)
        {
            if (t_Itr->first == t_uiCurID)
                continue;
            else if (t_pProto->IsChildrenOf(t_Itr->first))
                continue;
            else if (t_pDBOut->IsObjectPrototypeDeleted(t_Itr->first))
                continue;

            m_pObjectList->addItem(QString((ToString(t_Itr->first) + ":" + t_Itr->second).c_str()));
        }

        // add items in m_pParentList
        for (IDList::const_iterator t_Itr = t_pProto->GetParentList()->begin(); t_Itr != t_pProto->GetParentList()->end(); ++t_Itr)
        {
            if (*t_Itr == t_uiCurID)
                continue;
            else if (t_pDBOut->IsObjectPrototypeDeleted(*t_Itr))
                continue;

            std::map<uint32, std::string>::iterator t_ObjItr = t_sObjectNames.find(*t_Itr);
            if (t_ObjItr == t_sObjectNames.end())
                continue;

            m_pParentList->addItem(QString((ToString(t_ObjItr->first) + ":" + t_ObjItr->second).c_str()));
        }
    }
}