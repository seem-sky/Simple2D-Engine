#ifndef DATABASE_MODULE_TEXTURE_H
#define DATABASE_MODULE_TEXTURE_H

#include "UI/UI_DatabaseModuleTexture.h"
#include <Global.h>
#include "Color.h"

class DatabaseModuleTexture : public QWidget, protected Ui_DatabaseModuleTexture
{
    Q_OBJECT
public:
    DatabaseModuleTexture(QWidget *pParent = nullptr);

    // data
    inline void setDataRed(int32 color) { m_pColorRed->setValue(color); }
    inline int32 getDataRed() const { return m_pColorRed->value(); }
    inline void setDataGreen(int32 color) { m_pColorGreen->setValue(color); }
    inline int32 getDataGreen() const { return m_pColorGreen->value(); }
    inline void setDataBlue(int32 color) { m_pColorBlue->setValue(color); }
    inline int32 getDataBlue() const { return m_pColorBlue->value(); }

    // widgets
    inline QGraphicsView* getTextureView() { return m_pView; }
    void setPixmap(const QPixmap &pixmap);

private slots:
    void _onTransparencyColorChanged(int value);

signals:
    void transparencyColorChanged(const Color &color);
};

#endif
