#ifndef TRMGRAPHICS_QUATERNION_HEADER
#define TRMGRAPHICS_QUATERNION_HEADER

#include "preC.h"

namespace TrmGraphics {

    typedef class vec3D vec3D;

    class EXPORT quaternion {
    public:
        double w,x,y,z;

        quaternion();
        quaternion(const quaternion& other);
        quaternion(const double _w, const double _x, const double _y, const double _z);
        quaternion(const vec3D euler);

        vec3D getEulerAngles() const;
        vec3D forward();
        vec3D up();
        vec3D left();

        void normalize();

        //------------------
        // + - * / operators
        //------------------

        quaternion operator+(const quaternion& other) const;

        quaternion operator-(const quaternion& other) const;

        quaternion operator*(const quaternion& other) const;

        quaternion operator/(const quaternion& other) const;

        //----------------------
        // += -= *= /= operators
        //----------------------

        quaternion& operator+=(const quaternion& other);

        quaternion& operator-=(const quaternion& other);

        quaternion& operator*=(const quaternion& other);

        quaternion& operator/=(const quaternion& other);
    };
}

#endif
