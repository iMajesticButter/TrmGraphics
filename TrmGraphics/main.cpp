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

    int i = 0;
    while(true) {
        double sn = sin((double)i/1000)/2;

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

        console.addLine('/', 10, 10, 40, sn * 100, 255, 255, 255);

        console.addLine('-', 0, 0, width-1, 0);
        console.addLine('-', 0, height-1, width-1, height-1);
        console.addLine('|', 0, 0, 0, height-1);
        console.addLine('|', width-1, 0, width-1, height-1);

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

        console.draw();
        ++i;
    }

    //console.printAt("test", 5, 7, 25, 255, 96);

    //console.draw();

    system("pause");

    return 0;
}
