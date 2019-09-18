#include "vec2D.h"
#include "vec3D.h"
#include <math.h>

namespace TrmGraphics {

    vec3D::vec3D() : x(0), y(0), z(0) {

    }
    vec3D::vec3D(const vec2D& other) : x(other.x), y(other.y), z(0) {

    }
    vec3D::vec3D(const vec3D& other) : x(other.x), y(other.y), z(other.z) {

    }
    vec3D::vec3D(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {

    }

    void vec3D::normalize() {
        float dst = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
        x /= dst;
        y /= dst;
        z /= dst;
    }

    float vec3D::GetMagnitude(){
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    float vec3D::GetMagnitudeSquared(){
        return pow(x, 2) + pow(y, 2) + pow(z, 2);
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
    vec3D vec3D::operator+(float val) const {
        return vec3D(x + val, y + val, z + val);
    }

    vec3D vec3D::operator-(const vec2D& other) const {
        return vec3D(x - other.x, y - other.y, 0);
    }
    vec3D vec3D::operator-(const vec3D& other) const {
        return vec3D(x - other.x, y - other.y, z - other.z);
    }
    vec3D vec3D::operator-(float val) const {
        return vec3D(x + val, y + val, z + val);
    }

    vec3D vec3D::operator*(const vec2D& other) const {
        return vec3D(x * other.x, y * other.y, 0);
    }
    vec3D vec3D::operator*(const vec3D& other) const {
        return vec3D(x * other.x, y * other.y, z * other.z);
    }
    vec3D vec3D::operator*(float val) const {
        return vec3D(x + val, y + val, z + val);
    }

    vec3D vec3D::operator/(const vec2D& other) const {
        return vec3D(x / other.x, y / other.y, 0);
    }
    vec3D vec3D::operator/(const vec3D& other) const {
        return vec3D(x / other.x, y / other.y, z / other.z);
    }
    vec3D vec3D::operator/(float val) const {
        return vec3D(x + val, y + val, z + val);
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
    vec3D& vec3D::operator+=(float val) {
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
    vec3D& vec3D::operator-=(float val) {
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
    vec3D& vec3D::operator*=(float val) {
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
    vec3D& vec3D::operator/=(float val) {
        x /= val;
        y /= val;
        z /= val;
        return *this;
    }

}
