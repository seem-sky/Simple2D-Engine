#ifndef DATABASE_MODULE_RESOURCE_H
#define DATABASE_MODULE_RESOURCE_H

#include "UI/UI_DatabaseModuleResource.h"

class DatabaseModuleResource : public QWidget, protected Ui_DatabaseModuleResource
{
public:
    DatabaseModuleResource(QWidget *pParent = nullptr);

    // data
    inline void setDataFileName(const QString &fileName) { m_pFileName->setText(fileName); }
    inline QString getDataFileName() const { return m_pFileName->text(); }

    inline void setDataPath(const QString &path) { m_pPath->setText(path); }
    inline QString getDataPath() const { return m_pPath->text(); }

    inline QPushButton* getFileButton() { return m_pChoseFile; }
};

#endif
