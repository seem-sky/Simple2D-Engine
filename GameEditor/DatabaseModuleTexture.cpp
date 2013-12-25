#include "DatabaseModuleTexture.h"
#include "moc_DatabaseModuleTexture.h"

DatabaseModuleTexture::DatabaseModuleTexture(QWidget* pParent) : QWidget(pParent), Ui_DatabaseModuleTexture()
{
    setupUi(this);
    connect(m_pColorRed, SIGNAL(valueChanged(int)), this, SLOT(_onTransparencyColorChanged(int)));
    connect(m_pColorGreen, SIGNAL(valueChanged(int)), this, SLOT(_onTransparencyColorChanged(int)));
    connect(m_pColorBlue, SIGNAL(valueChanged(int)), this, SLOT(_onTransparencyColorChanged(int)));
}

void DatabaseModuleTexture::setPixmap(const QPixmap& pixmap)
{
    m_pView->setScene(new QGraphicsScene());
    m_pView->scene()->addPixmap(pixmap);
}

void DatabaseModuleTexture::_onTransparencyColorChanged(int value)
{
    emit transparencyColorChanged(Color(getDataRed(), getDataGreen(), getDataBlue()));
}

void DatabaseModuleTexture::clear()
{
    m_pColorRed->setValue(-1);
    m_pColorGreen->setValue(-1);
    m_pColorBlue->setValue(-1);

    m_pView->setScene(new QGraphicsScene());
}