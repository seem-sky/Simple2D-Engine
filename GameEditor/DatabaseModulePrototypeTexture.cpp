#include "DatabaseModulePrototypeTexture.h"
#include "moc_DatabaseModulePrototypeTexture.h"
#include <Database/Prototype/Derived.h>
#include <QtGlobal.h>
#include "ChoseColorDialog.h"

using namespace database;
using namespace ui::module;

// free functions
void setupTexture(QGraphicsView* pView, const QString& absolutePathName, Color color)
{
    pView->scene()->clear();
    QPixmap pixmap;
    if (createPixmap(absolutePathName, color, pixmap))
        pView->centerOn(pView->scene()->addPixmap(pixmap));
}

QString getAbsolutePathName(const QString& root, const QString& path, const QString& file)
{
    return root + "/" + path + "/" + file;
}

// member methods
PrototypeTexture::PrototypeTexture(QWidget* pParent)
    : PrototypeInterface(pParent), Ui_DatabaseModulePrototypeTexture()
{
    setupUi(this);

    m_pView->setScene(new QGraphicsScene(m_pView));

    connect(m_pPrototypeResource, SIGNAL(imported(const QString&)), this, SLOT(_onImported(const QString&)));
    connect(m_pChoseColor, SIGNAL(clicked()), this, SLOT(_onChoseColorButtonClicked()));
    connect(m_pRed, SIGNAL(editingFinished()), this, SLOT(_onTransparencyColorChanged()));
    connect(m_pGreen, SIGNAL(editingFinished()), this, SLOT(_onTransparencyColorChanged()));
    connect(m_pBlue, SIGNAL(editingFinished()), this, SLOT(_onTransparencyColorChanged()));
}

void PrototypeTexture::setup(const prototype::Texture* pPrototype)
{
    m_pPrototypeResource->setup(pPrototype);

    m_pRed->setValue(pPrototype->getTransparencyColor().getRed());
    m_pBlue->setValue(pPrototype->getTransparencyColor().getBlue());
    m_pGreen->setValue(pPrototype->getTransparencyColor().getGreen());

    setupTexture(m_pView, m_RootDirectory + "/" + pPrototype->getPathName(), pPrototype->getTransparencyColor());
}

void PrototypeTexture::setupFrom(prototype::Texture* pPrototype) const
{
    m_pPrototypeResource->setupFrom(pPrototype);

    pPrototype->setTransparencyColor(Color(m_pRed->value(), m_pGreen->value(), m_pBlue->value()));
}

void PrototypeTexture::clear()
{
    m_pPrototypeResource->clear();

    m_pView->scene()->clear();
    m_pRed->setValue(-1);
    m_pGreen->setValue(-1);
    m_pBlue->setValue(-1);
}

QGraphicsView* PrototypeTexture::getTextureView() const
{
    return m_pView;
}

void PrototypeTexture::setFileDialogInfos(const QString& dir, const QString& extensions, const QString& cutPath)
{
    m_pPrototypeResource->setFileDialogInfos(dir, extensions, cutPath);
    m_RootDirectory = dir + "/";
}

void PrototypeTexture::_onImported(const QString& pathName)
{
    setupTexture(m_pView, m_RootDirectory + "/" + pathName, Color(m_pRed->value(), m_pGreen->value(), m_pRed->value()));
}

void PrototypeTexture::_onTransparencyColorChanged()
{
    setupTexture(m_pView, getAbsolutePathName(m_RootDirectory, m_pPrototypeResource->getPath(), m_pPrototypeResource->getFileName()),
        Color(m_pRed->value(), m_pGreen->value(), m_pBlue->value()));
}

void PrototypeTexture::_onChoseColorButtonClicked()
{
    QPixmap pixmap;
    if (!createPixmap(getAbsolutePathName(m_RootDirectory, m_pPrototypeResource->getPath(), m_pPrototypeResource->getFileName()),
        Color(m_pRed->value(), m_pGreen->value(), m_pBlue->value()), pixmap))
        return;

    ChoseColorDialog dialog(pixmap, window());
    if (dialog.exec() == QDialog::Accepted)
    {
        auto color = dialog.getColor();
        m_pRed->setValue(color.red());
        m_pGreen->setValue(color.green());
        m_pBlue->setValue(color.blue());
        _onTransparencyColorChanged();
    }
}
