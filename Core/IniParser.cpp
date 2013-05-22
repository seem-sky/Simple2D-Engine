#include "IniParser.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>

bool IniParser::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QTextStream stream(&file);
    QString dataLine;
    QString dataBuffer;
    QString sectorBuffer;
    DataMultimap newData;
    while (!stream.atEnd())
    {
        dataLine = stream.readLine();
        // delete comments
        int32 sign = dataLine.indexOf(';');
        if (sign != -1)
            dataLine = dataLine.left(sign);
        sign = dataLine.indexOf('#');
        if (sign != -1)
            dataLine = dataLine.left(sign);
        if (dataLine.isEmpty())
            continue;

        int32 pos = dataLine.indexOf('=');
        if (pos != -1)
        {
            QString indexBuffer = dataLine.mid(0, pos);
            dataLine = dataLine.mid(pos+1);
            sectorBuffer.trimmed();
            newData.insert(std::make_pair(indexBuffer, dataLine));
        }
        else 
        {
            pos = dataLine.indexOf('[');
            int32 endPos = dataLine.indexOf(']');
            if (pos != -1 && endPos != -1 && pos+1 < endPos)
            {
                // store dataLine
                if (!newData.empty())
                {
                    m_FileData.insert(std::make_pair(sectorBuffer, newData));
                    newData.clear();
                }

                // read out new sector
                sectorBuffer = dataLine.mid(pos+1);
                sectorBuffer = sectorBuffer.mid(0, sectorBuffer.indexOf(']'));
                sectorBuffer.trimmed();
            }
        }
    }
    // store dataLine
    if (!newData.empty())
    {
        m_FileData.insert(std::make_pair(sectorBuffer, newData));
        newData.clear();
    }
    return true;
}

QString IniParser::getString(const QString &key, const QString &sector, const QString sDefault)
{
    SectorData::const_iterator sectorItr = m_FileData.find(sector);
    if (sectorItr == m_FileData.end())
        return sDefault;
    DataMultimap::const_iterator keyItr = sectorItr->second.find(key);
    if (keyItr == sectorItr->second.end())
        return sDefault;
    return keyItr->second;
}

void IniParser::getAllStrings(const QString &key, const QString &sector, StdStringVector &data)
{
    SectorData::const_iterator sectorItr = m_FileData.find(sector);
    if (sectorItr == m_FileData.end())
        return;

    for (DataMultimap::const_iterator keyItr = sectorItr->second.find(key); keyItr != sectorItr->second.end() && keyItr->first == key; ++keyItr)
        data.push_back(keyItr->second);
}

int IniParser::getInt(const QString &key, const QString &sector, const int32 defaultValue)
{
    QString result = getString(key, sector, "");
    return result.isEmpty() ? defaultValue : result.toInt();
}

bool IniParser::getBool(const QString &key, const QString &sector, const bool defaultValue)
{
    QString result = getString(key, sector, "");
    return result.isEmpty() ? defaultValue : static_cast<bool>(result.toInt());
}

uint32 IniParser::getUInt(const QString &key, const QString &sector, const uint32 defaultValue)
{
    QString result = getString(key, sector, "");
    return result.isEmpty() ? defaultValue : result.toUInt();
}

void IniParser::addData(const QString &key, const QString &sector, const QString &data)
{
    if (key.isEmpty() || data.isEmpty())
        return;

    SectorData::iterator itr = m_FileData.find(sector);
    if (itr != m_FileData.end())
        (*itr).second.insert(std::make_pair(key, data));

    else
    {
        DataMultimap temp;
        temp.insert(std::make_pair(key, data));
        m_FileData.insert(std::make_pair(sector, temp));
    }
}

void IniParser::saveDataToFile(QString sFileName)
{
    QFile file(sFileName);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QTextStream stream(&file);
    for (SectorData::iterator sectorItr = m_FileData.begin(); sectorItr != m_FileData.end(); ++sectorItr)
    {
        stream << "[" + sectorItr->first + "]\n";
        for (DataMultimap::iterator keyItr = (*sectorItr).second.begin(); keyItr != (*sectorItr).second.end(); ++keyItr)
            stream << keyItr->first + "=" + keyItr->second + "\n";

        stream << "\n";
    }
}