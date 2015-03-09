#include "AnimationModuleSelectedItem.h"
#include "moc_AnimationModuleSelectedItem.h"
#include <Animation/ItemData.h>

using namespace animation::ui::module;

SelectedItem::SelectedItem(QWidget* pParent)
    : QWidget(pParent), Ui_AnimationModuleSelectedItem()
{
    setupUi(this);

    m_pRotation->setMinimum(animation::ItemData::minimumRotation());
    m_pRotation->setMaximum(animation::ItemData::maximumRotation());
    m_pScale->setMinimum(animation::ItemData::minimumScale());
    m_pScale->setMaximum(animation::ItemData::maximumScale());
    m_pZValue->setMinimum(animation::ItemData::minimumZValue());
    m_pZValue->setMaximum(animation::ItemData::maximumZValue());
    m_pOpacity->setMinimum(animation::ItemData::minimumOpacity());
    m_pOpacity->setMaximum(animation::ItemData::maximumOpacity());

    _connect();
}

void SelectedItem::_onPositionChanged(int value)
{
    emit positionChanged(QPoint(m_pPosX->value(), m_pPosY->value()));
}

void SelectedItem::_onZValueChanged(qreal value)
{
    emit zValueChanged(value);
}

void SelectedItem::_onScaleChanged(qreal value)
{
    emit scaleChanged(value);
}

void SelectedItem::_onOpacityChanged(qreal value)
{
    emit opacityChanged(value);
}

void SelectedItem::_onRotationChanged(qreal value)
{
    emit rotationChanged(value);
}

void SelectedItem::_connect()
{
    connect(m_pPosX, SIGNAL(valueChanged(int)), this, SLOT(_onPositionChanged(int)));
    connect(m_pPosY, SIGNAL(valueChanged(int)), this, SLOT(_onPositionChanged(int)));
    connect(m_pZValue, SIGNAL(valueChanged(qreal)), this, SLOT(_onZValueChanged(qreal)));
    connect(m_pScale, SIGNAL(valueChanged(qreal)), this, SLOT(_onScaleChanged(qreal)));
    connect(m_pOpacity, SIGNAL(valueChanged(qreal)), this, SLOT(_onOpacityChanged(qreal)));
    connect(m_pRotation, SIGNAL(valueChanged(qreal)), this, SLOT(_onRotationChanged(qreal)));
}

void SelectedItem::_disconnect()
{
    disconnect(m_pPosX, SIGNAL(valueChanged(int)), this, SLOT(_onPositionChanged(int)));
    disconnect(m_pPosY, SIGNAL(valueChanged(int)), this, SLOT(_onPositionChanged(int)));
    disconnect(m_pZValue, SIGNAL(valueChanged(qreal)), this, SLOT(_onZValueChanged(qreal)));
    disconnect(m_pScale, SIGNAL(valueChanged(qreal)), this, SLOT(_onScaleChanged(qreal)));
    disconnect(m_pOpacity, SIGNAL(valueChanged(qreal)), this, SLOT(_onOpacityChanged(qreal)));
    disconnect(m_pRotation, SIGNAL(valueChanged(qreal)), this, SLOT(_onRotationChanged(qreal)));
}

void SelectedItem::clear()
{
    _disconnect();
    m_pPosX->setValue(0);
    m_pPosY->setValue(0);
    m_pZValue->setValue(0);
    m_pRotation->setValue(0);
    m_pOpacity->setValue(1);
    m_pScale->setValue(1);
    _connect();
}

void SelectedItem::onPositionChanged(const QPoint& pos)
{
    _disconnect();
    m_pPosX->setValue(pos.x());
    m_pPosY->setValue(pos.y());
    _connect();
}

void SelectedItem::onZValueChanged(qreal value)
{
    _disconnect();
    m_pZValue->setValue(value);
    _connect();
}

void SelectedItem::onScaleChanged(qreal value)
{
    _disconnect();
    m_pScale->setValue(value);
    _connect();
}

void SelectedItem::onOpacityChanged(qreal value)
{
    _disconnect();
    m_pOpacity->setValue(value);
    _connect();
}

void SelectedItem::onRotationChanged(qreal value)
{
    _disconnect();
    m_pRotation->setValue(value);
    _connect();
}
