#include <ConsoleGraphics.h>
#include <bass.h>
#include <basswasapi.h>
#include <string>
#include <iostream>
#include <math.h>

#define UNUSED(x) (void)(x)

DWORD CALLBACK Process(void* buffer, DWORD length, void* user) {
    UNUSED(buffer);
    UNUSED(user);
    return length;
}

int main() {

    const int width = 200;
    const int height = 31;
    const int margin = 6;

    TrmGraphics::ConsoleGraphics console(width, height);

    BASS_WASAPI_DEVICEINFO info;
    int device;
    for(int i = 0; BASS_WASAPI_GetDeviceInfo(i, &info); ++i) {
        if(info.flags & 1 && info.flags & 8) {
            device = i;
            char buf[32];
            sprintf(buf, "Device: %d Selected\n", i);
            console.printAt(buf, 1, 1, 0, 255, 0);
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

    console.printAt("BASS INITAILIZED\n", 1, 2, 0, 255, 0);
    console.draw();

    Sleep(500);

    BASS_WASAPI_Start();

    //clear the screen
    console.draw();

    float fft[2048];

    //update loop
    while(true) {
        int ret = BASS_WASAPI_GetData(fft, (int)BASS_DATA_FFT2048);
        if(ret < 0) {
            std::cout << "GET DATA ERROR: " << BASS_ErrorGetCode() << std::endl;
            Sleep(10000);
            //return 2;
            break;
        }

        for(int x = 0; x < width; ++x) {
            if(x < margin || x >= width-margin-3) {
                console.printAt("#", x, height/2, 0, 255, 255);
                continue;
            }
            float size = pow(fft[x], 0.9)*25;
            console.printAt("#", x, height/2, size*255, 0, 0);
            for(int y = 0; y < size*((height/2)-2)+1; ++y) {
                if(y > height-1)
                    continue;
                float r = (float)y/(float)height;
                r *= 255;
                console.printAt("#", x, (height/2) + (y/2), r, 255-r, 255);
                console.printAt("#", x, (height/2) - (y/2), r, 255-r, 255);
            }
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
