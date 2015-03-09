#include "DatabaseSectionTile.h"
#include "moc_DatabaseSectionTile.h"
#include "DatabaseModulePrototypeResource.h"
#include "DatabaseModulePrototypeTile.h"
#include "DatabaseModuleMassImport.h"
#include "Config.h"

using namespace database;
using namespace ui::section;

static const char* IMAGE_STRING = "Images (*.png *.bmp *.jpg)";
static const char* CHILD_DIRECTORY = "/Textures";

Tile::Tile(TileDatabase& DB, QWidget* pParent)
    : Base(DB, new module::PrototypeTile(Config::get()->getProjectDirectory() + CHILD_DIRECTORY, IMAGE_STRING, CHILD_DIRECTORY), pParent)
{
    // setup mass_import_button
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        auto pMassImport = new module::MassImport(Config::get()->getProjectDirectory() + CHILD_DIRECTORY, IMAGE_STRING, this);
        connect(pMassImport, SIGNAL(selected(const QStringList&)), this, SLOT(_onMassImport(const QStringList&)));
        pLayout->addWidget(pMassImport, 2, 0);
    }
}

void Tile::_onMassImport(const QStringList& files)
{
    for (auto& fileName : files)
    {
        if (fileName.isEmpty())
            return;

        m_Merger.setSize(m_Merger.getSize() + 1);
        std::unique_ptr<prototype::Tile> pPrototype(new prototype::Tile(m_Merger.getSize()));
        ui::module::PrototypeResource::import(*pPrototype, fileName, "/Texture");
    }
}
