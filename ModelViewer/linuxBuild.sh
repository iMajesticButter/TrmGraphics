sudo g++ -o ./build-linux/modelViewer.out main.cpp -I./lib/TrmGraphics/Include ./lib/TrmGraphics/lib-linux/*.so -L/usr/X11/lib -lX11 -Wall -Wextra -Werror -std=c++11 -pedantic

