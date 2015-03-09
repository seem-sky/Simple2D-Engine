#pragma once

#include <QtWidgets/QGraphicsScene>
#include "AnimationEditorItem.h"
#include <animation/FrameData.h>

namespace animation
{
    class EditorScene : public QGraphicsScene
    {
        Q_OBJECT
    protected:
        void dragEnterEvent(QGraphicsSceneDragDropEvent* pEvent);
        void dragMoveEvent(QGraphicsSceneDragDropEvent* pEvent);
        void dropEvent(QGraphicsSceneDragDropEvent* pEvent);
        void drawBackground(QPainter* pPainter, const QRectF& rect);

    public:
        EditorScene(const helper::CreatorInterface& creator, QObject* pParent = nullptr);

        AdditionEditorItem* addAddition(const animation::AdditionData& data);
        SpriteEditorItem* addSprite(const animation::SpriteData& data);

        FrameData getFrame() const;

    private slots:
        void _onSelectionChanged();
        void _onSpriteItemChanged(const SpriteEditorItem* pItem);
        void _onSpriteItemChanged(const AdditionEditorItem* pItem);

    public slots:
        void onPositionChanged(const QPoint& pos);
        void onSizeChanged(const QSize& size);
        void onZValueChanged(qreal zValue);
        void onScaleChanged(qreal scale);
        void onOpacityChanged(qreal opacity);
        void onRotationChanged(qreal rotation);
        void onAdditionTypeIDChanged(uint32 ID);
        void onPointAdditionAdd();
        void onRectAdditionAdd();
        void setup(const FrameData& frame);

    signals:
        void selectionCleared();
        void selectionDataChanged(const animation::AdditionData& data);
        void selectionDataChanged(const animation::SpriteData& data);

    private:
        const helper::CreatorInterface& m_Creator;
    };
}
