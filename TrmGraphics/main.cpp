#include "ConsoleGraphics.h"
#include <string>
#include <iostream>
#include <math.h>
#include <windows.h>
#include "vec2D.h"
#include "vec3D.h"

using TrmGraphics::vec2D;
using TrmGraphics::vec3D;

int main() {

    int width = 100;
    int height = 50;

    TrmGraphics::ConsoleGraphics console(width, height);

    bool pressed = false;
    int blue = 0;

    char buf[32];
    //sprintf(buf, "%c", (char)178);

    //set background'
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            console.printAt((char)178, vec2D(x, y), 255*((float)x / width), 0, 255*((float)y / height));
        }
    }

    console.saveBackground();

    float i = 0;
    while(true) {
        double sn = sin(i)/2;

        int x = (int)(((double)width/2)+(sn * width));
        int y = (int)(((double)height/2)+(sn * height));

        //print lots of #'s
        /*for(int i = 0; i < width; ++i) {
            for(int j = 0; j < height; ++j) {
                console.printAt("#", i, (height-1)-j, i*(255/width), j*(255/height), 0);
            }
        }

        console.printAt("test", x, y, 255-(int)(((float)x/(float)width)*255), (int)(((float)y/(float)height)*255), 0);*/

        int red = 0;
        if(console.keyPressed(VK_SPACE)) {
            red = 255;
        }

        console.addRect('#', vec2D(x-10, y-5), vec2D(x+10, y+5), red, 255, 0, 255, 0, blue);
        console.addRect('#', vec2D(y-10, x-5), vec2D(y+10, x+5), red, 255, 0, 255, 0, blue);

        console.addLine('/', vec2D(10, 10), vec2D(30, sn*30), 255, 255, 255);

        //console.addTri((char)219, 10, 10, 15, 15, 5, 15, 255, 255, 255, 255, 0, 0);
        console.addTri((char)219, vec2D(5, 15), vec2D((sn*width) + (width/2), (sn*height) + (height/2)), vec2D(80, 8), 0, 255, 0, 255, 0, 0);

        vec2D vert1( 1, 30);
        vec2D vert2(21, 25);
        vec2D vert3(26, 40);
        vec2D vert4( 6, 45);

        console.addTri((char)219, vert1, vert2, vert3, 0, 0, 0, 255, 255, 255);
        console.addTri((char)219, vert1, vert4, vert3, 0, 0, 0, 255, 255, 255);

        console.addTri((char)219, vec2D(width, height), vec2D(width/2, height/2), vec2D(width, 0), false);

        console.addEllipse('#', vec2D(sn*width + width/2, sn*height + height/2), vec2D(sn*20, (cos(i)/2)*10), 255, 0, 255, 255, 255, 0);

        if(console.keyPressed('A')) {
            if(!pressed) {
                if(blue == 0) {
                    blue = 255;
                } else {
                    blue = 0;
                }
            }
            pressed = true;
        } else {
            pressed = false;
        }

        console.addLine('-', vec2D(0, 0), vec2D(width-1, 0));
        console.addLine('-', vec2D(0, height-1), vec2D(width-1, height-1));
        console.addLine('|', vec2D(0, 0), vec2D(0, height-1));
        console.addLine('|', vec2D(width-1, 0), vec2D(width-1, height-1));

        sprintf(buf, "deltaTime: %f", console.getDeltaTime());

        console.printAt(buf, vec2D(0, height-1));

        i += console.getDeltaTime()*0.25;
        console.draw();
    }

    //console.printAt("test", 5, 7, 25, 255, 96);

    //console.draw();

    system("pause");

    return 0;
}
