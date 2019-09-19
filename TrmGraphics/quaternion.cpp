#include "quaternion.h"
#include "vec3D.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace TrmGraphics {

    quaternion::quaternion() : w(0), x(0), y(0), z(0) {

    }
    quaternion::quaternion(const quaternion& other) : w(other.w), x(other.x), y(other.y), z(other.z) {

    }
    quaternion::quaternion(const double _w, const double _x, const double _y, const double _z) : w(_w), x(_x), y(_y), z(_z) {

    }
    quaternion::quaternion(const vec3D euler) {
        double cy = cos(euler.z * 0.5);
        double sy = sin(euler.z * 0.5);
        double cr = cos(euler.x * 0.5);
        double sr = sin(euler.x * 0.5);
        double cp = cos(euler.y * 0.5);
        double sp = sin(euler.y * 0.5);

        w = cy * cr * cp + sy * sr * sp;
        x = cy * sr * cp - sy * cr * sp;
        y = cy * cr * sp + sy * sr * cp;
        z = sy * cr * cp - cy * sr * sp;
    }

    vec3D quaternion::getEulerAngles() {
        vec3D euler;

        double sinr_cosp = 2 * (w * x + y * z);
        double cosr_cosp = 1 - 2 * (x * x + y * y);
        euler.x = atan2(sinr_cosp, cosr_cosp);

        double sinp = 2 * (w * y - z * x);
        if(fabs(sinp) >= 1) {
            euler.y = copysign(M_PI / 2, sinp);
        } else {
            euler.y = asin(sinp);
        }

        double siny_cosp = 2 * (w * z + x * y);
        double cosy_cosp = 1 - 2 * (y * y + z * z);
        euler.z = atan2(siny_cosp, cosy_cosp);

        return euler;
    }

    void quaternion::normalize() {
        double dst = sqrt(pow(w, 2) + pow(x, 2) + pow(y, 2) + pow(z, 2));
        w /= dst;
        x /= dst;
        y /= dst;
        z /= dst;
    }

    //------------------
    // + - * / operators
    //------------------

    quaternion quaternion::operator+(const quaternion& other) const {
        return quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
    }

    quaternion quaternion::operator-(const quaternion& other) const {
        return quaternion(w - other.w, x - other.x, y - other.y, z - other.z);
    }

    quaternion quaternion::operator*(const quaternion& other) const {
        return quaternion(w * other.w, x * other.x, y * other.y, z * other.z);
    }

    quaternion quaternion::operator/(const quaternion& other) const {
        return quaternion(w / other.w, x / other.x, y / other.y, z / other.z);
    }

    //----------------------
    // += -= *= /= operators
    //----------------------

    quaternion& quaternion::operator+=(const quaternion& other) {
        w += other.w;
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    quaternion& quaternion::operator-=(const quaternion& other) {
        w -= other.w;
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    quaternion& quaternion::operator*=(const quaternion& other) {
        w *= other.w;
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    quaternion& quaternion::operator/=(const quaternion& other) {
        w /= other.w;
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

}
