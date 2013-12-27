#ifndef DATABASE_WIDGET_SPRITE_H
#define DATABASE_WIDGET_SPRITE_H

#include "DatabaseWidgetTexture.h"

class DatabaseWidgetSprite : public DatabaseWidgetTexture
{
public:
    DatabaseWidgetSprite(QWidget* pParent = nullptr);

    void setDatabaseMgr(DATABASE::DatabaseMgr& DBMgr);
};

#endif
