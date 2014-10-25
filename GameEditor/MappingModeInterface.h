#ifndef MAPPING_MODE_INTERFACE_H
#define MAPPING_MODE_INTERFACE_H

#include <QtGui/QKeyEvent>
#include <QtCore/QObject>
#include <Global.h>
#include "EditorGlobal.h"

class MapEditor;

namespace MAPPING_MODE
{
    class Interface : public QObject
    {
    public:
        Interface(QObject* pParent = nullptr) : QObject(pParent) {}

        virtual void press(MapEditor& editor, QMouseEvent* pEvent) = 0;
        virtual void release(MapEditor& editor, QMouseEvent* pEvent) = 0;
        virtual void move(MapEditor& editor, QMouseEvent* pEvent) = 0;
        virtual void copy(const MapEditor& editor) = 0;
        virtual void cut(const MapEditor& editor) = 0;
        virtual void paste(MapEditor& editor, const QPoint& pos) = 0;
        virtual void remove(MapEditor& editor) = 0;
        virtual void keyPress(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent) = 0;
        virtual void keyRelease(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent) = 0;

        virtual Type getModeType() const = 0;
        virtual QString getModeName() const = 0;
    };
}
#endif
