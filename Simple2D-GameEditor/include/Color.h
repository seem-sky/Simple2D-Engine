#ifndef COLOR_H
#define COLOR_H

#include <Global.h>

class Color
{
public:
    Color(int16 red = -1, int16 green = -1, int16 blue = -1);
    Color(const std::string& colorText);

    void setColor(int16 red, int16 green, int16 blue) { m_Red = red; m_Green = green; m_Blue = blue; }
    void setColor(const std::string& sColorText);

    std::string getColorString() const;
    int16 getRed() const { return m_Red; }
    int16 getGreen() const { return m_Green; }
    int16 getBlue() const { return m_Blue; }

    bool isValid() const { return m_Red >= 0 && m_Green >= 0 && m_Blue >= 0; }
    void clear() { m_Red = -1; m_Green = -1; m_Blue = -1; }

private:
    int16 m_Red;
    int16 m_Green;
    int16 m_Blue;
};
#endif