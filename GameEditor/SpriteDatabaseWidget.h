#ifndef SPRITE_DATABASE_WIDGET_H
#define SPRITE_DATABASE_WIDGET_H

#include "TextureDatabaseWidget.h"
#include "UI/UI_SpriteDatabaseWidget.h"

class SpriteDatabaseWidget : public TextureDatabaseWidget<DATABASE::SpritePrototype>, Ui_SpriteDatabaseWidget
{
    Q_OBJECT
private:
    void _updatePixmap();

protected:
    void changeCurrentItem(uint32 uiID);
    QPixmap showPixmap(const QString &sFilePath);
    bool eventFilter(QObject *pObj, QEvent *pEvent);

public:
    SpriteDatabaseWidget(QWidget *pParent = NULL);

private slots:
    TEXTURE_DATABASE_WIDGET_SLOTS

private:
    QPixmap m_Pixmap;
    float m_CurScale;
};
#endif