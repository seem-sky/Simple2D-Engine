#ifndef TEXTURE_PROTOTYPE_FRAME_H
#define TEXTURE_PROTOTYPE_FRAME_H

#include <QtWidgets/QFrame>
#include "DatabasePrototypes.h"

class TexturePrototypeFrame : public QFrame
{
protected:
    virtual void paintEvent(QPaintEvent* pEvent);

public:
    TexturePrototypeFrame(QWidget* pParent = nullptr);

    void setPrototype(const DATABASE::TexturePrototype* pPrototype);
    uint32 getID() const;

private:
    const DATABASE::TexturePrototype* m_pPrototype;
};

#endif
