#include "AnimationModuleSelectedSprite.h"
#include "moc_AnimationModuleSelectedSprite.h"

using namespace animation::ui::module;

SelectedSprite::SelectedSprite(QWidget* pParent)
    : QWidget(pParent), Ui_AnimationModuleSelectedSprite()
{
    setupUi(this);
    
    clear();
}

void SelectedSprite::_onPositionChanged(const QPoint& pos)
{
    emit positionChanged(pos);
}

void SelectedSprite::_onZValueChanged(qreal value)
{
    emit zValueChanged(value);
}

void SelectedSprite::_onScaleChanged(qreal value)
{
    emit scaleChanged(value);
}

void SelectedSprite::_onOpacityChanged(qreal value)
{
    emit opacityChanged(value);
}

void SelectedSprite::_onRotationChanged(qreal value)
{
    emit rotationChanged(value);
}

void SelectedSprite::_connect()
{
    connect(m_pSelectedItem, SIGNAL(positionChanged(const QPoint&)), this, SLOT(_onPositionChanged(const QPoint&)));
    connect(m_pSelectedItem, SIGNAL(zValueChanged(qreal)), this, SLOT(_onZValueChanged(qreal)));
    connect(m_pSelectedItem, SIGNAL(scaleChanged(qreal)), this, SLOT(_onScaleChanged(qreal)));
    connect(m_pSelectedItem, SIGNAL(opacityChanged(qreal)), this, SLOT(_onOpacityChanged(qreal)));
    connect(m_pSelectedItem, SIGNAL(rotationChanged(qreal)), this, SLOT(_onRotationChanged(qreal)));
}

void SelectedSprite::_disconnect()
{
    disconnect(m_pSelectedItem, SIGNAL(positionChanged(const QPoint&)), this, SLOT(_onPositionChanged(const QPoint&)));
    disconnect(m_pSelectedItem, SIGNAL(zValueChanged(qreal)), this, SLOT(_onZValueChanged(qreal)));
    disconnect(m_pSelectedItem, SIGNAL(scaleChanged(qreal)), this, SLOT(_onScaleChanged(qreal)));
    disconnect(m_pSelectedItem, SIGNAL(opacityChanged(qreal)), this, SLOT(_onOpacityChanged(qreal)));
    disconnect(m_pSelectedItem, SIGNAL(rotationChanged(qreal)), this, SLOT(_onRotationChanged(qreal)));
}

void SelectedSprite::onSelectedChanged(const animation::SpriteData& sprite)
{
    _disconnect();
    m_pBox->setEnabled(true);
    m_pSelectedItem->onPositionChanged(QPoint(sprite.getX(), sprite.getY()));
    m_pSelectedItem->onZValueChanged(sprite.getZValue());
    m_pSelectedItem->onScaleChanged(sprite.getScale());
    m_pSelectedItem->onRotationChanged(sprite.getRotation());
    m_pSelectedItem->onOpacityChanged(sprite.getOpacity());
    m_pSpriteID->setValue(sprite.getID());
    _connect();
}

void SelectedSprite::clear()
{
    _disconnect();
    m_pBox->setEnabled(false);
    m_pSelectedItem->clear();
    m_pSpriteID->setValue(0);
    _connect();
}

void SelectedSprite::onSelectionCleared()
{
    clear();
}
