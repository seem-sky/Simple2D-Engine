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
    private:
        GEOMETRY::Point<uint32> _getEmptyPosition() const
        {
            GEOMETRY::Point<uint32> pos;
            for (; pos.getY() < m_Bitset.height(); ++pos.getY())
            {
                for (pos.getX() = 0; pos.getX() < m_Bitset.width(); ++pos.getX())
                {
                    if (!m_Bitset.get(pos))
                        return pos;
                }
            }

            throw std::runtime_error("No empty position available.");
        }

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

        GEOMETRY::Point<uint32> get(uint32 ID) const
        {
            if (!ID || ID > m_Positions.size())
                return GEOMETRY::Point<uint32>(MATH::maximum<uint32>(), MATH::maximum<uint32>());
            return m_Positions.at(ID - 1);
        }

        void add(uint32 ID, const QPixmap& pixmap)
        {
            if (!ID)
                return;

            if (m_Positions.size() <= ID)
                m_Positions.resize(ID, GEOMETRY::Point<uint32>(MATH::maximum<uint32>(), MATH::maximum<uint32>()));

            if (isEmpty(ID))
            {
                m_Positions.at(ID - 1) = _getEmptyPosition();
                m_Bitset.set(m_Positions.at(ID - 1));
            }

            QPainter painter(&m_Texture);
            painter.drawPixmap(m_Positions.at(ID - 1).getX() * textureWidth, m_Positions.at(ID - 1).getY() * textureHeight, textureWidth, textureHeight, pixmap);
        }

        void remove(uint32 ID)
        {
            if (!ID || isEmpty(ID))
                return;

            m_Bitset.set(m_Positions.at(ID).getX(), m_Positions.at(ID).getY(), false);
            m_Positions.at(ID) = GEOMETRY::Point<uint32>(MATH::maximum<uint32>(), MATH::maximum<uint32>());
        }

        bool isEmpty(uint32 ID) const
        {
            if (!ID || m_Positions.size() < ID)
                return true;
            return m_Positions.at(ID - 1) == GEOMETRY::Point<uint32>(MATH::maximum<uint32>(), MATH::maximum<uint32>());
        }

        void clear()
        {
            m_Texture = QPixmap(m_Texture.size());
            m_Texture.fill(Qt::transparent);

            m_Bitset.clear();
            m_Bitset.resize(m_Texture.width() / textureWidth, m_Texture.height() / textureHeight);

            m_Positions.clear();
        }

        const QPixmap& getTexture() const
        {
            return m_Texture;
        }

    private:
        QPixmap m_Texture;
        PointVec<uint32> m_Positions;
        Bitset2D m_Bitset;
    };
}
#endif