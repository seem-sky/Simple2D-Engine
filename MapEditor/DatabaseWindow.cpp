#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include "MainWindow.h"
#include <QTGui/QFileDialog>
#include <StringAdditions.h>
#include <QtCore/QTimer>
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

    tabWidget->setCurrentIndex(0);
    TextureTabs->setCurrentIndex(0);
    TextureTabChangeCur(0);
}

DatabaseWindow::~DatabaseWindow(void)
{
    if (QWidget *t_pParent = (QWidget*)parent())
        t_pParent->setEnabled(true);
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
}

bool DatabaseWindow::eventFilter(QObject *p_pObj, QEvent *p_pEvent)
{
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
    // change tile passability right
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
        t_pDBOut->GetTextureNames(TextureTabs->tabText(p_Index).toStdString(), t_sTextureNames);

    QString t_sName;
    for (std::map<UINT, std::string>::iterator t_itr = t_sTextureNames.begin(); t_itr != t_sTextureNames.end(); ++t_itr)
    {
        m_uilIDCache.insert(t_itr->first);
        t_sName = (ToString(t_itr->first) + ":" + t_itr->second).c_str();
        TextureBox->addItem(t_sName);
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

bool DatabaseWindow::SelectTexture(std::string p_sType, UINT p_uiID)
{
    // disconnect this connections, otherwise selection call a sprite change
    disconnect(TileAutotileCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TileAutotileCheckBoxChanged(int)));
    disconnect(TextureObjectsBoundingUP, SIGNAL(valueChanged(int)), this, SLOT(TextureObjectsBoundingChanged(int)));
    disconnect(TextureObjectsBoundingLEFT, SIGNAL(valueChanged(int)), this, SLOT(TextureObjectsBoundingChanged(int)));
    disconnect(TextureObjectsBoundingBOTTOM, SIGNAL(valueChanged(int)), this, SLOT(TextureObjectsBoundingChanged(int)));
    disconnect(TextureObjectsBoundingRIGHT, SIGNAL(valueChanged(int)), this, SLOT(TextureObjectsBoundingChanged(int)));

    ClearWidgets();

    bool t_bSuccess = false;
    if (const SpritePrototype *t_pProto = GetActualSpritePrototype(p_sType, p_uiID))
    {
        if (MainWindow *t_pParent = (MainWindow*)parent())
        {
            if (Database *t_pDB = Database::Get())
            {
                ShowTexturePixmap(QPixmap(t_pParent->GetProjectDir() + QString(("/" + t_pDB->GetSpritePath(t_pProto->m_uiSpriteType) + t_pProto->m_sFileName).c_str())));
                TextureName->setText(QString(t_pProto->m_sFileName.c_str()));
                TextureIDLabel->setText(QString(ToString(t_pProto->m_uiID).c_str()));
                TextureTransparentColor->setText(QString(t_pProto->m_sTransparentColor.c_str()));

                // different texture types
                QString t_sCurTextureType = TextureTabs->tabText(TextureTabs->currentIndex());
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
                    TextureObjectsBoundingBOTTOM->setValue(t_pProto->Type.Object.m_uiBoundingXRange);
                }
            }

            t_bSuccess = true;
        }
    }

    connect(TileAutotileCheckBox, SIGNAL(stateChanged(int)), this, SLOT(TileAutotileCheckBoxChanged(int)));
    connect(TextureObjectsBoundingUP, SIGNAL(valueChanged(int)), this, SLOT(TextureObjectsBoundingChanged(int)));
    connect(TextureObjectsBoundingLEFT, SIGNAL(valueChanged(int)), this, SLOT(TextureObjectsBoundingChanged(int)));
    connect(TextureObjectsBoundingBOTTOM, SIGNAL(valueChanged(int)), this, SLOT(TextureObjectsBoundingChanged(int)));
    connect(TextureObjectsBoundingRIGHT, SIGNAL(valueChanged(int)), this, SLOT(TextureObjectsBoundingChanged(int)));
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
    /*####
    # Texture Widgets
    ####*/
    TextureName->clear();
    TextureView->clear();
    TextureIDLabel->clear();
    TextureTransparentColor->clear();

    // tile specific widgets
    TilePassability->clear();
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
    if (tabWidget->tabText(tabWidget->currentIndex()) != "Textures")
        return "";

    return TextureTabs->tabText(TextureTabs->currentIndex()).toStdString();
}

void DatabaseWindow::TextureObjectsBoundingChanged(int p_Value)
{
    ChangeTexture(TextureIDLabel->text().toUInt());
}