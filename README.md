# tornado

Artist friendly simulation of a tornado 

This tool allows you to draw a 3D line called the centerline of the tornado and simulates a tornado around this centerline. 

## Compilation

go to extern
unzip here
go to libQGLViewer-2.6.1/QGLViewer/
open QGLViewer.pro
configure le projet en Release dans le rep libQGLViewer-2.6.1/QGLViewer/ (là où est le .pro)
click on "configure project"
press the green arrow, it's going to compile the library
it will complain in the end by saying "could not find the executable. please specify one": DO NOT CARE ABOUT IT, CLOSE THE WINDOW
check that you have now in the repertory a file that looks like "libQGLViewer.so": that is the library.

## Shortkeys 

D : after drawing a 2D line, convert it in 3D
A : creates particles around the curve
P : Draws the paths of particles around the curve 
