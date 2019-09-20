#include "vec2D.h"
#include "vec3D.h"
#include <math.h>

namespace TrmGraphics {

    vec2D::vec2D() : x(0), y(0) {

    }
    vec2D::vec2D(const vec2D& other) : x(other.x), y(other.y) {

    }
    vec2D::vec2D(const vec3D& other) : x(other.x), y(other.y) {

    }
    vec2D::vec2D(const double _x, const double _y) : x(_x), y(_y) {

    }

    vec2D& vec2D::normalize() {
        float dst = sqrt(pow(x, 2) + pow(y, 2));
        x /= dst;
        y /= dst;
        return *this;
    }

    float vec2D::GetMagnitude(){
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    float vec2D::GetMagnitudeSquared(){
        return pow(x, 2) + pow(y, 2);
    }

    //------------------
    // + - * / operators
    //------------------

    vec2D vec2D::operator+(const vec2D& other) const {
        return vec2D(x + other.x, y + other.y);
    }
    vec2D vec2D::operator+(const vec3D& other) const {
        return vec2D(x + other.x, y + other.y);
    }
    vec2D vec2D::operator+(const float val) const {
        return vec2D(x + val, y + val);
    }

    vec2D vec2D::operator-(const vec2D& other) const {
        return vec2D(x - other.x, y - other.y);
    }
    vec2D vec2D::operator-(const vec3D& other) const {
        return vec2D(x - other.x, y - other.y);
    }
    vec2D vec2D::operator-(const float val) const {
        return vec2D(x - val, y - val);
    }

    vec2D vec2D::operator*(const vec2D& other) const {
        return vec2D(x * other.x, y * other.y);
    }
    vec2D vec2D::operator*(const vec3D& other) const {
        return vec2D(x * other.x, y * other.y);
    }
    vec2D vec2D::operator*(const float val) const {
        return vec2D(x * val, y * val);
    }

    vec2D vec2D::operator/(const vec2D& other) const {
        return vec2D(x / other.x, y / other.y);
    }
    vec2D vec2D::operator/(const vec3D& other) const {
        return vec2D(x / other.x, y / other.y);
    }
    vec2D vec2D::operator/(const float val) const {
        return vec2D(x / val, y / val);
    }

    //----------------------
    // += -= *= /= operators
    //----------------------

    vec2D& vec2D::operator+=(const vec2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    vec2D& vec2D::operator+=(const vec3D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    vec2D& vec2D::operator+=(const float val) {
        x += val;
        y += val;
        return *this;
    }

    vec2D& vec2D::operator-=(const vec2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    vec2D& vec2D::operator-=(const vec3D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    vec2D& vec2D::operator-=(const float val) {
        x -= val;
        y -= val;
        return *this;
    }

    vec2D& vec2D::operator*=(const vec2D& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }
    vec2D& vec2D::operator*=(const vec3D& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }
    vec2D& vec2D::operator*=(const float val) {
        x *= val;
        y *= val;
        return *this;
    }

    vec2D& vec2D::operator/=(const vec2D& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    vec2D& vec2D::operator/=(const vec3D& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    vec2D& vec2D::operator/=(const float val) {
        x /= val;
        y /= val;
        return *this;
    }

}
