#ifndef STRING_ADDITIONS_H
#define STRING_ADDITIONS_H

#include <string>
#include <sstream> 

template< class T>
inline std::string ToString(const T &value)
{
    std::ostringstream streamOut;
    streamOut << value;
    return streamOut.str();
}

static inline std::string& deleteStringFront(const std::string &sSign, std::string &sData)
{
    return sData = sData.substr(sData.find_first_not_of(sSign));
}

static inline std::string& deleteStringBack(const std::string &sSign, std::string &sData)
{
    return sData = sData.substr(0, sData.find_last_not_of(sSign)+1);
}
#endif