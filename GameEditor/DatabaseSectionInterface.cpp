#include "DatabaseSectionInterface.h"
#include "moc_DatabaseSectionInterface.h"

using namespace database::ui::section;

Interface::Interface(QWidget* pParent)
    : QWidget(pParent)
{}

void Interface::onSaveChanges()
{
    _saveChanges();
}

void Interface::_onSelectionChanged(uint32 newID, uint32 oldID)
{
    _changeSelection(newID, oldID);
}