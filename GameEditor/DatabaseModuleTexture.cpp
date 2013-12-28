#include "DatabaseModuleTexture.h"
#include "moc_DatabaseModuleTexture.h"

DatabaseModuleTexture::DatabaseModuleTexture(QWidget* pParent) : QWidget(pParent), Ui_DatabaseModuleTexture()
{
    setupUi(this);
    connect(m_pColorRed, SIGNAL(valueChanged(int)), this, SLOT(_onTransparencyColorChanged(int)));
    connect(m_pColorGreen, SIGNAL(valueChanged(int)), this, SLOT(_onTransparencyColorChanged(int)));
    connect(m_pColorBlue, SIGNAL(valueChanged(int)), this, SLOT(_onTransparencyColorChanged(int)));

    connect(m_pChoseColor, SIGNAL(clicked()), this, SLOT(_onChoseColorButtonClicked()));
}

void DatabaseModuleTexture::setPixmap(const QPixmap& pixmap)
{
    m_pView->setScene(new QGraphicsScene());
    m_pView->scene()->addPixmap(pixmap);
}

void DatabaseModuleTexture::_onTransparencyColorChanged(int value)
{
    emit transparencyColorChanged(getTransparencyColor());
}

void DatabaseModuleTexture::clear()
{
    m_pColorRed->setValue(-1);
    m_pColorGreen->setValue(-1);
    m_pColorBlue->setValue(-1);

    m_pView->setScene(new QGraphicsScene());
}

void DatabaseModuleTexture::_onChoseColorButtonClicked()
{
    emit choseColorButtonClicked();
}

void DatabaseModuleTexture::setTransparencyColor(const Color& color)
{
    if (!color.isValid())
        return;
    m_pColorRed->setValue(color.getRed());
    m_pColorGreen->setValue(color.getGreen());
    m_pColorBlue->setValue(color.getBlue());
}

Color DatabaseModuleTexture::getTransparencyColor() const
{
    return Color(m_pColorRed->value(), m_pColorGreen->value(), m_pColorBlue->value());
}