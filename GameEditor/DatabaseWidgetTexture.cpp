#include "DatabaseWidgetTexture.h"
#include <QtWidgets/QFileDialog>
#include "Config.h"
#include "moc_DatabaseWidgetTexture.h"
#include "QtGlobal.h"
#include <QtGui/QPixmapCache>

DatabaseWidgetTexture::DatabaseWidgetTexture(QWidget *pParent) : DatabaseWidgetRessource(pParent),
    m_pModuleTexture(new DatabaseModuleTexture(this))
{
    //insert module into layout
    if (QGridLayout *pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->addWidget(m_pModuleTexture, 1,1);
        pLayout->setRowStretch(1, 1);
    }

    connect(m_pModuleTexture, SIGNAL(transparencyColorChanged(const Color&)), this, SLOT(_onTransparencyColorChanged(const Color&)));
}

void DatabaseWidgetTexture::setupWidgetsFromPrototype(const DATABASE::Prototype *pPrototype)
{
    if (auto pProto = dynamic_cast<const DATABASE::TexturePrototype*>(pPrototype))
    {
        m_pModuleTexture->setDataRed(pProto->getTransparencyColor().getRed());
        m_pModuleTexture->setDataGreen(pProto->getTransparencyColor().getGreen());
        m_pModuleTexture->setDataBlue(pProto->getTransparencyColor().getBlue());

        // setup pixmaps (when overwritten, the pixmap widgets in subclasses, too)
        setupPixmap(_createPrototypePixmap(pProto->getID()));
    }
    DatabaseWidgetRessource::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetTexture::setupPrototypeFromWidgets(DATABASE::Prototype *pPrototype)
{
    if (auto pProto = dynamic_cast<DATABASE::TexturePrototype*>(pPrototype))
        pProto->setTransparencyColor(Color(m_pModuleTexture->getDataRed(), m_pModuleTexture->getDataGreen(), m_pModuleTexture->getDataBlue()));
    DatabaseWidgetRessource::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetTexture::clear()
{
    m_pModuleTexture->clear();
    DatabaseWidgetRessource::clear();
}

QString DatabaseWidgetTexture::_selectFile() const
{
    return QFileDialog::getOpenFileName(nullptr, tr("Chose Texture"), Config::get()->getProjectDirectory()+"/Textures", tr("Images (*.png)"));
}

QStringList DatabaseWidgetTexture::_selectFiles() const
{
    return QFileDialog::getOpenFileNames(nullptr, tr("Chose Textures"), Config::get()->getProjectDirectory()+"/Textures", tr("Images (*.png)"));
}

QPixmap DatabaseWidgetTexture::_createPrototypePixmap(uint32 uiID) const
{
    QPixmap pixmap;
    if (auto pProto = dynamic_cast<const DATABASE::TexturePrototype*>(getDatabase()->getPrototype(uiID)))
        createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), pProto, pixmap);
    return pixmap;
}

void DatabaseWidgetTexture::_onFileImport(uint32 uiID)
{
    setupPixmap(_createPrototypePixmap(uiID));
}

void DatabaseWidgetTexture::setupPixmap(const QPixmap &pixmap)
{
    m_pModuleTexture->setPixmap(pixmap);
}

void DatabaseWidgetTexture::_onTransparencyColorChanged(const Color &color)
{
    if (auto pProto = dynamic_cast<const DATABASE::TexturePrototype*>(getDatabase()->getPrototype(m_pModuleList->getDataID())))
    {
        QPixmap pixmap;
        createPixmap(Config::get()->getProjectDirectory(), pProto->getPathName(), color, pixmap);
        setupPixmap(pixmap);
    }
}