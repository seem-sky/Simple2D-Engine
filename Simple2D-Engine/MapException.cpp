#include "MapException.h"

using namespace MAP::EXCEPTION;

/*#####
# LayerException
#####*/
LayerException::LayerException(const std::string& what_arg) : std::runtime_error(what_arg)
{
}

LayerException::LayerException(const char* what_arg) : std::runtime_error(what_arg)
{
}

/*#####
# LayerOutOfRangeException
#####*/
LayerOutOfRangeException::LayerOutOfRangeException(const std::string& what_arg) : LayerException(what_arg)
{
}

LayerOutOfRangeException::LayerOutOfRangeException(const char* what_arg) : LayerException(what_arg)
{
}

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
TileOutOfRangeException::TileOutOfRangeException(const std::string& what_arg) : TileException(what_arg)
{
}

TileOutOfRangeException::TileOutOfRangeException(const char* what_arg) : TileException(what_arg)
{
}
