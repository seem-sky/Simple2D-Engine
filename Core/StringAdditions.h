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

inline std::string strReplace(const std::string& sString, const std::string& sFind, const std::string& sReplace)
{
    std::string sResult(sString);
    if(!sResult.empty() && !sFind.empty())
    {
        unsigned int uiPos = 0;
        while((uiPos = sResult.find(sFind, uiPos)) != std::string::npos)
        {
            sResult = sResult.replace(uiPos, sFind.length(), sReplace);
            uiPos += sReplace.length();
        }
    }
    return sResult;
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