#include "DynamicObjectDatabaseWidget.h"
#include "moc_DynamicObjectDatabaseWidget.h"

using namespace DATABASE;

/*#####
# DynamicObjectDatabaseWidget
#####*/
DynamicObjectDatabaseWidget::DynamicObjectDatabaseWidget(QWidget *pParent) : ObjectDatabaseWidget(pParent), m_pMovementSpeed(new QSpinBox(this)),
    m_pMovementSpeedLabel(new QLabel("movement speed", this)), m_pPixelPerSecLabel(new QLabel("(pixel per sec)", this))
{
    // setup own widgets
    m_pMovementSpeed->setMinimum(0);
    m_pMovementSpeed->setMaximum(10000);
    m_pMovementSpeed->setGeometry(210, 370, 71, 21);

    // labels
    QFont font = m_pMovementSpeedLabel->font();
    font.setPointSize(12);
    font.setBold(true);
    m_pMovementSpeedLabel->setFont(font);
    m_pMovementSpeedLabel->setGeometry(210, 350, 161, 21);

    m_pPixelPerSecLabel->setGeometry(290, 370, 81, 21);

    connectWidgets();
}