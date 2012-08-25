#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include "MainWindow.h"
#include <QTGui/QFileDialog>
#include <StringAdditions.h>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsPixmapItem>
#include "TransparencyWindow.h"

using namespace DATABASE;
const QString PASSABLE_RIGHT_ICON   = "icons/arrow-right.png";
const QString PASSABLE_LEFT_ICON    = "icons/arrow-left.png";
const QString PASSABLE_UP_ICON      = "icons/arrow-up.png";
const QString PASSABLE_DOWN_ICON    = "icons/arrow-down.png";
const QString PASSABLE_NONE_ICON    = "icons/cross.png";

DatabaseWindow::DatabaseWindow(QMainWindow *p_pParent) : m_DirPressed(PASSABLE_NONE), QMainWindow(p_pParent), Ui_Database()
{
    setupUi(this);

    // check Database
    if (Database *t_pDB = Database::Get())
    {
        if (t_pDB->IsDBEmpty())
            return;
    }
    else
        return;

    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "DatabaseWindow : ";

    TextureName->installEventFilter(this);
    TextureView->installEventFilter(this);
    TilePassabilityIconRight->installEventFilter(this);
    TilePassabilityIconLeft->installEventFilter(this);
    TilePassabilityIconUp->installEventFilter(this);
    TilePassabilityIconDown->installEventFilter(this);

    connect(ButtonOK, SIGNAL(clicked()), this, SLOT(ClickButtonOK()));
    connect(ButtonApply, SIGNAL(clicked()), this, SLOT(ClickButtonApply()));

    connect(NewTexture, SIGNAL(clicked()), this, SLOT(ClickButtonNewTexture()));
    connect(DeleteTexture, SIGNAL(clicked()), this, SLOT(ClickButtonDeleteTexture()));

    connect(TextureBox, SIGNAL(currentIndexChanged(int)), this, SLOT(TextureBoxChangeCur(int)));
    connect(TextureTabs, SIGNAL(currentChanged(int)), this, SLOT(TextureTabChangeCur(int)));
    connect(TextureTransparentColor, SIGNAL(editingFinished()), this, SLOT(TextureTransparencyColorEntered()));

    connect(ObjectsNewBUTTON, SIGNAL(clicked()), this, SLOT(ClickButtonNewObject()));
    connect(ObjectsDeleteBUTTON, SIGNAL(clicked()), this, SLOT(ClickButtonDeleteObject()));
    connect(ObjectsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ObjectBoxChangeCur(int)));
    connect(ButtonObjectsVariablesBoolNew, SIGNAL(clicked()), this, SLOT(ClickButtonNewObjectVariable()));
    connect(ButtonObjectsVariablesIntNew, SIGNAL(clicked()), this, SLOT(ClickButtonNewObjectVariable()));
    connect(ButtonObjectsVariablesFloatNew, SIGNAL(clicked()), this, SLOT(ClickButtonNewObjectVariable()));
    connect(ButtonObjectsVariablesStringNew, SIGNAL(clicked()), this, SLOT(ClickButtonNewObjectVariable()));
    connect(ButtonObjectsVariablesBoolDelete, SIGNAL(clicked()), this, SLOT(ClickButtonDeleteObjectVariable()));
    connect(ButtonObjectsVariablesIntDelete, SIGNAL(clicked()), this, SLOT(ClickButtonDeleteObjectVariable()));
    connect(ButtonObjectsVariablesFloatDelete, SIGNAL(clicked()), this, SLOT(ClickButtonDeleteObjectVariable()));
    connect(ButtonObjectsVariablesStringDelete, SIGNAL(clicked()), this, SLOT(ClickButtonDeleteObjectVariable()));
    connect(ObjectsVariablesBoolBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ObjectVariableIndexChanged(int)));
    connect(ObjectsVariablesIntBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ObjectVariableIndexChanged(int)));
    connect(ObjectsVariablesFloatBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ObjectVariableIndexChanged(int)));
    connect(ObjectsVariablesStringBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ObjectVariableIndexChanged(int)));
    connect(ObjectsCustomVariablesTab, SIGNAL(currentChanged(int)), this, SLOT(ObjectVariableTypeIndexChanged(int)));

    connect(DatabaseSection, SIGNAL(currentChanged(int)), this, SLOT(ChangeDatabaseSection(int)));
    DatabaseSection->setCurrentIndex(0);
}

DatabaseWindow::~DatabaseWindow(void)
{
    if (QWidget *t_pParent = (QWidget*)parent())
        t_pParent->setEnabled(true);
}

void DatabaseWindow::ChangeDatabaseSection(int p_Index)
{
    std::string t_sDBSection = GetCurrentDatabaseSection();
    if (t_sDBSection == "Textures")
        TextureTabChangeCur(TextureTabs->currentIndex());
    else if (t_sDBSection == "Objects")
    {
        ObjectsBox->clear();
        m_uilIDCache.clear();
        if (p_Index == -1)
            return;

        std::map<UINT, std::string> t_sObjectNames;
        if (Database *t_pDB = Database::Get())
            t_pDB->GetObjectNames(t_sObjectNames);

        if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
        {
            t_pDBOut->GetObjectNames(t_sObjectNames);

            for (std::map<UINT, std::string>::iterator t_itr = t_sObjectNames.begin(); t_itr != t_sObjectNames.end(); ++t_itr)
            {
                // if object is deleted, do not show it in list
                if (t_pDBOut->IsObjectPrototypeDeleted(t_itr->first))
                    continue;

                m_uilIDCache.insert(t_itr->first);
                InsertItemInObjectBox(t_itr->first, QString((ToString(t_itr->first) + ":" + t_itr->second).c_str()));
            }
        }
    }
}

void DatabaseWindow::closeEvent(QCloseEvent *event)
{
    event->accept();

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
        t_pDBOut->ClearOutput();

    delete this;
}

void DatabaseWindow::ShowTexturePixmap(QPixmap &p_Pixmap)
{
    TextureView->setPixmap(p_Pixmap);
    QString t_sCurType(GetCurrentTextureType().c_str());
    if (t_sCurType == "Tiles")
        TilePassability->setPixmap(p_Pixmap);
    else if (t_sCurType == "Objects")
    {
        QGraphicsScene *t_Scene = TextureObjectsBoundingShow->scene();
        if (!t_Scene)
        {
            t_Scene = new QGraphicsScene();
            TextureObjectsBoundingShow->setScene(t_Scene);
        }

        t_Scene->addPixmap(p_Pixmap);

        ChangeTextureBounding();
    }
    else if (t_sCurType == "NPCs")
    {
        QGraphicsScene *t_Scene = TextureNPCsBoundingShow->scene();
        if (t_Scene)
        {
            delete TextureObjectsBoundingShow->scene();
            TextureObjectsBoundingShow->setScene(NULL);
        }

        t_Scene = new QGraphicsScene();
        TextureNPCsBoundingShow->setScene(t_Scene);

        t_Scene->addPixmap(p_Pixmap);

        ChangeTextureBounding();
    }
}

void DatabaseWindow::ChangeTextureBounding()
{
    std::string t_sType = GetCurrentTextureType();
    if (t_sType == "Objects")
    {
        if (!TextureObjectsBoundingShow->scene())
            return;

        QList<QGraphicsItem *> t_ItemList = TextureObjectsBoundingShow->items();
        if (t_ItemList.size() >= 2)
            TextureObjectsBoundingShow->scene()->removeItem(t_ItemList[0]);

        if (TextureObjectsBoundingRIGHT->value() > 0 && TextureObjectsBoundingBOTTOM->value() > 0)
            TextureObjectsBoundingShow->scene()->addRect(TextureObjectsBoundingLEFT->value(), TextureObjectsBoundingUP->value(), TextureObjectsBoundingRIGHT->value()-1, TextureObjectsBoundingBOTTOM->value()-1, QPen(QColor(255, 0, 0)));
    }
    else if (t_sType == "NPCs")
    {
        if (!TextureNPCsBoundingShow->scene())
            return;

        QList<QGraphicsItem *> t_ItemList = TextureNPCsBoundingShow->items();
        if (t_ItemList.size() >= 2)
            TextureNPCsBoundingShow->scene()->removeItem(t_ItemList[0]);

        if (TextureNPCsBoundingRIGHT->value() > 0 && TextureNPCsBoundingBOTTOM->value() > 0)
            TextureNPCsBoundingShow->scene()->addRect(TextureNPCsBoundingLEFT->value(), TextureNPCsBoundingUP->value(), TextureNPCsBoundingRIGHT->value()-1, TextureNPCsBoundingBOTTOM->value()-1, QPen(QColor(255, 0, 0)));
    }
}

bool DatabaseWindow::eventFilter(QObject *p_pObj, QEvent *p_pEvent)
{
    /*#####
    # Texture
    #####*/
    // change texture
    if (p_pObj == TextureName)
    {
        if (p_pEvent->type() == QEvent::MouseButtonDblClick)
        {
            QString t_sFileName = QFileDialog::getOpenFileName(this, tr("Open Texture"), ((MainWindow *)parent()) ? ((MainWindow *)parent())->GetProjectDir()+"/MapSprites" : "/MapSprites", tr("Images (*.png)"));
            ShowTexturePixmap(QPixmap(t_sFileName));
            t_sFileName.remove(0, t_sFileName.lastIndexOf("/")+1);
            // change TextureName
            TextureName->setText(t_sFileName);
            // change TextureBox
            QString t_sID = TextureIDLabel->text();
            QString t_sItemText = t_sID+":"+t_sFileName;
            TextureBox->setItemText(TextureBox->currentIndex(), t_sItemText);
            ChangeTexture(t_sID.toUInt());

            return true;
        }
    }
    // open "TransparencyWindow"
    if (p_pObj == TextureView)
    {
        if (p_pEvent->type() == QEvent::MouseButtonDblClick)
        {
            if (const SpritePrototype* t_pProto = GetActualSpritePrototype(GetCurrentTextureType(),
                TextureIDLabel->text().toUInt()))
            {
                if (MainWindow *t_pParent = (MainWindow*)parent())
                {
                    if (Database *t_pDB = Database::Get())
                    {
                        TransparencyWindow *t_pTWindow = new TransparencyWindow(this, QPixmap(t_pParent->GetProjectDir() + QString(("/" +
                            t_pDB->GetSpritePath(t_pProto->m_uiSpriteType) + t_pProto->m_sFileName).c_str())));
                        connect(t_pTWindow, SIGNAL(ColorChosen(QString)), this, SLOT(TextureChangeTransparencyColor(QString)));
                        t_pTWindow->show();
                        setEnabled(false);
                        t_pTWindow->setEnabled(true);
                        return true;
                    }
                }
            }
        }
    }
    /*#####
    # Texture Tiles
    #####*/
    // change tile passability
    else if (p_pObj == TilePassabilityIconRight || p_pObj == TilePassabilityIconLeft || p_pObj == TilePassabilityIconUp || p_pObj == TilePassabilityIconDown)
    {
        PassabilityFlag t_Dir = PASSABLE_NONE;
        if (p_pObj == TilePassabilityIconRight)
            t_Dir = PASSABLE_RIGHT;
        else if (p_pObj == TilePassabilityIconLeft)
            t_Dir = PASSABLE_LEFT;
        else if (p_pObj == TilePassabilityIconDown)
            t_Dir = PASSABLE_DOWN;
        else if (p_pObj == TilePassabilityIconUp)
            t_Dir = PASSABLE_UP;

        if (p_pEvent->type() == QEvent::MouseButtonPress)
        {
            if (const SpritePrototype* t_pProto = GetActualSpritePrototype(GetCurrentTextureType(), TextureIDLabel->text().toUInt()))
            {
                // if is passable
                QString t_sIconName;
                if ((t_pProto->Type.Tile.m_uiPassable & t_Dir) != 0)
                    t_sIconName = PASSABLE_NONE_ICON;

                else
                {
                    switch(t_Dir)
                    {
                    case PASSABLE_RIGHT:
                        t_sIconName = PASSABLE_RIGHT_ICON;
                        break;
                    case PASSABLE_LEFT:
                        t_sIconName = PASSABLE_LEFT_ICON;
                        break;
                    case PASSABLE_UP:
                        t_sIconName = PASSABLE_UP_ICON;
                        break;
                    case PASSABLE_DOWN:
                        t_sIconName = PASSABLE_DOWN_ICON;
                        break;
                    default:
                        break;
                    }
                }

                if (t_sIconName.size())
                    ((QLabel*)p_pObj)->setPixmap(QPixmap(t_sIconName));

                m_DirPressed = t_Dir;

                ChangeTexture(TextureIDLabel->text().toUInt());
                return true;
            }
        }
    }
    else // pass the event on to the parent class
        return QMainWindow::eventFilter(p_pObj, p_pEvent);

    return false;
}

void DatabaseWindow::TextureTabChangeCur(int p_Index)
{
    TextureBox->clear();
    m_uilIDCache.clear();
    if (p_Index == -1)
        return;

    std::map<UINT, std::string> t_sTextureNames;
    if (Database *t_pDB = Database::Get())
        t_pDB->GetTextureNames(TextureTabs->tabText(p_Index).toStdString(), t_sTextureNames);

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        t_pDBOut->GetTextureNames(TextureTabs->tabText(p_Index).toStdString(), t_sTextureNames);

        QString t_sName;
        for (std::map<UINT, std::string>::iterator t_itr = t_sTextureNames.begin(); t_itr != t_sTextureNames.end(); ++t_itr)
        {
            // if texture is deleted, do not show it in list
            if (t_pDBOut->IsSpritePrototypeDeleted(GetCurrentTextureType(), t_itr->first))
                continue;

            m_uilIDCache.insert(t_itr->first);
            t_sName = (ToString(t_itr->first) + ":" + t_itr->second).c_str();
            TextureBox->addItem(t_sName);
        }
    }
}

void DatabaseWindow::TextureBoxChangeCur(int p_Index)
{
    if (p_Index != -1)
    {
        QString t_sID = TextureBox->currentText();
        t_sID.truncate(t_sID.indexOf(":"));
        SelectTexture(TextureTabs->tabText(TextureTabs->currentIndex()).toStdString(), t_sID.toUInt());
    }
}

const SpritePrototype* DatabaseWindow::GetActualSpritePrototype(std::string p_sType, unsigned int p_uiID)
{
    if (Database *t_pDB = Database::Get())
    {
        if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
        {
            const SpritePrototype *t_pProto = t_pDBOut->GetSpritePrototype(p_sType, p_uiID);
            if (!t_pProto)
                t_pProto = t_pDB->GetSpritePrototype(p_sType, p_uiID);

            return t_pProto;
        }
    }

    return NULL;
}

void DatabaseWindow::TextureNPCSpriteCountChanged(int p_Value)
{
    ChangeTexture(TextureIDLabel->text().toUInt());
}

bool DatabaseWindow::SelectTexture(std::string p_sType, UINT p_uiID)
{
    // disconnect this connections, otherwise selection call a sprite change
    /*#####
    # Tiles
    #####*/
    disconnect(TileAutotileCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TileAutotileCheckBoxChanged(int)));

    /*#####
    # Objects
    #####*/
    disconnect(TextureObjectsBoundingUP, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    disconnect(TextureObjectsBoundingLEFT, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    disconnect(TextureObjectsBoundingBOTTOM, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    disconnect(TextureObjectsBoundingRIGHT, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));

    /*#####
    # NPCs
    #####*/
    disconnect(TextureNPCsBoundingUP, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    disconnect(TextureNPCsBoundingLEFT, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    disconnect(TextureNPCsBoundingBOTTOM, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    disconnect(TextureNPCsBoundingRIGHT, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    disconnect(TextureNPCsRows, SIGNAL(valueChanged(int)), this, SLOT(TextureNPCSpriteCountChanged(int)));
    disconnect(TextureNPCsColumns, SIGNAL(valueChanged(int)), this, SLOT(TextureNPCSpriteCountChanged(int)));

    ClearWidgets();

    bool t_bSuccess = false;
    
    if (MainWindow *t_pParent = (MainWindow*)parent())
    {
        if (Database *t_pDB = Database::Get())
        {
            if (const SpritePrototype *t_pProto = GetActualSpritePrototype(p_sType, p_uiID))
            {
                // different texture types
                std::string t_sCurTextureType = GetCurrentTextureType();
                if (t_sCurTextureType == "Tiles")
                {
                    // passability
                    if ((t_pProto->Type.Tile.m_uiPassable & PASSABLE_RIGHT) != 0)
                        TilePassabilityIconRight->setPixmap(QPixmap(PASSABLE_RIGHT_ICON));
                    else
                        TilePassabilityIconRight->setPixmap(QPixmap(PASSABLE_NONE_ICON));

                    if ((t_pProto->Type.Tile.m_uiPassable & PASSABLE_LEFT) != 0)
                        TilePassabilityIconLeft->setPixmap(QPixmap(PASSABLE_LEFT_ICON));
                    else
                        TilePassabilityIconLeft->setPixmap(QPixmap(PASSABLE_NONE_ICON));

                    if ((t_pProto->Type.Tile.m_uiPassable & PASSABLE_UP) != 0)
                        TilePassabilityIconUp->setPixmap(QPixmap(PASSABLE_UP_ICON));
                    else
                        TilePassabilityIconUp->setPixmap(QPixmap(PASSABLE_NONE_ICON));

                    if ((t_pProto->Type.Tile.m_uiPassable & PASSABLE_DOWN) != 0)
                        TilePassabilityIconDown->setPixmap(QPixmap(PASSABLE_DOWN_ICON));
                    else
                        TilePassabilityIconDown->setPixmap(QPixmap(PASSABLE_NONE_ICON));

                    // is autotile
                    TileAutotileCheckBox->setChecked(t_pProto->Type.Tile.m_bAutotile);
                }
                else if (t_sCurTextureType == "Objects")
                {
                    // bounding rect
                    TextureObjectsBoundingLEFT->setValue(t_pProto->Type.Object.m_uiBoundingXBegin);
                    TextureObjectsBoundingRIGHT->setValue(t_pProto->Type.Object.m_uiBoundingXRange);
                    TextureObjectsBoundingUP->setValue(t_pProto->Type.Object.m_uiBoundingYBegin);
                    TextureObjectsBoundingBOTTOM->setValue(t_pProto->Type.Object.m_uiBoundingYRange);
                }
                else if (t_sCurTextureType == "NPCs")
                {
                    // bounding rect
                    TextureNPCsBoundingLEFT->setValue(t_pProto->Type.AnimatedObject.m_uiBoundingXBegin);
                    TextureNPCsBoundingRIGHT->setValue(t_pProto->Type.AnimatedObject.m_uiBoundingXRange);
                    TextureNPCsBoundingUP->setValue(t_pProto->Type.AnimatedObject.m_uiBoundingYBegin);
                    TextureNPCsBoundingBOTTOM->setValue(t_pProto->Type.AnimatedObject.m_uiBoundingYRange);

                    // texture rows
                    TextureNPCsRows->setValue(t_pProto->Type.AnimatedObject.m_uiSpritesY);

                    // texture columns
                    TextureNPCsColumns->setValue(t_pProto->Type.AnimatedObject.m_uiSpritesX);
                }
                ShowTexturePixmap(QPixmap(t_pParent->GetProjectDir() + QString(("/" + t_pDB->GetSpritePath(t_pProto->m_uiSpriteType) + t_pProto->m_sFileName).c_str())));
                TextureName->setText(QString(t_pProto->m_sFileName.c_str()));
                TextureIDLabel->setText(QString(ToString(p_uiID).c_str()));
                TextureTransparentColor->setText(QString(t_pProto->m_sTransparentColor.c_str()));

                t_bSuccess = true;
            }
        }
    }

    /*#####
    # Tiles
    #####*/
    connect(TileAutotileCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TileAutotileCheckBoxChanged(int)));

    /*#####
    # Objects
    #####*/
    connect(TextureObjectsBoundingUP, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    connect(TextureObjectsBoundingLEFT, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    connect(TextureObjectsBoundingBOTTOM, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    connect(TextureObjectsBoundingRIGHT, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));

    /*#####
    # NPCs
    #####*/
    connect(TextureNPCsBoundingUP, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    connect(TextureNPCsBoundingLEFT, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    connect(TextureNPCsBoundingBOTTOM, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    connect(TextureNPCsBoundingRIGHT, SIGNAL(valueChanged(int)), this, SLOT(TextureBoundingChanged(int)));
    connect(TextureNPCsRows, SIGNAL(valueChanged(int)), this, SLOT(TextureNPCSpriteCountChanged(int)));
    connect(TextureNPCsColumns, SIGNAL(valueChanged(int)), this, SLOT(TextureNPCSpriteCountChanged(int)));
    return t_bSuccess;
}

void DatabaseWindow::ClickButtonNewTexture()
{
    Database *t_pDB = Database::Get();
    if (!t_pDB)
        return;

    DatabaseOutput *t_pDBOut = DatabaseOutput::Get();
    if (!t_pDBOut)
        return;

    std::string t_sType = GetCurrentTextureType();
    UINT t_uiID = 1;
    for (; t_uiID < (UINT)(0-1); ++t_uiID)
    {
        std::set<UINT>::iterator t_Itr = m_uilIDCache.find(t_uiID);
        if (t_Itr == m_uilIDCache.end())
            break;
    }
    if (t_uiID == (UINT)(0-1))
    {
        ERROR_LOG(m_sLogLocationName + "Unable to insert new Texture. Texture type" + TextureTabs->tabText(TextureTabs->currentIndex()).toStdString() + 
            "\" reached ID maximum.");
        return;
    }

    BASIC_LOG(m_sLogLocationName + "Insert new Texture of type \"" + TextureTabs->tabText(TextureTabs->currentIndex()).toStdString() + "\" with ID \"" +
        ToString(t_uiID) + "\".");

    m_uilIDCache.insert(t_uiID);
    ClearWidgets();
    ChangeTexture(t_uiID);
    InsertItemInTextureBox(t_uiID, QString((ToString(t_uiID) + ":").c_str()));
    TextureBox->setCurrentIndex(TextureBox->findText(QString((ToString(t_uiID) + ":").c_str())));
}

void DatabaseWindow::ClickButtonDeleteTexture()
{
    BASIC_LOG(m_sLogLocationName + "Delete Texture of type \"" + TextureTabs->tabText(TextureTabs->currentIndex()).toStdString() + "\" with ID \"" + TextureIDLabel->text().toStdString() + "\".");
    ChangeTexture(TextureIDLabel->text().toUInt(), true);
    TextureBox->removeItem(TextureBox->currentIndex());
}

void DatabaseWindow::ChangeTexture(unsigned int p_uiID, bool p_bDel)
{
    SpritePrototype t_NewProto;
    t_NewProto.m_uiID = p_uiID;

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        if (p_bDel)
        {
            t_pDBOut->DeleteSpritePrototype(GetCurrentTextureType(), t_NewProto);
            std::set<UINT>::iterator t_Itr = m_uilIDCache.find(p_uiID);
            if (t_Itr != m_uilIDCache.end())
                m_uilIDCache.erase(t_Itr);
        }
        else
        {
            t_NewProto.m_uiSpriteType       = TextureTabs->currentIndex();
            t_NewProto.m_sFileName          = TextureName->text().toStdString();
            t_NewProto.m_sTransparentColor  = TextureTransparentColor->text().toStdString();

            QString t_sCurTextureType(GetCurrentTextureType().c_str());
            if (t_sCurTextureType == "Tiles")
            {
                // set passability
                switch(m_DirPressed)
                {
                case PASSABLE_DOWN:
                case PASSABLE_LEFT:
                case PASSABLE_RIGHT:
                case PASSABLE_UP:
                    if (const SpritePrototype* t_pOldProto = GetActualSpritePrototype(t_sCurTextureType.toStdString(), t_NewProto.m_uiID))
                        t_NewProto.Type.Tile.m_uiPassable = t_pOldProto->Type.Tile.m_uiPassable ^ m_DirPressed;
                default:
                    m_DirPressed = PASSABLE_NONE;
                    break;
                }

                // set is autotile?
                t_NewProto.Type.Tile.m_bAutotile = TileAutotileCheckBox->isChecked();
            }
            else if (t_sCurTextureType == "Objects")
            {
                // bounding rect
                t_NewProto.Type.Object.m_uiBoundingXBegin = TextureObjectsBoundingLEFT->value();
                t_NewProto.Type.Object.m_uiBoundingXRange = TextureObjectsBoundingRIGHT->value();
                t_NewProto.Type.Object.m_uiBoundingYBegin = TextureObjectsBoundingUP->value();
                t_NewProto.Type.Object.m_uiBoundingYRange = TextureObjectsBoundingBOTTOM->value();
            }
            else if (t_sCurTextureType == "NPCs")
            {
                // bounding rect
                t_NewProto.Type.AnimatedObject.m_uiBoundingXBegin = TextureNPCsBoundingLEFT->value();
                t_NewProto.Type.AnimatedObject.m_uiBoundingXRange = TextureNPCsBoundingRIGHT->value();
                t_NewProto.Type.AnimatedObject.m_uiBoundingYBegin = TextureNPCsBoundingUP->value();
                t_NewProto.Type.AnimatedObject.m_uiBoundingYRange = TextureNPCsBoundingBOTTOM->value();

                // rows
                t_NewProto.Type.AnimatedObject.m_uiSpritesY = TextureNPCsRows->value();

                // columns
                t_NewProto.Type.AnimatedObject.m_uiSpritesY = TextureNPCsColumns->value();
            }

            t_pDBOut->ChangeSpritePrototype(GetCurrentTextureType(), t_NewProto);
        }
    }
}

void DatabaseWindow::ClickButtonApply()
{
    QString t_sDBDir;
    if (MainWindow *t_pParent = (MainWindow*)parent())
        t_sDBDir = t_pParent->GetProjectDir();

    t_sDBDir += "\\Game\\GameDatabase.db";

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        t_pDBOut->SaveChangesTo(t_sDBDir.toStdString());
        if (MainWindow *t_pParent = (MainWindow*)parent())
        {
            QTimer *t_pTimer = new QTimer(this);
            t_pParent->SetNewTimer(t_pTimer);
            t_pParent->SetWindowAction(WINDOW_SAVE_DB);
            connect(t_pTimer,SIGNAL(timeout()), t_pParent, SLOT(CustomUpdate()));
            t_pTimer->start(100);
            setEnabled(false);
        }
    }
}

void DatabaseWindow::ClickButtonOK()
{
    ClickButtonApply();
    if (MainWindow *p_pParent = (MainWindow*)parent())
        connect(p_pParent, SIGNAL(WindowActionDone()), this, SLOT(close()));
}

void DatabaseWindow::ClearWidgets()
{
    std::string t_sCurSection = GetCurrentDatabaseSection();
    /*####
    # Texture Widgets
    ####*/
    if (t_sCurSection == "Textures")
    {
        TextureName->clear();
        TextureView->clear();
        TextureIDLabel->clear();
        TextureTransparentColor->clear();

        // tile specific widgets
        TilePassability->clear();
        TilePassabilityIconDown->clear();
        TilePassabilityIconRight->clear();
        TilePassabilityIconLeft->clear();
        TilePassabilityIconUp->clear();

        // object specific widgets
        TextureObjectsBoundingUP->clear();
        TextureObjectsBoundingLEFT->clear();
        TextureObjectsBoundingRIGHT->clear();
        TextureObjectsBoundingBOTTOM->clear();
        if (TextureObjectsBoundingShow->scene())
        {
            delete TextureObjectsBoundingShow->scene();
            TextureObjectsBoundingShow->setScene(new QGraphicsScene());
        }

        // NPCs specific widgets
        TextureNPCsBoundingBOTTOM->clear();
        TextureNPCsBoundingLEFT->clear();
        TextureNPCsBoundingRIGHT->clear();
        TextureNPCsBoundingUP->clear();
        TextureNPCsColumns->clear();
        TextureNPCsRows->clear();
        if (TextureNPCsBoundingShow->scene())
        {
            delete TextureNPCsBoundingShow->scene();
            TextureNPCsBoundingShow->setScene(new QGraphicsScene());
        }
    }
    /*####
    # Object Widgets
    ####*/
    else if (t_sCurSection == "Objects")
    {
        ObjectsID->clear();
        ObjectsName->clear();
        ObjectsTextureSelectionBox->clear();
        ObjectsTextureView->clear();
    }
}

void DatabaseWindow::InsertItemInTextureBox(UINT p_uiID, QString p_sData)
{
    if ((int)p_uiID > TextureBox->count())
    {
        TextureBox->addItem(p_sData);
        return;
    }

    QString t_sText;
    UINT t_uiID = 0;
    for (int t_uiI = 0; t_uiI < TextureBox->count(); ++t_uiI)
    {
        t_sText = TextureBox->itemText(t_uiI);
        t_sText.truncate(t_sText.indexOf(":"));
        t_uiID = t_sText.toUInt();
        if (p_uiID < t_uiID)
        {
            TextureBox->insertItem(t_uiI, p_sData);
            break;
        }
    }
}

void DatabaseWindow::TileAutotileCheckBoxChanged(int p_State)
{
    ChangeTexture(TextureIDLabel->text().toUInt());
}

void DatabaseWindow::TextureChangeTransparencyColor(QString p_Color)
{
    TextureTransparentColor->setText(p_Color);
    TextureTransparencyColorEntered();
}

void DatabaseWindow::TextureTransparencyColorEntered()
{
    ChangeTexture(TextureIDLabel->text().toUInt());
}

std::string DatabaseWindow::GetCurrentTextureType()
{
    if (GetCurrentDatabaseSection() != "Textures")
        return "";

    return TextureTabs->tabText(TextureTabs->currentIndex()).toStdString();
}

void DatabaseWindow::TextureBoundingChanged(int p_Value)
{
    ChangeTextureBounding();
    ChangeTexture(TextureIDLabel->text().toUInt());
}

std::string DatabaseWindow::GetCurrentDatabaseSection()
{
    return DatabaseSection->tabText(DatabaseSection->currentIndex()).toStdString();
}

bool DatabaseWindow::SelectObject(unsigned int p_uiID)
{
    disconnect(ObjectsName, SIGNAL(editingFinished()), this, SLOT(ChangeObjectName()));
    disconnect(ObjectTabs, SIGNAL(currentChanged(int)), this, SLOT(ChangeObjectType(int)));
    disconnect(ObjectsNPCsAnimationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeObjectNPCAttribute(int)));
    disconnect(ObjectsNPCsSpeedBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeObjectNPCAttribute(int)));
    disconnect(ObjectsTextureSelectionBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeObjectTextureID(int)));

    ClearWidgets();

    bool t_bSuccess = false;
    if (MainWindow *t_pParent = (MainWindow*)parent())
    {
        if (const ObjectPrototype *t_pProto = GetActualObjectPrototype(p_uiID))
        {
            // ID
            ObjectsID->setText(QString(ToString(p_uiID).c_str()));

            // Object Name
            ObjectsName->setText(QString(t_pProto->m_sName.c_str()));

            // type
            ObjectTabs->setCurrentIndex(t_pProto->m_uiType);

            // textures
            std::string t_sTextureType = TextureTabs->tabText(ObjectTabs->tabText(ObjectTabs->currentIndex()) == "NPC" ? SPRITE_TYPE_ANIMATED_OBJECT : SPRITE_TYPE_OBJECT).toStdString();
            ChangeObjectTexturesInBox(t_sTextureType);
            bool t_bTextureSuccess = false;
            if (const SpritePrototype *t_pTextureProto = GetActualSpritePrototype(t_sTextureType, t_pProto->m_uiTextureID))
            {
                int t_Index = ObjectsTextureSelectionBox->findText(QString((ToString(t_pTextureProto->m_uiID) + ":" + t_pTextureProto->m_sFileName).c_str()));
                if (t_Index != -1)
                {
                    ObjectsTextureSelectionBox->setCurrentIndex(t_Index);
                    ChangeObjectTextureID(t_Index, false);
                    t_bTextureSuccess = true;
                }
            }
            
            if (!t_bTextureSuccess)
            {
                ObjectsTextureSelectionBox->setCurrentIndex(0);
                ChangeObjectTextureID(0, true);
            }

            switch (t_pProto->m_uiType)
            {
            case OBJECT_TYPE_NPC:
                // animation frequency
                ObjectsNPCsAnimationBox->setCurrentIndex(t_pProto->ObjectType.NPC.m_uiAnimationFrequency);

                // movement speed
                ObjectsNPCsSpeedBox->setCurrentIndex(t_pProto->ObjectType.NPC.m_uiMoveSpeed);
            case OBJECT_TYPE_MAP_OBJECT:
                break;
            default:
                break;
            }

            ObjectsCustomVariablesTab->setCurrentIndex(0);
            ObjectVariableTypeIndexChanged(0);
        }
    }

    connect(ObjectsTextureSelectionBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeObjectTextureID(int)));
    connect(ObjectTabs, SIGNAL(currentChanged(int)), this, SLOT(ChangeObjectType(int)));
    connect(ObjectsNPCsAnimationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeObjectNPCAttribute(int)));
    connect(ObjectsNPCsSpeedBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeObjectNPCAttribute(int)));
    connect(ObjectsName, SIGNAL(editingFinished()), this, SLOT(ChangeObjectName()));
    return t_bSuccess;
}

std::string DatabaseWindow::GetCurrentObjectType()
{
    if (GetCurrentDatabaseSection() != "Objects")
        return "";

    return ObjectTabs->tabText(ObjectTabs->currentIndex()).toStdString();
}

const ObjectPrototype* DatabaseWindow::GetActualObjectPrototype(UINT p_uiID)
{
    if (Database *t_pDB = Database::Get())
    {
        if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
        {
            const ObjectPrototype *t_pProto = t_pDBOut->GetObjectPrototype(p_uiID);
            if (!t_pProto)
                t_pProto = t_pDB->GetObjectPrototype(p_uiID);

            return t_pProto;
        }
    }

    return NULL;
}

void DatabaseWindow::ChangeObjectName()
{
    QString t_sID = ObjectsID->text();
    QString t_sItemText = t_sID+":"+ObjectsName->text();
    ObjectsBox->setItemText(ObjectsBox->currentIndex(), t_sItemText);
    ChangeObject(ObjectsID->text().toUInt());
}

void DatabaseWindow::ChangeObject(unsigned int p_uiID, bool p_bDel /* = false */)
{
    ObjectPrototype t_NewProto;
    // ID
    t_NewProto.m_uiID = t_NewProto.m_uiID = p_uiID;

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        if (p_bDel)
        {
            t_pDBOut->DeleteObjectPrototype(t_NewProto);
            std::set<UINT>::iterator t_Itr = m_uilIDCache.find(p_uiID);
            if (t_Itr != m_uilIDCache.end())
                m_uilIDCache.erase(t_Itr);
        }
        else
        {
            // Type
            t_NewProto.m_uiType     = ObjectTabs->currentIndex();

            // object name
            t_NewProto.m_sName      = ObjectsName->text().toStdString();

            // texture ID
            t_NewProto.m_uiTextureID  = GetCurrentObjectTextureID();
            switch (t_NewProto.m_uiType)
            {
            case OBJECT_TYPE_NPC:
                // animation frequency
                t_NewProto.ObjectType.NPC.m_uiAnimationFrequency = ObjectsNPCsAnimationBox->currentIndex();

                // movement speed
                t_NewProto.ObjectType.NPC.m_uiMoveSpeed = ObjectsNPCsSpeedBox->currentIndex();
            case OBJECT_TYPE_MAP_OBJECT:
                break;
            default:
                break;
            }
        }
        t_pDBOut->ChangeObjectPrototype(t_NewProto);
    }
}

void DatabaseWindow::ChangeObjectNPCAttribute(int p_Index)
{
    ChangeObject(ObjectsID->text().toUInt());
}

void DatabaseWindow::ChangeObjectTextureID(int p_Index, bool p_bChangeData)
{
    if (p_Index == -1)
        return;

    ObjectsTextureView->clear();
    if (const SpritePrototype *t_pProto = GetActualSpritePrototype(TextureTabs->tabText(ObjectTabs->tabText(ObjectTabs->currentIndex()) == "NPC" ?
            SPRITE_TYPE_ANIMATED_OBJECT : SPRITE_TYPE_OBJECT).toStdString(), GetCurrentObjectTextureID()))
    {
        if (MainWindow *t_pParent = (MainWindow *)parent())
        {
            if (Database* t_pDB = Database::Get())
                ObjectsTextureView->setPixmap(t_pParent->GetProjectDir() + (("/" + t_pDB->GetSpritePath(t_pProto->m_uiSpriteType)+t_pProto->m_sFileName).c_str()));
        }
    }

    if (p_bChangeData)
        ChangeObject(ObjectsID->text().toUInt());
}

unsigned int DatabaseWindow::GetCurrentObjectTextureID()
{
    QString t_sTextureID = ObjectsTextureSelectionBox->itemText(ObjectsTextureSelectionBox->currentIndex());
    t_sTextureID.truncate(t_sTextureID.indexOf(":"));
    return t_sTextureID.toUInt();
}

void DatabaseWindow::ChangeObjectTexturesInBox(std::string p_sType)
{
    ObjectsTextureSelectionBox->clear();
    std::map<UINT, std::string> t_pTextureNames;
    if (Database *t_pDB = Database::Get())
        t_pDB->GetTextureNames(p_sType, t_pTextureNames);

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        t_pDBOut->GetTextureNames(p_sType, t_pTextureNames);

        QString t_sName;
        ObjectsTextureSelectionBox->addItem(QString("0:None"));
        for (std::map<UINT, std::string>::iterator t_itr = t_pTextureNames.begin(); t_itr != t_pTextureNames.end(); ++t_itr)
        {
            // if texture is deleted, do not show it in list
            if (t_pDBOut->IsSpritePrototypeDeleted(GetCurrentTextureType(), t_itr->first))
                continue;

            t_sName = (ToString(t_itr->first) + ":" + t_itr->second).c_str();
            ObjectsTextureSelectionBox->addItem(t_sName);
        }
    }
}

void DatabaseWindow::ObjectBoxChangeCur(int p_Index)
{
    if (p_Index != -1)
    {
        QString t_sID = ObjectsBox->currentText();
        t_sID.truncate(t_sID.indexOf(":"));
        SelectObject(t_sID.toUInt());
    }
}

void DatabaseWindow::ChangeObjectType(int p_Index)
{
    ChangeObjectTexturesInBox(TextureTabs->tabText(ObjectTabs->tabText(ObjectTabs->currentIndex()) == "NPC" ?
                                    SPRITE_TYPE_ANIMATED_OBJECT : SPRITE_TYPE_OBJECT).toStdString());
     ChangeObject(ObjectsID->text().toUInt());
}

void DatabaseWindow::ClickButtonNewObject()
{
    Database *t_pDB = Database::Get();
    if (!t_pDB)
        return;

    DatabaseOutput *t_pDBOut = DatabaseOutput::Get();
    if (!t_pDBOut)
        return;

    UINT t_uiID = 1;
    for (; t_uiID < (UINT)(0-1); ++t_uiID)
    {
        std::set<UINT>::iterator t_Itr = m_uilIDCache.find(t_uiID);
        if (t_Itr == m_uilIDCache.end())
            break;
    }
    if (t_uiID == (UINT)(0-1))
    {
        ERROR_LOG(m_sLogLocationName + "Unable to insert new Object. Object ID maximum reached.");
        return;
    }

    BASIC_LOG(m_sLogLocationName + "Insert new Object with ID \"" + ToString(t_uiID) + "\".");

    m_uilIDCache.insert(t_uiID);
    ClearWidgets();
    ChangeObject(t_uiID);
    InsertItemInObjectBox(t_uiID, QString((ToString(t_uiID) + ":").c_str()));
    ObjectsBox->setCurrentIndex(ObjectsBox->findText(QString((ToString(t_uiID) + ":").c_str())));
}

void DatabaseWindow::ClickButtonDeleteObject()
{
    BASIC_LOG(m_sLogLocationName + "Delete Object with ID \"" + ObjectsID->text().toStdString() + "\".");
    ChangeObject(ObjectsID->text().toUInt(), true);
    ObjectsBox->removeItem(ObjectsBox->currentIndex());
}

void DatabaseWindow::InsertItemInObjectBox(UINT p_uiID, QString p_sData)
{
    if ((int)p_uiID > ObjectsBox->count())
    {
        ObjectsBox->addItem(p_sData);
        return;
    }

    QString t_sText;
    UINT t_uiID = 0;
    for (int t_uiI = 0; t_uiI < ObjectsBox->count(); ++t_uiI)
    {
        t_sText = ObjectsBox->itemText(t_uiI);
        t_sText.truncate(t_sText.indexOf(":"));
        t_uiID = t_sText.toUInt();
        if (p_uiID < t_uiID)
        {
            ObjectsBox->insertItem(t_uiI, p_sData);
            break;
        }
    }
}


/*#####
# Objects - Custom Variables
#####*/
void DatabaseWindow::ClickButtonNewObjectVariable()
{
    CUSTOM_VARIABLE_TYPE t_Type = GetCurrentObjectCustomVariablesType();
    if (t_Type == VARIABLE_NONE)
        return;

    if (QComboBox* t_pBox = GetObjectVariableBox(t_Type))
        NewObjectCustomVariable(t_Type);
}

void DatabaseWindow::ClickButtonDeleteObjectVariable()
{
    CUSTOM_VARIABLE_TYPE t_Type = GetCurrentObjectCustomVariablesType();
    if (t_Type == VARIABLE_NONE)
        return;

    QComboBox* t_pBox = GetObjectVariableBox(t_Type);
    if (!t_pBox)
        return;

    DeleteObjectCustomVariable(t_Type, t_pBox->currentIndex());
}

int DatabaseWindow::InsertItemInObjectCustomVariablesBox(CUSTOM_VARIABLE_TYPE p_Type, UINT p_uiID, QString p_sData)
{
    QComboBox* t_pBox = GetObjectVariableBox(p_Type);
    if (!t_pBox)
        return -1;

    if ((int)p_uiID > t_pBox->count())
    {
        t_pBox->addItem(p_sData);
        return t_pBox->count()-1;
    }

    QString t_sText;
    UINT t_uiID = 0;
    for (int t_uiI = 0; t_uiI < t_pBox->count(); ++t_uiI)
    {
        t_sText = t_pBox->itemText(t_uiI);
        t_sText.truncate(t_sText.indexOf(":"));
        t_uiID = t_sText.toUInt();
        if (p_uiID < t_uiID)
        {
            t_pBox->insertItem(t_uiI, p_sData);
            return t_uiI;
        }
    }

    return -1;
}

void DatabaseWindow::NewObjectCustomVariable(CUSTOM_VARIABLE_TYPE p_Type)
{
    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
    {
        UINT t_uiID = t_pDB->AddNewCustomObjectVariable(ObjectsID->text().toUInt(), p_Type);
        if (t_uiID)
        {
            int t_Index = InsertItemInObjectCustomVariablesBox(p_Type, t_uiID, QString((ToString(t_uiID)+":").c_str()));
            if (t_Index != -1)
            {
                if (QComboBox *t_pBox = GetObjectVariableBox(p_Type))
                    t_pBox->setCurrentIndex(t_Index);
            }
        }
    }
}

void DatabaseWindow::DeleteObjectCustomVariable(CUSTOM_VARIABLE_TYPE p_Type, UINT p_Index)
{
    if (p_Index == -1)
        return;

    if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
    {
        QComboBox* t_pBox = GetObjectVariableBox(p_Type);
        if (!t_pBox)
            return;

        QString t_sText = t_pBox->itemText(p_Index);
        t_sText.truncate(t_sText.indexOf(":"));
        UINT t_uiID = t_sText.toUInt();
        t_pDB->DeleteCustomObjectVariable(ObjectsID->text().toUInt(), p_Type, t_uiID);
        t_pBox->removeItem(p_Index);
    }
}

void DatabaseWindow::SelectObjectCustomVariable(CUSTOM_VARIABLE_TYPE p_Type, UINT p_uiVariableID)
{
    disconnect(ObjectsVariablesBoolID, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    disconnect(ObjectsVariablesBoolDefaultValue, SIGNAL(currentIndexChanged(int)), this, SLOT(ObjectCustomVariableChanged(int)));
    disconnect(ObjectsVariablesBoolName, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    disconnect(ObjectsVariablesIntID, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    disconnect(ObjectsVariablesIntDefaultValue, SIGNAL(valueChanged(int)), this, SLOT(ObjectCustomVariableChanged(int)));
    disconnect(ObjectsVariablesIntName, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    disconnect(ObjectsVariablesFloatID, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    disconnect(ObjectsVariablesFloatDefaultValue, SIGNAL(valueChanged(int)), this, SLOT(ObjectCustomVariableChanged(int)));
    disconnect(ObjectsVariablesFloatName, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    disconnect(ObjectsVariablesStringID, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    disconnect(ObjectsVariablesStringDefaultValue, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    disconnect(ObjectsVariablesStringName, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));

    QString t_sText = ObjectsBox->itemText(ObjectsBox->currentIndex());
    t_sText.truncate(t_sText.indexOf(":"));
    UINT p_uiObjectID = t_sText.toUInt();
    const ObjectPrototype *t_Proto = GetActualObjectPrototype(p_uiObjectID);
    if (!t_Proto)
        return;

    bool t_bSuccess = false;
    switch (p_Type)
    {
    case VARIABLE_BOOL:
        {
            ObjectBoolList::const_iterator t_Itr = t_Proto->m_ObjectBoolList.find(p_uiVariableID);
            if (t_Itr == t_Proto->m_ObjectBoolList.end())
                return;

            ObjectsVariablesBoolName->setText(QString(t_Itr->second.m_sName.c_str()));
            ObjectsVariablesBoolID->setText(QString(ToString(t_Itr->second.m_uiID).c_str()));
            ObjectsVariablesBoolDefaultValue->setCurrentIndex(t_Itr->second.m_Value ? 1 : 0);
            t_bSuccess = true;
            break;
        }
    case VARIABLE_INT:
        {
            ObjectIntegerList::const_iterator t_Itr = t_Proto->m_ObjectIntegerList.find(p_uiVariableID);
            if (t_Itr == t_Proto->m_ObjectIntegerList.end())
                return;

            ObjectsVariablesIntName->setText(QString(t_Itr->second.m_sName.c_str()));
            ObjectsVariablesIntID->setText(QString(ToString(t_Itr->second.m_uiID).c_str()));
            ObjectsVariablesIntDefaultValue->setValue(t_Itr->second.m_Value);
            t_bSuccess = true;
            break;
        }
    case VARIABLE_FLOAT:
        {
            ObjectFloatList::const_iterator t_Itr = t_Proto->m_ObjectFloatList.find(p_uiVariableID);
            if (t_Itr == t_Proto->m_ObjectFloatList.end())
                return;

            ObjectsVariablesFloatName->setText(QString(t_Itr->second.m_sName.c_str()));
            ObjectsVariablesFloatID->setText(QString(ToString(t_Itr->second.m_uiID).c_str()));
            ObjectsVariablesFloatDefaultValue->setValue(t_Itr->second.m_Value);
            t_bSuccess = true;
            break;
        }
    case VARIABLE_STRING:
        {
            ObjectStringList::const_iterator t_Itr = t_Proto->m_ObjectStringList.find(p_uiVariableID);
            if (t_Itr == t_Proto->m_ObjectStringList.end())
                return;

            ObjectsVariablesStringName->setText(QString(t_Itr->second.m_sName.c_str()));
            ObjectsVariablesStringID->setText(QString(ToString(t_Itr->second.m_uiID).c_str()));
            ObjectsVariablesStringDefaultValue->setText(QString(t_Itr->second.m_Value.c_str()));
            t_bSuccess = true;
            break;
        }
    default:
        break;
    }

    connect(ObjectsVariablesBoolID, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    connect(ObjectsVariablesBoolDefaultValue, SIGNAL(currentIndexChanged(int)), this, SLOT(ObjectCustomVariableChanged(int)));
    connect(ObjectsVariablesBoolName, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    connect(ObjectsVariablesIntID, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    connect(ObjectsVariablesIntDefaultValue, SIGNAL(valueChanged(int)), this, SLOT(ObjectCustomVariableChanged(int)));
    connect(ObjectsVariablesIntName, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    connect(ObjectsVariablesFloatID, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    connect(ObjectsVariablesFloatDefaultValue, SIGNAL(valueChanged(int)), this, SLOT(ObjectCustomVariableChanged(int)));
    connect(ObjectsVariablesFloatName, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    connect(ObjectsVariablesStringID, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    connect(ObjectsVariablesStringDefaultValue, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
    connect(ObjectsVariablesStringName, SIGNAL(editingFinished()), this, SLOT(ObjectCustomVariableChanged()));
}

QComboBox* DatabaseWindow::GetObjectVariableBox(CUSTOM_VARIABLE_TYPE p_Type)
{
    QComboBox* t_pBox = NULL;
    switch(p_Type)
    {
    case VARIABLE_BOOL:
        t_pBox = ObjectsVariablesBoolBox;
        break;
    case VARIABLE_INT:
        t_pBox = ObjectsVariablesIntBox;
        break;
    case VARIABLE_FLOAT:
        t_pBox = ObjectsVariablesFloatBox;
        break;
    case VARIABLE_STRING:
        t_pBox = ObjectsVariablesStringBox;
        break;
    default:
        break;
    }

    return t_pBox;
}

CUSTOM_VARIABLE_TYPE DatabaseWindow::GetCurrentObjectCustomVariablesType()
{
    CUSTOM_VARIABLE_TYPE t_Type = VARIABLE_NONE;
    int t_Index = ObjectsCustomVariablesTab->currentIndex();
    if (t_Index == -1)
        return t_Type;

    QString t_sTabText = ObjectsCustomVariablesTab->tabText(t_Index);
    if (t_sTabText == "bool")
        t_Type = VARIABLE_BOOL;
    else if (t_sTabText == "integer")
        t_Type = VARIABLE_INT;
    else if (t_sTabText == "float")
        t_Type = VARIABLE_FLOAT;
    else if (t_sTabText == "string")
        t_Type = VARIABLE_STRING;

    return t_Type;
}

void DatabaseWindow::ChangeObjectCustomVariable(UINT p_uiObjectID, DATABASE::CUSTOM_VARIABLE_TYPE p_Type, UINT p_uiVariableID)
{
    ObjectPrototype t_Proto;
    if (GetActualObjectPrototype(p_uiObjectID))
        t_Proto = *GetActualObjectPrototype(p_uiObjectID);
    else
        return;

    bool t_bSuccess = false;
    switch (p_Type)
    {
    case VARIABLE_BOOL:
        {
            TObjectVariable<bool> t_Variable;
            t_Variable.m_sName  = ObjectsVariablesBoolName->text().toStdString();
            t_Variable.m_uiID   = ObjectsVariablesBoolID->text().toUInt();
            if (ObjectsVariablesBoolDefaultValue->currentIndex() != -1)
                t_Variable.m_Value  = ObjectsVariablesBoolDefaultValue->currentIndex();

            ObjectBoolList::iterator t_Itr = t_Proto.m_ObjectBoolList.find(t_Variable.m_uiID);
            if (t_Itr != t_Proto.m_ObjectBoolList.end())
                t_Itr->second = t_Variable;
            else
                t_Proto.m_ObjectBoolList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

            t_bSuccess = true;
    	    break;
        }
    case VARIABLE_INT:
        {
            TObjectVariable<int> t_Variable;
            t_Variable.m_sName  = ObjectsVariablesIntName->text().toStdString();
            t_Variable.m_uiID   = ObjectsVariablesIntID->text().toUInt();
            t_Variable.m_Value  = ObjectsVariablesIntDefaultValue->value();

            ObjectIntegerList::iterator t_Itr = t_Proto.m_ObjectIntegerList.find(t_Variable.m_uiID);
            if (t_Itr != t_Proto.m_ObjectIntegerList.end())
                t_Itr->second = t_Variable;
            else
                t_Proto.m_ObjectIntegerList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

            t_bSuccess = true;
            break;
        }
    case VARIABLE_FLOAT:
        {
            TObjectVariable<float> t_Variable;
            t_Variable.m_sName  = ObjectsVariablesFloatName->text().toStdString();
            t_Variable.m_uiID   = ObjectsVariablesFloatID->text().toUInt();
            t_Variable.m_Value  = ObjectsVariablesFloatDefaultValue->value();

            ObjectFloatList::iterator t_Itr = t_Proto.m_ObjectFloatList.find(t_Variable.m_uiID);
            if (t_Itr != t_Proto.m_ObjectFloatList.end())
                t_Itr->second = t_Variable;
            else
                t_Proto.m_ObjectFloatList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

            t_bSuccess = true;
            break;
        }
    case VARIABLE_STRING:
        {
            TObjectVariable<std::string> t_Variable;
            t_Variable.m_sName  = ObjectsVariablesStringName->text().toStdString();
            t_Variable.m_uiID   = ObjectsVariablesStringID->text().toUInt();
            t_Variable.m_Value  = ObjectsVariablesStringDefaultValue->text().toStdString();

            ObjectStringList::iterator t_Itr = t_Proto.m_ObjectStringList.find(t_Variable.m_uiID);
            if (t_Itr != t_Proto.m_ObjectStringList.end())
                t_Itr->second = t_Variable;
            else
                t_Proto.m_ObjectStringList.insert(std::make_pair(t_Variable.m_uiID, t_Variable));

            t_bSuccess = true;
            break;
        }
    default:
        break;
    }

    if (t_bSuccess)
    {
        if (DatabaseOutput *t_pDB = DatabaseOutput::Get())
            t_pDB->ChangeObjectPrototype(t_Proto);
    }
}

void DatabaseWindow::ObjectCustomVariableChanged(int p_Unused /* = 0 */)
{
    UINT t_uiObjectID = ObjectsID->text().toUInt();
    UINT t_uiVariableID = 0;
    CUSTOM_VARIABLE_TYPE t_Type = VARIABLE_NONE;
    QString t_sName;
    QString t_sVarType = ObjectsCustomVariablesTab->tabText(ObjectsCustomVariablesTab->currentIndex());
    if (t_sVarType == "bool")
    {
        t_uiVariableID = ObjectsVariablesBoolID->text().toUInt();
        t_sName = ObjectsVariablesBoolName->text();
        t_Type = VARIABLE_BOOL;
    }
    else if (t_sVarType == "integer")
    {
        t_uiVariableID = ObjectsVariablesIntID->text().toUInt();
         t_sName = ObjectsVariablesIntName->text();
        t_Type = VARIABLE_INT;
    }
    else if (t_sVarType == "float")
    {
        t_uiVariableID = ObjectsVariablesFloatID->text().toUInt();
        t_sName = ObjectsVariablesFloatName->text();
        t_Type = VARIABLE_FLOAT;
    }
    else if (t_sVarType == "string")
    {
        t_uiVariableID = ObjectsVariablesStringID->text().toUInt();
         t_sName = ObjectsVariablesStringName->text();
        t_Type = VARIABLE_STRING;
    }

    if (!t_uiVariableID || t_Type == VARIABLE_NONE)
        return;

    if (QComboBox *t_pBox = GetObjectVariableBox(t_Type))
    {
        QString t_sText;
        UINT t_uiID = 0;
        for (int t_uiI = 0; t_uiI < t_pBox->count(); ++t_uiI)
        {
            t_sText = t_pBox->itemText(t_uiI);
            t_sText.truncate(t_sText.indexOf(":"));
            t_uiID = t_sText.toUInt();
            if (t_uiVariableID == t_uiID)
            {
                t_pBox->setItemText(t_uiI, QString((ToString(t_uiVariableID)+":").c_str())+t_sName);
                break;
            }
        }
    }

    ChangeObjectCustomVariable(t_uiObjectID, t_Type, t_uiVariableID);
}

void DatabaseWindow::ObjectVariableTypeIndexChanged(int p_Index)
{
    if (p_Index == -1)
        return;

    const ObjectPrototype *t_pProto = GetActualObjectPrototype(ObjectsID->text().toUInt());
    if (!t_pProto)
        return;

    CUSTOM_VARIABLE_TYPE t_Type = GetCurrentObjectCustomVariablesType();
    if (QComboBox *t_pBox = GetObjectVariableBox(t_Type))
    {
        t_pBox->clear();
        switch (t_Type)
        {
        case VARIABLE_BOOL:
            for (ObjectBoolList::const_iterator t_Itr = t_pProto->m_ObjectBoolList.begin(); t_Itr != t_pProto->m_ObjectBoolList.end(); ++t_Itr)
            {
                std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
                t_pBox->addItem(QString(t_sText.c_str()));
            }
        	break;
        case VARIABLE_INT:
            for (ObjectIntegerList::const_iterator t_Itr = t_pProto->m_ObjectIntegerList.begin(); t_Itr != t_pProto->m_ObjectIntegerList.end(); ++t_Itr)
            {
                std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
                t_pBox->addItem(QString(t_sText.c_str()));
            }
            break;
        case VARIABLE_FLOAT:
            for (ObjectFloatList::const_iterator t_Itr = t_pProto->m_ObjectFloatList.begin(); t_Itr != t_pProto->m_ObjectFloatList.end(); ++t_Itr)
            {
                std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
                t_pBox->addItem(QString(t_sText.c_str()));
            }
            break;
        case VARIABLE_STRING:
            for (ObjectStringList::const_iterator t_Itr = t_pProto->m_ObjectStringList.begin(); t_Itr != t_pProto->m_ObjectStringList.end(); ++t_Itr)
            {
                std::string t_sText = ToString(t_Itr->first)+":"+t_Itr->second.m_sName;
                t_pBox->addItem(QString(t_sText.c_str()));
            }
            break;
        }
        t_pBox->setCurrentIndex(0);
    }
}

void DatabaseWindow::ObjectVariableIndexChanged(int p_Index)
{
    ObjectsVariablesBoolID->clear();
    ObjectsVariablesBoolDefaultValue->setCurrentIndex(0);
    ObjectsVariablesBoolName->clear();
    ObjectsVariablesIntID->clear();
    ObjectsVariablesIntDefaultValue->clear();
    ObjectsVariablesIntName->clear();
    ObjectsVariablesFloatID->clear();
    ObjectsVariablesFloatDefaultValue->clear();
    ObjectsVariablesFloatName->clear();
    ObjectsVariablesStringID->clear();
    ObjectsVariablesStringDefaultValue->clear();
    ObjectsVariablesStringName->clear();

    if (p_Index == -1)
        return;

    CUSTOM_VARIABLE_TYPE t_Type = GetCurrentObjectCustomVariablesType();
    QComboBox *t_pBox = GetObjectVariableBox(t_Type);
    if (!t_pBox)
        return;

    QString t_sText = t_pBox->itemText(p_Index);
    t_sText.truncate(t_sText.indexOf(":"));
    UINT t_uiID = t_sText.toUInt();
    SelectObjectCustomVariable(t_Type, t_uiID);
}