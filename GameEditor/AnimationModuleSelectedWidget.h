#pragma once

#include <QtWidgets/QWidget>
#include "AnimationModuleSelectedAddition.h"
#include "AnimationModuleSelectedSprite.h"

namespace animation
{
    namespace ui
    {
        namespace module
        {
            class SelectedWidget : public QWidget
            {
                Q_OBJECT
            public:
                SelectedWidget(QWidget* pParent = nullptr);

                SelectedSprite* getModuleSelectedSprite() const;
                SelectedAddition* getModuleSelectedAddition() const;

                void setAdditionTypeModel(database::Model* pModel);

                void clear();

            public slots:
                void onSelectedChanged(const animation::AdditionData& data);
                void onSelectedChanged(const animation::SpriteData& data);
                void onSelectionCleared();

            private:
                SelectedSprite* m_pSelectedSprite = nullptr;
                SelectedAddition* m_pSelectedAddition = nullptr;
            };
        }
    }
}
