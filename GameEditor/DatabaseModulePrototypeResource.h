#pragma once

#include "DatabaseModulePrototypeInterface.h"
#include "UI/UI_DatabaseModulePrototypeResource.h"
#include <Global.h>

namespace database
{
    namespace prototype
    {
        class Resource;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeResource : public PrototypeInterface<prototype::Resource>, Ui_DatabaseModulePrototypeResource
            {
                Q_OBJECT
            public:
                PrototypeResource(QWidget* pParent = nullptr);

                void setup(const prototype::Resource* pPrototype) override;
                void setupFrom(prototype::Resource* pPrototype) const override;

                QString getPath() const;
                QString getFileName() const;

                void clear();

                void setFileDialogInfos(const QString& dir, const QString& extensions, const QString& cutPath);

                static void import(prototype::Resource& prototype, const QString& fileName, const QString& cutPath);

            private slots:
                void _onImportButtonClicked();

            signals:
                void imported(const QString& pathName);

            private:
                QString m_Directory;
                QString m_Extensions;
                QString m_CutPath;
            };
        }
    }
}