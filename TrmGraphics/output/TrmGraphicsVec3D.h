#ifndef TrmGraphics_Vec3D_HEADER
#define TrmGraphics_Vec3D_HEADER

namespace TrmGraphics {

    typedef class vec2D vec2D;
    typedef class quaternion quaternion;

    //! 3d vector containing x, y, and z
    class vec3D {
    public:

        double x,y,z; //! > xyz coordnates

        vec3D();
        vec3D(const vec2D& other);
        vec3D(const vec3D& other);
        vec3D(const double _x, const double _y, const double _z);

        //! normalize the vector
        vec3D& normalize();

        //! get the vectors magnitude
        float GetMagnitude();
        //! get the vectors magnitude squared (more efficient)
        float GetMagnitudeSquared();

        //dot and cross products
        float dot(const vec3D& other);
        vec3D cross(const vec3D& other);

        //------------------
        // + - * / operators
        //------------------

        vec3D operator+(const vec2D& other) const;
        vec3D operator+(const vec3D& other) const;
        vec3D operator+(const double val) const;

        vec3D operator-(const vec2D& other) const;
        vec3D operator-(const vec3D& other) const;
        vec3D operator-(const double val) const;

        vec3D operator*(const vec2D& other) const;
        vec3D operator*(const vec3D& other) const;
        vec3D operator*(const double val) const;

        vec3D operator/(const vec2D& other) const;
        vec3D operator/(const vec3D& other) const;
        vec3D operator/(const double val) const;

        //----------------------
        // += -= *= /= operators
        //----------------------

        vec3D& operator+=(const vec2D& other);
        vec3D& operator+=(const vec3D& other);
        vec3D& operator+=(const double val);

        vec3D& operator-=(const vec2D& other);
        vec3D& operator-=(const vec3D& other);
        vec3D& operator-=(const double val);

        vec3D& operator*=(const vec2D& other);
        vec3D& operator*=(const vec3D& other);
        vec3D& operator*=(const double val);

        vec3D& operator/=(const vec2D& other);
        vec3D& operator/=(const vec3D& other);
        vec3D& operator/=(const double val);

        //---------------------------
        //rotate vector by quaternion
        //---------------------------

        vec3D operator*(const quaternion& q) const;

    };

}
#endif
