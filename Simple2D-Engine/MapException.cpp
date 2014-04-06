#include "MapException.h"

using namespace MAP::EXCEPTION;

/*#####
# TileException
#####*/
TileException::TileException(const std::string& what_arg) : std::runtime_error(what_arg)
{
}

TileException::TileException(const char* what_arg) : std::runtime_error(what_arg)
{
}

/*#####
# TileRangeException
#####*/
TileRangeException::TileRangeException(const std::string& what_arg) : TileException(what_arg)
{
}

TileRangeException::TileRangeException(const char* what_arg) : TileException(what_arg)
{
}
