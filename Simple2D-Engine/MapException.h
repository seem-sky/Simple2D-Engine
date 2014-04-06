#ifndef MAP_EXCEPTION_H
#define MAP_EXCEPTION_H

#include <stdexcept>

namespace MAP
{
    namespace EXCEPTION
    {
        /*#####
        # TileException
        #####*/
        class TileException : public std::runtime_error
        {
        public:
            TileException(const std::string& what_arg);
            TileException(const char* what_arg);
        };

        /*#####
        # TileRangeException
        #####*/
        class TileRangeException : public TileException
        {
        public:
            TileRangeException(const std::string& what_arg);
            TileRangeException(const char* what_arg);
        };
    }
}

#endif
