#pragma once

#include <QtWidgets/QWidget>
#include "UI/UI_AnimationModuleSelectedItem.h"

namespace animation
{
    namespace ui
    {
        namespace module
        {
            class SelectedItem : public QWidget, Ui_AnimationModuleSelectedItem
            {
                Q_OBJECT
            private:
                void _connect();
                void _disconnect();

            public:
                SelectedItem(QWidget* pParent = nullptr);

                virtual void clear();

            private slots:
                void _onPositionChanged(int);
                void _onZValueChanged(qreal);
                void _onScaleChanged(qreal);
                void _onOpacityChanged(qreal);
                void _onRotationChanged(qreal);

            public slots:
                void onPositionChanged(const QPoint& pos);
                void onZValueChanged(qreal value);
                void onScaleChanged(qreal value);
                void onOpacityChanged(qreal value);
                void onRotationChanged(qreal value);

            signals:
                void positionChanged(const QPoint& pos);
                void zValueChanged(qreal);
                void scaleChanged(qreal);
                void opacityChanged(qreal);
                void rotationChanged(qreal);
            };
        }
    }
}
