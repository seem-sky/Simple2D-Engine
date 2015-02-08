#ifndef CACHE_TEXTURE_ATLAS_H
#define CACHE_TEXTURE_ATLAS_H

#include <QtGui/QPixmap>
#include <Typedefs.h>
#include <Bitset2D.h>

namespace CACHE
{
    template <uint16 textureWidth, uint16 textureHeight>
    class TextureAtlas
    {
    public:
        TextureAtlas(uint32 width, uint32 height)
            : m_Texture(width, height)
        {
            m_Texture.fill(Qt::transparent);
            m_Bitset.resize(width / textureWidth, height / textureHeight);
        }

        TextureAtlas(const GEOMETRY::Point<uint32>& pixmapSize)
            : TextureAtlas(pixmapSize.getX(), pixmapSize.getY())
        {}

        void add(const GEOMETRY::Point<uint32>& pos, const QPixmap& pixmap)
        {
            m_Bitset.set(pos, true);
            QPainter painter(&m_Texture);
            painter.drawPixmap(pos.getX() * textureWidth, pos.getY() * textureHeight, textureWidth, textureHeight, pixmap);
        }

        void remove(const GEOMETRY::Point<uint32>& pos)
        {
            m_Bitset.set(pos, false);
        }

        bool isEmpty(const GEOMETRY::Point<uint32>& pos) const
        {
            return !m_Bitset.get(pos);
        }

        bool isFilled() const
        {
            GEOMETRY::Point<uint32> pos;
            for (; pos.getY() < m_Bitset.height(); ++pos.getY())
            {
                for (pos.getX() = 0; pos.getX() < m_Bitset.width(); ++pos.getX())
                {
                    if (!m_Bitset.get(pos))
                        return false;
                }
            }
            return true;
        }

        GEOMETRY::Point<uint32> getEmptyPosition() const
        {
            GEOMETRY::Point<uint32> pos;
            for (; pos.getY() < m_Bitset.height(); ++pos.getY())
            {
                for (pos.getX() = 0; pos.getX() < m_Bitset.width(); ++pos.getX())
                {
                    if (isEmpty(pos))
                        return pos;
                }
            }

            throw std::runtime_error("No empty position available.");
        }

        void clear()
        {
            m_Texture = QPixmap(m_Texture.size());
            m_Texture.fill(Qt::transparent);

            m_Bitset.clear();
            m_Bitset.resize(m_Texture.width() / textureWidth, m_Texture.height() / textureHeight);
        }

        const QPixmap* getTexture() const
        {
            return &m_Texture;
        }

    private:
        QPixmap m_Texture;
        Bitset2D m_Bitset;
    };
}
#endif