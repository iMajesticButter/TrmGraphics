#include "vec2D.h"
#include "vec3D.h"
#include "quaternion.h"
#include <math.h>

namespace TrmGraphics {

    vec3D::vec3D() : x(0), y(0), z(0) {

    }
    vec3D::vec3D(const vec2D& other) : x(other.x), y(other.y), z(0) {

    }
    vec3D::vec3D(const vec3D& other) : x(other.x), y(other.y), z(other.z) {

    }
    vec3D::vec3D(const double _x, const double _y, const double _z) : x(_x), y(_y), z(_z) {

    }

    vec3D& vec3D::normalize() {
        float dst = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
        x /= dst;
        y /= dst;
        z /= dst;
        return *this;
    }

    float vec3D::GetMagnitude(){
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    float vec3D::GetMagnitudeSquared(){
        return pow(x, 2) + pow(y, 2) + pow(z, 2);
    }

    //dot and cross products
    float vec3D::dot(const vec3D& other) {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    vec3D vec3D::cross(const vec3D& other) {
        return vec3D((y * other.z) - (z * other.y),
                     (z * other.x) - (x * other.z),
                     (x * other.y) - (y * other.x));
    }

    //------------------
    // + - * / operators
    //------------------

    vec3D vec3D::operator+(const vec2D& other) const {
        return vec3D(x + other.x, y + other.y, 0);
    }
    vec3D vec3D::operator+(const vec3D& other) const {
        return vec3D(x + other.x, y + other.y, z + other.z);
    }
    vec3D vec3D::operator+(double val) const {
        return vec3D(x + val, y + val, z + val);
    }

    vec3D vec3D::operator-(const vec2D& other) const {
        return vec3D(x - other.x, y - other.y, 0);
    }
    vec3D vec3D::operator-(const vec3D& other) const {
        return vec3D(x - other.x, y - other.y, z - other.z);
    }
    vec3D vec3D::operator-(double val) const {
        return vec3D(x - val, y - val, z - val);
    }

    vec3D vec3D::operator*(const vec2D& other) const {
        return vec3D(x * other.x, y * other.y, 0);
    }
    vec3D vec3D::operator*(const vec3D& other) const {
        return vec3D(x * other.x, y * other.y, z * other.z);
    }
    vec3D vec3D::operator*(double val) const {
        return vec3D(x * val, y * val, z * val);
    }

    vec3D vec3D::operator/(const vec2D& other) const {
        return vec3D(x / other.x, y / other.y, 0);
    }
    vec3D vec3D::operator/(const vec3D& other) const {
        return vec3D(x / other.x, y / other.y, z / other.z);
    }
    vec3D vec3D::operator/(double val) const {
        return vec3D(x / val, y / val, z / val);
    }

    //----------------------
    // += -= *= /= operators
    //----------------------

    vec3D& vec3D::operator+=(const vec2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    vec3D& vec3D::operator+=(const vec3D& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    vec3D& vec3D::operator+=(double val) {
        x += val;
        y += val;
        z += val;
        return *this;
    }

    vec3D& vec3D::operator-=(const vec2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    vec3D& vec3D::operator-=(const vec3D& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    vec3D& vec3D::operator-=(double val) {
        x -= val;
        y -= val;
        z -= val;
        return *this;
    }

    vec3D& vec3D::operator*=(const vec2D& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }
    vec3D& vec3D::operator*=(const vec3D& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }
    vec3D& vec3D::operator*=(double val) {
        x *= val;
        y *= val;
        z *= val;
        return *this;
    }

    vec3D& vec3D::operator/=(const vec2D& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    vec3D& vec3D::operator/=(const vec3D& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }
    vec3D& vec3D::operator/=(double val) {
        x /= val;
        y /= val;
        z /= val;
        return *this;
    }

    //---------------------------
    //rotate vector by quaternion
    //---------------------------

    vec3D vec3D::operator*(const quaternion& q) const {
        vec3D t = vec3D(q.x, q.y, q.z).cross(*this) * 2;
        return *this + (t * q.w) + vec3D(q.x, q.y, q.z).cross(t);
    }

}
