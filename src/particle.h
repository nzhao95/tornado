#ifndef PARTICLE_H
#define PARTICLE_H

#include "vectorfield.h"
#include "point3.h"
#include <math.h>
#include <QGLViewer/qglviewer.h>
#include <stdlib.h> // RAND_MAX

using namespace qglviewer;
using namespace std;


class particle {

public :
  point3d pos;
  point3d advectedTrajectory;

    int nSteps = 2;

    vectorField field;

    void init() {
        pos = Vec(0, 0, 0);
        advectedTrajectory = Vec(0.0, 0.0, 0.0);
    }


    void RungeKutta_RK4() {
        advectedTrajectory = point3d(0,0,0);
        double timeStep = 1.0 / (double)(nSteps);
        for( unsigned int s = 0 ; s < nSteps ; ++s ){
            point3d xN = pos + advectedTrajectory;
            point3d k1 = field.computeVelocity(xN);
            point3d k2 = field.computeVelocity(xN + (timeStep/2)*k1);
            point3d k3 = field.computeVelocity(xN + (timeStep/2)*k2);
            point3d k4 = field.computeVelocity(xN + (timeStep)*k3);
            advectedTrajectory += (timeStep/6) * (k1 + 2*k2 + 2*k3 + k4);
        }
    }

    void animate() {
        RungeKutta_RK4();

        pos = pos + advectedTrajectory;
    }

    void draw() {
        glColor3f(0, 0, 1.0);
        glVertex3f(pos[0], pos[1],pos[2]);
    }


};



#endif // PARTICLE_H
