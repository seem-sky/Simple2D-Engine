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

// slots for child class, paste them after "private slots:"
//void _choseTransparentColorButtonClicked() { choseTransparencyColor(); }
//void _choseFileButtonClicked() { choseFile(); }
//void _changeTransparencyColor() { transparencyColorChanged(); }

template <class T>
class TextureDatabaseWidget : public DatabaseWidget<T>, protected Ui_TextureDatabaseWidget
{
protected:
    virtual void connectWidgets()
    {
        DatabaseWidget::connectWidgets();
        connect(m_pChoseFile, SIGNAL(clicked()), this, SLOT(_choseFileButtonClicked()));
        connect(m_pChoseColor, SIGNAL(clicked()), this, SLOT(_choseTransparentColorButtonClicked()));
        connect(m_pTransparencyColor, SIGNAL(editingFinished()), this, SLOT(_changeTransparencyColor()));
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
        // set transparency first, so that pixmap is displayed correctly.
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

    void choseFile()
    {

        QString sProjectDir = Config::Get()->getProjectDirectory();
        QString sFileName = QFileDialog::getOpenFileName(this, tr("Chose Texture"), sProjectDir+"/Textures", tr("Images (*.png)"));
        QFileInfo fileInfo(sFileName);
        if (fileInfo.isFile())
        {
            // clear previous data
            m_pPath->clear();
            if (m_pView->scene())
                m_pView->scene()->clear();

            QString sPath(fileInfo.absolutePath());
            sPath = sPath.remove(0, sPath.lastIndexOf("/Textures")+10);
            if (!sPath.isEmpty())
            {
                sPath += "/";
                m_pPath->setText(sPath);
            }
            m_pFileName->setText(fileInfo.fileName());
            if (m_pName->text().isEmpty())
                setName(fileInfo.fileName().left(fileInfo.fileName().indexOf(".")));
            updateItem();
            boost::shared_ptr<T> proto;
            if (m_pDBChanger->getItem(getCurrentID(), proto))
                showPixmap(proto);
        }
    }

public:
    TextureDatabaseWidget(QWidget *pParent = NULL) : DatabaseWidget(pParent), Ui_TextureDatabaseWidget()
    {
        Ui_TextureDatabaseWidget::setupUi(this);
    }

private:
    QPixmap m_Pixmap;
};
#endif
