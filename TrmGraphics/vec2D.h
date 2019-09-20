#ifndef TRMGRAPHICS_VEC2D_HEADER
#define TRMGRAPHICS_VEC2D_HEADER

#include "preC.h"

namespace TrmGraphics {

    typedef class vec3D vec3D;

    //! 2d vector containing x and y
    class EXPORT vec2D {
    public:

        double x,y; //! > xy coordnates

        vec2D();
        vec2D(const vec2D& other);
        vec2D(const vec3D& other);
        vec2D(const double _x, const double _y);

        //! normalize the vector
        vec2D& normalize();

        //! get the vectors magnitude
        float GetMagnitude();
        //! get the vectors magnitude squared (more efficient)
        float GetMagnitudeSquared();

        //------------------
        // + - * / operators
        //------------------

        vec2D operator+(const vec2D& other) const;
        vec2D operator+(const vec3D& other) const;
        vec2D operator+(const float val) const;

        vec2D operator-(const vec2D& other) const;
        vec2D operator-(const vec3D& other) const;
        vec2D operator-(const float val) const;

        vec2D operator*(const vec2D& other) const;
        vec2D operator*(const vec3D& other) const;
        vec2D operator*(const float val) const;

        vec2D operator/(const vec2D& other) const;
        vec2D operator/(const vec3D& other) const;
        vec2D operator/(const float val) const;

        //----------------------
        // += -= *= /= operators
        //----------------------

        vec2D& operator+=(const vec2D& other);
        vec2D& operator+=(const vec3D& other);
        vec2D& operator+=(const float val);

        vec2D& operator-=(const vec2D& other);
        vec2D& operator-=(const vec3D& other);
        vec2D& operator-=(const float val);

        vec2D& operator*=(const vec2D& other);
        vec2D& operator*=(const vec3D& other);
        vec2D& operator*=(const float val);

        vec2D& operator/=(const vec2D& other);
        vec2D& operator/=(const vec3D& other);
        vec2D& operator/=(const float val);

    };

}
#endif
