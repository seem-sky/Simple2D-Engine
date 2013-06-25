#ifndef TEXTURE_DATABASE_WIDGET_H
#define TEXTURE_DATABASE_WIDGET_H

#include "Ui/Ui_TextureDatabaseWidget.h"
#include <QtGui/QWidget>
#include <QtGui/QPixmap>
#include <QtGui/QTreeWidgetItem>
#include "Config.h"
#include "DatabaseWidget.h"
#include <QtGui/QFileDialog>
#include <QtGui/QBitmap>
#include "TransparencyWindow.h"
#include "QtGlobal.h"
#include "Logfile.h"

// slots for child class, paste them after "private slots:"
//void _choseTransparentColorButtonClicked() { choseTransparencyColor(); }
//void _choseFileButtonClicked() { choseFile(); }
//void _changeTransparencyColor() { transparencyColorChanged(); }
//void _massImportClicked() { massImport(); }

template <class T>
class TextureDatabaseWidget : public DatabaseWidget<T>, protected Ui_TextureDatabaseWidget
{
private:
    void import(const QString fileName, uint32 uiID)
    {
        if (uiID == 0)
            return;
        QFileInfo fileInfo(fileName);
        if (fileInfo.isFile())
        {
            // clear previous data
            m_pPath->clear();
            if (m_pView->scene())
                m_pView->scene()->clear();

            boost::shared_ptr<T> proto;
            bool exist = true;
            if (!m_pDBChanger->getItem(uiID, proto))
            {
                proto = boost::shared_ptr<T>(new T(uiID));
                exist = false;
            }

            QString path(fileInfo.absolutePath());
            path = path.remove(0, path.lastIndexOf("/Textures")+10);
            if (!path.isEmpty())
            {
                path += "/";
                proto->setPath(path);
            }
            proto->setFileName(fileInfo.fileName());
            if (proto->getName().isEmpty())
                proto->setName(fileInfo.fileName());
            m_pDBChanger->setItem(uiID, proto);
            if (!exist)
            {
                QStringList stringList;
                stringList.push_back(QString::number(uiID));
                stringList.push_back(proto->getName());
                m_pList->addTopLevelItem(new PrototypeTreeWidgetItem(stringList));
            }
            BASIC_LOG("File import succeeded: ID: " + QString::number(proto->getID()) + "; file name: " + proto->getPathName());
        }
    }

protected:
    virtual void connectWidgets()
    {
        DatabaseWidget::connectWidgets();
        connect(m_pChoseFile, SIGNAL(clicked()), this, SLOT(_choseFileButtonClicked()));
        connect(m_pChoseColor, SIGNAL(clicked()), this, SLOT(_choseTransparentColorButtonClicked()));
        connect(m_pTransparencyColor, SIGNAL(editingFinished()), this, SLOT(_changeTransparencyColor()));
        connect(m_pMassImport, SIGNAL(clicked()), this, SLOT(_massImportClicked()));
    }

    virtual void clearWidgets()
    {
        DatabaseWidget::clearWidgets();
        m_pFileName->clear();
        m_pTransparencyColor->clear();
        m_pView->setScene(new QGraphicsScene());
    }

    virtual bool getItemFromWidgets(boost::shared_ptr<T> &proto)
    {
        if (!proto || !DatabaseWidget::getItemFromWidgets(proto))
            return false;

        proto->setFileName(m_pFileName->text());
        proto->setPath(m_pPath->text());
        proto->setTransparencyColor(Color(m_pTransparencyColor->text()));
        return true;
    }

    virtual bool setWidgetsFromPrototype(const boost::shared_ptr<T> &proto)
    {
        if (!proto || !DatabaseWidget::setWidgetsFromPrototype(proto))
            return false;

        m_pFileName->setText(proto->getFileName());
        m_pPath->setText(proto->getPath());
        // set transparency first, so pixmap will be displayed correctly.
        m_pTransparencyColor->setText(proto->getTransparencyColor().getColorString());
        showPixmap(proto);
        return true;
    }

    virtual QPixmap showPixmap(const boost::shared_ptr<T> &proto)
    {
        QPixmap pixmap;
        if (createPixmapFromTexturePrototype(proto, pixmap))
        {
            QGraphicsScene *pScene = new QGraphicsScene();
            pScene->addPixmap(pixmap);
            m_pView->setScene(pScene);
        }
        return pixmap;
    }

    void choseTransparencyColor()
    {
        QFileInfo info(Config::Get()->getProjectDirectory()+"/Textures/" + m_pPath->text() + m_pFileName->text());
        if (info.isFile())
        {
            TransparencyWindow transparencyWindow(this, QPixmap(info.absoluteFilePath()));
            if (transparencyWindow.exec())
            {
                Color color = transparencyWindow.getColor();
                if (color.hasValidColor())
                {
                    m_pTransparencyColor->setText(color.getColorString());
                    transparencyColorChanged();
                }
            }
        }
    }

    void transparencyColorChanged()
    {
        if (!m_pDBChanger)
            return;
        updateItem();
        boost::shared_ptr<T> proto;
        if (m_pDBChanger->getItem(getCurrentID(), proto))
            showPixmap(proto);
    }

    void massImport()
    {
        QString projectDir = Config::Get()->getProjectDirectory();
        QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Chose Textures"), Config::Get()->getProjectDirectory()+"/Textures", tr("Images (*.png)"));
        if (!fileNames.isEmpty())
        {
            uint32 currentID = m_pDBChanger->getSize();
            for (QStringList::const_iterator itr = fileNames.begin(); itr != fileNames.end(); ++itr)
                import(*itr, ++currentID);
            change();
            changeCurrentItem(currentID);
        }
    }

    void choseFile()
    {

        import(QFileDialog::getOpenFileName(this, tr("Chose Texture"), Config::Get()->getProjectDirectory()+"/Textures", tr("Images (*.png)")), getCurrentID());
        changeCurrentItem(getCurrentID());
    }

public:
    TextureDatabaseWidget(QWidget *pParent = NULL) : DatabaseWidget(pParent), Ui_TextureDatabaseWidget()
    {
        Ui_TextureDatabaseWidget::setupUi(this);
        m_pList->move(m_pList->x(), m_pList->y() + m_pMassImport->height() + 5);
    }

private:
    QPixmap m_Pixmap;
};
#endif
