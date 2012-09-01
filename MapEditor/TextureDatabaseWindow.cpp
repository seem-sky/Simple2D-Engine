#include "TextureDatabaseWindow.h"
#include "moc_TextureDatabaseWindow.h"
#include "EditorConfig.h"
#include <QTGui/QFileDialog>
#include "TransparencyWindow.h"

using namespace DATABASE;
const QString PASSABLE_RIGHT_ICON   = "icons/arrow-right.png";
const QString PASSABLE_LEFT_ICON    = "icons/arrow-left.png";
const QString PASSABLE_UP_ICON      = "icons/arrow-up.png";
const QString PASSABLE_DOWN_ICON    = "icons/arrow-down.png";
const QString PASSABLE_NONE_ICON    = "icons/cross.png";

TextureDatabaseWindow::TextureDatabaseWindow(QWidget *p_pParent) : DatabasePageTemplate(p_pParent), m_DirPressed(PASSABLE_NONE)
{
    Ui_TextureDatabase::setupUi(this);
    m_pName->setReadOnly(true);
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "TextureDatabaseWindow : ";
    m_ResizeObj.AddResizeWidget(m_pTab);

    m_pView->installEventFilter(this);
    m_pName->installEventFilter(this);
    m_pPassabilityDown->installEventFilter(this);
    m_pPassabilityUp->installEventFilter(this);
    m_pPassabilityRight->installEventFilter(this);
    m_pPassabilityLeft->installEventFilter(this);

    connect(m_pTab, SIGNAL(currentChanged(int)), this, SLOT(CurrentTabChanged(int)));
    m_pTab->setCurrentIndex(-1);
    m_pTab->setCurrentIndex(0);
}

TextureDatabaseWindow::~TextureDatabaseWindow(void)
{
}

void TextureDatabaseWindow::ClickButtonNew()
{
    std::string t_sType = GetCurrentType();
    uint32 t_uiID = GetNewID();
    if (t_uiID == (uint32)(0-1))
    {
        ERROR_LOG(m_sLogLocationName + "Unable to insert new Texture. Texture type" + m_pTab->tabText(m_pTab->currentIndex()).toStdString() + 
            "\" reached ID maximum.");
        return;
    }

    BASIC_LOG(m_sLogLocationName + "Insert new Texture of type \"" + m_pTab->tabText(m_pTab->currentIndex()).toStdString() + "\" with ID \"" +
        ToString(t_uiID) + "\".");

    ClearWidgets();
    ChangeItem(t_uiID);
    InsertItem(t_uiID, QString((ToString(t_uiID) + ":").c_str()));
    m_pStoreBox->setCurrentIndex(m_pStoreBox->findText(QString((ToString(t_uiID) + ":").c_str())));
}

void TextureDatabaseWindow::ChangeItem(uint32 p_uiID, bool p_bDelete)
{
    QString t_sCurTextureType(GetCurrentType().c_str());
    SpritePrototype t_NewProto;
    t_NewProto.m_uiID = p_uiID;

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        if (p_bDelete)
        {
            t_pDBOut->DeleteSpritePrototype(t_sCurTextureType.toStdString(), t_NewProto);
            std::set<uint32>::iterator t_Itr = m_uilIDCache.find(p_uiID);
            if (t_Itr != m_uilIDCache.end())
                m_uilIDCache.erase(t_Itr);
        }
        else
        {
            t_NewProto.m_uiSpriteType       = m_pTab->currentIndex();
            t_NewProto.m_sFileName          = m_pName->text().toStdString();
            t_NewProto.m_sTransparencyColor = m_pTransparencyColor->text().toStdString();

            if (t_sCurTextureType == "Tiles")
            {
                // set passability
                switch(m_DirPressed)
                {
                case PASSABLE_DOWN:
                case PASSABLE_LEFT:
                case PASSABLE_RIGHT:
                case PASSABLE_UP:
                    if (const SpritePrototype* t_pOldProto = GetLatestPrototype(t_sCurTextureType.toStdString(), t_NewProto.m_uiID))
                        t_NewProto.Type.Tile.m_uiPassable = t_pOldProto->Type.Tile.m_uiPassable ^ m_DirPressed;
                default:
                    m_DirPressed = PASSABLE_NONE;
                    break;
                }

                // set is autotile?
                t_NewProto.Type.Tile.m_bAutotile = m_pIsAutotile->isChecked();
            }
            else if (t_sCurTextureType == "Objects")
            {
                // bounding rect
                t_NewProto.Type.Object.m_uiBoundingXBegin = m_pObjectBoundingLeft->value();
                t_NewProto.Type.Object.m_uiBoundingXRange = m_pObjectBoundingRight->value();
                t_NewProto.Type.Object.m_uiBoundingYBegin = m_pObjectBoundingUp->value();
                t_NewProto.Type.Object.m_uiBoundingYRange = m_pObjectBoundingBottom->value();
            }
            else if (t_sCurTextureType == "NPCs")
            {
                // bounding rect
                t_NewProto.Type.AnimatedObject.m_uiBoundingXBegin = m_pNPCBoundingLeft->value();
                t_NewProto.Type.AnimatedObject.m_uiBoundingXRange = m_pNPCBoundingRight->value();
                t_NewProto.Type.AnimatedObject.m_uiBoundingYBegin = m_pNPCBoundingUp->value();
                t_NewProto.Type.AnimatedObject.m_uiBoundingYRange = m_pNPCBoundingBottom->value();

                // rows
                t_NewProto.Type.AnimatedObject.m_uiSpritesY = m_pNPCRow->value();

                // columns
                t_NewProto.Type.AnimatedObject.m_uiSpritesY = m_pNPCColumn->value();
            }

            t_pDBOut->ChangeSpritePrototype(t_sCurTextureType.toStdString(), t_NewProto);
        }
    }
}

bool TextureDatabaseWindow::SelectItem(uint32 p_uiID)
{
    ClearWidgets();
    DisconnectWidgets();
    bool t_bSuccess = false;
    std::string t_sCurTextureType = GetCurrentType();
    if (Database *t_pDB = Database::Get())
    {
        if (const SpritePrototype *t_pProto = GetLatestPrototype(t_sCurTextureType, p_uiID))
        {
            // different texture types
            if (t_sCurTextureType == "Tiles")
            {
                // passability
                if ((t_pProto->Type.Tile.m_uiPassable & PASSABLE_RIGHT) != 0)
                    m_pPassabilityRight->setPixmap(QPixmap(PASSABLE_RIGHT_ICON));
                else
                    m_pPassabilityRight->setPixmap(QPixmap(PASSABLE_NONE_ICON));

                if ((t_pProto->Type.Tile.m_uiPassable & PASSABLE_LEFT) != 0)
                    m_pPassabilityLeft->setPixmap(QPixmap(PASSABLE_LEFT_ICON));
                else
                    m_pPassabilityLeft->setPixmap(QPixmap(PASSABLE_NONE_ICON));

                if ((t_pProto->Type.Tile.m_uiPassable & PASSABLE_UP) != 0)
                    m_pPassabilityUp->setPixmap(QPixmap(PASSABLE_UP_ICON));
                else
                    m_pPassabilityUp->setPixmap(QPixmap(PASSABLE_NONE_ICON));

                if ((t_pProto->Type.Tile.m_uiPassable & PASSABLE_DOWN) != 0)
                    m_pPassabilityDown->setPixmap(QPixmap(PASSABLE_DOWN_ICON));
                else
                    m_pPassabilityDown->setPixmap(QPixmap(PASSABLE_NONE_ICON));

                // is autotile
                m_pIsAutotile->setChecked(t_pProto->Type.Tile.m_bAutotile);
            }
            else if (t_sCurTextureType == "Objects")
            {
                // bounding rect
                m_pObjectBoundingLeft->setValue(t_pProto->Type.Object.m_uiBoundingXBegin);
                m_pObjectBoundingRight->setValue(t_pProto->Type.Object.m_uiBoundingXRange);
                m_pObjectBoundingUp->setValue(t_pProto->Type.Object.m_uiBoundingYBegin);
                m_pObjectBoundingBottom->setValue(t_pProto->Type.Object.m_uiBoundingYRange);
            }
            else if (t_sCurTextureType == "NPCs")
            {
                // bounding rect
                m_pNPCBoundingLeft->setValue(t_pProto->Type.AnimatedObject.m_uiBoundingXBegin);
                m_pNPCBoundingRight->setValue(t_pProto->Type.AnimatedObject.m_uiBoundingXRange);
                m_pNPCBoundingUp->setValue(t_pProto->Type.AnimatedObject.m_uiBoundingYBegin);
                m_pNPCBoundingBottom->setValue(t_pProto->Type.AnimatedObject.m_uiBoundingYRange);

                // texture rows
                m_pNPCRow->setValue(t_pProto->Type.AnimatedObject.m_uiSpritesY);

                // texture columns
                m_pNPCColumn->setValue(t_pProto->Type.AnimatedObject.m_uiSpritesX);
            }

            QString t_sProjectDir;
            if (EditorConfig *t_pConf = EditorConfig::Get())
                SetTexturePixmap(QPixmap(QString((t_pConf->GetProjectDirectory()+"/" + t_pDB->GetSpritePath(t_pProto->m_uiSpriteType) + t_pProto->m_sFileName).c_str())));
            m_pName->setText(QString(t_pProto->m_sFileName.c_str()));
            m_pID->setValue(p_uiID);
            m_pTransparencyColor->setText(QString(t_pProto->m_sTransparencyColor.c_str()));

            t_bSuccess = true;
        }
    }

    ConnectWidgets();
    return t_bSuccess;
}

void TextureDatabaseWindow::ConnectWidgets()
{
    DatabasePageTemplate::ConnectWidgets();
    connect(m_pTransparencyColor, SIGNAL(editingFinished()), this, SLOT(TileAutotileStateChanged(int)));

    /*#####
    # Tiles
    #####*/
    connect(m_pIsAutotile, SIGNAL(stateChanged(int)), this, SLOT(TileAutotileStateChanged(int)));

    /*#####
    # Objects
    #####*/
    connect(m_pObjectBoundingUp, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    connect(m_pObjectBoundingLeft, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    connect(m_pObjectBoundingBottom, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    connect(m_pObjectBoundingRight, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));

    /*#####
    # NPCs
    #####*/
    connect(m_pNPCBoundingBottom, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    connect(m_pNPCBoundingUp, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    connect(m_pNPCBoundingLeft, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    connect(m_pNPCBoundingRight, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    connect(m_pNPCRow, SIGNAL(valueChanged(int)), this, SLOT(TextureNPCSpriteCountChanged(int)));
    connect(m_pNPCColumn, SIGNAL(valueChanged(int)), this, SLOT(TextureNPCSpriteCountChanged(int)));
}

void TextureDatabaseWindow::DisconnectWidgets()
{
    DatabasePageTemplate::DisconnectWidgets();
    disconnect(m_pTransparencyColor, SIGNAL(editingFinished()), this, SLOT(TileAutotileStateChanged(int)));
    /*#####
    # Tiles
    #####*/
    disconnect(m_pIsAutotile, SIGNAL(stateChanged(int)), this, SLOT(TileAutotileStateChanged(int)));

    /*#####
    # Objects
    #####*/
    disconnect(m_pObjectBoundingUp, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    disconnect(m_pObjectBoundingLeft, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    disconnect(m_pObjectBoundingBottom, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    disconnect(m_pObjectBoundingRight, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));

    /*#####
    # NPCs
    #####*/
    disconnect(m_pNPCBoundingBottom, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    disconnect(m_pNPCBoundingUp, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    disconnect(m_pNPCBoundingLeft, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    disconnect(m_pNPCBoundingRight, SIGNAL(valueChanged(int)), this, SLOT(BoundingChanged(int)));
    disconnect(m_pNPCRow, SIGNAL(valueChanged(int)), this, SLOT(TextureNPCSpriteCountChanged(int)));
    disconnect(m_pNPCColumn, SIGNAL(valueChanged(int)), this, SLOT(TextureNPCSpriteCountChanged(int)));
}

const SpritePrototype* TextureDatabaseWindow::GetLatestPrototype(std::string p_sType, uint32 p_uiID)
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

void TextureDatabaseWindow::ClearWidgets()
{
    DisconnectWidgets();
    DatabasePageTemplate::ClearWidgets();

    m_pView->clear();
    m_pTransparencyColor->clear();

    m_pPassability->clear();
    m_pPassabilityDown->clear();
    m_pPassabilityLeft->clear();
    m_pPassabilityRight->clear();
    m_pPassabilityUp->clear();

    m_pObjectBoundingRight->setValue(0);
    m_pObjectBoundingBottom->setValue(0);
    m_pObjectBoundingLeft->setValue(0);
    m_pObjectBoundingUp->setValue(0);
    if (m_pObjectBoundingView->scene())
    {
        delete m_pObjectBoundingView->scene();
        m_pObjectBoundingView->setScene(new QGraphicsScene());
    }

    m_pNPCRow->setValue(0);
    m_pNPCColumn->setValue(0);
    m_pNPCBoundingRight->setValue(0);
    m_pNPCBoundingLeft->setValue(0);
    m_pNPCBoundingUp->setValue(0);
    m_pNPCBoundingBottom->setValue(0);
    if (m_pNPCBoundingView->scene())
    {
        delete m_pNPCBoundingView->scene();
        m_pNPCBoundingView->setScene(new QGraphicsScene());
    }
    ConnectWidgets();
}

void TextureDatabaseWindow::CurrentTabChanged(int p_Index)
{
    m_uilIDCache.clear();
    m_pStoreBox->clear();
    ClearWidgets();
    if (p_Index == -1)
        return;

    std::map<uint32, std::string> t_sTextureNames;
    if (Database *t_pDB = Database::Get())
        t_pDB->GetTextureNames(GetCurrentType(), t_sTextureNames);

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        t_pDBOut->GetTextureNames(GetCurrentType(), t_sTextureNames);

        QString t_sName;
        for (std::map<uint32, std::string>::iterator t_itr = t_sTextureNames.begin(); t_itr != t_sTextureNames.end(); ++t_itr)
        {
            // if texture is deleted, do not show it in list
            if (t_pDBOut->IsSpritePrototypeDeleted(GetCurrentType(), t_itr->first))
                continue;

            InsertItem(t_itr->first, QString((ToString(t_itr->first) + ":" + t_itr->second).c_str()));
        }
    }
}

void TextureDatabaseWindow::SetTexturePixmap(QPixmap &p_Pixmap)
{
    m_pView->setPixmap(p_Pixmap);
    QString t_sCurType(GetCurrentType().c_str());
    if (t_sCurType == "Tiles")
        m_pPassability->setPixmap(p_Pixmap);
    else if (t_sCurType == "Objects")
    {
        QGraphicsScene *t_Scene = m_pObjectBoundingView->scene();
        if (t_Scene)
        {
            delete m_pObjectBoundingView->scene();
            m_pObjectBoundingView->setScene(NULL);
        }
        t_Scene = new QGraphicsScene();
        m_pObjectBoundingView->setScene(t_Scene);
        t_Scene->addPixmap(p_Pixmap);
        ChangeBounding();
    }
    else if (t_sCurType == "NPCs")
    {
        QGraphicsScene *t_Scene = m_pNPCBoundingView->scene();
        if (t_Scene)
        {
            delete m_pNPCBoundingView->scene();
            m_pNPCBoundingView->setScene(NULL);
        }
        t_Scene = new QGraphicsScene();
        m_pNPCBoundingView->setScene(t_Scene);
        t_Scene->addPixmap(p_Pixmap);
        ChangeBounding();
    }
}

void TextureDatabaseWindow::ChangeBounding()
{
    std::string t_sType = GetCurrentType();
    if (t_sType == "Objects")
    {
        if (!m_pObjectBoundingView->scene())
            return;

        QList<QGraphicsItem *> t_ItemList = m_pObjectBoundingView->items();
        if (t_ItemList.size() >= 2)
            m_pObjectBoundingView->scene()->removeItem(t_ItemList[0]);

        if (m_pObjectBoundingRight->value() > 0 && m_pObjectBoundingBottom->value() > 0)
            m_pObjectBoundingView->scene()->addRect(m_pObjectBoundingLeft->value(), m_pObjectBoundingUp->value(), m_pObjectBoundingRight->value()-1, m_pObjectBoundingBottom->value()-1, QPen(QColor(255, 0, 0)));
    }
    else if (t_sType == "NPCs")
    {
        if (!m_pNPCBoundingView->scene())
            return;

        QList<QGraphicsItem *> t_ItemList = m_pNPCBoundingView->items();
        if (t_ItemList.size() >= 2)
            m_pNPCBoundingView->scene()->removeItem(t_ItemList[0]);

        if (m_pNPCBoundingRight->value() > 0 && m_pNPCBoundingBottom->value() > 0)
            m_pNPCBoundingView->scene()->addRect(m_pNPCBoundingLeft->value(), m_pNPCBoundingUp->value(), m_pNPCBoundingRight->value()-1, m_pNPCBoundingBottom->value()-1, QPen(QColor(255, 0, 0)));
    }
}

void TextureDatabaseWindow::BoundingChanged(int p_Value)
{
    ChangeBounding();
    ChangeItem(GetCurrentItemID());
}

bool TextureDatabaseWindow::eventFilter(QObject *p_pObj, QEvent *p_pEvent)
{
    // change texture
    if (p_pObj == m_pName)
    {
        if (p_pEvent->type() == QEvent::MouseButtonDblClick)
        {
            QString t_sProjectDir;
            if (EditorConfig *t_pConf = EditorConfig::Get())
                t_sProjectDir = QString(t_pConf->GetProjectDirectory().c_str());
            QString t_sFileName = QFileDialog::getOpenFileName(this, tr("Open Texture"), t_sProjectDir+"/MapSprites", tr("Images (*.png)"));
            SetTexturePixmap(QPixmap(t_sFileName));
            t_sFileName.remove(0, t_sFileName.lastIndexOf("/")+1);
            // change TextureName
            m_pName->setText(t_sFileName);
            // change TextureBox
            QString t_sID = m_pID->text();
            QString t_sItemText = t_sID+":"+t_sFileName;
            m_pStoreBox->setItemText(m_pStoreBox->currentIndex(), t_sItemText);
            ChangeItem(t_sID.toUInt());
            return true;
        }
    }
    // open "TransparencyWindow"
    if (p_pObj == m_pView)
    {
        if (p_pEvent->type() == QEvent::MouseButtonDblClick)
        {
            if (const SpritePrototype* t_pProto = GetLatestPrototype(GetCurrentType(), m_pID->text().toUInt()))
            {
                if (EditorConfig *t_pConf = EditorConfig::Get())
                {
                    if (Database *t_pDB = Database::Get())
                    {
                        if (QMainWindow *t_pDBWindow = (QMainWindow*)window())
                        {
                            TransparencyWindow *t_pTWindow = new TransparencyWindow(t_pDBWindow, QPixmap( QString((t_pConf->GetProjectDirectory()+"/" +
                                t_pDB->GetSpritePath(t_pProto->m_uiSpriteType) + t_pProto->m_sFileName).c_str())));
                            connect(t_pTWindow, SIGNAL(ColorChosen(QString)), this, SLOT(TransparencyColorChanged(QString)));
                            t_pTWindow->show();
                            t_pDBWindow->setEnabled(false);
                            t_pTWindow->setEnabled(true);
                            return true;
                        }
                    }
                }
            }
        }
    }
    // change tile passability
    else if (p_pObj == m_pPassabilityDown || p_pObj == m_pPassabilityUp || p_pObj == m_pPassabilityRight || p_pObj == m_pPassabilityLeft)
    {
        PassabilityFlag t_Dir = PASSABLE_NONE;
        if (p_pObj == m_pPassabilityRight)
            t_Dir = PASSABLE_RIGHT;
        else if (p_pObj == m_pPassabilityLeft)
            t_Dir = PASSABLE_LEFT;
        else if (p_pObj == m_pPassabilityDown)
            t_Dir = PASSABLE_DOWN;
        else if (p_pObj == m_pPassabilityUp)
            t_Dir = PASSABLE_UP;

        if (p_pEvent->type() == QEvent::MouseButtonPress)
        {
            if (const SpritePrototype* t_pProto = GetLatestPrototype(GetCurrentType(), m_pName->text().toUInt()))
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

                ChangeItem(m_pID->text().toUInt());
                return true;
            }
        }
    }
    else // pass the event on to the parent class
        return QWidget::eventFilter(p_pObj, p_pEvent);

    return false;
}