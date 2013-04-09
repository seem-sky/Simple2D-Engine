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

    virtual bool getPrototypeFromWidgets(boost::shared_ptr<T> &proto)
    {
        if (!proto || !DatabaseWidget::getPrototypeFromWidgets(proto))
            return false;

        proto->setFileName(m_pFileName->text().toStdString());
        proto->setPath(m_pPath->text().toStdString());
        proto->setTransparencyColor(Color(m_pTransparencyColor->text().toStdString()));
        return true;
    }

    virtual bool setWidgetsFromPrototype(const boost::shared_ptr<T> &proto)
    {
        if (!proto || !DatabaseWidget::setWidgetsFromPrototype(proto))
            return false;

        m_pFileName->setText(QString::fromStdString(proto->getFileName()));
        m_pPath->setText(QString::fromStdString(proto->getPath()));
        // set transparency first, so that pixmap is displayed correctly.
        m_pTransparencyColor->setText(QString::fromStdString(proto->getTransparencyColor().getColorString()));
        showPixmap(QString::fromStdString(Config::Get()->getProjectDirectory() + "/Textures/" + proto->getPathName()));
        return true;
    }

    virtual QPixmap showPixmap(const QString &sFilePath)
    {
        QPixmap pixmap(sFilePath);
        if (!pixmap.isNull())
        {
            // set transparency color
            Color color(m_pTransparencyColor->text().toStdString());
            if (color.hasValidColor())
                pixmap.setMask(pixmap.createMaskFromColor(QColor(color.getRed(), color.getGreen(), color.getBlue())));

            QGraphicsScene *pScene = new QGraphicsScene();
            pScene->addPixmap(pixmap);
            m_pView->setScene(pScene);
        }
        return pixmap;
    }

    void choseTransparencyColor()
    {
        QFileInfo info(QString::fromStdString(Config::Get()->getProjectDirectory()+"/Textures/") + m_pPath->text() + m_pFileName->text());
        if (info.isFile())
        {
            TransparencyWindow transparencyWindow(this, QPixmap(info.absoluteFilePath()));
            if (transparencyWindow.exec())
            {
                Color color = transparencyWindow.getColor();
                if (color.hasValidColor())
                {
                    m_pTransparencyColor->setText(QString::fromStdString(color.getColorString()));
                    transparencyColorChanged();
                }
            }
        }
    }

    void transparencyColorChanged()
    {
        showPixmap(QString::fromStdString(Config::Get()->getProjectDirectory() + "/Textures/") + m_pPath->text() + m_pFileName->text());
        updateItem();
    }

    void choseFile()
    {
        QString sProjectDir = QString::fromStdString(Config::Get()->getProjectDirectory());
        QString sFileName = QFileDialog::getOpenFileName(this, tr("Chose Texture"), sProjectDir+"/Textures", tr("Images (*.png)"));
        QFileInfo fileInfo(sFileName);
        if (fileInfo.isFile())
        {
            QString sPath(fileInfo.absolutePath());
            sPath = sPath.remove(0, sPath.lastIndexOf("/Textures")+10);
            if (!sPath.isEmpty())
            {
                sPath += "/";
                m_pPath->setText(sPath);
            }
            showPixmap(sFileName);
            m_pFileName->setText(fileInfo.fileName());
            if (m_pName->text().isEmpty())
                setName(fileInfo.fileName().left(fileInfo.fileName().indexOf(".")));
            updateItem();
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
