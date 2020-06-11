#ifndef PARTICLE_H
#define PARTICLE_H

#include "point3.h"
#include <math.h>
#include <QGLViewer/qglviewer.h>
#include <stdlib.h> // RAND_MAX

using namespace qglviewer;
using namespace std;


class particle {

public :
  point3d pos;
  point3d color;
  point3d advectedTrajectory;
  float life_span;

    void init() {
        pos = Vec(0, 0, 0);
        advectedTrajectory = Vec(0.0, 0.0, 0.0);
    }

    void animate() {
        pos = pos + advectedTrajectory;
    }

    void draw() {
        glColor3f(color[0], color[1], color[2]);
        glVertex3f(pos[0], pos[1],pos[2]);
    }


};



#endif // PARTICLE_H
