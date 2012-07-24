#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include "MainWindow.h"
#include <QTGui/QFileDialog>
#include <Database.h>
#include <StringAdditions.h>

using namespace DATABASE;

DatabaseWindow::DatabaseWindow(QMainWindow *p_pParent) : QMainWindow(p_pParent, Qt::X11BypassWindowManagerHint|Qt::WindowStaysOnTopHint)
{
    setupUi(this);
    TextureName->installEventFilter(this);

    connect(TextureBox, SIGNAL(currentIndexChanged(int)), this, SLOT(TextureBoxChangeCur(int)));
    connect(TextureTabs, SIGNAL(currentChanged(int)), this, SLOT(TextureTabChangeCur(int)));
    tabWidget->setCurrentIndex(0);
    TextureTabs->setCurrentIndex(0);

    connect(NewTexture, SIGNAL(clicked()), this, SLOT(ClickButtonNewTexture()));
    connect(DeleteTexture, SIGNAL(clicked()), this, SLOT(ClickButtonDeleteTexture()));
}

DatabaseWindow::~DatabaseWindow(void)
{
    if (QWidget *t_pParent = (QWidget*)parent())
        t_pParent->setEnabled(true);
}

void DatabaseWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    delete this;
}

bool DatabaseWindow::eventFilter(QObject *p_pObj, QEvent *p_pEvent)
{
    if (p_pObj == TextureName)
    {
        if (p_pEvent->type() == QEvent::MouseButtonDblClick)
        {
            QString t_sFileName = QFileDialog::getOpenFileName(this, tr("Open Texture"), ((MainWindow *)parent()) ? ((MainWindow *)parent())->GetProjectDir()+"/MapSprites" : "/MapSprites", tr("Images (*.png)"));
            TextureView->setPixmap(QPixmap(t_sFileName));
            t_sFileName.remove(0, t_sFileName.lastIndexOf("/")+1);
            TextureName->setText(t_sFileName);

            return true;
        }
        else
            return false;
    }
    else // pass the event on to the parent class
        return QMainWindow::eventFilter(p_pObj, p_pEvent);
}

void DatabaseWindow::TextureTabChangeCur(int p_Index)
{
    TextureBox->clear();
    if (p_Index == -1)
        return;

    std::map<UINT, std::string> t_sTextureNames;
    if (Database *t_pDB = Database::Get())
        t_pDB->GetTextureNames(TextureTabs->tabText(p_Index).toStdString(), t_sTextureNames);

    QString t_sName;
    for (std::map<UINT, std::string>::iterator t_itr = t_sTextureNames.begin(); t_itr != t_sTextureNames.end(); ++t_itr)
    {
        t_sName = (ToString(t_itr->first) + ":" + t_itr->second).c_str();
        TextureBox->addItem(t_sName);
    }
}

void DatabaseWindow::TextureBoxChangeCur(int p_Index)
{
    TextureName->clear();
    TextureView->clear();
    TextureIDLabel->clear();
    TextureTypeLabel->clear();

    if (p_Index != -1)
    {
        QString t_sID = TextureBox->currentText();
        t_sID.truncate(t_sID.indexOf(":"));
        SelectTexture(TextureTabs->tabText(TextureTabs->currentIndex()).toStdString(), t_sID.toUInt());
    }
}

bool DatabaseWindow::SelectTexture(std::string p_sType, UINT p_uiID)
{
    if (Database *t_pDB = Database::Get())
    {
        if (const SpritePrototype *t_pProto = t_pDB->GetSpritePrototype(p_sType, p_uiID))
        {
            if (MainWindow *t_pParent = (MainWindow*)parent())
            {
                TextureView->setPixmap(QPixmap(t_pParent->GetProjectDir() + QString(("/" + t_pDB->GetSpritePath(t_pProto->m_uiSpriteType) + t_pProto->m_sFileName).c_str())));
                TextureName->setText(QString(t_pProto->m_sFileName.c_str()));
                TextureIDLabel->setText(QString(ToString(t_pProto->m_uiID).c_str()));
                TextureTypeLabel->setText(QString(ToString(t_pProto->m_uiSpriteType).c_str()));

                return true;
            }
        }
    }
    return false;
}

void DatabaseWindow::ClickButtonNewTexture()
{
    // TODO: get new ID
    TextureBox->addItem("");
    TextureBox->setCurrentIndex(TextureBox->findText(""));

}

void DatabaseWindow::ClickButtonDeleteTexture()
{
    TextureBox->removeItem(TextureBox->currentIndex());
}