#include "DatabaseModulePrototypeSprite.h"
#include "DatabaseModulePrototypeTexture.h"
#include <Database/Prototype/Derived.h>

using namespace database;
using namespace ui::module;

PrototypeSprite::PrototypeSprite(const QString& directory, const QString& extension, const QString& cutPath, QWidget* pParent)
    : PrototypeInterface(pParent), m_pModuleTexture(new PrototypeTexture(this))
{
    auto pLayout = new QGridLayout();
    pLayout->setContentsMargins(0, 0, 0, 0);
    m_pModuleTexture->setFileDialogInfos(directory, extension, cutPath);
    pLayout->addWidget(m_pModuleTexture);
    setLayout(pLayout);
}

void PrototypeSprite::setup(const prototype::Sprite* pPrototype)
{
    m_pModuleTexture->setup(pPrototype);
}

void PrototypeSprite::setupFrom(prototype::Sprite* pPrototype) const
{
    m_pModuleTexture->setupFrom(pPrototype);
}

void PrototypeSprite::clear()
{
    m_pModuleTexture->clear();
}
