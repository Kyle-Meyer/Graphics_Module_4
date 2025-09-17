#include "geometry/matrix.hpp"

#include "geometry/geometry.hpp"

#include <cmath>

namespace cg
{

// Forward declare logging function
void logmsg(const char *message, ...);

Matrix4x4::Matrix4x4() { set_identity(); }

void Matrix4x4::set_identity()
{
    a_[0] = 1.0f;
    a_[4] = 0.0f;
    a_[8] = 0.0f;
    a_[12] = 0.0f;
    a_[1] = 0.0f;
    a_[5] = 1.0f;
    a_[9] = 0.0f;
    a_[13] = 0.0f;
    a_[2] = 0.0f;
    a_[6] = 0.0f;
    a_[10] = 1.0f;
    a_[14] = 0.0f;
    a_[3] = 0.0f;
    a_[7] = 0.0f;
    a_[11] = 0.0f;
    a_[15] = 1.0f;
}

Matrix4x4::Matrix4x4(const Matrix4x4 &n)
{
    m00() = n.m00();
    m01() = n.m01();
    m02() = n.m02();
    m03() = n.m03();
    m10() = n.m10();
    m11() = n.m11();
    m12() = n.m12();
    m13() = n.m13();
    m20() = n.m20();
    m21() = n.m21();
    m22() = n.m22();
    m23() = n.m23();
    m30() = n.m30();
    m31() = n.m31();
    m32() = n.m32();
    m33() = n.m33();
}

Matrix4x4 &Matrix4x4::operator=(const Matrix4x4 &n)
{
    m00() = n.m00();
    m01() = n.m01();
    m02() = n.m02();
    m03() = n.m03();
    m10() = n.m10();
    m11() = n.m11();
    m12() = n.m12();
    m13() = n.m13();
    m20() = n.m20();
    m21() = n.m21();
    m22() = n.m22();
    m23() = n.m23();
    m30() = n.m30();
    m31() = n.m31();
    m32() = n.m32();
    m33() = n.m33();
    return *this;
}

bool Matrix4x4::operator==(const Matrix4x4 &n) const
{
    return (m00() == n.m00() && m01() == n.m01() && m02() == n.m02() && m03() == n.m03() &&
            m10() == n.m10() && m11() == n.m11() && m12() == n.m12() && m13() == n.m13() &&
            m20() == n.m20() && m21() == n.m21() && m22() == n.m22() && m23() == n.m23() &&
            m30() == n.m30() && m31() == n.m31() && m32() == n.m32() && m33() == n.m33());
}

void Matrix4x4::set(const float *m)
{
    for(size_t i = 0; i < 16; i++) a_[i] = m[i];
}

const float *Matrix4x4::get() const { return a_.data(); }

// Read-only access functions
float Matrix4x4::m00() const { return a_[0]; }
float Matrix4x4::m01() const { return a_[4]; }
float Matrix4x4::m02() const { return a_[8]; }
float Matrix4x4::m03() const { return a_[12]; }
float Matrix4x4::m10() const { return a_[1]; }
float Matrix4x4::m11() const { return a_[5]; }
float Matrix4x4::m12() const { return a_[9]; }
float Matrix4x4::m13() const { return a_[13]; }
float Matrix4x4::m20() const { return a_[2]; }
float Matrix4x4::m21() const { return a_[6]; }
float Matrix4x4::m22() const { return a_[10]; }
float Matrix4x4::m23() const { return a_[14]; }
float Matrix4x4::m30() const { return a_[3]; }
float Matrix4x4::m31() const { return a_[7]; }
float Matrix4x4::m32() const { return a_[11]; }
float Matrix4x4::m33() const { return a_[15]; }

// Read-write access functions
float &Matrix4x4::m00() { return a_[0]; }
float &Matrix4x4::m01() { return a_[4]; }
float &Matrix4x4::m02() { return a_[8]; }
float &Matrix4x4::m03() { return a_[12]; }
float &Matrix4x4::m10() { return a_[1]; }
float &Matrix4x4::m11() { return a_[5]; }
float &Matrix4x4::m12() { return a_[9]; }
float &Matrix4x4::m13() { return a_[13]; }
float &Matrix4x4::m20() { return a_[2]; }
float &Matrix4x4::m21() { return a_[6]; }
float &Matrix4x4::m22() { return a_[10]; }
float &Matrix4x4::m23() { return a_[14]; }
float &Matrix4x4::m30() { return a_[3]; }
float &Matrix4x4::m31() { return a_[7]; }
float &Matrix4x4::m32() { return a_[11]; }
float &Matrix4x4::m33() { return a_[15]; }

float Matrix4x4::m(uint32_t row, uint32_t col) const
{
    return (row < 4 && col < 4) ? a_[col * 4 + row] : 0.0f;
}

float &Matrix4x4::m(uint32_t row, uint32_t col)
{
    return (row < 4 && col < 4) ? a_[col * 4 + row] : a_[0];
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &n) const
{
    // Unroll the loop, do 1 row at a time.
    Matrix4x4 t;
    float     a0 = m00();
    float     a1 = m01();
    float     a2 = m02();
    float     a3 = m03();
    t.m00() = a0 * n.m00() + a1 * n.m10() + a2 * n.m20() + a3 * n.m30();
    t.m01() = a0 * n.m01() + a1 * n.m11() + a2 * n.m21() + a3 * n.m31();
    t.m02() = a0 * n.m02() + a1 * n.m12() + a2 * n.m22() + a3 * n.m32();
    t.m03() = a0 * n.m03() + a1 * n.m13() + a2 * n.m23() + a3 * n.m33();
    a0 = m10();
    a1 = m11();
    a2 = m12();
    a3 = m13();
    t.m10() = a0 * n.m00() + a1 * n.m10() + a2 * n.m20() + a3 * n.m30();
    t.m11() = a0 * n.m01() + a1 * n.m11() + a2 * n.m21() + a3 * n.m31();
    t.m12() = a0 * n.m02() + a1 * n.m12() + a2 * n.m22() + a3 * n.m32();
    t.m13() = a0 * n.m03() + a1 * n.m13() + a2 * n.m23() + a3 * n.m33();
    a0 = m20();
    a1 = m21();
    a2 = m22();
    a3 = m23();
    t.m20() = a0 * n.m00() + a1 * n.m10() + a2 * n.m20() + a3 * n.m30();
    t.m21() = a0 * n.m01() + a1 * n.m11() + a2 * n.m21() + a3 * n.m31();
    t.m22() = a0 * n.m02() + a1 * n.m12() + a2 * n.m22() + a3 * n.m32();
    t.m23() = a0 * n.m03() + a1 * n.m13() + a2 * n.m23() + a3 * n.m33();
    a0 = m30();
    a1 = m31();
    a2 = m32();
    a3 = m33();
    t.m30() = a0 * n.m00() + a1 * n.m10() + a2 * n.m20() + a3 * n.m30();
    t.m31() = a0 * n.m01() + a1 * n.m11() + a2 * n.m21() + a3 * n.m31();
    t.m32() = a0 * n.m02() + a1 * n.m12() + a2 * n.m22() + a3 * n.m32();
    t.m33() = a0 * n.m03() + a1 * n.m13() + a2 * n.m23() + a3 * n.m33();
    return t;
}

Matrix4x4 &Matrix4x4::operator*=(const Matrix4x4 &n)
{
    *this = *this * n;
    return *this;
}

Matrix4x4 &Matrix4x4::operator*=(float s)
{
    for(size_t r = 0; r < 16; r++) a_[r] *= s;
    return *this;
}

HPoint3 Matrix4x4::operator*(const HPoint3 &v) const
{
    return HPoint3((a_[0] * v.x + a_[4] * v.y + a_[8] * v.z + a_[12] * v.w),
                   (a_[1] * v.x + a_[5] * v.y + a_[9] * v.z + a_[13] * v.w),
                   (a_[2] * v.x + a_[6] * v.y + a_[10] * v.z + a_[14] * v.w),
                   (a_[3] * v.x + a_[7] * v.y + a_[11] * v.z + a_[15] * v.w));
}

HPoint3 Matrix4x4::operator*(const Point3 &v) const
{
    return HPoint3((a_[0] * v.x + a_[4] * v.y + a_[8] * v.z + a_[12]),
                   (a_[1] * v.x + a_[5] * v.y + a_[9] * v.z + a_[13]),
                   (a_[2] * v.x + a_[6] * v.y + a_[10] * v.z + a_[14]),
                   (a_[3] * v.x + a_[7] * v.y + a_[11] * v.z + a_[15]));
}

Vector3 Matrix4x4::operator*(const Vector3 &v) const
{
  // For vectors, we only use the upper 3x3 portion (no translation)
  // Vectors have implicit w=0, so translation components are ignored
  return Vector3((a_[0] * v.x + a_[4] * v.y + a_[8] * v.z),
                 (a_[1] * v.x + a_[5] * v.y + a_[9] * v.z),
                 (a_[2] * v.x + a_[6] * v.y + a_[10] * v.z));
}

Ray3 Matrix4x4::operator*(const Ray3 &ray) const
{
  //convert origin and direrction 
  HPoint3 transformedOrigin = *this * ray.o;
  Vector3 transformedDirection = *this * ray.d;

  Point3 newOrigin = transformedOrigin.to_cartesian();

  return Ray3(newOrigin, transformedDirection);
}

Matrix4x4 &Matrix4x4::transpose()
{
    *this = get_transpose();
    return *this;
}

Matrix4x4 Matrix4x4::get_transpose() const
{
    Matrix4x4 t;
    t.m00() = m00();
    t.m01() = m10();
    t.m02() = m20();
    t.m03() = m30();
    t.m10() = m01();
    t.m11() = m11();
    t.m12() = m21();
    t.m13() = m31();
    t.m20() = m02();
    t.m21() = m12();
    t.m22() = m22();
    t.m23() = m32();
    t.m30() = m03();
    t.m31() = m13();
    t.m32() = m23();
    t.m33() = m33();
    return t;
}

void Matrix4x4::translate(float x, float y, float z)
{
   Matrix4x4 translation;
   translation.m03() = x;  // a_[12]
   translation.m13() = y;  // a_[13]
   translation.m23() = z;  // a_[14]
   
   *this *= translation;
}

void Matrix4x4::scale(float x, float y, float z)
{
  Matrix4x4 scaling;
  scaling.m00() = x;
  scaling.m11() = y;
  scaling.m22() = z;
  
  *this *= scaling;
}

void Matrix4x4::rotate(float angle, float x, float y, float z)
{
    // Normalize the axis vector
    float length = std::sqrt(x*x + y*y + z*z);
    if (length == 0.0f) return; // Invalid axis
    
    x /= length;
    y /= length;
    z /= length;
    
    // Convert angle from degrees to radians
    float radians = angle * M_PI / 180.0f;
    float c = std::cos(radians);
    float s = std::sin(radians);
    float oneMinusC = 1.0f - c;
    
    // Direct axis-angle rotation matrix
    // Detailed here: https://en.wikipedia.org/wiki/Rotation_matrix
    // ... wait this was on the SLERP slide
    Matrix4x4 rotation;
    rotation.m00() = c + x*x*oneMinusC;
    rotation.m01() = x*y*oneMinusC - z*s;
    rotation.m02() = x*z*oneMinusC + y*s;
    rotation.m03() = 0.0f;
    
    rotation.m10() = y*x*oneMinusC + z*s;
    rotation.m11() = c + y*y*oneMinusC;
    rotation.m12() = y*z*oneMinusC - x*s;
    rotation.m13() = 0.0f;
    
    rotation.m20() = z*x*oneMinusC - y*s;
    rotation.m21() = z*y*oneMinusC + x*s;
    rotation.m22() = c + z*z*oneMinusC;
    rotation.m23() = 0.0f;
    
    rotation.m30() = 0.0f;
    rotation.m31() = 0.0f;
    rotation.m32() = 0.0f;
    rotation.m33() = 1.0f;
    
    *this *= rotation;
}

void Matrix4x4::rotate_x(float angle)
{
    // Convert angle from degrees to radians
    float radians = angle * M_PI / 180.0f;
    float c = std::cos(radians);
    float s = std::sin(radians);

    Matrix4x4 rotation;
    rotation.m11() = c;
    rotation.m12() = -s;
    rotation.m21() = s;
    rotation.m22() = c;
    
    *this *= rotation;
}

void Matrix4x4::rotate_y(float angle)
{
    // Convert angle from degrees to radians
    float radians = angle * M_PI / 180.0f;
    float c = std::cos(radians);
    float s = std::sin(radians);
    
    Matrix4x4 rotation;
    rotation.m00() = c;
    rotation.m02() = s;
    rotation.m20() = -s;
    rotation.m22() = c;
    
    *this *= rotation;
}

void Matrix4x4::rotate_z(float angle)
{
    // Convert angle from degrees to radians
    float radians = angle * M_PI / 180.0f;
    float c = std::cos(radians);
    float s = std::sin(radians);
    
    Matrix4x4 rotation;
    rotation.m00() = c;
    rotation.m01() = -s;
    rotation.m10() = s;
    rotation.m11() = c;
    
    *this *= rotation;
}

Matrix4x4 Matrix4x4::get_inverse() const
{
    int32_t   j, k;
    int32_t   ind;
    float     v1, v2;
    Matrix4x4 t = *this;
    Matrix4x4 b;
    for(int32_t i = 0; i < 4; i++)
    {
        // Find pivot
        v1 = t.m(i, i);
        ind = i;
        for(j = i + 1; j < 4; j++)
        {
            if(std::abs(t.m(j, i)) > std::abs(v1))
            {
                ind = j;
                v1 = t.m(j, i);
            }
        }

        // Swap columns
        if(ind != i)
        {
            for(j = 0; j < 4; j++)
            {
                v2 = b.m(i, j);
                b.m(i, j) = b.m(ind, j);
                b.m(ind, j) = v2;
                v2 = t.m(i, j);
                t.m(i, j) = t.m(ind, j);
                t.m(ind, j) = v2;
            }
        }

        // The matrix is singular (has no inverse), set the inverse
        // to the identity matrix.
        if(v1 == 0.0f)
        {
            logmsg("InvertMatrix: Singular matrix");
            b.set_identity();
            return b;
        }

        for(j = 0; j < 4; j++)
        {
            t.m(i, j) /= v1;
            b.m(i, j) /= v1;
        }

        // Eliminate column
        for(j = 0; j < 4; j++)
        {
            if(j == i) continue;

            v1 = t.m(j, i);
            for(k = 0; k < 4; k++)
            {
                t.m(j, k) -= t.m(i, k) * v1;
                b.m(j, k) -= b.m(i, k) * v1;
            }
        }
    }
    return b;
}

void Matrix4x4::log(const char *str) const
{
    logmsg("  %s", str);
    logmsg("%.3f %.3f %.3f %.3f", m00(), m01(), m02(), m03());
    logmsg("%.3f %.3f %.3f %.3f", m10(), m11(), m12(), m13());
    logmsg("%.3f %.3f %.3f %.3f", m20(), m21(), m22(), m23());
    logmsg("%.3f %.3f %.3f %.3f", m30(), m31(), m32(), m33());
}

} // namespace cg
