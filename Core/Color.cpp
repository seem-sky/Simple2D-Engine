#include "Color.h"
Color::Color(int32 red, int32 green, int32 blue) : m_Red(red), m_Green(green), m_Blue(blue)
{}

Color::Color(const QString &p_sColorText) : m_Red(-1), m_Green(-1), m_Blue(-1)
{
    setColor(p_sColorText);
}

void Color::setColor(const QString &colorText)
{
    if (colorText.isEmpty())
        return;
    QString tempColorText = colorText.left(colorText.indexOf("R")+1);
    // setup red
    m_Red = tempColorText.left(tempColorText.indexOf("G")).toUInt();
    tempColorText = tempColorText.left(tempColorText.indexOf("G")+1);
    // setup green
    m_Green = tempColorText.left(tempColorText.indexOf("B")).toUInt();
    tempColorText = tempColorText.left(tempColorText.indexOf("B")+1);
    // setup blue
    m_Blue = tempColorText.toUInt();
}

QString Color::getColorString() const
{
    if (hasValidColor())
        return "R" + QString::number(m_Red) + "G" + QString::number(m_Green) + "B" + QString::number(m_Blue);
    return "";
}