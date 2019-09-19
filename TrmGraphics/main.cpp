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

    const int width = 200;
    const int height = 100;
    const bool faces = false;
    const float moveSpd = 10;

    TrmGraphics::ConsoleGraphics console(width, height);

    //camera position
    vec3D camPos(0, 0, 110);
    quaternion camRot(vec3D(0, 0, 0));

    //point for a cube
    /*vec3D v1(0, 0,  2);
    vec3D v2( 2, 0,  2);
    vec3D v3( 2,  2,  2);
    vec3D v4(0,  2,  2);

    vec3D v5(0, 0, 0);
    vec3D v6( 2, 0, 0);
    vec3D v7( 2,  2, 0);
    vec3D v8(0,  2, 0);*/

    vec3D v1(-1, -1,  1);
    vec3D v2( 1, -1,  1);
    vec3D v3( 1,  1,  1);
    vec3D v4(-1,  1,  1);

    vec3D v5(-1, -1, -1);
    vec3D v6( 1, -1, -1);
    vec3D v7( 1,  1, -1);
    vec3D v8(-1,  1, -1);

    v1 *= 10;
    v2 *= 10;
    v3 *= 10;
    v4 *= 10;

    v5 *= 10;
    v6 *= 10;
    v7 *= 10;
    v8 *= 10;

    char c = (char)219;

    while(true) {

        console.addTri3D(c, v1, v2, v3, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);
        console.addTri3D(c, v1, v4, v3, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);

        console.addTri3D(c, v2, v6, v7, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);
        console.addTri3D(c, v2, v3, v7, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);

        console.addTri3D(c, v1, v5, v8, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);
        console.addTri3D(c, v1, v4, v8, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);

        console.addTri3D(c, v5, v6, v7, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);
        console.addTri3D(c, v5, v8, v7, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);

        console.addTri3D(c, v1, v2, v6, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);
        console.addTri3D(c, v1, v5, v6, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);

        console.addTri3D(c, v4, v3, v7, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);
        console.addTri3D(c, v4, v8, v7, camPos, camRot, 155, 155, 155, 255, 255, 255, faces);

        console.draw();

        //camRot = quaternion(camRot.getEulerAngles() + vec3D(1, 0, 0) * console.getDeltaTime()*1);
        vec3D rot;
        vec3D mov;
        if(console.keyPressed(VK_SPACE)) {
            mov += vec3D(0, -1, 0);
        }
        if(console.keyPressed(VK_SHIFT)) {
            mov += vec3D(0, 1, 0);
        }
        if(console.keyPressed('A')) {
            mov += vec3D(-1, 0, 0);
        }
        if(console.keyPressed('D')) {
            mov += vec3D(1, 0, 0);
        }
        if(console.keyPressed('W')) {
            mov += vec3D(0, 0, -1);
        }
        if(console.keyPressed('S')) {
            mov += vec3D(0, 0, 1);
        }
        if(console.keyPressed(VK_UP)) {
            rot += vec3D(1, 0, 0);
        }
        if(console.keyPressed(VK_DOWN)) {
            rot += vec3D(-1, 0, 0);
        }
        if(console.keyPressed(VK_LEFT)) {
            rot += vec3D(0, 1, 0);
        }
        if(console.keyPressed(VK_RIGHT)) {
            rot += vec3D(0, -1, 0);
        }



        camPos += mov * console.getDeltaTime() * moveSpd;

        camRot = quaternion(camRot.getEulerAngles() + rot * console.getDeltaTime()*1);

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
