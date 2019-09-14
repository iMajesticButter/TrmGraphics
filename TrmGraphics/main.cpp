#include "ConsoleGraphics.h"
#include <string>
#include <iostream>
#include <math.h>

int main() {

    int width = 100;
    int height = 50;

    TrmGraphics::ConsoleGraphics console(width, height);

    int i = 0;
    while(true) {
        double sn = sin((double)i/1000)/2;

        int x = (int)(((double)width/2)+(sn * width));
        int y = (int)(((double)height/2)+(sn * height));

        //print lots of #'s
        for(int i = 0; i < width; ++i) {
            for(int j = 0; j < height; ++j) {
                console.printAt("#", i, (height-1)-j, i*(255/width), j*(255/height), 0);
            }
        }

        console.printAt("test", x, y, 255-(int)(((float)x/(float)width)*255), (int)(((float)y/(float)height)*255), 0);
        
        console.draw();
        ++i;
    }

    //console.printAt("test", 5, 7, 25, 255, 96);

    //console.draw();

    system("pause");

    return 0;
}
