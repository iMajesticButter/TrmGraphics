#ifndef TrmGraphics_Vec2D_HEADER
#define TrmGraphics_Vec2D_HEADER

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

        //dot product
        float dot(const vec2D& other);

        //------------------
        // + - * / operators
        //------------------

        vec2D operator+(const vec2D& other) const;
        vec2D operator+(const vec3D& other) const;
        vec2D operator+(const double val) const;

        vec2D operator-(const vec2D& other) const;
        vec2D operator-(const vec3D& other) const;
        vec2D operator-(const double val) const;

        vec2D operator*(const vec2D& other) const;
        vec2D operator*(const vec3D& other) const;
        vec2D operator*(const double val) const;

        vec2D operator/(const vec2D& other) const;
        vec2D operator/(const vec3D& other) const;
        vec2D operator/(const double val) const;

        //----------------------
        // += -= *= /= operators
        //----------------------

        vec2D& operator+=(const vec2D& other);
        vec2D& operator+=(const vec3D& other);
        vec2D& operator+=(const double val);

        vec2D& operator-=(const vec2D& other);
        vec2D& operator-=(const vec3D& other);
        vec2D& operator-=(const double val);

        vec2D& operator*=(const vec2D& other);
        vec2D& operator*=(const vec3D& other);
        vec2D& operator*=(const double val);

        vec2D& operator/=(const vec2D& other);
        vec2D& operator/=(const vec3D& other);
        vec2D& operator/=(const double val);

    };

}
#endif
