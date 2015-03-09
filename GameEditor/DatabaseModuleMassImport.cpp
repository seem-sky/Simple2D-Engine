#include "DatabaseModuleMassImport.h"
#include "moc_DatabaseModuleMassImport.h"
#include <QtWidgets/QFileDialog>

using namespace database::ui::module;

MassImport::MassImport(const QString& dir, const QString& extensions, QWidget* pParent)
    : QPushButton(pParent), m_Directory(dir), m_FileExtensions(extensions)
{
    setText("mass import");
    setToolTip("Allows you to import one or more resources. They will append at end of list.");
    connect(this, SIGNAL(clicked()), this, SLOT(_onClicked()));
}

void MassImport::_onClicked()
{
    auto files = QFileDialog::getOpenFileNames(parentWidget(), tr("Chose Files"), m_Directory, m_FileExtensions);
    if (!files.isEmpty())
        emit selected(files);
}
