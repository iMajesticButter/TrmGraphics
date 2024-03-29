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

    TrmGraphics::ConsoleGraphics console(width, height, true, 16, false);

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
    system("pause");
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

    const int modes = 6;
    int mode = 0;
    bool pressed = false;

    int brightness = 255;

    float fft[4096];

    //update loop
    while(true) {
        //ups couter
        double deltaTime = console.getDeltaTime();

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

        switch(mode) {
        case 0:
            {
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
            }
            break;
        case 1:
            {
                int c = 254;
                buf[0] = (char)c;
                buf[1] = 0;

                //const float radiusInner = height/6;
                const float radiusInner = 5;
                //const float scale = ((height/2) - radiusInner)/5;
                const float scale = 15;
                for(int i = 0, a = 0; a < 360; ++i, a += 4) {

                    float xUnit = std::sin((float)(a + 180) * 0.0174533);
                    float yUnit = std::cos((float)(a + 180) * 0.0174533);

                    //float size = pow(fft[i], 0.9)*5 + (i/90);
                    float size = (fft[i] * 25) + (1.0f/scale);
                    //if(size < 1) {
                        //size = 1;
                    //}
                    for(float j = 0; j < (size*scale); j += 0.25) {
                        float r = ((float)j/(float)(height/2)) * brightness;

                        //get vector
                        float x = xUnit;
                        float y = yUnit;

                        x *= radiusInner + j;
                        y *= radiusInner + j;

                        x = (width/2) + x*2;
                        y = (height/2) + y*1.009;

                        if(y > height || y < 0 || x > width || x < 0) {
                            continue;
                        }

                        if(r > brightness) {
                            console.printAt(buf, vec2D(x, y), brightness, 0, brightness - (r - brightness));
                        } else {
                            console.printAt(buf, vec2D(x, y), r, brightness-r, brightness);
                        }

                    }

                    // draw inner purple circle

                    xUnit *= radiusInner;
                    yUnit *= radiusInner;

                    xUnit = (width/2) + xUnit*2;
                    yUnit = (height/2) + yUnit*1.009;

                    float r = brightness/2;
                    console.printAt(buf, vec2D(xUnit, yUnit), r, brightness-r, brightness);
                }
            }
            break;
        case 2:
            {
                for(int x = 0; x < width; x += 3) {

                    float xfft = fft[x] + fft[x+1] + fft[x+2];
                    xfft /= 3;

                    float size = pow(xfft, 0.9)*20;
                    //float size = fft[i]*12;
                    //if(size < 1) {
                    //    size = 1;
                    //}
                    vec2D p1 = vec2D(x-2, height);
                    vec2D p2 = vec2D(x, (height)-(size*height/2));
                    vec2D p3 = vec2D(x+2, height);

                    console.addTri((char)254, p1, p2, p3, 0, brightness, brightness);

                }
            }
            break;
        case 3:
            {
                for(int y = 0; y < height; ++y) {
                    float size = pow(fft[height - y], 0.9)*15;

                    for(int x = 0; x < size*((width/4)-2)+1; ++x) {
                        if(x > width-4)
                            continue;

                        float r = ((float)x/((float)width/2)) * brightness;
                        if(r > brightness)
                            r = brightness;
                        console.printAt(buf, vec2D(x, height - y), r, brightness-r, brightness);
                        console.printAt(buf, vec2D(width - x, y), r, brightness-r, brightness);
                    }
                }
            }
            break;
        case 4:
            {
                for(int x = 0; x < width/2; ++x) {
                    float y = pow(fft[x], 0.9)*10;

                    y *= height/2;

                    float r = ((float)y/((float)height/2)) * brightness;
                    if(r > brightness)
                        r = brightness;

                    y = height - y;

                    console.addRect((char)219, vec2D((x*2), y), vec2D((x*2)+1, y-1), (int)r, (int)(brightness-r), brightness);
                }
            }
            break;
        case 5:
            {

                vec2D lastPos = vec2D(0,0);

                for(int x = 0; x < width; ++x) {
                    float y = pow(fft[x], 0.9)*14;

                    y *= height/2;

                    y = height - y;

                    if(x != 0) {
                        console.addLine((char)219, lastPos, vec2D(x,y), 0, brightness, brightness);
                    }

                    lastPos = vec2D(x,y);
                }
            }
            break;
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

    }

    return 0;
}
