


#ifndef TRMGRAPHICS_TRANSLATION_MATRIX_HEADER
#define TRMGRAPHICS_TRANSLATION_MATRIX_HEADER


namespace TrmGraphics {

    typedef class vec3D vec3D;
    typedef class quaternion quaternion;

    class translationMatrix {
    public:

        translationMatrix(bool identity = true);
        translationMatrix(const translationMatrix& other);

        static translationMatrix getTranslation(vec3D trans);
        static translationMatrix getRotation(quaternion rot);

        vec3D operator*(const vec3D& other) const;

        translationMatrix operator+(const translationMatrix& other) const;
        translationMatrix operator-(const translationMatrix& other) const;
        translationMatrix operator*(const translationMatrix& other) const;

        float m_matrix[4][4];

    };

}

#endif
