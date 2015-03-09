#pragma once

#include "DatabaseModulePrototypeInterface.h"
#include "HelperCreatorInterface.h"
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QComboBox>
#include "DatabaseModel.h"
#include "DatabaseModuleList.h"
#include "AnimationAdditionEditorScene.h"
#include "UI/UI_DatabaseModulePrototypeAnimationAddition.h"

namespace database
{
    namespace prototype
    {
        class AnimationAddition;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeBase;
            class PrototypeAnimationAddition : public PrototypeInterface <prototype::AnimationAddition>, Ui_DatabaseModulePrototypeAnimationAddition
            {
                Q_OBJECT
            public:
                PrototypeAnimationAddition(animation::addition::EditorScene* pScene, const helper::CreatorInterface& creator, database::Model* pTypeModel,
                    QWidget* pParent = nullptr);

                void setup(const prototype::AnimationAddition* pPrototype);
                void setupFrom(prototype::AnimationAddition* pPrototype) const;

                void clear();

            private slots:
                void _onZoomChanged(int value);

            private:
                animation::addition::EditorScene* m_pScene = nullptr;
            };
        }
    }
}