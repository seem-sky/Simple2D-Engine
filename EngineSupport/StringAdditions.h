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
#endif