#ifndef GLOBAL_H
#define GLOBAL_H

#include <array>
#include <set>
#include <unordered_set>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <string>
#include "Rect.h"
#include "math_extensions.h"

const std::string ENGINE_VERSION = "v0.0.1";

typedef unsigned int uint32;
typedef int int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char uint8;
typedef char int8;
const uint32 MAX_UINT32 = -1;
const uint16 MAX_UINT16 = -1;

const uint32 IN_MILLISECOND  = 1000;
const uint32 MINUTE          = 60;
const uint32 HOUR            = 60;
const uint32 DAY             = 24;

const uint32 MAX_TILE_PER_PIXMAP    = 20;
const uint32 TILE_SIZE              = 32;

// Point typedefs
typedef Point<int32> Int32Point;
typedef Point<uint32> UInt32Point;
typedef Point<double> DoublePoint;

typedef Point3D<int32> Int32Point3D;
typedef Point3D<uint32> UInt32Point3D;

// unordered_set typedefs
typedef std::unordered_set<uint32> UInt32UnorderedSet;

// set typedefs
typedef std::set<Point<uint16>> UInt16PointSet;
typedef std::set<UInt32Point> UInt32PointSet;
typedef std::set<Int32Point> Int32PointSet;

typedef std::set<uint32> UInt32Set;

// vector typedefs
typedef std::vector<uint16> UInt16Vector;
typedef std::vector<UInt16Vector> UInt16Vector2D;
typedef std::vector<uint32> UInt32Vector;
typedef std::vector<UInt32Vector> UInt32Vector2D;
typedef std::vector<UInt32Vector2D> UInt32Vector3D;

typedef std::vector<Point<uint16>> UInt16PointVector;
typedef std::vector<UInt32Point> UInt32PointVector;
typedef std::vector<UInt32Point3D> UInt32Point3DVector;

// rect typedefs
typedef Rect<uint32, uint32> UInt32Rect;
typedef Rect<int32, uint32> Int32Rect;

#endif