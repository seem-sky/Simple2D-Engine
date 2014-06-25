#ifndef BRUSH_EXCEPTION_H
#define BRUSH_EXCEPTION_H

#include <exception>

namespace MAP
{
    namespace BRUSH
    {
        namespace EXCEPTION
        {
            class Exception : std::exception
            {
            };

            class InvalidArea : Exception
            {
            };

            class InvalidType : Exception
            {
            };
        }
    }
}
#endif
