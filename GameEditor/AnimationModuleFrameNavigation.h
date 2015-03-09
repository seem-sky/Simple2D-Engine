#pragma once

#include <QtWidgets/QWidget>
#include "UI/UI_AnimationModuleFrameNavigation.h"
#include <Animation/FrameData.h>

namespace animation
{
    namespace ui
    {
        namespace module
        {
            class FrameNavigation : public QWidget, Ui_AnimationModuleFrameNavigation
            {
                Q_OBJECT
            private:
                void _setupFrame(uint32 index);
                void _connect();
                void _disconnect();

            public:
                FrameNavigation(QWidget* pParent = nullptr);

                void setup(FrameVector& frame);

                void clear();
                
                void saveCurrent();

            private slots:
                void _onTimeChanged(int value);
                void _onIndexChanged(int value);
                void _onFrameAdded();
                void _onFrameRemoved();
                void _onFrameNext();
                void _onFramePrevious();

            signals:
                void saveFrame(uint32 index, uint32 time);
                void frameSelectionChanged(const FrameData& frame);

            private:
                FrameVector* m_pFrames = nullptr;
                uint32 m_CurrentIndex = 0;
            };
        }
    }
}
