#include <TrmGraphics.h>
#include <TrmGraphicsStlLoader.h>
#include <iostream>
#include <string>
//#include <windows.h>

#if defined(__linux__)
    #define XK_MISCELLANY
    #include <X11/keysymdef.h>

    #define VK_UP XK_Up
    #define VK_DOWN XK_Down
    #define VK_LEFT XK_Left
    #define VK_RIGHT XK_Right
    #define VK_SPACE ' '

    #include <unistd.h>

    #define Sleep(x) usleep(x*1000)

#else
    #include <windows.h>
#endif

using TrmGraphics::vec2D;
using TrmGraphics::vec3D;
using TrmGraphics::quaternion;
using TrmGraphics::translationMatrix;
using TrmGraphics::stlLoader;
using TrmGraphics::triangle;

int main() {

    const float moveSpd = 3;
    const float rotSpd = 1;
    const float sunSpinSpd = 3;
    const vec3D startPos(0, 0, -30);


    unsigned width = 50;
    unsigned height = 25;
    unsigned fontSize = 16;
    std::string path;

    //input settings
    std::cout << "Please Select Viewer Width: ";
    std::cin >> width;

    std::cout << "Please Select Viewer Height: ";
    std::cin >> height;

    std::cout << "Please Input Your Consoles Current Font Size: ";
    std::cin >> fontSize;

    std::cout << "Please Input The Path To The Model You Want To Veiw:" << std::endl;
    std::cin >> path;

    //load the stl file
    stlLoader stl;

    if(!stl.loadFile(path)) {
        std::cout << "ERROR: The file: " << std::endl << path << std::endl << "Was not found!";
        return 1;
    }

    //preferences
    int red = 0;
    int green = 0;
    int blue = 0;
    bool fill = false;
    int lred = -1;
    int lgreen = -1;
    int lblue = -1;
    bool wire = false;

    std::cout << "Model Loaded Successfully!" << std::endl;

    while(true) {
        std::cout << "Do you want to draw faces? (y,n): ";
        char input;
        std::cin >> input;
        if(input == 'y') {
            fill = true;
            break;
        }
        if(input == 'n') {
            fill = false;
            break;
        }
        std::cout << "Invalid Input, Try Again:" << std::endl;
    }

    if(fill) {
        std::cout << "Input Desired Face Color:" << std::endl;
        std::cout << "red:   ";
        std::cin >> red;
        std::cout << "green: ";
        std::cin >> green;
        std::cout << "blue:  ";
        std::cin >> blue;
    }

    while(true) {
        std::cout << "Do you want to draw wireframe? (y,n): ";
        char input;
        std::cin >> input;
        if(input == 'y') {
            wire = true;
            break;
        }
        if(input == 'n') {
            wire = false;
            break;
        }
        std::cout << "Invalid Input, Try Again:" << std::endl;
    }

    if(wire) {
        std::cout << "Input Desired Wireframe Color:" << std::endl;
        std::cout << "red:   ";
        std::cin >> lred;
        std::cout << "green: ";
        std::cin >> lgreen;
        std::cout << "blue:  ";
        std::cin >> lblue;
    }

    if(!fill && !wire) {
        std::cout << "umm... nothing will be drawn, so the program is just going to end now";
        for(int i = 0; i < 8; ++i) {
            Sleep(300);
            std::cout << ".";
        }
        return 0;
    }


    char c;
    if(fill) {
        while(true) {
            std::cout << "Do you want to do askii shading? (y,n): ";
            char input;
            std::cin >> input;
            if(input == 'y') {
                c = (char)0;
                break;
            }
            if(input == 'n') {
                c = (char)219;
                break;
            }
            std::cout << "Invalid Input, Try Again:" << std::endl;
        }
    } else {
        c = (char)219;
    }

    int backR = 0;
    int backG = 0;
    int backB = 0;

    std::cout << "Input Desired Background Color:" << std::endl;
    std::cout << "red:   ";
    std::cin >> backR;
    std::cout << "green: ";
    std::cin >> backG;
    std::cout << "blue:  ";
    std::cin >> backB;

    double ambientLight = 0.5;
    double sunLight = 0.5;

    if(fill) {
        std::cout << "Input Desired Ambient Light Level: ";
        std::cin >> ambientLight;

        std::cout << "Input Desired Sun Light Level: ";
        std::cin >> sunLight;
    }

    double d = 100;
    std::cout << "Input Desired Distance From Camera To Draw Plane (Larger Number = Lower FOV): ";
    std::cin >> d;

    //initialize the console
    TrmGraphics::ConsoleGraphics console(width, height, false, fontSize, true);

    if(backR != 0 || backG != 0 || backB != 0) {
        console.setBackground((char)219, backR, backG, backB);
    }

    console.setAmbientLight(ambientLight);
    console.setSunLamp(vec3D(0,0,0), sunLight);

    //camera
    quaternion camRot;
    vec3D camPos;
    vec3D sunDir = vec3D(0, 1, 0);

    while(true) {

        console.setSunLamp(quaternion(sunDir).forward(), sunLight);

        console.printAt("Press 'X' to quit", vec2D(0, 0));

        //draw model to backbuffer
        for(unsigned i = 0; i < (unsigned)stl.triCount(); ++i) {
            triangle& t = stl[i];
            console.addTri3D(c, t.v1, t.v2, t.v3, camPos, camRot, t.normal, red, green, blue, lred, lgreen, lblue, fill, d);
        }

        //draw to screen
        console.draw();

        //do input
        vec3D mov;
        vec3D rot;

        if(console.keyPressed('W')) {
            mov += camRot.forward();
        }
        if(console.keyPressed('S')) {
            mov -= camRot.forward();
        }
        if(console.keyPressed('A')) {
            mov += camRot.left();
        }
        if(console.keyPressed('D')) {
            mov -= camRot.left();
        }
        if(console.keyPressed(VK_SPACE)) {
            mov += camRot.up();
        }
        if(console.keyPressed('C')) {
            mov -= camRot.up();
        }

        if(console.keyPressed(VK_UP)) {
            rot += vec3D(0, 1, 0);
        }
        if(console.keyPressed(VK_DOWN)) {
            rot -= vec3D(0, 1, 0);
        }
        if(console.keyPressed(VK_LEFT)) {
            rot -= vec3D(0, 0, 1);
        }
        if(console.keyPressed(VK_RIGHT)) {
            rot += vec3D(0, 0, 1);
        }

        if(console.keyPressed('I')) {
            sunDir += vec3D(0, 0, 1) * console.getDeltaTime() * sunSpinSpd;
        }
        if(console.keyPressed('K')) {
            sunDir += vec3D(0, 0, -1) * console.getDeltaTime() * sunSpinSpd;
        }
        if(console.keyPressed('J')) {
            sunDir += vec3D(0, 1, 0) * console.getDeltaTime() * sunSpinSpd;
        }
        if(console.keyPressed('L')) {
            sunDir += vec3D(0, -1, 0) * console.getDeltaTime() * sunSpinSpd;
        }

        if(console.keyPressed('X')) {
            break;
        }

        camPos += mov * console.getDeltaTime() * moveSpd;

        camRot = quaternion(camRot.getEulerAngles() + rot * console.getDeltaTime() * rotSpd);

    }


    return 0;
}
