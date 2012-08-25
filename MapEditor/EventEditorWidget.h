#ifndef EVENT_EDITOR_WIDGET
#define EVENT_EDITOR_WIDGET

#include "UI/UI_EventEditor.h"

class EventEditorWidget : public QWidget, Ui_EventEditor
{
    Q_OBJECT
public:
    EventEditorWidget(QWidget *p_pParent = NULL);
    ~EventEditorWidget(void);
};
#endif