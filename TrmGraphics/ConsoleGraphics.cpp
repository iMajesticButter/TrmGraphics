#include "preC.h"

#include "ConsoleGraphics.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <math.h>
#include <ctime>
#include <time.h>
#include <sstream>

#include "vec2D.h"
#include "vec3D.h"

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
        unsigned int r, g, b;

        bool operator==(cPixel& other) {
            return c == other.c && r == other.r && g == other.g && b == other.b;
        }

        bool operator!=(cPixel& other) {
            return c != other.c || r != other.r || g != other.g || b != other.b;
        }

    };

    //----------LOCAL FUNCTIONS-----------

    //edge function for triangle rasterizer
    bool edgeFunc(const float x1, const float y1, const float x2, const float y2, const float pX, const float pY) {
        return ((pX - x1) * (y2 - y1) - (pY - y1) * (x2 - x1) >= 0);
    }

    //basic min max functions that take 3 arguments
    int min3(const int a, const int b, const int c) {
        return std::min(a, b) < std::min(b, c) ? std::min(a, b) : std::min(b, c);
    }
    int max3(const int a, const int b, const int c) {
        return std::max(a, b) > std::max(b, c) ? std::max(a, b) : std::max(b, c);
    }

    //basic clamp function
    void clamp(int& i, const int min, const int max) {
        i = i >= min ? i : min;
        i = i <= max ? i : max;
    }

    //----------PUBLIC FUNCTIONS----------
    // ConsoleGraphics constructor
    ConsoleGraphics::ConsoleGraphics(int columns, int rows, bool askForFontSize, int fontSize) {

        m_deltaTime = 0;

        m_lastTime = clock();

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

        //m_ansiSupported = false;

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
        MoveWindow(consoleHWND, r.left, r.top, (m_columns+5)*(int)((float)fontSize/2.0f), (3+m_rows)*fontSize, TRUE);

        if(!m_ansiSupported) {
            //hide cursor
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(hConsole, &cursorInfo);
            cursorInfo.bVisible = false;
            SetConsoleCursorInfo(hConsole, &cursorInfo);
        }

    #else
        UNREF_PARAM(askForFontSize);
        UNREF_PARAM(fontSize);
    #endif

        if(m_ansiSupported) {
            //attempt to resize the console using ansi escape codes
            printf("\033[%d;%df", m_rows+1, m_columns+1);
            printf("\033[=7h");
            printf("\033[%d;%df", m_rows+1, m_columns+1);
            printf("\033[8;%d;%dt", m_rows+1, m_columns+1);

            //try to hide the cursor
            printf("\033[?25l");

            //printf("\033[2t");
            //printf("set console size to rows: %d columns: %d", rows, columns);

        }

        //initialize front and back buffers
        m_backBuffer = new cPixel[m_rows * m_columns]();
        m_frontBuffer = new cPixel[m_rows * m_columns]();
        m_background = new cPixel[m_rows * m_columns]();

        clearConsole();

    }

    // ConsoleGraphics destructor
    ConsoleGraphics::~ConsoleGraphics() {

        //free front and back buffers
        delete[] m_backBuffer;
        delete[] m_frontBuffer;
        delete[] m_background;

        m_backBuffer = nullptr;
        m_frontBuffer = nullptr;
        m_background = nullptr;
    }

    //! Get time between last updates
    float ConsoleGraphics::getDeltaTime() {
        return m_deltaTime;
    }

    //! Set the cursor position to x,y
    void ConsoleGraphics::setCursorPos(int x, int y){
        m_cursor_index = getIndex(x, y);
        if(m_cursor_index >= m_rows * m_columns) {
            m_cursor_index = 0;
        }
    }

    // Operator<<
    void ConsoleGraphics::operator<< (short val){
        char buf[32];
        sprintf(buf, "%hd", val);
        print(buf);
    }
    void ConsoleGraphics::operator<< (unsigned short val){
        char buf[32];
        sprintf(buf, "%hu", val);
        print(buf);
    }
    void ConsoleGraphics::operator<< (int val){
        char buf[32];
        sprintf(buf, "%d", val);
        print(buf);
    }
    void ConsoleGraphics::operator<< (unsigned int val){
        char buf[32];
        sprintf(buf, "%hd", val);
        print(buf);
    }
    void ConsoleGraphics::operator<< (long val){
        char buf[32];
        sprintf(buf, "%ld", val);
        print(buf);
    }
    void ConsoleGraphics::operator<< (unsigned long val){
        char buf[32];
        sprintf(buf, "%lu", val);
        print(buf);
    }
    void ConsoleGraphics::operator<< (float val){
        char buf[32];
        sprintf(buf, "%f", val);
        print(buf);
    }
    void ConsoleGraphics::operator<< (double val){
        char buf[32];
        sprintf(buf, "%f", val);
        print(buf);
    }
    void ConsoleGraphics::operator<< (long double val){
        char buf[32];
        sprintf(buf, "%f", (double)val);
        print(buf);
    }
    void ConsoleGraphics::operator<< (void* val){
        char buf[32];
        sprintf(buf, "%p", val);
        print(buf);
    }
    void ConsoleGraphics::operator<< (std::streambuf* sb ){
        std::ostringstream ss;
        ss << sb;
        print(ss.str());
    }
    void ConsoleGraphics::operator<< (std::ostream& (*pf)(std::ostream&)){
        std::ostringstream ss;
        ss << pf;
        print(ss.str());
    }
    void ConsoleGraphics::operator<< (std::ios& (*pf)(std::ios&)){
        std::ostringstream ss;
        ss << pf;
        print(ss.str());
    }
    void ConsoleGraphics::operator<< (std::ios_base& (*pf)(std::ios_base&)){
        std::ostringstream ss;
        ss << pf;
        print(ss.str());
    }
    void ConsoleGraphics::operator<< (std::string str){
        print(str);
    }
    void ConsoleGraphics::operator<< (bool val){
        val ? print("true") : print("false");
    }
    void ConsoleGraphics::operator<< (const char* val){
        print(std::string(val));
    }




    // print at cursor position
    void ConsoleGraphics::print(std::string str, int r, int g, int b) {
        //print str with color (r,g,b) at the cursor position + 1
        printAt(str, m_cursor_index + 1, 0, r, g, b);
    }

    // move cursor, then print at new cursor position
    void ConsoleGraphics::printAt(std::string str, int x, int y, int r, int g, int b) {
        if(x < 0) {
            x = 0;
        }
        if(y < 0) {
            y = 0;
        }

        //add str to backbuffer at position (x, y)
        for(unsigned int i = 0; i < str.length(); ++i) {
            if(getIndex(y, x + i) == -1) {
                break;
            }
            m_backBuffer[getIndex(y, x+i)].c = str[i];
            m_backBuffer[getIndex(y, x+i)].r = r;
            m_backBuffer[getIndex(y, x+i)].g = g;
            m_backBuffer[getIndex(y, x+i)].b = b;
            m_cursor_index = getIndex(y , x + i);
        }

        //char buf[] = {0x1B, '[', 2, ';', 5, 'f', 0};

        //printf("\033[%d;%df", 3, 5);

        //printf("test");

        //std::cout << "test" << std::endl;

    }

    //! print at cursor position
    void ConsoleGraphics::print(char c, int r, int g, int b) {
        char buf[2];
        sprintf(buf, "%c", c);
        print(buf, r, g, b);
    }

    //! move cursor, then print at new cursor position
    void ConsoleGraphics::printAt(char c, int x, int y, int r, int g, int b) {
        char buf[2];
        sprintf(buf, "%c", c);
        printAt(buf, x, y, r, g, b);
    }

    // print a rectangle to the back buffers
    void ConsoleGraphics::addRect(char c, int x1, int y1, int x2, int y2, int rFill, int gFill, int bFill, int rBorder, int gBorder, int bBorder, bool fill) {

        //if border colors are unset, set them to fill colors
        rBorder = rBorder == -1 ? rFill : rBorder;
        gBorder = gBorder == -1 ? gFill : gBorder;
        bBorder = bBorder == -1 ? bFill : bBorder;

        //make sure coordnates are valid
        x1 = x1 < m_columns ? x1 : m_columns - 1;
        y1 = y1 < m_rows ? y1 : m_rows - 1;
        x2 = x2 < m_columns ? x2 : m_columns - 1;
        y2 = y2 < m_rows ? y2 : m_rows - 1;

        x1 = x1 >= 0 ? x1 : 0;
        y1 = y1 >= 0 ? y1 : 0;
        x2 = x2 >= 0 ? x2 : 0;
        y2 = y2 >= 0 ? y2 : 0;

        //get min/max values
        unsigned int minX = x1 < x2 ? x1 : x2;
        unsigned int minY = y1 < y2 ? y1 : y2;
        unsigned int maxX = x1 > x2 ? x1 : x2;
        unsigned int maxY = y1 > y2 ? y1 : y2;

        for(unsigned int y = minY; y <= maxY; ++y) {
            for(unsigned int x = minX; x <= maxX; ++x) {
                //check if edge
                if(x == minX || x == maxX || y == minY || y == maxY) {
                    m_backBuffer[getIndex(y, x)].c = c;
                    m_backBuffer[getIndex(y, x)].r = rBorder;
                    m_backBuffer[getIndex(y, x)].g = gBorder;
                    m_backBuffer[getIndex(y, x)].b = bBorder;
                    continue;
                }

                if(fill) {
                    m_backBuffer[getIndex(y, x)].c = c;
                    m_backBuffer[getIndex(y, x)].r = rFill;
                    m_backBuffer[getIndex(y, x)].g = gFill;
                    m_backBuffer[getIndex(y, x)].b = bFill;
                }

            }
        }

    }
    // print a rectangle to the back buffers
    void ConsoleGraphics::addRect(char c, int x1, int y1, int x2, int y2, bool fill, int rBorder, int gBorder, int bBorder, int rFill, int gFill, int bFill) {
        addRect(c, x1, y1, x2, y2, rFill, gFill, bFill, rBorder, gBorder, bBorder, fill);
    }

    //! print a line to the back buffer
    void ConsoleGraphics::addLine(char c, int x1, int y1, int x2, int y2, int r, int g, int b) {

        //clamp coordinates to valid coordiantes
        //x1 = x1 >= 0 ? x1 : 0;
        //x2 = x2 >= 0 ? x2 : 0;
        //y1 = y1 >= 0 ? y1 : 0;
        //y2 = y2 >= 0 ? y2 : 0;

        //x1 = x1 < m_columns ? x1 : m_columns - 1;
        //x2 = x2 < m_columns ? x2 : m_columns - 1;
        //y1 = y1 < m_rows ? y1 : m_rows - 1;
        //y2 = y2 < m_rows ? y2 : m_rows - 1;

        //get distance between two points
        float dst = sqrt(pow(abs(x1 - x2),2) + pow(abs(y1 - y2), 2));
        //float dst = abs(x1 - x2) + abs(y1 - y2);

        //get vector from point1 to point2
        float vecX = x2 - x1;
        float vecY = y2 - y1;

        //normalize vector
        float a = atan2(vecY, vecX);
        vecX = cos(a);
        vecY = sin(a);

        //draw line to backbuffer
        for(int i = 0; i <= dst; ++i) {
            float nVecX = round(vecX * i);
            float nVecY = round(vecY * i);
            int index = getIndex(y1 + nVecY, x1 + nVecX);
            //int index = getIndex(x1 + (vecX * dst), y1 + (vecY * dst));

            if(index >= 0 && index < m_rows * m_columns) {

                m_backBuffer[index].c = c;
                m_backBuffer[index].r = r;
                m_backBuffer[index].g = g;
                m_backBuffer[index].b = b;

            }

        }

    }

    //! print a triangle to the back buffer
    void ConsoleGraphics::addTri(char c, int x1, int y1, int x2, int y2, int x3, int y3, int rFill, int gFill, int bFill, int rBorder, int gBorder, int bBorder, bool fill) {
        //if any of the border colors are set, draw the borders later, otherwise dont
        bool drawBorders = rBorder != -1 || gBorder != -1 || bBorder != -1;
        if(!fill)
            drawBorders = true;

        //set border color to fill color if unset
        rBorder = rBorder == -1 ? rFill : rBorder;
        gBorder = gBorder == -1 ? gFill : gBorder;
        bBorder = bBorder == -1 ? bFill : bBorder;

        if(fill) {
            //get range of triangle
            int minX = min3(x1, x2, x3);
            int minY = min3(y1, y2, y3);
            int maxX = max3(x1, x2, x3);
            int maxY = max3(y1, y2, y3);

            //clamp tri range to be inside valid area
            clamp(minX, 0, m_columns-1);
            clamp(minY, 0, m_rows-1);
            clamp(maxX, 0, m_columns-1);
            clamp(maxY, 0, m_rows-1);

            //run rasterization algorithm
            for(int x = minX; x <= maxX; ++x) {
                for(int y = minY; y <= maxY; ++y) {

                    bool fill = true;
                    fill &= edgeFunc(x1, y1, x2, y2, x, y);
                    fill &= edgeFunc(x2, y2, x3, y3, x, y);
                    fill &= edgeFunc(x3, y3, x1, y1, x, y);
                    if(!fill){
                        fill = true;
                        fill &= !edgeFunc(x1, y1, x2, y2, x, y);
                        fill &= !edgeFunc(x2, y2, x3, y3, x, y);
                        fill &= !edgeFunc(x3, y3, x1, y1, x, y);
                    }

                    if(fill) {
                        //pixel is part of the triangle!
                        m_backBuffer[getIndex(y, x)].c = c;
                        m_backBuffer[getIndex(y, x)].r = rFill;
                        m_backBuffer[getIndex(y, x)].g = gFill;
                        m_backBuffer[getIndex(y, x)].b = bFill;
                    }
                }
            }
        }

        //draw borders
        if(drawBorders) {
            addLine(c, x1, y1, x2, y2, rBorder, gBorder, bBorder);
            addLine(c, x2, y2, x3, y3, rBorder, gBorder, bBorder);
            addLine(c, x3, y3, x1, y1, rBorder, gBorder, bBorder);
        }

    }

    void ConsoleGraphics::addTri(char c, int x1, int y1, int x2, int y2, int x3, int y3, bool fill, int rBorder, int gBorder, int bBorder, int rFill, int gFill, int bFill) {
        addTri(c, x1, y1, x2, y2, x3, y3, rFill, gFill, bFill, rBorder, gBorder, bBorder, fill);
    }

    //! print an ellipse to the back buffer
    void ConsoleGraphics::addEllipse(char c, int xPos, int yPos, float xSize, float ySize, int rFill, int gFill, int bFill, int rBorder, int gBorder, int bBorder, bool fill) {
        //if any of the border colors are set, draw the borders later, otherwise dont
        bool drawBorders = rBorder != -1 || gBorder != -1 || bBorder != -1;
        if(!fill)
            drawBorders = true;

        //set border color to fill color if unset
        rBorder = rBorder == -1 ? rFill : rBorder;
        gBorder = gBorder == -1 ? gFill : gBorder;
        bBorder = bBorder == -1 ? bFill : bBorder;

        if(fill) {
            //draw ellipse fill

            //hange negative sizes
            xSize = abs(xSize);
            ySize = abs(ySize);

            //get range of ellipse
            int minX = xPos - xSize;
            int minY = yPos - ySize;
            int maxX = xPos + xSize;
            int maxY = yPos + ySize;

            //clamp ellipse range to be inside valid area
            clamp(minX, 0, m_columns-1);
            clamp(minY, 0, m_rows-1);
            clamp(maxX, 0, m_columns-1);
            clamp(maxY, 0, m_rows-1);

            //fill in ellispse
            for(int x = minX; x <= maxX; ++x) {
                for(int y = minY; y <= maxY; ++y) {

                    int xrel = xPos - x;
                    int yrel = yPos - y;

                    if(fill && xrel*xrel*ySize*ySize+yrel*yrel*xSize*xSize <= ySize*ySize*xSize*xSize) {

                        int index = getIndex(y, x);

                        m_backBuffer[index].c = c;
                        m_backBuffer[index].r = rFill;
                        m_backBuffer[index].g = gFill;
                        m_backBuffer[index].b = bFill;

                    }

                }
            }
        }

        /*float vecX = cos(0.876058042) * xSize;
        float vecY = sin(0.876058042) * ySize;

        int index = getIndex(yPos + vecY, xPos + vecX);

        m_backBuffer[index].c = c;
        m_backBuffer[index].r = rBorder;
        m_backBuffer[index].g = gBorder;
        m_backBuffer[index].b = bBorder;*/
        if(drawBorders) {
            for(int a = 0; a < 360; ++a) {
                float vecX = cos(a * 0.0174533) * xSize;
                float vecY = sin(a * 0.0174533) * ySize;

                if(xPos + vecX < 0 || xPos+vecX >= m_columns || yPos + vecY < 0 || yPos + vecY >= m_rows) {
                    continue;
                }

                int index = getIndex(std::round(yPos + vecY), std::round(xPos + vecX));

                m_backBuffer[index].c = c;
                m_backBuffer[index].r = rBorder;
                m_backBuffer[index].g = gBorder;
                m_backBuffer[index].b = bBorder;
            }
        }

    }

    //! print an ellipse to the back buffer
    void ConsoleGraphics::addEllipse(char c, int xPos, int yPos, float xSize, float ySize, bool fill, int rBorder, int gBorder, int bBorder, int rFill, int gFill, int bFill) {
        addEllipse(c, xPos, yPos, xSize, ySize, rFill, gFill, bFill, rBorder, gBorder, bBorder, fill);
    }

    //! saved the current back buffer as the background
    /*!
      When draw is called, the back_buffer will be set to the background instead of empty.
    */
    void ConsoleGraphics::saveBackground() {
        memcpy(m_background, m_backBuffer, sizeof(cPixel) * (m_rows * m_columns));
    }

    //! will set the background to this char in the color r,g,b
    void ConsoleGraphics::setBackground(char c, int r, int g, int b) {
        addRect(c, 0, 0, m_columns, m_rows, r,g,b,r,g,b);
        saveBackground();
    }

    //! returns pressed state of give key
    bool ConsoleGraphics::keyPressed(int k) {

    #if defined(PLATFORM_WINDOWS)
        if(GetConsoleWindow() == GetForegroundWindow()) {
            short state = GetKeyState(k);
            return (((unsigned short)state) >> 15) == 1;
        }
        return false;
    #elif defined(PLATFORM_LINUX)

        //---------------------------------------------
        //TODO: add linux implementation of keyPressed!
        //---------------------------------------------

    #endif

    }

    // draw everything to the terminal
    void ConsoleGraphics::draw(bool override) {

        //check for differences between the front and back buffers
        //when one is found, set the color to its color, then set the cursor to its position and print it!
        unsigned int lastR = 0, lastG = 0, lastB=0;
        for(int r = 0; r < m_rows; ++r) {
            for(int c = 0; c < m_columns; ++c) {
                cPixel &backPix = m_backBuffer[getIndex(r, c)];
                if(backPix != m_frontBuffer[getIndex(r, c)]) {
                    if(backPix.r != lastR || backPix.g != lastG || backPix.b != lastB) {
                        lastR = backPix.r;
                        lastG = backPix.g;
                        lastB = backPix.b;
                        setConsoleColor(backPix.r,backPix.g,backPix.b);
                    }
                    bool winWay = false;
                #if defined(PLATFORM_WINDOWS)
                    winWay = true;
                #endif
                    if(m_ansiSupported && !winWay) {
                        printf("\033[%d;%df%c", r+1, c+1, backPix.c);
                    } else {
                    #if defined(PLATFORM_WINDOWS)
                        int nC = c, nR = r;
                        if(c > m_columns)
                            nC = m_columns;
                        if(r > m_rows)
                            nR = m_rows;
                        COORD p = {(short)nC, (short)nR};
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
                        printf("%c", backPix.c);
                    #elif defined(PLATFORM_LINUX)
                        //-------------------------------------------------------------------------
                        //TODO: add linux implementation of *non-ansi escape code* Cursor Movement!
                        //-------------------------------------------------------------------------
                    #endif
                    }
                    m_frontBuffer[getIndex(r, c)] = backPix;
                }
            }
        }

        //clear backbuffer
        if(override) {
            memcpy(m_backBuffer, m_background, sizeof(cPixel) * (m_rows * m_columns));
        }

        //get delta time
        m_deltaTime = float(clock() - m_lastTime) / CLOCKS_PER_SEC;
        m_lastTime = clock();

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

    void ConsoleGraphics::setConsoleColor(unsigned int r, unsigned int g, unsigned int b) {

        //precautions
        r = r < 255 ? r : 255;
        g = g < 255 ? g : 255;
        b = b < 255 ? b : 255;

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
