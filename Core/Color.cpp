#include "Color.h"
Color::Color(int32 red, int32 green, int32 blue) : m_Red(red), m_Green(green), m_Blue(blue)
{}

Color::Color(const std::string &p_sColorText) : m_Red(-1), m_Green(-1), m_Blue(-1)
{
    setColor(p_sColorText);
}

void Color::setColor(const std::string &p_sColorText)
{
    if (p_sColorText.empty())
        return;
    std::string t_sColorBuffer = p_sColorText.substr(p_sColorText.find("R")+1);
    m_Red = atoi(t_sColorBuffer.c_str());
    t_sColorBuffer = t_sColorBuffer.substr(t_sColorBuffer.find("G")+1);
    m_Green = atoi(t_sColorBuffer.c_str());
    t_sColorBuffer = t_sColorBuffer.substr(t_sColorBuffer.find("B")+1);
    m_Blue = atoi(t_sColorBuffer.c_str());
}

std::string Color::getColorString() const
{
    if (hasValidColor())
        return "R"+ ToString(m_Red) + "G" + ToString(m_Green) + "B" + ToString(m_Blue);
    return "";
}