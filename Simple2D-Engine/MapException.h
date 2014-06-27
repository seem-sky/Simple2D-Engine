#ifndef MAP_EXCEPTION_H
#define MAP_EXCEPTION_H

#include <stdexcept>

namespace MAP
{
    namespace EXCEPTION
    {
        /*#####
        # LayerException
        #####*/
        class LayerException : public std::runtime_error
        {
        public:
            LayerException(const std::string& what_arg);
            LayerException(const char* what_arg);
        };

        /*#####
        # LayerOutOfRangeException
        #####*/
        class LayerOutOfRangeException : public LayerException
        {
        public:
            LayerOutOfRangeException(const std::string& what_arg);
            LayerOutOfRangeException(const char* what_arg);
        };

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
        # IncorrectTileException
        #####*/
        class IncorrectTileException : public TileException
        {
        public:
            IncorrectTileException(const std::string& what_arg);
            IncorrectTileException(const char* what_arg);
        };

        /*#####
        # TileRangeException
        #####*/
        class TileOutOfRangeException : public TileException
        {
        public:
            TileOutOfRangeException(const std::string& what_arg);
            TileOutOfRangeException(const char* what_arg);
        };
    }
}

#endif
