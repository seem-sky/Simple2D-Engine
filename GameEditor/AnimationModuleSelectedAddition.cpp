#include "AnimationModuleSelectedAddition.h"
#include "moc_AnimationModuleSelectedAddition.h"
#include "DatabaseModuleList.h"

using namespace animation::ui::module;

SelectedAddition::SelectedAddition(QWidget* pParent)
    : QWidget(pParent), Ui_AnimationModuleSelectedAddition()
{
    setupUi(this);
}

void SelectedAddition::_onPositionChanged(const QPoint& pos)
{
    emit positionChanged(pos);
}

void SelectedAddition::_onZValueChanged(qreal value)
{
    emit zValueChanged(value);
}

void SelectedAddition::_onScaleChanged(qreal value)
{
    emit scaleChanged(value);
}

void SelectedAddition::_onOpacityChanged(qreal value)
{
    emit opacityChanged(value);
}

void SelectedAddition::_onRotationChanged(qreal value)
{
    emit rotationChanged(value);
}

void SelectedAddition::_onSizeChanged(int value)
{
    emit sizeChanged(QSize(m_pWidth->value(), m_pHeight->value()));
}

void SelectedAddition::_onAdditionTypeIDChanged(int ID)
{
    if (ID > -1)
        emit additionTypeIDChanged(ID + 1);
}

void SelectedAddition::_connect()
{
    connect(m_pSelectedItem, SIGNAL(positionChanged(const QPoint&)), this, SLOT(_onPositionChanged(const QPoint&)));
    connect(m_pSelectedItem, SIGNAL(zValueChanged(qreal)), this, SLOT(_onZValueChanged(qreal)));
    connect(m_pSelectedItem, SIGNAL(scaleChanged(qreal)), this, SLOT(_onScaleChanged(qreal)));
    connect(m_pSelectedItem, SIGNAL(opacityChanged(qreal)), this, SLOT(_onOpacityChanged(qreal)));
    connect(m_pSelectedItem, SIGNAL(rotationChanged(qreal)), this, SLOT(_onRotationChanged(qreal)));
    connect(m_pAdditionType, SIGNAL(currentIndexChanged(int)), this, SLOT(_onAdditionTypeIDChanged(int)));
}

void SelectedAddition::_disconnect()
{
    disconnect(m_pSelectedItem, SIGNAL(positionChanged(const QPoint&)), this, SLOT(_onPositionChanged(const QPoint&)));
    disconnect(m_pSelectedItem, SIGNAL(zValueChanged(qreal)), this, SLOT(_onZValueChanged(qreal)));
    disconnect(m_pSelectedItem, SIGNAL(scaleChanged(qreal)), this, SLOT(_onScaleChanged(qreal)));
    disconnect(m_pSelectedItem, SIGNAL(opacityChanged(qreal)), this, SLOT(_onOpacityChanged(qreal)));
    disconnect(m_pSelectedItem, SIGNAL(rotationChanged(qreal)), this, SLOT(_onRotationChanged(qreal)));
}

void SelectedAddition::onSelectedChanged(const animation::AdditionData& data)
{
    _disconnect();
    m_pBox->setEnabled(true);
    m_pSelectedItem->onPositionChanged(QPoint(data.getX(), data.getY()));
    m_pSelectedItem->onZValueChanged(data.getZValue());
    m_pSelectedItem->onScaleChanged(data.getScale());
    m_pSelectedItem->onRotationChanged(data.getRotation());
    m_pSelectedItem->onOpacityChanged(data.getOpacity());

    // setup type
    switch (data.getType())
    {
    case AdditionData::Type::point: m_pType->setText("point"); break;
    case AdditionData::Type::rect:
        m_pType->setText("rect");
        m_pWidth->setMinimum(animation::AdditionData::minimumWidth());
        m_pWidth->setMaximum(animation::AdditionData::maximumWidth());
        m_pWidth->setValue(data.getSize().getWidth());
        m_pHeight->setMinimum(animation::AdditionData::minimumHeight());
        m_pHeight->setMaximum(animation::AdditionData::maximumHeight());
        m_pHeight->setValue(data.getSize().getHeight());
        break;
    }
    m_pHeight->setEnabled(data.getType() == AdditionData::Type::rect);
    m_pWidth->setEnabled(data.getType() == AdditionData::Type::rect);
    _connect();
}

void SelectedAddition::clear()
{
    _disconnect();
    m_pBox->setEnabled(false);
    m_pSelectedItem->clear();

    m_pType->clear();
    m_pWidth->setMinimum(-1);
    m_pWidth->setMaximum(-1);
    m_pHeight->setMinimum(-1);
    m_pHeight->setMaximum(-1);
    m_pAdditionType->setCurrentIndex(-1);
    _connect();
}

void SelectedAddition::setAdditionTypeModel(database::Model* pModel)
{
    m_pAdditionType->setModel(pModel);
    m_pAdditionType->setView(new database::ui::module::List(pModel, this));
    m_pAdditionType->setModelColumn(1);
}

void SelectedAddition::onSelectionCleared()
{
    clear();
}
