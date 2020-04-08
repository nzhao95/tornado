#ifndef VECTORFIELD_H
#define VECTORFIELD_H


#include <vector>
#include "point3.h"
#include "centerline.h"
#include "beziercurve.h"
#include "polygonise.h"
#include "particle.h"


class vectorField{

public :

    //curve stuff
    std::vector<bezierCurve<point3d>> curves;
    std::vector<float> times;
    float curve_step = 0.1;


    //grid stuff
    std::vector< std::vector< std::vector< point3d >>> grid;
    unsigned int grid_size;
    float grid_step = 1.f;
    point3d grid_bl;

    //kelvinlets stuff
    float a = 10;
    float b = 0.05;
    float epsilon = 0.0001;

    //polygon stuff

    std::vector<std::vector<point3d>> triangles;

    //particle stuff

    std::vector<particle> particles;
    float current_time = 0;
    float time_step = 0.01;

    void clear() {
        curves.clear();
        grid.clear();
        triangles.clear();
        particles.clear();
    }

    void addCurve(bezierCurve<point3d> curve, float t ) {
        curves.push_back(curve);
        times.push_back(t);
    }


    point3d curveLinearInterpValue(float t, float s) {
        if (curves.size() == 0)
            return point3d(0,0,0);
        if (curves.size() == 1)
            return curves[0].getValue(s);
        int start = 0;
        int end = curves.size()- 1;
        if (curves.size() > 2) {

            for (int n = 0; n < curves.size(); ++n) {
                if (times[n] < t && times[n] > times[start])
                    start = n;
                else if (times[n] > t && times[n] < times[end])
                    end = n;
            }
        }
        float a = (t - times[start])/(times[end] - times[start]);
        return (1-a) * curves[start].getValue(s) + a * curves[end].getValue(s);
    }

    point3d curveLinearInterpDerivative(float t, float s) {
        if (curves.size() == 0)
            return point3d(0,0,0);
        if (curves.size() == 1)
            return curves[0].getDerivative(s);
        int start = 0;
        int end = curves.size()- 1;
        for (int n = 0; n < curves.size(); ++n) {
            if (times[n] < t && times[n] > times[start])
                start = n;
            else if (times[n] > t && times[n] < times[end])
                end = n;
        }
        float a = (t - times[start])/(times[end] - times[start]);
        return (1-a) * curves[start].getDerivative(s) + a * curves[end].getDerivative(s);
    }

    void initGrid() {
        grid.clear();
        float s = 0;
        point3d bb(FLT_MAX,FLT_MAX,FLT_MAX) , BB(-FLT_MAX,-FLT_MAX,-FLT_MAX);
        while (s<1) {
            bb = point3d::min(bb , curveLinearInterpValue(0, s));
            BB = point3d::max(BB , curveLinearInterpValue(0, s));
            s += curve_step;
        }
        point3d center = (BB + bb) * 0.5;
        float length = std::max(BB[0] - bb[0], std::max(BB[1] - bb[1], BB[2] - bb[2]));
        grid_size = (unsigned int) ((length + 10) / grid_step);
        grid_bl = center - point3d(length, length, length) * 0.5 - point3d(5,5,5) / grid_step;
        grid.resize(grid_size);
        for (unsigned int i = 0; i < grid_size; ++i) {
            grid[i].resize(grid_size);
            for (unsigned int j = 0; j < grid_size; ++j) {
                grid[i][j].resize(grid_size);
            }
        }
        triangles.clear();

    }

    void computeGrid() {
        for (unsigned int i = 0; i < grid_size; ++i) {
            for (unsigned int j = 0; j < grid_size; ++j) {
                for (unsigned int k = 0; k < grid_size; ++k) {
                   point3d x = point3d(i*grid_step, j*grid_step, k*grid_step) + grid_bl;
                   point3d r = x - curveLinearInterpValue(current_time, 0);
                   point3d q;
                   float s = curve_step;
                   while (s < 1) {
                       point3d newr = x - curveLinearInterpValue(current_time, s);
                       if (newr.sqrnorm() < r.sqrnorm()) {
                           r = newr;
                           q = curveLinearInterpDerivative(current_time, s);
                       }
                       s += curve_step;
                   }
                   point3d c = point3d::cross(q, r);
                   float r_e = sqrt(r.sqrnorm() + pow(epsilon, 2));
                   grid[i][j][k] = - a * (1/pow(r_e,3)+ 3*pow(epsilon, 3) / (2*pow(r_e, 5))) * c;
                }
            }
        }
    }

    void computePolygon(float isovalue) {
        std::vector<point3d> polygon;
        polygon.clear();
        for (unsigned int i = 0; i < grid_size-1; ++i) {
            for (unsigned int j = 0; j < grid_size-1; ++j) {
                for (unsigned int k = 0; k < grid_size-1; ++k) {
                    point3d x = point3d(i*grid_step, j*grid_step, k*grid_step) + grid_bl;
                    IsoSurfacePolygonizer::GRIDCELL gridcell;
                    point3d vertex_order[8] = {point3d(0, 0, 0), point3d(1, 0, 0), point3d(1, 0, 1), point3d(0, 0, 1),
                                               point3d(0, 1, 0), point3d(1, 1, 0), point3d(1, 1, 1), point3d(0, 1, 1) };
                    for (int n = 0; n < 8; ++n){
                        gridcell.p[n] = x + vertex_order[n] * grid_step;
                        gridcell.val[n] = grid[i+vertex_order[n][0]][j+vertex_order[n][1]][k+vertex_order[n][2]].norm();
                    }
                    std::vector<point3d> new_triangles = Polygonise(gridcell, isovalue);
                    for (auto t : new_triangles) {
                        polygon.push_back(t);

                    }
                }
            }
        }
        triangles.push_back(polygon);
    }

    point3d computeVelocity (float t, point3d x) {
        point3d result;
        if (isEmpty()){
            return point3d(0, 0, 0);
        }
        float s = curve_step;
        while (s < 1) {
            point3d r = x - curveLinearInterpValue(t, s);
            point3d q = curveLinearInterpDerivative(t, s);
            point3d c = point3d::cross(q, r);
            float r_e = sqrt(r.sqrnorm() + pow(epsilon, 2));
            result += (- a * (1/pow(r_e,3)+ 3*pow(epsilon, 3) / (2*pow(r_e, 5))) * c) * curve_step;
            result += (b/r_e + b * 0.5 * pow(epsilon, 2) / pow(r_e, 3)) * q * curve_step;
            s += curve_step;
        }
        return result;
    }

    point3d RungeKutta_RK4(point3d pos) {
        point3d advectedTrajectory = point3d(0,0,0);
        int nSteps = 2;
        double timeStep = 1.0 / (double)(nSteps);
        for( unsigned int s = 0 ; s < nSteps ; ++s ){
            point3d xN = pos + advectedTrajectory;
            point3d k1 = computeVelocity(current_time + timeStep * s * time_step, xN);
            point3d k2 = computeVelocity(current_time + timeStep * s * time_step, xN + (timeStep/2)*k1);
            point3d k3 = computeVelocity(current_time + timeStep * s * time_step, xN + (timeStep/2)*k2);
            point3d k4 = computeVelocity(current_time + timeStep * s * time_step, xN + (timeStep)*k3);
            advectedTrajectory += (timeStep/6) * (k1 + 2*k2 + 2*k3 + k4);
        }
        return advectedTrajectory;
    }

    void animate() {
        if (particles.size() < 100) {
            particle p;
            float x = rand();
            p.pos = curveLinearInterpValue(current_time, 0.1) + point3d(10 * cos(x), 0, 10*sin(x));
            addParticle(p);
        }
        for (unsigned int pIt; pIt < particles.size(); ++pIt) {
            particles[pIt].advectedTrajectory = RungeKutta_RK4(particles[pIt].pos);
            particles[pIt].animate();
            if (particles[pIt].pos[1] > curveLinearInterpValue(current_time, 1)[1])
                deleteParticle(pIt);

        }
        current_time += time_step;
    }

    void draw() {
        glBegin(GL_POINTS);
        if (particles.size() > 0) {
            for (unsigned int pIt; pIt < particles.size(); ++pIt) {
                particles[pIt].draw();

            }
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (int n = 0; n < 101; n++ ){
            point3d p = curveLinearInterpValue(current_time, 0.01 * n);
            glVertex3f(p[0], p[1], p[2]);
        }
        glEnd();
    }

    void addParticle(particle p) {
        particles.push_back(p);
    }

    void deleteParticle(int n) {
        particles.erase(particles.begin() + n);
    }

    bool isEmpty() {
        if (curves.size() == 0)
            return true;
        else
            return curves[0].control_p.size() == 0;
    }

};


#endif // VECTORFIELD_H
