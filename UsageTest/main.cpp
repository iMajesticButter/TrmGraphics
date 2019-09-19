#include <bass.h>
#include <basswasapi.h>
#include <string>
#include <iostream>
#include <math.h>
#include <ctime>
#include <time.h>
#include <windows.h>
#include <TrmGraphics.h>

#define UNUSED(x) (void)(x)

using TrmGraphics::vec2D;
using TrmGraphics::vec3D;

DWORD CALLBACK Process(void* buffer, DWORD length, void* user) {
    UNUSED(buffer);
    UNUSED(user);
    return length;
}

int main() {

    const int width = 110;
    const int height = 51;
    const int margin = 6;

    TrmGraphics::ConsoleGraphics console(width, height);

    BASS_WASAPI_DEVICEINFO info;
    int device;
    for(int i = 0; BASS_WASAPI_GetDeviceInfo(i, &info); ++i) {
        if(info.flags & 1 && info.flags & 8) {
            device = i;
            char buf[32];
            sprintf(buf, "Device: %d Selected\n", i);
            console.printAt(buf, vec2D(0, 0), 0, 255, 0);
            console.draw(false);
            break;
        }
    }
    BASS_SetConfig(24, false);

    if(!BASS_Init(0, 44100, BASS_DEVICE_DEFAULT, nullptr, nullptr)) {
        std::cerr << "INIT ERROR: " << BASS_ErrorGetCode() << std::endl;
        Sleep(10000);
        return 1;
    }

    if(!BASS_WASAPI_Init(device, 0, 0, BASS_WASAPI_BUFFER, 1.0f, 0.05f, Process, nullptr)) {
        std::cerr << "INIT ERROR: " << BASS_ErrorGetCode() << std::endl;
        Sleep(10000);
        return 1;
    }

    console.printAt("BASS INITAILIZED\n", vec2D(0, 1), 0, 255, 0);
    console.draw();

    Sleep(500);

    BASS_WASAPI_Start();

    //clear the screen
    console.draw();

    //set background
    console.setBackground((char)178, 1, 0, 15);

    const int modes = 3;
    int mode = 0;
    bool pressed = false;

    int brightness = 255;

    float fft[4096];

    clock_t lastTime = clock();
    //update loop
    while(true) {
        //ups couter
        double deltaTime = (double)(clock() - lastTime) / 1000;
        lastTime = clock();

        float ups = 1/deltaTime;
        char buf[32];
        sprintf(buf, "Updates Per Second: %f", ups);

        console.printAt(buf, vec2D(0, 0));

        //get fft data
        int ret = BASS_WASAPI_GetData(fft, (int)BASS_DATA_FFT8192);
        if(ret < 0) {
            std::cout << "GET DATA ERROR: " << BASS_ErrorGetCode() << std::endl;
            Sleep(10000);
            //return 2;
            break;
        }

        int c = 254;
        buf[0] = (char)c;
        buf[1] = 0;

        //display fft
        if(mode == 0) {
            for(int x = 0; x < width; ++x) {
                if(x < margin || x >= width-margin-3) {
                    console.printAt(buf, vec2D(x, height/2), 0, brightness, brightness);
                    continue;
                }
                float size = pow(fft[x], 0.9)*15;
                console.printAt(buf, vec2D(x, height/2), size*brightness, 0, 0);
                for(int y = 0; y < size*((height/2)-2)+1; ++y) {
                    if(y > height-4)
                        continue;
                    float r = ((float)y/(float)height) * brightness;
                    console.printAt(buf, vec2D(x, (height/2) + (y/2)), r, brightness-r, brightness);
                    console.printAt(buf, vec2D(x, (height/2) - (y/2)), r, brightness-r, brightness);
                }
            }
        } else if(mode == 1) {

            int c = 254;
            buf[0] = (char)c;
            buf[1] = 0;

            //const float radiusInner = height/6;
            const float radiusInner = 5;
            //const float scale = ((height/2) - radiusInner)/5;
            const float scale = 15;
            for(int i = 0, a = 0; a < 360; ++i, a += 4) {

                //float size = pow(fft[i], 0.9)*5 + (i/90);
                float size = fft[i] * 25;
                //if(size < 1) {
                    //size = 1;
                //}
                for(int j = 0; j < (size*scale)+1; ++j) {
                    float r = ((float)j/(float)(height/2)) * brightness;

                    //get vector
                    float x = std::sin((float)(a + 180) * 0.0174533);
                    float y = std::cos((float)(a + 180) * 0.0174533);

                    x *= radiusInner + j;
                    y *= radiusInner + j;

                    x = (width/2) + x*2;
                    y = (height/2) + y*1.009;

                    if(y > height || y < 0 || x > width || x < 0) {
                        continue;
                    }

                    if(j == 0)
                        r = brightness/2;

                    if(r > brightness) {
                        console.printAt(buf, vec2D(x, y), brightness, 0, brightness - (r - brightness));
                    } else {
                        console.printAt(buf, vec2D(x, y), r, brightness-r, brightness);
                    }

                }

            }
        } else if(mode == 2) {

            //const float radiusInner = height/6;
            const float radiusInner = 5;
            //const float scale = ((height/2) - radiusInner)/5;
            const float scale = 15;
            for(int i = 0, a = 0; a < 360; ++i, a += 4) {

                //float size = pow(fft[i], 0.9)*5 + (i/90);
                float size = fft[i] * 25;
                //if(size < 1) {
                    //size = 1;
                //}

                vec2D vec(sin((float)(a+180) * 0.0174533), cos((float)(a+180) * 0.0174533));
                vec2D pos(width/2, height/2);

                vec2D start = vec*radiusInner;
                vec2D end = vec*((size*scale)+radiusInner);

                start *= vec2D(2, 1);
                end *= vec2D(2, 1);

                console.addLine((char)254, pos+start, pos+end);

            }
        }

        sprintf(buf, "Mode: %d", mode);
        console.printAt(buf, vec2D(1, height-1));

        if(console.keyPressed(VK_LEFT)) {
            if(!pressed) {
                --mode;
                if(mode < 0) {
                    mode = modes-1;
                }
            }
            pressed = true;
        } else if(console.keyPressed(VK_RIGHT)) {
            if(!pressed) {
                ++mode;
                if(mode >= modes) {
                    mode = 0;
                }
            }
            pressed = true;
        } else if(console.keyPressed('S')) {
            if(!pressed) {
                console.saveBackground();
            }
            pressed = true;
        } else if(console.keyPressed('C')) {
            if(!pressed) {
                console.setBackground((char)178, 1, 0, 15);
            }
            pressed = true;
        } else if(console.keyPressed(VK_UP)) {
            if(!pressed) {
                brightness += 10;
                if(brightness > 255)
                    brightness = 255;
            }
            pressed = true;
        } else if(console.keyPressed(VK_DOWN)) {
            if(!pressed) {
                brightness -= 10;
                if(brightness < 0)
                    brightness = 0;
            }
            pressed = true;
        } else {
            pressed = false;
        }

        //char buf[32];
        //sprintf(buf, "0: %f", pow(fft[0], 0.9) * 25);
        //console.printAt(buf, 1, 1);
        //console.addRect('#', fft[0] * 100, fft[0] * 100, 0, 0);
        //console.printAt("#", 0, -1);
        console.draw();

        //limit updates per second to 100
        while(1/((double)(clock() - lastTime)/1000) > 100) {
            Sleep(10);
        }

    }

    return 0;
}
