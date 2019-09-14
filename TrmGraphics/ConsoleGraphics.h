
#ifndef TrmGraphics_ConsoleGraphics_HEADER
#define TrmGraphics_ConsoleGraphics_HEADER

#include <string>

namespace TrmGraphics {

    #define DEFAULT_FONT_SIZE 16

    //typedef class string string;

    typedef struct cPixel cPixel;

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

        //! print at cursor position
        /*!
          print somthing to the terminal starting at current cursor position, with color (r, g, b)\n
          note: This function **only** prints to the back buffer, to actually draw the draw() function has to be called
          \param str the string to be printed
          \param r the red color value between 0 and 255. deafults to *255*
          \param g the green color value between 0 and 255. deafults to *255*
          \param b the blue color value between 0 and 255. deafults to *255*
          \sa printAt() and draw()
          */
        void print(std::string str, int r = 255, int g = 255, int b = 255);

        //! move cursor, then print at new cursor position
        /*!
          print somthing to the terminal starting at cursor position (x, y), with color (r, g, b)\n
          note: This function **only** prints to the back buffer, to actually draw the draw() function has to be called
          \param str the string to be printed
          \param x the x position to print at (row)
          \param y the y position to print at (column)
          \param r the red color value between 0 and 255. deafults to *255*
          \param g the green color value between 0 and 255. deafults to *255*
          \param b the blue color value between 0 and 255. deafults to *255*
          \sa printAt() and draw()
        */
        void printAt(std::string str, unsigned int x, unsigned int y, int r = 255, int g = 255, int b = 255);

        //! draw everything to the terminal
        /*!
          this is the function that actually prints to the terminal.\n
          draw everything that has been written to the back buffer.
          this is **required** if you want anything that you printed to actually show up on screen.
          it is recommended that you only call this **once per program loop** as it is more efficient.\n
          note: will not display anything if nothing has been printed.
          \param override clear the backbuffer erasing everything on next draw. deafults to *true*
          \sa print() and printAt()
        */
        void draw(bool override = true);

    private:

        //functions
        void clearConsole();
        int getIndex(int x, int y);
        void setConsoleColor(int r, int g, int b);

        //variables
        bool m_ansiSupported = true;

        cPixel* m_backBuffer;
        cPixel* m_frontBuffer;

        int m_rows;
        int m_columns;

        int m_cursor_index;

    };

}
#endif
