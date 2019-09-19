#ifndef TRMGRAPHICS_VEC3D_HEADER
#define TRMGRAPHICS_VEC3D_HEADER

#include "preC.h"

namespace TrmGraphics {

    typedef class vec2D vec2D;

    //! 3d vector containing x, y, and z
    class EXPORT vec3D {
    public:

        double x,y,z; //! > xyz coordnates

        vec3D();
        vec3D(const vec2D& other);
        vec3D(const vec3D& other);
        vec3D(const double _x, const double _y, const double _z);

        //! normalize the vector
        void normalize();

        //! get the vectors magnitude
        float GetMagnitude();
        //! get the vectors magnitude squared (more efficient)
        float GetMagnitudeSquared();

        //------------------
        // + - * / operators
        //------------------

        vec3D operator+(const vec2D& other) const;
        vec3D operator+(const vec3D& other) const;
        vec3D operator+(const float val) const;

        vec3D operator-(const vec2D& other) const;
        vec3D operator-(const vec3D& other) const;
        vec3D operator-(const float val) const;

        vec3D operator*(const vec2D& other) const;
        vec3D operator*(const vec3D& other) const;
        vec3D operator*(const float val) const;

        vec3D operator/(const vec2D& other) const;
        vec3D operator/(const vec3D& other) const;
        vec3D operator/(const float val) const;

        //----------------------
        // += -= *= /= operators
        //----------------------

        vec3D& operator+=(const vec2D& other);
        vec3D& operator+=(const vec3D& other);
        vec3D& operator+=(const float val);

        vec3D& operator-=(const vec2D& other);
        vec3D& operator-=(const vec3D& other);
        vec3D& operator-=(const float val);

        vec3D& operator*=(const vec2D& other);
        vec3D& operator*=(const vec3D& other);
        vec3D& operator*=(const float val);

        vec3D& operator/=(const vec2D& other);
        vec3D& operator/=(const vec3D& other);
        vec3D& operator/=(const float val);

    };

}
#endif
