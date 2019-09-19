
#ifndef TrmGraphics_ConsoleGraphics_HEADER
#define TrmGraphics_ConsoleGraphics_HEADER

#include <string>
#include <iostream>
#include <vec2D.h>
#include <vec3D.h>

namespace TrmGraphics {

    #define DEFAULT_FONT_SIZE 16

    //typedef class string string;

    typedef struct cPixel cPixel;
    typedef class vec2D vec2D;
    typedef class vec3D vec3D;

    //! Graphical Console Manager
    /*!
      Manages drawing text efficiently in 24bit color
      If compiled for windows, it will try to enable ANSI escape code in the windows terminal for 24bit color,
      however if this fails, it will translate the rgb color to the closest color in the default windows color pallette and use that.
      It will also handle resizing the terminal, however if on windows and the terminal cannot use ANSI escape codes, it will need to know the
      font size being used by the terminal.
      to actually display what is printed, you must call the draw() function
      \see draw()
    */
    class ConsoleGraphics {
    public:
        //! ConsoleGraphics constructor
        /*!
          \param columns The desired width of the terminal.
          \param rows The desired height of the terminal.
          \param askForFontSize Sould the Console Manager automatically ask the user for the terminals font size. **Only required on windows**. defaults to *false*
          \param fontSize Allows you to input the terminals font size **Only works when askForFontSize is false**. deafults to *16* as that is the deafult font size on the windwos terminal
          \sa print(), printAt() and draw()
        */
        ConsoleGraphics(int columns, int rows, bool askForFontSize = false, int fontSize = DEFAULT_FONT_SIZE);

        //! ConsoleGraphics destructor
        ~ConsoleGraphics();

        //! Get time between last updates
        float getDeltaTime();

        //! Set the cursor position to x,y
        /*!
          \param x The columns to set the cursor position to
          \param y The row to set the cursor position to
        */
        void setCursorPos(int x, int y);

        // Operator<<
        void operator<< (short val);
        void operator<< (unsigned short val);
        void operator<< (int val);
        void operator<< (unsigned int val);
        void operator<< (long val);
        void operator<< (unsigned long val);
        void operator<< (float val);
        void operator<< (double val);
        void operator<< (long double val);
        void operator<< (void* val);
        void operator<< (std::streambuf* sb );
        void operator<< (std::ostream& (*pf)(std::ostream&));
        void operator<< (std::ios& (*pf)(std::ios&));
        void operator<< (std::ios_base& (*pf)(std::ios_base&));
        void operator<< (std::string str);
        void operator<< (bool val);
        void operator<< (const char* val);

        //! print at cursor position
        /*!
          print somthing to the terminal starting at current cursor position, with color (r, g, b).\n
          note: This function **only** prints to the back buffer, to actually draw the draw() function has to be called.
          \param str the string to be printed
          \param r the red color value between 0 and 255. deafults to *255*
          \param g the green color value between 0 and 255. deafults to *255*
          \param b the blue color value between 0 and 255. deafults to *255*
          \sa printAt() and draw()
        */
        void print(std::string str, int r = 255, int g = 255, int b = 255);

        //! move cursor, then print at new cursor position
        /*!
          print somthing to the terminal starting at cursor position (x, y), with color (r, g, b).\n
          note: This function **only** prints to the back buffer, to actually draw the draw() function has to be called.
          \param str the string to be printed
          \param pos the position to print at
          \param r the red color value between 0 and 255. deafults to *255*
          \param g the green color value between 0 and 255. deafults to *255*
          \param b the blue color value between 0 and 255. deafults to *255*
          \sa printAt(), draw(), addRect()
        */
        void printAt(std::string str, vec2D pos, int r = 255, int g = 255, int b = 255);

        //! print at cursor position
        /*!
          print character to the terminal starting at current cursor position, with color (r, g, b).\n
          note: This function **only** prints to the back buffer, to actually draw the draw() function has to be called.
          \param c the character to be printed
          \param r the red color value between 0 and 255. deafults to *255*
          \param g the green color value between 0 and 255. deafults to *255*
          \param b the blue color value between 0 and 255. deafults to *255*
          \sa printAt() and draw()
        */
        void print(char c, int r = 255, int g = 255, int b = 255);

        //! move cursor, then print at new cursor position
        /*!
          print character to the terminal starting at cursor position (x, y), with color (r, g, b).\n
          note: This function **only** prints to the back buffer, to actually draw the draw() function has to be called.
          \param c the character to be printed
          \param pos the position to print at
          \param r the red color value between 0 and 255. deafults to *255*
          \param g the green color value between 0 and 255. deafults to *255*
          \param b the blue color value between 0 and 255. deafults to *255*
          \sa printAt(), draw(), addRect()
        */
        void printAt(char c, vec2D pos, int r = 255, int g = 255, int b = 255);

        //! print a rectangle to the back buffers
        /*!
          print a fill-able rectangle to the back buffer to be drawn.\n
          note: This function **only** prints to the back buffer, to actually draw, the draw() function has to be called.
          \param c The character to print the rectangle out of
          \param pos1 position of the *first* corner
          \param pos2 position of the *opposite* corner
          \param rFill the red *fill* color value between 0 and 255. deafults to *255*
          \param gFill the green *fill* color value between 0 and 255. deafults to *255*
          \param bFill the blue *fill* color value between 0 and 255. deafults to *255*
          \param rBorder the red *border* color value between 0 and 255. deafults to *rFill*
          \param gBorder the green *border* color value between 0 and 255. deafults to *gFill*
          \param bBorder the blue *border* color value between 0 and 255. deafults to *bFill*
          \param fill Should the rectangle be filled
          \sa printAt(), print(), draw()
        */
        void addRect(char c, vec2D pos1, vec2D pos2, int rFill = 255, int gFill = 255, int bFill = 255, int rBorder = -1, int gBorder = -1, int bBorder = -1, bool fill = true);

        //! print a rectangle to the back buffers
        /*!
          print a fill-able rectangle to the back buffer to be drawn, this overload is more suitable for non-filled rectangles.\n
          note: This function **only** prints to the back buffer, to actually draw, the draw() function has to be called.
          \param c The character to print the rectangle out of
          \param pos1 position of the *first* corner
          \param pos2 position of the *opposite* corner
          \param fill Should the rectangle be filled
          \param rBorder the red *border* color value between 0 and 255. deafults to *255*
          \param gBorder the green *border* color value between 0 and 255. deafults to *255*
          \param bBorder the blue *border* color value between 0 and 255. deafults to *255*
          \param rFill the red *fill* color value between 0 and 255. deafults to *255*
          \param gFill the green *fill* color value between 0 and 255. deafults to *255*
          \param bFill the blue *fill* color value between 0 and 255. deafults to *255*
          \sa printAt(), print(), draw()
        */
        void addRect(char c, vec2D pos1, vec2D pos2, bool fill, int rBorder = 255, int gBorder = 255, int bBorder = 255, int rFill = 255, int gFill = 255, int bFill = 255);

        //! print a line to the back buffer
        /*!
          print a color-able line from x1,y1 to x2,y2 on the back buffer\n
          note: This function **only** prints to the back buffer, to actually draw, the draw() function has to be called.
          \param c The character to print the line out of
          \param pos1 The position of the line starting point
          \param pos2 The position of the line ending point
          \param r the red color value of the line between 0 and 255. defalts to *255*
          \param g the green color value of the line between 0 and 255. defalts to *255*
          \param b the blue color value of the line between 0 and 255. defalts to *255*
          \sa draw()
        */
        void addLine(char c, vec2D pos1, vec2D pos2, int r = 255, int g = 255, int b = 255);

        //! print a triangle to the back buffer
        /*!
          print a color-able triangle with vertecies x1,y1 x2,y2 x3,y3 to the back buffer\n
          note: This function **only** prints to the back buffer, to actually draw, the draw() function has to be called.
          \param c The character to prin the triangle out out of
          \param pos1 The position of vertex 1
          \param pos2 The position of vertex 2
          \param pos3 The position of vertex 3
          \param rFill the red color value of the triangle fill between 0 and 255. defalts to *255*
          \param gFill the green color value of the triangle fill between 0 and 255. defalts to *255*
          \param bFill the blue color value of the triangle fill between 0 and 255. defalts to *255*
          \param rBorder the red color value of the triangle border between 0 and 255. defalts to *rFill*
          \param gBorder the green color value of the triangle border between 0 and 255. defalts to *gFill*
          \param bBorder the blue color value of the triangle border between 0 and 255. defalts to *bFill*
          \param fill should the triangle be filled
          \sa draw()
        */
        void addTri(char c, vec2D pos1, vec2D pos2, vec2D pos3, int rFill = 255, int gFill = 255, int bFill = 255, int rBorder = -1, int gBorder = -1, int bBorder = -1, bool fill = true);

        //! print a triangle to the back buffer
        /*!
          print a color-able triangle with vertecies x1,y1 x2,y2 x3,y3 to the back buffer, this Overload is sutible for non-filled triangles\n
          note: This function **only** prints to the back buffer, to actually draw, the draw() function has to be called.
          \param c The character to prin the triangle out out of
          \param pos1 The position of vertex 1
          \param pos2 The position of vertex 2
          \param pos3 The position of vertex 3
          \param fill should the triangle be filled
          \param rFill the red color value of the triangle fill between 0 and 255. defalts to *255*
          \param gFill the green color value of the triangle fill between 0 and 255. defalts to *255*
          \param bFill the blue color value of the triangle fill between 0 and 255. defalts to *255*
          \param rBorder the red color value of the triangle border between 0 and 255. defalts to *rFill*
          \param gBorder the green color value of the triangle border between 0 and 255. defalts to *gFill*
          \param bBorder the blue color value of the triangle border between 0 and 255. defalts to *bFill*
          \sa draw()
        */
        void addTri(char c, vec2D pos1, vec2D pos2, vec2D pos3, bool fill, int rBorder = 255, int gBorder = 255, int bBorder = 255, int rFill = 255, int gFill = 255, int bFill = 255);

        //! print an ellipse to the back buffer
        /*!
          print a color-able ellipse with mid point xPos,yPos and scale xSize,ySize to the back buffer\n
          note: This function **only** prints to the back buffer, to actually draw, the draw() function has to be called.
          \param c The character to print the ellipse out out out of
          \param pos The position of the ellipse
          \param size The size of the ellipse
          \param rFill the red color value of the ellipse fill between 0 and 255. defalts to *255*
          \param gFill the green color value of the ellipse fill between 0 and 255. defalts to *255*
          \param bFill the blue color value of the ellipse fill between 0 and 255. defalts to *255*
          \param rBorder the red color value of the ellipse border between 0 and 255. defalts to *rFill*
          \param gBorder the green color value of the ellipse border between 0 and 255. defalts to *gFill*
          \param bBorder the blue color value of the ellipse border between 0 and 255. defalts to *bFill*
          \sa draw()
        */
        void addEllipse(char c, vec2D pos, vec2D size, int rFill = 255, int gFill = 255, int bFill = 255, int rBorder = -1, int gBorder = -1, int bBorder = -1, bool fill = true);

        //! print an ellipse to the back buffer
        /*!
          print a color-able ellipse with mid point xPos,yPos and scale xSize,ySize to the back buffer, this Overload is sutible for non-filled ellipses\n
          note: This function **only** prints to the back buffer, to actually draw, the draw() function has to be called.
          \param c The character to print the ellipse out out out of
          \param pos The position of the ellipse
          \param size The size of the ellipse
          \param rFill the red color value of the ellipse fill between 0 and 255. defalts to *255*
          \param gFill the green color value of the ellipse fill between 0 and 255. defalts to *255*
          \param bFill the blue color value of the ellipse fill between 0 and 255. defalts to *255*
          \param rBorder the red color value of the ellipse border between 0 and 255. defalts to *rFill*
          \param gBorder the green color value of the ellipse border between 0 and 255. defalts to *gFill*
          \param bBorder the blue color value of the ellipse border between 0 and 255. defalts to *bFill*
          \sa draw()
        */
        void addEllipse(char c, vec2D pos, vec2D size, bool fill, int rBorder = 255, int gBorder = 255, int bBorder = 255, int rFill = 255, int gFill = 255, int bFill = 255);


        //! saved the current back buffer as the background
        /*!
          When draw is called, the back_buffer will be set to the background instead of empty.
        */
        void saveBackground();

        //! will set the background to this char in the color r,g,b
        /*!
          \param c The character to set as the background.
          \param r The red background color value between 0 and 255. defaults to *255*
          \param g The green background color value between 0 and 255. defaults to *255*
          \param b The blue background color value between 0 and 255. defaults to *255*
        */
        void setBackground(char c, int r = 255, int g = 255, int b = 255);

        //! returns pressed state of give key
        /*!
          Input the key in caps
          \param k The key to get state from.
          \returns The pressed state of key *k*
        */
        bool keyPressed(int k);

        //! draw everything to the terminal
        /*!
          this is the function that actually prints to the terminal.\n
          draw everything that has been written to the back buffer.
          this is **required** if you want anything that you printed to actually show up on screen.
          it is recommended that you only call this **once per program loop** as it is more efficient.\n
          note: will not display anything if nothing has been printed.
          \param override clear the backbuffer erasing everything on next draw. deafults to *true*
          \sa print(), printAt(), addRect()
        */
        void draw(bool override = true);

    private:

        //functions
        void clearConsole();
        int getIndex(int x, int y);
        void setConsoleColor(unsigned int r, unsigned int g, unsigned int b);

        //variables
        bool m_ansiSupported = true;

        cPixel* m_backBuffer;
        cPixel* m_frontBuffer;
        cPixel* m_background;

        int m_rows;
        int m_columns;

        int m_cursor_index;

        float m_deltaTime;

        double m_lastTime;

    };

}
#endif
