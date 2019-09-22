#include "stlLoader.h"
#include <fstream>
#include <stdint.h>
#include <iostream>

namespace TrmGraphics {


    //--------------------
    // Public Functions
    //--------------------

    stlLoader::stlLoader() {

    }
    stlLoader::stlLoader(const stlLoader& other) {
        m_triangles = other.m_triangles;
    }
    stlLoader::stlLoader(std::string path) {
        loadFile(path);
    }

    bool stlLoader::loadFile(std::string path) {

        std::ifstream stl (path, std::ios::in | std::ios::binary);

        if(stl.fail()) {
            std::cerr << "ERROR OPENING FILE: " << path;
            return false;
        }
        //get file header
        uint8_t header[80];
        stl.read((char*)&header, sizeof(uint8_t) * 80);

        //get number of triangles
        uint32_t numTri;
        stl.read((char*)&numTri, sizeof(uint32_t));

        //get all triangles
        for(uint32_t i = 0; i < numTri; ++i) {

            float norm[3];
            float v1[3];
            float v2[3];
            float v3[3];

            stl.read((char*)norm, sizeof(float)*3);
            stl.read((char*)v1, sizeof(float)*3);
            stl.read((char*)v2, sizeof(float)*3);
            stl.read((char*)v3, sizeof(float)*3);

            triangle tri;
            tri.normal = vec3D(norm[0], norm[1], norm[2]);
            tri.v1 = vec3D(v1[0], v1[1], v1[2]);
            tri.v2 = vec3D(v2[0], v2[1], v2[2]);
            tri.v3 = vec3D(v3[0], v3[1], v3[2]);

            m_triangles.push_back(tri);

            uint16_t attributeByteCount; //should be zero

            stl.read((char*)&attributeByteCount, sizeof(uint16_t));

        }

        stl.close();

        return true;
    }

    triangle& stlLoader::operator[](const int& index) {
        return m_triangles[index];
    }

}
