#ifndef DATABASE_MODULE_TEXTURE_H
#define DATABASE_MODULE_TEXTURE_H

#include "UI/UI_DatabaseModuleTexture.h"
#include <Global.h>
#include "Color.h"

class DatabaseModuleTexture : public QWidget, protected Ui_DatabaseModuleTexture
{
    Q_OBJECT
public:
    DatabaseModuleTexture(QWidget* pParent = nullptr);

    // data
    void setTransparencyColor(const Color& color);
    Color getTransparencyColor() const;
    void setDataRed(int32 color) { m_pColorRed->setValue(color); }
    int32 getDataRed() const { return m_pColorRed->value(); }
    void setDataGreen(int32 color) { m_pColorGreen->setValue(color); }
    int32 getDataGreen() const { return m_pColorGreen->value(); }
    void setDataBlue(int32 color) { m_pColorBlue->setValue(color); }
    int32 getDataBlue() const { return m_pColorBlue->value(); }

    // widgets
    inline QGraphicsView* getTextureView() { return m_pView; }
    void setPixmap(const QPixmap& pPixmap);

    void clear();

private slots:
    void _onTransparencyColorChanged(int value);
    void _onChoseColorButtonClicked();

signals:
    void transparencyColorChanged(const Color& color);
    void choseColorButtonClicked();
};

#endif
