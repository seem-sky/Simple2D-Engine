#ifndef TEXTURE_PROTOTYPE_FRAME_H
#define TEXTURE_PROTOTYPE_FRAME_H

#include <QtWidgets/QFrame>
#include "DatabasePrototypes.h"

class TexturePrototypeFrame : public QFrame
{
protected:
    virtual void paintEvent(QPaintEvent *pEvent);

public:
    TexturePrototypeFrame(QWidget *pParent = nullptr);

    virtual const DATABASE::TexturePrototype* getTexturePrototype() const = 0;
};

#endif
