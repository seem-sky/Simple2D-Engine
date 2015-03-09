#ifndef TEXTURE_PROTOTYPE_FRAME_H
#define TEXTURE_PROTOTYPE_FRAME_H

#include <Global.h>
#include <QtWidgets/QFrame>

namespace database
{
    namespace prototype
    {
        class Texture;
    }
}

class TexturePrototypeFrame : public QFrame
{
protected:
    virtual void paintEvent(QPaintEvent* pEvent);

public:
    TexturePrototypeFrame(QWidget* pParent = nullptr);

    void setPrototype(const database::prototype::Texture* pPrototype);
    uint32 getID() const;

private:
    const database::prototype::Texture* m_pPrototype;
};

#endif
