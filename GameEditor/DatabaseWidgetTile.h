#ifndef DATABASE_WIDGET_TILE_H
#define DATABASE_WIDGET_TILE_H

#include "DatabaseWidgetTexture.h"

typedef std::array<QPushButton*, 4> ButtonPtrArray;

class DatabaseWidgetTile : public DatabaseWidgetTexture
{
    Q_OBJECT
private:
    void _updatePassabilityIcons();

protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::PROTOTYPE::Prototype* pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::PROTOTYPE::Prototype* pPrototype);
    virtual void clear();

    void setDataPassability(uint8 passability);
    uint8 getDataPassability() const;

public:
    DatabaseWidgetTile(QWidget* pParent = nullptr);

    DATABASE::TileDatabase* getDatabase() const;
    void setDatabaseMgr(DATABASE::DatabaseMgr& DBMgr);

private slots:
    void _onPassabilityButtonClicked();

private:
    ButtonPtrArray m_Buttons;
};

#endif
