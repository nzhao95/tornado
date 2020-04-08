# Tornado

Artist friendly simulation of a tornado 

This tool allows you to draw a 3D line called the centerline of the tornado and simulates a tornado around this centerline. 

## Compilation

go to extern \n
unzip here \n
go to libQGLViewer-2.6.1/QGLViewer/ \n
open QGLViewer.pro\n
configure le projet en Release dans le rep libQGLViewer-2.6.1/QGLViewer/ (là où est le .pro)\n
click on "configure project"\n
press the green arrow, it's going to compile the library\n
it will complain in the end by saying "could not find the executable. please specify one": DO NOT CARE ABOUT IT, CLOSE THE WINDOW\n
check that you have now in the repertory a file that looks like "libQGLViewer.so": that is the library.
\n
## Shortkeys 

D : after drawing a 2D line, convert it in 3D\n
A : creates particles around the curve\n
P : Draws the paths of particles around the curve \n
