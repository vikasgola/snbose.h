#/bin/sh

set -xe

CPPFLAGS="-std=c++11"
CPPLIBS="`pkg-config --libs glew glfw3` -Ilib -framework Cocoa -framework IOKit"
INCLUDE="src/*.cpp lib/2dphysics/src/*.cpp"

clang++ $CPPFLAGS $CPPLIBS $INCLUDE 2dbox.cpp -o 2dbox
clang++ $CPPFLAGS $CPPLIBS $INCLUDE checker_board.cpp -o checker_board
clang++ $CPPFLAGS $CPPLIBS $INCLUDE 3dbox.cpp -o 3dbox
clang++ $CPPFLAGS $CPPLIBS $INCLUDE primitives.cpp -o primitives
