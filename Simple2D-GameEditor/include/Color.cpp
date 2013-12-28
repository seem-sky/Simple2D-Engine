#include "Color.h"

Color::Color(int32 red, int32 green, int32 blue) : m_Red(red), m_Green(green), m_Blue(blue)
{}

Color::Color(const std::string& colorText) : m_Red(-1), m_Green(-1), m_Blue(-1)
{
    setColor(colorText);
}

void Color::setColor(const std::string& colorText)
{
    if (colorText.empty())
        return;
    std::string tempColorText = colorText.substr(colorText.find("R")+1);
    // setup red
    m_Red = std::stoi(tempColorText.substr(0, tempColorText.find("G")));
    tempColorText = tempColorText.substr(tempColorText.find("G")+1);
    // setup green
    m_Green = std::stoi(tempColorText.substr(0, tempColorText.find("B")));
    tempColorText = tempColorText.substr(tempColorText.find("B")+1);
    // setup blue
    m_Blue = std::stoi(tempColorText);
}

std::string Color::getColorString() const
{
    if (isValid())
        return "R" + std::to_string(m_Red) + "G" + std::to_string(m_Green) + "B" + std::to_string(m_Blue);
    return "";
}