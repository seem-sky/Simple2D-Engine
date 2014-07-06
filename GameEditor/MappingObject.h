#ifndef MAPPING_OBJECT_H
#define MAPPING_OBJECT_H

#include <QtCore/QObject>
#include <QtGui/QMouseEvent>
#include <memory>
#include "MappingModeInterface.h"
#include <QtCore/QStringList>

class MapEditorModuleWorldObjects;
class MapViewer;
namespace BRUSH
{
    class MapEditorWidgetBrush;
}

class MappingObject : public QObject
{
private:
    const MAPPING_MODE::Interface* _getMode(MAPPING_MODE::Type mode) const;
    MAPPING_MODE::Interface* _getMode(MAPPING_MODE::Type mode);

public:
    MappingObject(QWidget* pParent, const BRUSH::MapEditorWidgetBrush& brushWidget, const MapEditorModuleWorldObjects& objectWidget);

    void setMappingMode(MAPPING_MODE::Type mode);
    MAPPING_MODE::Type getMappingModeType() const;

    void press(MapViewer* pViewer, const QMouseEvent* pEvent);
    void release(MapViewer* pViewer, const QMouseEvent* pEvent);
    void move(MapViewer* pViewer, const QMouseEvent* pEvent);

    QStringList getMappingModeNames() const;

private:
    typedef std::unique_ptr<MAPPING_MODE::Interface> MappingModePtr;
    std::vector<MappingModePtr> m_MappingModes;
    MAPPING_MODE::Interface* m_pCurrentMappingMode = nullptr;
};
#endif
