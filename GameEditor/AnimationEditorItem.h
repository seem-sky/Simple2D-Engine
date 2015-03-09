#pragma once

#include <QtWidgets/QGraphicsObject>
#include <animation/SpriteData.h>
#include <animation/AdditionData.h>
#include "HelperCreatorInterface.h"
#include <QtGlobal.h>
#include <FlagManager.h>

namespace animation
{

    class EditorItem : public QGraphicsObject
    {
    private:
        virtual void _emitChange() = 0;

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant& value);
        void keyPressEvent(QKeyEvent* pEvent);
        void wheelEvent(QGraphicsSceneWheelEvent* pEvent);

        void _setup(const ItemData& data);
        void _setupFrom(ItemData& data) const;

    public:
        EditorItem();

        enum class ModifyFlags
        {
            none = 0x00,
            rotating = 0x01,
            scalable = 0x02,
            opacity = 0x04,
            zValue = 0x08
        };
        void setModifyFlag(ModifyFlags flag, bool apply);

    private:
        FlagManager<ModifyFlags, uint8> m_Flags;
    };

    class SpriteEditorItem : public EditorItem
    {
        Q_OBJECT
    private:
        void _emitChange() override;

    public:
        SpriteEditorItem(const helper::CreatorInterface& creator);
        SpriteEditorItem(const SpriteData& sprite, const helper::CreatorInterface& creator);

        void setID(uint32 ID);
        uint32 getID() const;

        void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr);
        QRectF boundingRect() const;

        SpriteData createSpriteData() const;
        void setup(const SpriteData& sprite);

    signals:
        void changed(const SpriteEditorItem* pItem);

    private:
        uint32 m_ID = 0;
        QPixmap m_Pixmap;
        const helper::CreatorInterface& m_Creator;
    };

    class AdditionEditorItem : public EditorItem
    {
        Q_OBJECT
    private:
        void _emitChange() override;

    protected:
        void _setup(const AdditionData& data);
        void _setupFrom(AdditionData& data) const;

    public:
        AdditionEditorItem();

        virtual AdditionData createAdditionData() const = 0;
        virtual void setup(const AdditionData& data) = 0;

        uint32 getAdditionTypeID() const;
        void setAdditionTypeID(uint32 ID);

    signals:
        void changed(const AdditionEditorItem* pItem);

    private:
        uint32 m_AdditionTypeID = 0;
    };

    class PointAdditionEditorItem : public AdditionEditorItem
    {
    public:
        PointAdditionEditorItem();
        PointAdditionEditorItem(const AdditionData& sprite);

        void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr);
        QRectF boundingRect() const;

        AdditionData createAdditionData() const;
        void setup(const AdditionData& data);

        const QPen& getPen() const;
        void setPen(const QPen& pen);

        void setCrossSize(const QSize& size);
        void setCrossSize(int width, int height);
        QSize getCrossSize() const;

    private:
        DiagonalCross m_Cross;
    };
}
