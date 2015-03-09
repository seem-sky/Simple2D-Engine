#ifndef COLOR_H
#define COLOR_H

#include <Global.h>

class Color
{
public:
    Color(int16 red = -1, int16 green = -1, int16 blue = -1);

    void setColor(int16 red, int16 green, int16 blue);

    int16 getRed() const;
    int16 getGreen() const;
    int16 getBlue() const;

    void setRed(int16 red);
    void setGreen(int16 green);
    void setBlue(int16 blue);

    bool isEmpty() const;
    void clear();

private:
    int16 m_Red;
    int16 m_Green;
    int16 m_Blue;
};

bool operator==(const Color& lhs, const Color& rhs);
bool operator!=(const Color& lhs, const Color& rhs);
#endif
