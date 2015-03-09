#include "Color.h"

Color::Color(int16 red, int16 green, int16 blue)
    : m_Red(red), m_Green(green), m_Blue(blue)
{}

void Color::setColor(int16 red, int16 green, int16 blue)
{
    m_Red = red;
    m_Green = green;
    m_Blue = blue;
}

int16 Color::getRed() const
{
    return m_Red;
}

int16 Color::getGreen() const
{
    return m_Green;
}

int16 Color::getBlue() const
{
    return m_Blue;
}

void Color::setRed(int16 red)
{
    m_Red = red;
}

void Color::setGreen(int16 green)
{
    m_Green = green;
}

void Color::setBlue(int16 blue)
{
    m_Blue = blue;
}

bool Color::isEmpty() const
{
    return m_Red < 0 || m_Green < 0 || m_Blue < 0;
}

void Color::clear()
{
    m_Red = -1;
    m_Green = -1;
    m_Blue = -1;
}

bool operator==(const Color& lhs, const Color& rhs)
{
    return lhs.getRed() == rhs.getRed() && lhs.getGreen() == rhs.getGreen() && lhs.getBlue() == rhs.getBlue();
}
