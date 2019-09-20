#include "ConsoleGraphics.h"
#include <string>
#include <iostream>
#include <math.h>
#include <windows.h>
#include "vec2D.h"
#include "vec3D.h"
#include "quaternion.h"

using TrmGraphics::vec2D;
using TrmGraphics::vec3D;
using TrmGraphics::quaternion;

int main() {

    const int width = 100;
    const int height = 50;
    const bool faces = true;
    const int edges = -1;
    const float moveSpd = 10;

    TrmGraphics::ConsoleGraphics console(width, height, false, 6);

    //camera position
    vec3D camPos(0, 0, 80);
    quaternion camRot(vec3D(0, 0, 0));
    vec3D camEulerAngles = camRot.getEulerAngles();
    vec3D sunLampAngles = vec3D(0, 1, 0);

    //point for a cube
    /*vec3D v1(0, 0,  2);
    vec3D v2( 2, 0,  2);
    vec3D v3( 2,  2,  2);
    vec3D v4(0,  2,  2);

    vec3D v5(0, 0, 0);
    vec3D v6( 2, 0, 0);
    vec3D v7( 2,  2, 0);
    vec3D v8(0,  2, 0);*/

    vec3D v1( 1,  1, -1);
    vec3D v2( 1, -1, -1);
    vec3D v3( 1,  1,  1);
    vec3D v4( 1, -1,  1);

    vec3D v5(-1,  1, -1);
    vec3D v6(-1, -1, -1);
    vec3D v7(-1,  1,  1);
    vec3D v8(-1, -1,  1);

    vec3D vn1( 0,  1,  0);
    vec3D vn2( 0,  0,  1);
    vec3D vn3(-1,  0,  0);
    vec3D vn4( 0, -1,  0);
    vec3D vn5( 1,  0,  0);
    vec3D vn6( 0,  0, -1);

    v1 *= 10;
    v2 *= 10;
    v3 *= 10;
    v4 *= 10;

    v5 *= 10;
    v6 *= 10;
    v7 *= 10;
    v8 *= 10;

    char c = (char)219;
    //char c = '#';

    float i = 0;

    while(true) {

        i += console.getDeltaTime() * 0.1;

        console.setAmbientLight(0.15);
        quaternion sunLampQ(sunLampAngles);
        console.setSunLamp(sunLampQ.forward(), 1);

        /*console.addTri3D(c, v1, v3, v4, camPos, camRot, 255, 0, 0, edges, edges, edges, faces);
        console.addTri3D(c, v1, v2, v3, camPos, camRot, 255, 0, 0, edges, edges, edges, faces);

        console.addTri3D(c, v2, v7, v6, camPos, camRot, 0, 255, 0, edges, edges, edges, faces);
        console.addTri3D(c, v2, v3, v7, camPos, camRot, 0, 255, 0, edges, edges, edges, faces);

        console.addTri3D(c, v1, v8, v5, camPos, camRot, 0, 0, 255, edges, edges, edges, faces);
        console.addTri3D(c, v1, v4, v8, camPos, camRot, 0, 0, 255, edges, edges, edges, faces);

        console.addTri3D(c, v5, v7, v6, camPos, camRot, 255, 255, 0, edges, edges, edges, faces);
        console.addTri3D(c, v5, v8, v7, camPos, camRot, 255, 255, 0, edges, edges, edges, faces);

        console.addTri3D(c, v1, v6, v2, camPos, camRot, 0, 255, 255, edges, edges, edges, faces);
        console.addTri3D(c, v1, v5, v6, camPos, camRot, 0, 255, 255, edges, edges, edges, faces);

        console.addTri3D(c, v4, v7, v3, camPos, camRot, 255, 255, 255, edges, edges, edges, faces);
        console.addTri3D(c, v4, v8, v7, camPos, camRot, 255, 255, 255, edges, edges, edges, faces);*/

        console.addTri3D(c, v5, v3, v1, camPos, camRot, vn1, 255, 255, 255, edges, edges, edges, faces);
        console.addTri3D(c, v3, v8, v4, camPos, camRot, vn2, 255, 255, 255, edges, edges, edges, faces);

        console.addTri3D(c, v7, v6, v8, camPos, camRot, vn3, 255, 255, 255, edges, edges, edges, faces);
        console.addTri3D(c, v2, v8, v6, camPos, camRot, vn4, 255, 255, 255, edges, edges, edges, faces);

        console.addTri3D(c, v1, v4, v2, camPos, camRot, vn5, 255, 255, 255, edges, edges, edges, faces);
        console.addTri3D(c, v5, v2, v6, camPos, camRot, vn6, 255, 255, 255, edges, edges, edges, faces);

        console.addTri3D(c, v5, v7, v3, camPos, camRot, vn1, 255, 255, 255, edges, edges, edges, faces);
        console.addTri3D(c, v3, v7, v8, camPos, camRot, vn2, 255, 255, 255, edges, edges, edges, faces);

        console.addTri3D(c, v7, v5, v6, camPos, camRot, vn3, 255, 255, 255, edges, edges, edges, faces);
        console.addTri3D(c, v2, v4, v8, camPos, camRot, vn4, 255, 255, 255, edges, edges, edges, faces);

        console.addTri3D(c, v1, v3, v4, camPos, camRot, vn5, 255, 255, 255, edges, edges, edges, faces);
        console.addTri3D(c, v5, v1, v2, camPos, camRot, vn6, 255, 255, 255, edges, edges, edges, faces);

        //console.addTri3D(c, vec3D(sunLampQ.x, sunLampQ.y, sunLampQ.z) * 30, vec3D(0, 0, 0), vec3D(0, 0, 0), camPos, camRot, vec3D(20, 20, 20), 255, 0, 0, 255, 255, 255, faces);

        console.draw();

        //camRot = quaternion(camRot.getEulerAngles() + vec3D(1, 0, 0) * console.getDeltaTime()*1);
        vec3D rot;
        vec3D mov;
        if(console.keyPressed('Q')) {
            mov += vec3D(0, 1, 0);
        }
        if(console.keyPressed('E')) {
            mov += vec3D(0, -1, 0);
        }
        if(console.keyPressed('A')) {
            mov += vec3D(cos(camEulerAngles.y), 0, sin(camEulerAngles.y));
        }
        if(console.keyPressed('D')) {
            mov -= vec3D(cos(camEulerAngles.y), 0, sin(camEulerAngles.y));
        }
        if(console.keyPressed('W')) {
            mov += vec3D(cos(camEulerAngles.y - 1.570796325), 0, sin(camEulerAngles.y - 1.570796325));
            //mov += vec3D(0, 0, -1);
        }
        if(console.keyPressed('S')) {
            mov -= vec3D(cos(camEulerAngles.y - 1.570796325), 0, sin(camEulerAngles.y - 1.570796325));
            //mov += vec3D(0, 0, 1);
        }
        if(console.keyPressed(VK_UP)) {
            rot += vec3D(-1, 0, 0);
        }
        if(console.keyPressed(VK_DOWN)) {
            rot += vec3D(1, 0, 0);
        }
        if(console.keyPressed(VK_LEFT)) {
            rot += vec3D(0, 1, 0);
        }
        if(console.keyPressed(VK_RIGHT)) {
            rot += vec3D(0, -1, 0);
        }

        if(console.keyPressed('I')) {
            sunLampAngles += vec3D(1, 0, 0) * console.getDeltaTime();
        }
        if(console.keyPressed('K')) {
            sunLampAngles += vec3D(-1, 0, 0) * console.getDeltaTime();
        }
        if(console.keyPressed('J')) {
            sunLampAngles += vec3D(0, 1, 0) * console.getDeltaTime();
        }
        if(console.keyPressed('L')) {
            sunLampAngles += vec3D(0, -1, 0) * console.getDeltaTime();
        }

        //

        //mov *= dir;

        camPos += mov * console.getDeltaTime() * moveSpd;

        camRot = quaternion(camEulerAngles += rot * console.getDeltaTime()*1);

        console.printAt("", vec2D(0,0));
        console << "camPos: (";
        console << camPos.x;
        console << ", ";
        console << camPos.y;
        console << ", ";
        console << camPos.z;
        console << ")";

        console.printAt("", vec2D(0,1));
        console << "camRotEuler: (";
        console << camRot.getEulerAngles().x;
        console << ", ";
        console << camRot.getEulerAngles().y;
        console << ", ";
        console << camRot.getEulerAngles().z;
        console << ")";

    }

    system("pause");

    return 0;
}
