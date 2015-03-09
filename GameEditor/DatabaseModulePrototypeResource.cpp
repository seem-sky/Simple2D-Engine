#include "DatabaseModulePrototypeResource.h"
#include "moc_DatabaseModulePrototypeResource.h"
#include <Database/Prototype/Derived.h>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileDialog>

using namespace database;
using namespace ui::module;

PrototypeResource::PrototypeResource(QWidget* pParent)
    : PrototypeInterface(pParent), Ui_DatabaseModulePrototypeResource()
{
    setupUi(this);
    connect(m_pChoseFile, SIGNAL(clicked()), this, SLOT(_onImportButtonClicked()));
}

void PrototypeResource::setup(const prototype::Resource* pPrototype)
{
    m_pPrototypeBase->setup(pPrototype);

    m_pFileName->setText(pPrototype->getFileName());
    m_pPath->setText(pPrototype->getPath());
}

void PrototypeResource::setupFrom(prototype::Resource* pPrototype) const
{
    m_pPrototypeBase->setupFrom(pPrototype);

    pPrototype->setFileName(m_pFileName->text());
    pPrototype->setPath(m_pPath->text());
}

void PrototypeResource::clear()
{
    m_pPrototypeBase->clear();
    m_pFileName->clear();
    m_pPath->clear();
}

QString PrototypeResource::getPath() const
{
    return m_pPath->text();
}

QString PrototypeResource::getFileName() const
{
    return m_pFileName->text();
}

void PrototypeResource::setFileDialogInfos(const QString& dir, const QString& extensions, const QString& cutPath)
{
    m_Directory = dir;
    m_Extensions = extensions;
    m_CutPath = cutPath;
}

void getFileNameAndPath(const QFileInfo& file, const QString& cutPath, QString& resultName, QString& resultPath)
{
    resultName.clear();
    resultPath.clear();

    if (file.isFile())
    {
        QString path(file.absolutePath());
        path = path.remove(0, path.lastIndexOf(cutPath) + cutPath.size());
        if (!path.isEmpty())
        {
            path += "/";
            resultPath = path;
        }
        resultName = file.fileName();
    }
}

void PrototypeResource::import(prototype::Resource& prototype, const QString& fileName, const QString& cutPath)
{
    // clear previous data
    prototype.setFileName("");
    prototype.setPath("");

    QFileInfo fileInfo(fileName);
    if (fileInfo.isFile())
    {
        QString fileName, path;
        getFileNameAndPath(fileInfo, cutPath, fileName, path);
        prototype.setPath(path);
        prototype.setFileName(fileName);
        // if name is empty fill with filename
        if (prototype.getName().isEmpty())
            prototype.setName(fileInfo.fileName());
    }
}

void PrototypeResource::_onImportButtonClicked()
{
    QFileInfo fileInfo(QFileDialog::getOpenFileName(window(), tr("Chose File"), m_Directory, m_Extensions));
    if (!fileInfo.isFile())
        return;

    QString fileName, path;
    getFileNameAndPath(fileInfo, m_CutPath, fileName, path);
    m_pFileName->setText(fileName);
    m_pPath->setText(path);

    emit imported(path + "/" + fileName);
}
