TEMPLATE = app
TARGET = myProject
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ./src
QT += opengl xml
CONFIG += qt \
    link_pkgconfig \
    release
MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp/obj

# Input
HEADERS += ./src/point3.h \
    ./src/Mesh.h \
    ./src/MyViewer.h \
    ./src/gl/GLUtilityMethods.h \
    ./src/gl/BasicColors.h \
    src/beziercurve.h \
    src/vectorfield.h \
    src/centerline.h \
    src/polygonise.h \
    src/particle.h \
    src/particle.h \
    src/densityfield.h
SOURCES += ./src/main.cpp \
    ./src/gl/GLUtilityMethods.cpp\
    ./src/gl/BasicColors.cpp \
    src/polygonise.cpp

#PKGCONFIG += gsl
#DEFINES += __USE_GSL_FOR_MAT33
#DEFINES += USE_GLU_ERROR_STRING
#PKGCONFIG += glu

LIBS += -lgomp -lHalf -ltbb -lopenvdb

EXT_DIR = extern



#QGLViewer
{
 INCLUDEPATH += $${EXT_DIR}/libQGLViewer-2.6.1
 LIBS +=    -L$${EXT_DIR}/libQGLViewer-2.6.1/QGLViewer \
            -lQGLViewer
}



release:QMAKE_CXXFLAGS_RELEASE += -O3 \
    -fopenmp
release:QMAKE_CFLAGS_RELEASE += -O3 \
    -fopenmp
