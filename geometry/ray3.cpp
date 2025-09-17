#include "geometry/ray3.hpp"

#include "geometry/geometry.hpp"

#include <cmath>

namespace cg
{

Ray3::Ray3() : o{0.0f, 0.0f, 0.0f}, d{1.0f, 0.0f, 0.0f} {}

Ray3::Ray3(const Point3 &p1, const Point3 &p2, bool normalize)
{
    // Student to define - Module 5
}

Ray3::Ray3(const Point3 &origin, const Vector3 &dir) : o(origin), d(dir) {}

Ray3::Ray3(const Point3 &origin, const Vector3 &dir, bool normalize)
{
    o = origin;
    d = dir;
    if(normalize) { d.normalize(); }
}

Ray3 Ray3::reflect(const Point3 &int_pt, const Vector3 &n) const
{
    // Required in 605.767
    return Ray3();
}

RayRefractionResult Ray3::refract(const Point3 &int_pt, Vector3 &n, float u1, float u2) const
{
    // Required in 605.767
    return RayRefractionResult{};
}

Point3 Ray3::intersect(float t) const
{
    // Student to define - Module 5
    return Point3();
}

RayObjectIntersectResult Ray3::intersect(const Plane &p) const
{
    // Required in 605.767
    return RayObjectIntersectResult{false, 0.0f};
}
RayObjectIntersectResult Ray3::intersect(const BoundingSphere &sphere) const
{
    // Student to define - Module 5
    return {false, 0.0f};
}

RayObjectIntersectResult Ray3::intersect(const AABB &box) const
{
    // Required in 605.767
    return {false, 0.0f};
}

RayObjectIntersectResult Ray3::intersect(const std::vector<Point3> &polygon,
                                         const Vector3             &normal) const
{
    // Required in 605.767
    return {false, 0.0f};
}

RayTriangleIntersectResult
    Ray3::intersect(const Point3 &v0, const Point3 &v1, const Point3 &v2) const
{
    // Required in 605.767
    return {false, 0.0f, 0.0f, 0.0f};
}

bool Ray3::does_intersect_exist(const Point3 &v0, const Point3 &v1, const Point3 &v2) const
{
    // Required in 605.767
    return false;
}

RayMeshIntersectResult Ray3::intersect(const std::vector<Point3>   &vertex_list,
                                       const std::vector<uint16_t> &face_list,
                                       float                        t_min) const
{
    // Required in 605.767
    return {false, 0.0f, 0.0f, 0.0f, 0};
}

bool Ray3::does_intersect_exist(const std::vector<Point3>   &vertex_list,
                                const std::vector<uint16_t> &face_list,
                                float                        t_min) const
{
    // Required in 605.767
    return false;
}

bool Ray3::does_intersect_exist(const std::vector<VertexAndNormal> &vertex_list,
                                const std::vector<uint16_t>        &face_list,
                                float                               t_min) const
{
    // Required in 605.767
    return false;
}

} // namespace cg
