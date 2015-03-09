#pragma once

#include "UI/UI_AnimationModuleSelectedSprite.h"
#include <Animation/SpriteData.h>

namespace animation
{
    namespace ui
    {
        namespace module
        {
            class SelectedSprite : public QWidget, Ui_AnimationModuleSelectedSprite
            {
                Q_OBJECT
            private:
                void _connect();
                void _disconnect();

            public:
                SelectedSprite(QWidget* pParent = nullptr);

                void clear();

            private slots:
                void _onPositionChanged(const QPoint&);
                void _onZValueChanged(qreal);
                void _onScaleChanged(qreal);
                void _onOpacityChanged(qreal);
                void _onRotationChanged(qreal);

            public slots:
                void onSelectedChanged(const animation::SpriteData& sprite);
                void onSelectionCleared();

            signals:
                void positionChanged(const QPoint& pos);
                void zValueChanged(qreal zValue);
                void scaleChanged(qreal scale);
                void opacityChanged(qreal opacity);
                void rotationChanged(qreal rotation);
            };
        }
    }
}
