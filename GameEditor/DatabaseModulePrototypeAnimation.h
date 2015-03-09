#pragma once

#include "DatabaseModulePrototypeInterface.h"
#include "HelperCreatorInterface.h"
#include "UI/UI_DatabaseModulePrototypeAnimation.h"
#include <Animation/FrameData.h>
#include "AnimationEditorScene.h"

namespace database
{
    namespace prototype
    {
        class Animation;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeBase;
            class PrototypeAnimation : public PrototypeInterface < prototype::Animation >, Ui_DatabaseModulePrototypeAnimation
            {
                Q_OBJECT
            public:
                PrototypeAnimation(const helper::CreatorInterface& creator, database::Model* pAdditionTypeModel, QWidget* pParent = nullptr);

                void setup(const prototype::Animation* pPrototype) override;
                void setupFrom(prototype::Animation* pPrototype) const override;

                void clear();

            private slots:
                void _onSaveCurrentFrame(uint32 index, uint32 time);

            private:
                animation::FrameVector m_Frames;
                animation::EditorScene* m_pScene = nullptr;
            };
        }
    }
}
