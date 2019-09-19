#include "translationMatrix.h"


#include "vec3D.h"
#include "quaternion.h"





namespace TrmGraphics {

    translationMatrix::translationMatrix(bool identity) {
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                if(i == j && identity) {
                    m_matrix[i][j] = 1;
                    continue;
                }
                m_matrix[i][j] = 0;
            }
        }
    }

    translationMatrix::translationMatrix(const translationMatrix& other) {
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                m_matrix[i][j] = other.m_matrix[i][j];
            }
        }
    }

    translationMatrix translationMatrix::getTranslation(vec3D trans) {

        translationMatrix mat;

        mat.m_matrix[3][0] = trans.x;
        mat.m_matrix[3][1] = trans.y;
        mat.m_matrix[3][2] = trans.z;

        return mat;
        //m_matrix[3][0] = trans.x;
        //m_matrix[3][1] = trans.y;
        //m_matrix[3][2] = trans.z;

    }
    translationMatrix translationMatrix::getRotation(quaternion q) {
        translationMatrix mat;

        q.normalize();

        double sqw = q.w * q.w;
        double sqx = q.x * q.x;
        double sqy = q.y * q.y;
        double sqz = q.z * q.z;

        mat.m_matrix[0][0] = ( sqx - sqy - sqz + sqw);
        mat.m_matrix[1][1] = (-sqx + sqy - sqz + sqw);
        mat.m_matrix[2][2] = (-sqx - sqy + sqz + sqw);

        double tmp1 = q.x * q.y;
        double tmp2 = q.z * q.w;
        mat.m_matrix[0][1] = 2 * (tmp1 + tmp2);
        mat.m_matrix[1][0] = 2 * (tmp1 - tmp2);

        tmp1 = q.x * q.z;
        tmp2 = q.y * q.w;
        mat.m_matrix[0][2] = 2 * (tmp1 - tmp2);
        mat.m_matrix[2][0] = 2 * (tmp1 + tmp2);
        tmp1 = q.y * q.z;
        tmp2 = q.x * q.w;
        mat.m_matrix[1][2] = 2 * (tmp1 + tmp2);
        mat.m_matrix[2][1] = 2 * (tmp1 - tmp2);

        return mat;

    }

    vec3D translationMatrix::operator*(const vec3D& other) const {
        vec3D vec;

        vec.x = (m_matrix[0][0] * other.x) + (m_matrix[1][0] * other.y) + (m_matrix[2][0] * other.z) + m_matrix[3][0];
        vec.y = (m_matrix[0][1] * other.x) + (m_matrix[1][1] * other.y) + (m_matrix[2][1] * other.z) + m_matrix[3][1];
        vec.z = (m_matrix[0][2] * other.x) + (m_matrix[1][2] * other.y) + (m_matrix[2][2] * other.z) + m_matrix[3][2];

        return vec;
    }


    translationMatrix translationMatrix::operator+(const translationMatrix& other) const {
        translationMatrix mat;
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                mat.m_matrix[i][j] = m_matrix[i][j] + other.m_matrix[i][j];
            }
        }
        return mat;
    }
    translationMatrix translationMatrix::operator-(const translationMatrix& other) const {
        translationMatrix mat;
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                mat.m_matrix[i][j] = m_matrix[i][j] - other.m_matrix[i][j];
            }
        }
        return mat;
    }
    translationMatrix translationMatrix::operator*(const translationMatrix& other) const {
        translationMatrix mat(false);
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                for(int x = 0; x < 4; ++x) {
                    mat.m_matrix[i][j] += m_matrix[x][j] * other.m_matrix[i][x];
                }
            }
        }
        return mat;
    }
}
