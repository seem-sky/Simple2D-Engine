#ifndef STRING_ADDITIONS_H
#define STRING_ADDITIONS_H

#include <string>
#include <sstream> 

using namespace std;

template< class type>
inline string to_string( const type & value)
{
    ostringstream streamOut;
    streamOut << value;
    return streamOut.str();
}
#endif;