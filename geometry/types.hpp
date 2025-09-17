//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.667 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:  Brian Russin
//	File:    structs.hpp
//	Purpose: Helper stucts to group vertex attributes
//
//============================================================================

#ifndef __GEOMETRY_TYPES_HPP__
#define __GEOMETRY_TYPES_HPP__

#include "geometry/point3.hpp"
#include "geometry/vector3.hpp"

namespace cg
{

/**
 * Structure to hold a vertex position and normal
 */
struct VertexAndNormal
{
    Point3  vertex;
    Vector3 normal;

    VertexAndNormal();

    VertexAndNormal(const Point3 &v);
};

} // namespace cg

#endif
