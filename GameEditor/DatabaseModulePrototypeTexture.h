#pragma once

#include "DatabaseModulePrototypeInterface.h"
#include "UI/UI_DatabaseModulePrototypeTexture.h"
#include <Global.h>

namespace database
{
    namespace prototype
    {
        class Texture;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeTexture : public PrototypeInterface<prototype::Texture>, Ui_DatabaseModulePrototypeTexture
            {
                Q_OBJECT
            public:
                PrototypeTexture(QWidget* pParent = nullptr);

                void setup(const prototype::Texture* pPrototype) override;
                void setupFrom(prototype::Texture* pPrototype) const override;

                QGraphicsView* getTextureView() const;
                void setFileDialogInfos(const QString& dir, const QString& extensions, const QString& cutPath);

                void clear();

            private slots:
                void _onImported(const QString& pathName);
                void _onTransparencyColorChanged();
                void _onChoseColorButtonClicked();

            private:
                QString m_RootDirectory;
            };
        }
    }
}