#include "DatabaseWidgetRessource.h"
#include "moc_DatabaseWidgetRessource.h"
#include <QtCore/QFileInfo>
#include "Logfile.h"

using namespace DATABASE;
using namespace PROTOTYPE;

DatabaseWidgetRessource::DatabaseWidgetRessource(QWidget* pParent) : DatabaseWidgetBase(pParent),
    m_pMassImport(new QPushButton(this)), m_pModuleResource(new DatabaseModuleResource(this))
{
    // setup mass import button
    m_pMassImport->setText("mass import");
    m_pMassImport->setToolTip("Allows you to import one or more resources. They will append at end of list.");
    // insert button into layout
    if (QWidget* pBase = layout()->itemAt(0)->widget())
        pBase->layout()->addWidget(m_pMassImport);

    // insert module resource
    if (QGridLayout* pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->addWidget(m_pModuleResource, 0, 1);
        pLayout->setColumnStretch(2, 1);
        pLayout->setColumnStretch(1, 0);
    }

    connect(m_pModuleResource->getFileButton(), SIGNAL(clicked()), this, SLOT(_onClickFileButton()));
    connect(m_pMassImport, SIGNAL(clicked()), this, SLOT(_onClickMassFileImportButton()));
}

void DatabaseWidgetRessource::setupWidgetsFromPrototype(const Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<const RessourcePrototype*>(pPrototype))
    {
        m_pModuleResource->setDataFileName(pProto->getFileName());
        m_pModuleResource->setDataPath(pProto->getPath());
    }
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetRessource::setupPrototypeFromWidgets(Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<RessourcePrototype*>(pPrototype))
    {
        pProto->setFileName(m_pModuleResource->getDataFileName());
        pProto->setPath(m_pModuleResource->getDataPath());
    }
    DatabaseWidgetBase::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetRessource::clear()
{
    m_pModuleResource->setDataFileName("");
    m_pModuleResource->setDataPath("");
    DatabaseWidgetBase::clear();
}

void DatabaseWidgetRessource::_onClickMassFileImportButton()
{
    auto files = _selectFiles();
    auto pModel = dynamic_cast<DatabaseModel*>(getDatabaseModel());
    if (!pModel)
        return;
    auto pDatabase = pModel->getDatabase();
    if (!pDatabase)
        return;
    if (!files.isEmpty())
    {
        uint32 currentID = pDatabase->getSize();
        for (auto& file : files)
        {
            std::unique_ptr<RessourcePrototype> pPrototype(dynamic_cast<RessourcePrototype*>(pDatabase->getNewPrototype(++currentID)));
            if (!pPrototype || file.isEmpty())
                continue;

            QFileInfo fileInfo(file);
            if (fileInfo.isFile())
            {
                QString path(fileInfo.absolutePath());
                path = path.remove(0, path.lastIndexOf("/Textures")+10);
                if (!path.isEmpty())
                {
                    path += "/";
                    pPrototype->setPath(path);
                }
                pPrototype->setFileName(fileInfo.fileName());
                pPrototype->setName(fileInfo.fileName());
                pModel->resize(currentID);
                BASIC_LOG("File import succeeded: ID: " + QString::number(currentID) + "; file name: " + pPrototype->getPathName());

                pDatabase->setPrototype(pPrototype.release());
            }
        }
        m_pModuleList->selectItem(currentID);
    }
}

void DatabaseWidgetRessource::_onClickFileButton()
{
    _import(m_pModuleList->getDataID(), _selectFile());
}

void DatabaseWidgetRessource::_import(uint32 uiID, const QString& fileNamePath)
{
    if (!uiID || fileNamePath.isEmpty())
        return;

    QFileInfo fileInfo(fileNamePath);
    if (fileInfo.isFile())
    {
        // clear previous data
        m_pModuleResource->setDataFileName("");
        m_pModuleResource->setDataPath("");

        QString path(fileInfo.absolutePath());
        path = path.remove(0, path.lastIndexOf("/Textures")+10);
        if (!path.isEmpty())
        {
            path += "/";
            m_pModuleResource->setDataPath(path);
        }
        m_pModuleResource->setDataFileName(fileInfo.fileName());
        // if name is empty fill with filename
        if (m_pModuleList->getDataName().isEmpty())
            m_pModuleList->setDataName(fileInfo.fileName());
        BASIC_LOG("File import succeeded: ID: " + QString::number(m_pModuleList->getDataID()) + "; file name: " + path+fileInfo.fileName());

        saveCurrent();
        emit fileImport(m_pModuleList->getDataID());
    }
}

