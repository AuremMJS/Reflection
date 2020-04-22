# University of Leeds 2019-2020
# COMP 582M1 Assignment 2
# Manigandan Rajan
# 201378895
#
# Reflection
#

This is a program that simulates reflection by rendering a mirror image to a texture

To compile on the University Linux machines, you will need to use the following command:

[userid@machine Tree]$ module add qt/5.3.1
[userid@machine Tree]$ qmake -project QT+=opengl LIBS+=-lGLU CONFIG += c++11
[userid@machine Tree]$ qmake
[userid@machine Tree]$ make

To execute the program, pass the file name on the command line like below:

[userid@machine Tree]$ ./Reflection