#include "AnimationModuleSelectedWidget.h"
#include "moc_AnimationModuleSelectedWidget.h"
#include <QtWidgets/QGridLayout>

using namespace animation::ui::module;

SelectedWidget::SelectedWidget(QWidget* pParent)
    : QWidget(pParent)
{
    m_pSelectedAddition = new SelectedAddition(this);
    m_pSelectedSprite = new SelectedSprite(this);
    auto pLayout = new QGridLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
    pLayout->addWidget(m_pSelectedSprite, 0, 0);
    pLayout->addWidget(m_pSelectedAddition, 0, 0);

    clear();
}

void SelectedWidget::clear()
{
    m_pSelectedAddition->setVisible(false);
    m_pSelectedAddition->clear();
    m_pSelectedSprite->setVisible(false);
    m_pSelectedSprite->clear();
}

void SelectedWidget::onSelectedChanged(const animation::AdditionData& data)
{
    m_pSelectedAddition->setVisible(true);
    m_pSelectedAddition->onSelectedChanged(data);
}

void SelectedWidget::onSelectedChanged(const animation::SpriteData& data)
{
    m_pSelectedSprite->setVisible(true);
    m_pSelectedSprite->onSelectedChanged(data);
}

void SelectedWidget::onSelectionCleared()
{
    clear();
}

void SelectedWidget::setAdditionTypeModel(database::Model* pModel)
{
    m_pSelectedAddition->setAdditionTypeModel(pModel);
}

SelectedSprite* SelectedWidget::getModuleSelectedSprite() const
{
    return m_pSelectedSprite;
}

SelectedAddition* SelectedWidget::getModuleSelectedAddition() const
{
    return m_pSelectedAddition;
}
