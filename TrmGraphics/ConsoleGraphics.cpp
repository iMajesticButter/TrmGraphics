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
        unsigned int r, g, b;

        bool operator==(cPixel& other) {
            return c == other.c && r == other.r && g == other.g && b == other.b;
        }

        bool operator!=(cPixel& other) {
            return c != other.c || r != other.r || g != other.g || b != other.b;
        }

    };

    //----------PUBLIC FUNCTIONS----------
    // ConsoleGraphics constructor
    /*
      \param columns The desired width of the terminal.
      \param rows The desired height of the terminal.
      \param askForFontSize Sould the Console Manager automatically ask the user for the terminals font size. **Only required on windows**. defaults to *false*
      \param fontSize Allows you to input the terminals font size **Only works when askForFontSize is false**. deafults to *16* as that is the deafult font size on the windwos terminal
      \sa print(), printAt() and draw()
    */
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
        MoveWindow(consoleHWND, r.left, r.top, m_columns*(int)((float)fontSize/2.0f), (2+m_rows)*fontSize, TRUE);

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

    // print at cursor position
    /*
      print somthing to the terminal starting at current cursor position, with color (r, g, b).\n
      note: This function **only** prints to the back buffer, to actually draw the draw() function has to be called.
      \param str the string to be printed
      \param r the red color value between 0 and 255. deafults to *255*
      \param g the green color value between 0 and 255. deafults to *255*
      \param b the blue color value between 0 and 255. deafults to *255*
      \sa printAt() and draw()
    */
    void ConsoleGraphics::print(std::string str, int r, int g, int b) {
        //print str with color (r,g,b) at the cursor position + 1
        printAt(str, m_cursor_index + 1, 0, r, g, b);
    }

    // move cursor, then print at new cursor position
    /*
      print somthing to the terminal starting at cursor position (x, y), with color (r, g, b).\n
      note: This function **only** prints to the back buffer, to actually draw the draw() function has to be called.
      \param str the string to be printed
      \param x the x position to print at (row)
      \param y the y position to print at (column)
      \param r the red color value between 0 and 255. deafults to *255*
      \param g the green color value between 0 and 255. deafults to *255*
      \param b the blue color value between 0 and 255. deafults to *255*
      \sa printAt() and draw()
    */
    void ConsoleGraphics::printAt(std::string str, int x, int y, int r, int g, int b){
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

    // print a rectangle to the back buffers
    /*
      print a fill-able rectangle to the back buffer to be drawn.\n
      note: This function **only** prints to the back buffer, to actually draw the draw() function has to be called.
      \param c The character to print the rectangle out of
      \param x1 The x position of the *first* corner
      \param y1 The y position of the *first* corner
      \param x2 The x position of the *opposite* corner
      \param y2 The y position of the *opposite* corner
      \param rFill the red *fill* color value between 0 and 255. deafults to *255*
      \param gFill the green *fill* color value between 0 and 255. deafults to *255*
      \param bFill the blue *fill* color value between 0 and 255. deafults to *255*
      \param rBorder the red *border* color value between 0 and 255. deafults to *255*
      \param gBorder the green *border* color value between 0 and 255. deafults to *255*
      \param bBorder the blue *border* color value between 0 and 255. deafults to *255*
      \param fill Should the rectangle be filled
    */
    void ConsoleGraphics::addRect(char c, int x1, int y1, int x2, int y2, int rFill, int gFill, int bFill, int rBorder, int gBorder, int bBorder, bool fill){

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
    /*
      print a fill-able rectangle to the back buffer to be drawn, this overload is more suitable for non-filled rectangles.\n
      note: This function **only** prints to the back buffer, to actually draw the draw() function has to be called.
      \param c The character to print the rectangle out of
      \param x1 The x position of the *first* corner
      \param y1 The y position of the *first* corner
      \param x2 The x position of the *opposite* corner
      \param y2 The y position of the *opposite* corner
      \param fill Should the rectangle be filled
      \param rBorder the red *border* color value between 0 and 255. deafults to *255*
      \param gBorder the green *border* color value between 0 and 255. deafults to *255*
      \param bBorder the blue *border* color value between 0 and 255. deafults to *255*
      \param rFill the red *fill* color value between 0 and 255. deafults to *255*
      \param gFill the green *fill* color value between 0 and 255. deafults to *255*
      \param bFill the blue *fill* color value between 0 and 255. deafults to *255*
    */
    void ConsoleGraphics::addRect(char c, int x1, int y1, int x2, int y2, bool fill, int rBorder, int gBorder, int bBorder, int rFill, int gFill, int bFill){
        addRect(c, x1, y1, x2, y2, rFill, gFill, bFill, rBorder, gBorder, bBorder, fill);
    }

    //! saved the current back buffer as the background
    /*!
      When draw is called, the back_buffer will be set to the background instead of empty.
    */
    void ConsoleGraphics::saveBackground() {
        memcpy(m_background, m_backBuffer, sizeof(cPixel) * (m_rows * m_columns));
    }

    //! will set the background to this char in the color r,g,b
    /*!
      \param c The character to set as the background.
      \param r The red background color value between 0 and 255. defaults to *255*
      \param g The green background color value between 0 and 255. defaults to *255*
      \param b The blue background color value between 0 and 255. defaults to *255*
    */
    void ConsoleGraphics::setBackground(char c, int r, int g, int b) {
        addRect(c, 0, 0, m_columns, m_rows, r,g,b,r,g,b);
        saveBackground();
    }

    //! returns pressed state of give key
    /*!
      Input the key in caps or virtual key codes (e.g.'Z' or VK_SPACE)
      \param k The key to get state from.
      \returns The pressed state of key *k*
    */
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
    /*
      this is the function that actually prints to the terminal.\n
      draw everything that has been written to the back buffer.
      this is **required** if you want anything that you printed to actually show up on screen.
      it is recommended that you only call this **once per program loop** as it is more efficient.\n
      note: will not display anything if nothing has been printed.
      \param override clear the backbuffer erasing everything on next draw. deafults to *true*
      \sa print() and printAt()
    */
    void ConsoleGraphics::draw(bool override){

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
                    if(m_ansiSupported) {
                        printf("\033[%d;%df%c", r, c, backPix.c);
                    } else {
                    #if defined(PLATFORM_WINDOWS)
                        int nC = c, nR = r;
                        if(c > m_columns - 8)
                            nC = m_columns - 8;
                        if(r > m_rows - 3)
                            nR = m_rows - 3;
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
