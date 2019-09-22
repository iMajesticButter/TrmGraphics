#ifndef TRMGRAPHICS_STLLOADER_HEADER
#define TRMGRAPHICS_STLLOADER_HEADER

#include <vector>
#include "vec3D.h"
#include <string>

namespace TrmGraphics {

    struct triangle {
        vec3D v1, v2, v3, normal;
    };

    class stlLoader {
    public:
        stlLoader();
        stlLoader(const stlLoader& other);
        stlLoader(std::string path);

        bool loadFile(std::string path);

        triangle& operator[](const int& index);

        int triCount() {return m_triangles.size();}

    private:

        std::vector<triangle> m_triangles;

    };

}
#endif
