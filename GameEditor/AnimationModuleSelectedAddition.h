#pragma once

#include "UI/UI_AnimationModuleSelectedAddition.h"
#include <Animation/AdditionData.h>
#include "DatabaseModel.h"

namespace animation
{
    namespace ui
    {
        namespace module
        {
            class SelectedAddition : public QWidget, Ui_AnimationModuleSelectedAddition
            {
                Q_OBJECT
            private:
                void _connect();
                void _disconnect();

            public:
                SelectedAddition(QWidget* pParent = nullptr);

                void clear();

                void setAdditionTypeModel(database::Model* pModel);

            private slots:
                void _onPositionChanged(const QPoint&);
                void _onZValueChanged(qreal);
                void _onScaleChanged(qreal);
                void _onOpacityChanged(qreal);
                void _onRotationChanged(qreal);
                void _onSizeChanged(int);
                void _onAdditionTypeIDChanged(int);

            public slots:
                void onSelectedChanged(const animation::AdditionData& data);
                void onSelectionCleared();

            signals:
                void positionChanged(const QPoint& pos);
                void sizeChanged(const QSize& size);
                void zValueChanged(qreal zValue);
                void scaleChanged(qreal scale);
                void opacityChanged(qreal opacity);
                void rotationChanged(qreal rotation);
                void additionTypeIDChanged(uint32 ID);
            };
        }
    }
}
