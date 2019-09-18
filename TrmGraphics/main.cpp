#include "ConsoleGraphics.h"
#include <string>
#include <iostream>
#include <math.h>
#include <windows.h>

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
            console.printAt((char)178, x, y, 255*((float)x / width), 0, 255*((float)y / height));
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

        console.addRect('#', x-10, y-5, x+10, y+5, red, 255, 0, 255, 0, blue);
        console.addRect('#', y-10, x-5, y+10, x+5, red, 255, 0, 255, 0, blue);

        console.addLine('/', 10, 10, 30, sn*30, 255, 255, 255);

        //console.addTri((char)219, 10, 10, 15, 15, 5, 15, 255, 255, 255, 255, 0, 0);
        console.addTri((char)219, 5, 15, (sn*width) + (width/2), (sn*height) + (height/2), 80, 8, 0, 255, 0, 255, 0, 0);

        int x1 = 1;
        int y1 = 30;

        int x2 = 21;
        int y2 = 25;

        int x3 = 26;
        int y3 = 40;

        int x4 = 6;
        int y4 = 45;

        console.addTri((char)219, x1, y1, x2, y2, x3, y3, 0, 0, 0, 255, 255, 255);
        console.addTri((char)219, x1, y1, x4, y4, x3, y3, 0, 0, 0, 255, 255, 255);

        console.addTri((char)219, width, height, width/2, height/2, width, 0, false);

        console.addEllipse('#', sn*width + width/2, sn*height + height/2, sn*20, (cos(i)/2)*10, 255, 0, 255, 255, 255, 0);

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

        console.addLine('-', 0, 0, width-1, 0);
        console.addLine('-', 0, height-1, width-1, height-1);
        console.addLine('|', 0, 0, 0, height-1);
        console.addLine('|', width-1, 0, width-1, height-1);

        sprintf(buf, "deltaTime: %f", console.getDeltaTime());

        console.printAt(buf, 0, 0);

        i += console.getDeltaTime()*0.25;
        console.draw();
    }

    //console.printAt("test", 5, 7, 25, 255, 96);

    //console.draw();

    system("pause");

    return 0;
}
