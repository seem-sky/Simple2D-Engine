#include "Database/Prototype/Base.h"

using namespace database::prototype;

Base::Base(uint32 ID)
    : m_ID(ID)
{
    if (!m_ID)
        throw std::runtime_error("ID is not allowed to be 0.");
}

bool Base::isEmpty() const
{
    return getID() == 0;
}

uint32 Base::getID() const
{
    return m_ID;
}

QString Base::getName() const
{
    return m_Name;
}

void Base::setName(const QString& name)
{
    m_Name = name;
}
