#include "preC.h"

#include "ConsoleGraphics.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>

//windows stuff
#if defined(PLATFORM_WINDOWS)
    #include <Windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#endif

#define WHITE_THRESHHOLD 35

namespace TrmGraphics {

    //console pixel struct
    struct cPixel {
        char c;
        int r, g, b;

        bool operator==(cPixel& other) {
            return c == other.c && r == other.r && g == other.g && b == other.b;
        }

        bool operator!=(cPixel& other) {
            return c != other.c || r != other.r || g != other.g || b != other.b;
        }

    };

    //----------PUBLIC FUNCTIONS----------
    //constructor
    ConsoleGraphics::ConsoleGraphics(int columns, int rows, bool askForFontSize, int fontSize) {

        m_ansiSupported = true;

        m_rows = rows;
        m_columns = columns;

        m_cursor_index = 0;

//enable ansi escape codes on windows
    #if defined(PLATFORM_WINDOWS)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD consoleMode;
        GetConsoleMode(hConsole, &consoleMode);
        consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        if(!SetConsoleMode(hConsole, consoleMode)) {
            auto e = GetLastError();
            std::cerr << "SetConsoleMode ERROR: " << e << std::endl;
            m_ansiSupported = false;
        } else {
            m_ansiSupported = true;
        }

        //attempt to resize the console the windows way
        if(askForFontSize) {
            std::cout << "To determine the correct window scale, this program may need your terminal font size" << std::endl;
            std::cout << "you can find this by right clicking on the top of this window, " << std::endl;
            std::cout << "clicking 'Properties', and clicking on the 'Font' tab" << std::endl;
            std::cout << "Please Input Your Console Font Size: ";
            std::cin >> fontSize;

            if(fontSize < 5) {
                std::cerr << "INVALID FONT SIZE, SETTING TO DEFAULT: " << DEFAULT_FONT_SIZE << std:: endl;
                fontSize = DEFAULT_FONT_SIZE;
            }

        }

        HWND consoleHWND = GetConsoleWindow();
        RECT r;
        GetWindowRect(consoleHWND, &r);
        MoveWindow(consoleHWND, r.left, r.top, m_columns*(int)((float)fontSize/2.0f), (2+m_rows)*fontSize, TRUE);


    #else
        UNREF_PARAM(askForFontSize);
        UNREF_PARAM(fontSize);
    #endif

        if(m_ansiSupported) {
            //attempt to resize the console using ansi escape codes
            printf("\033[%d;%df", m_rows, m_columns);
            printf("\033[=7h");
            printf("\033[%d;%df", m_rows, m_columns);
            printf("\033[8;%d;%dt", m_rows, m_columns);

            //try to hide the cursor
            printf("\033[?25l");

            //printf("\033[2t");
            //printf("set console size to rows: %d columns: %d", rows, columns);

        }

        //initialize front and back buffers
        m_backBuffer = new cPixel[m_rows * m_columns]();
        m_frontBuffer = new cPixel[m_rows * m_columns]();

        clearConsole();

    }
    //destructor
    ConsoleGraphics::~ConsoleGraphics() {

        //free front and back buffers
        delete[] m_backBuffer;
        delete[] m_frontBuffer;

        m_backBuffer = nullptr;
        m_frontBuffer = nullptr;

        clearConsole();
    }

    //print somthing to the console starting at current cursor position with color (r, g, b)
    // str: the value to be printed
    // r: the red value for color
    // g: the green value for color
    // b: the blue value for color
    void ConsoleGraphics::print(std::string str, int r, int g, int b) {
        //print str with color (r,g,b) at the cursor position + 1
        printAt(str, m_cursor_index + 1, 0, r, g, b);
    }

    //print somthing to the console starting at cursor position (x, y) with color (r, g, b)
    // str: the value to be printed
    // x: the x position to print at (column)
    // y: the y position to print at (row)
    // r: the red value for color
    // g: the green value for color
    // b: the blue value for color
    void ConsoleGraphics::printAt(std::string str, unsigned int x, unsigned int y, int r, int g, int b){

        //add str to backbuffer at position (x, y)
        for(unsigned int i = 0; i < str.length(); ++i) {
            if(getIndex(y, x + i) == -1) {
                break;
            }
            cPixel newPix;
            newPix.c = str[i];
            newPix.r = r;
            newPix.g = g;
            newPix.b = b;
            m_backBuffer[getIndex(y, x+i)] = newPix;
            m_cursor_index = getIndex(y , x + i);
        }

        //char buf[] = {0x1B, '[', 2, ';', 5, 'f', 0};

        //printf("\033[%d;%df", 3, 5);

        //printf("test");

        //std::cout << "test" << std::endl;

    }

    //draw everything that has been printed to the buffer
    // override: clear the backbuffer eraseing everything on next draw
    void ConsoleGraphics::draw(bool override){

        //check for differences between the front and back buffers
        //when one is found, set the color to its color, then set the cursor to its position and print it!
        for(int r = 0; r < m_rows; ++r) {
            for(int c = 0; c < m_columns; ++c) {
                cPixel &backPix = m_backBuffer[getIndex(r, c)];
                if(backPix != m_frontBuffer[getIndex(r, c)]) {
                    printf("\033[%d;%df", r, c);
                    setConsoleColor(backPix.r,backPix.g,backPix.b);
                    printf("%c", backPix.c);
                    m_frontBuffer[getIndex(r, c)] = backPix;
                }
            }
        }

        //clear backbuffer
        if(override) {
            memset(m_backBuffer, 0, sizeof(cPixel) * (m_rows * m_columns));
        }

    }

    //--------PRIVATE FUNCTIONS -----------

    int dist(int a, int b) {
        return std::abs(a-b);
    }

    void ConsoleGraphics::clearConsole() {
        if(m_ansiSupported) {
            printf("\033[2J");
            return;
        }

    #if defined(PLATFORM_WINDOWS)
        std::system("cls");
    #elif defined(PLATFORM_LINUX)
        std::system("clear");
    #endif

    }

    int ConsoleGraphics::getIndex(int r, int c){
        if(((m_columns * r) + c) > (m_columns * m_rows)) {
            return -1;
        }
        return (m_columns * r) + c;
        //return (((m_columns * r) + c) > (m_columns * m_rows)) ? ((m_columns * r) + c) : -1;
    }

    void ConsoleGraphics::setConsoleColor(int r, int g, int b) {
        if(m_ansiSupported) {
            printf("\033[38;2;%d;%d;%dm", r, g, b);
            return;
        }

    #if defined(PLATFORM_WINDOWS)

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        bool brt = false;
        if(r > 127 || g > 127 || b > 127) {
            brt = true;
        }

        //check if black or white (or close to them)
        if(dist(r, g) <= WHITE_THRESHHOLD && dist(g, b) <= WHITE_THRESHHOLD) {
            if(r < WHITE_THRESHHOLD)
                SetConsoleTextAttribute(hConsole, 0);
            else if(brt)
                SetConsoleTextAttribute(hConsole, 15);
            else if(r >= 100)
                SetConsoleTextAttribute(hConsole, 7);
            else
                SetConsoleTextAttribute(hConsole, 8);
            return;
        }

        float nr = (float)r / 255;
        float ng = (float)g / 255;
        float nb = (float)b / 255;

        float hue = 0;

        //get the maximum of the 3 rgb values
        float max = (std::max(nr, ng) > std::max(ng, nb)) ? std::max(nr, ng) : std::max(ng, nb);
        //get the minimum of the 3 rgb values
        float min = (std::min(nr, ng) < std::min(ng, nb)) ? std::min(nr, ng) : std::min(ng, nb);

        //do the rgb to hue conversion algorithm
        if(max == nr) {
            hue = (ng-nb)/(max-min);
        } else if(max == ng) {
            hue = 2.0f + (nb-nr)/(max-min);
        } else {
            hue = 4.0f + (nr-ng)/(max-min);
        }
        hue *= 60;
        if(hue < 0)
            hue += 360;

        //an array of the approxamate hue values of the windows color set in order
        //              blu  grn  cyn red mgt ylw  red(again)
        int colors[7] = {240, 130, 180, 0, 300, 60, 360};

        //get closest color on the default windows colsole color set
        int closest = 0;
        int minDist = 360;
        for(int i = 0; i < 7; ++i) {
            if(dist(colors[i], hue) < minDist) {
                minDist = dist(colors[i], hue);
                closest = i;
            }
        }

        //combine reds
        if(closest == 6)
            closest = 3;

        //set bright color values
        if(brt)
            closest += 8;

        SetConsoleTextAttribute(hConsole, closest+1);

    #else
        std::cerr << "ERROR: ANSI ESCAPE CODES NOT SUPPORTED ON THIS CONSOLE" << std::endl;
        return;
    #endif
    }

}
