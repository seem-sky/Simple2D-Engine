#ifndef DATABASE_WIDGET_TEXTURE_H
#define DATABASE_WIDGET_TEXTURE_H

#include "DatabaseWidgetRessource.h"
#include "DatabaseModuleTexture.h"

class DatabaseWidgetTexture : public DatabaseWidgetRessource
{
    Q_OBJECT
private:
    virtual QString _selectFile() const;
    virtual QStringList _selectFiles() const;

    QPixmap _createPrototypePixmap(uint32 uiID) const;

protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype);
    virtual void clear();

    virtual void setupPixmap(const QPixmap& pixmap);

private slots:
    void _onFileImport(uint32 uiID);
    void _onTransparencyColorChanged(const Color& color);

public:
    DatabaseWidgetTexture(QWidget* pParent = nullptr);

protected:
    DatabaseModuleTexture* m_pModuleTexture;
};

#endif
