#ifndef COLOR_H
#define COLOR_H

#include "Global.h"

class Color
{
public:
    Color(const std::string &colorText);
    Color(int32 red = -1, int32 green = -1, int32 blue = -1);

    inline void setColor(int32 red, int32 green, int32 blue) { m_Red = red; m_Green = green; m_Blue = blue; }
    void setColor(const std::string &sColorText);

    inline void getColor(int32 &red, int32 &green, int32 &blue) const { red = m_Red; green = m_Green; blue = m_Blue; }
    std::string getColorString() const;
    inline int32 getRed() const { return m_Red; }
    inline int32 getGreen() const { return m_Green; }
    inline int32 getBlue() const { return m_Blue; }

    inline bool hasValidColor() const { if (m_Red >= 0 && m_Green >= 0 && m_Blue >= 0) return true; return false; }
    inline void clearColor() { m_Red = -1; m_Green = -1; m_Blue = -1; }

private:
    int32 m_Red;
    int32 m_Green;
    int32 m_Blue;
};
#endif