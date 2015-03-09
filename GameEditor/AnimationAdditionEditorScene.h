#pragma once

#include "AnimationEditorScene.h"

namespace database
{
    namespace prototype
    {
        class AnimationAddition;
        class AnimationAdditionPoint;
        class AnimationAdditionRect;
    }
}

namespace animation
{
    namespace addition
    {
        class EditorScene : public animation::EditorScene
        {
            Q_OBJECT

        public:
            EditorScene(const helper::CreatorInterface& creator, QObject* pParent = nullptr);

            virtual void clearContents() = 0;

            virtual void setup(const database::prototype::AnimationAddition* pAddition);
            virtual void setupFrom(database::prototype::AnimationAddition* pAddition) const;
        };

        /*#####
        # PointEditorScene
        #####*/
        class PointEditorScene : public EditorScene
        {
        public:
            PointEditorScene(const helper::CreatorInterface& creator, QObject* pParent);

            void clearContents() override;

            void setup(const database::prototype::AnimationAddition* pAddition);
            void setupFrom(database::prototype::AnimationAddition* pAddition) const;

        private:
            PointAdditionEditorItem* m_pPointItem = nullptr;
        };
    }
}
