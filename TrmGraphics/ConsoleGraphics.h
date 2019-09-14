
#ifndef TrmGraphics_ConsoleGraphics_HEADER
#define TrmGraphics_ConsoleGraphics_HEADER

#include <string>

namespace TrmGraphics {

    #define DEFAULT_FONT_SIZE 16

    //typedef class string string;

    typedef struct cPixel cPixel;

    class ConsoleGraphics {
    public:
        ConsoleGraphics(int columns, int rows, bool askForFontSize = false, int fontSize = DEFAULT_FONT_SIZE);
        ~ConsoleGraphics();

        //print somthing to the console starting at cursor position (x, y) with color (r, g, b)
        // str: the value to be printed
        // r: the red value for color
        // g: the green value for color
        // b: the blue value for color
        void print(std::string str, int r = 255, int g = 255, int b = 255);

        //print somthing to the console starting at cursor position (x, y) with color (r, g, b)
        // str: the value to be printed
        // x: the x position to print at (row)
        // y: the y position to print at (column)
        // r: the red value for color
        // g: the green value for color
        // b: the blue value for color
        void printAt(std::string str, unsigned int x, unsigned int y, int r = 255, int g = 255, int b = 255);

        //draw everything that has been printed to the buffer
        // override: clear the backbuffer eraseing everything on next draw
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
