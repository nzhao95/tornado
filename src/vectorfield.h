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
    bezierCurve<point3d> curve;
    float curve_step = 0.01;


    //grid stuff
    std::vector< std::vector< std::vector< point3d >>> grid;
    unsigned int grid_size;
    float grid_step = 1.f;
    point3d grid_bl;

    //kelvinlets stuff
    float a = 10;
    float epsilon = 0.0001;

    //polygon stuff

    std::vector<std::vector<point3d>> triangles;

    //particle stuff

    unsigned int nSteps = 2;
    std::vector<particle> particles;

    void clear() {
        curve.control_p.clear();
        grid.clear();
        triangles.clear();
        particles.clear();
    }

    void init() {
        grid.clear();
        float t = 0;
        point3d bb(FLT_MAX,FLT_MAX,FLT_MAX) , BB(-FLT_MAX,-FLT_MAX,-FLT_MAX);
        while (t<1) {
            bb = point3d::min(bb , curve.getValue(t));
            BB = point3d::max(BB , curve.getValue(t));
            t += curve_step;
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
                   point3d r = x - curve.getValue(0);
                   point3d q;
                   float t = curve_step;
                   while (t < 1) {
                       point3d newr = x - curve.getValue(t);
                       if (newr.sqrnorm() < r.sqrnorm()) {
                           r = newr;
                           q = curve.getDerivative(t);
                       }
                       t += curve_step;
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

    point3d computeVelocity (point3d x) {
        if (curve.control_p.empty()){
            return point3d(0, 0, 0);
        }
        point3d r = x - curve.getValue(0);
        point3d q;
        float t = curve_step;
        while (t < 1) {
            point3d newr = x - curve.getValue(t);
            if (newr.sqrnorm() < r.sqrnorm()) {
                r = newr;
                q = curve.getDerivative(t);
            }
            t += curve_step;
        }
        point3d c = point3d::cross(q, r);
        float r_e = sqrt(r.sqrnorm() + pow(epsilon, 2));
        return - a * (1/pow(r_e,3)+ 3*pow(epsilon, 3) / (2*pow(r_e, 5))) * c;
    }

    point3d RungeKutta_RK4(point3d pos) {
        point3d advectedTrajectory = point3d(0,0,0);
        double timeStep = 1.0 / (double)(nSteps);
        for( unsigned int s = 0 ; s < nSteps ; ++s ){
            point3d xN = pos + advectedTrajectory;
            point3d k1 = computeVelocity(xN);
            point3d k2 = computeVelocity(xN + (timeStep/2)*k1);
            point3d k3 = computeVelocity(xN + (timeStep/2)*k2);
            point3d k4 = computeVelocity(xN + (timeStep)*k3);
            advectedTrajectory += (timeStep/6) * (k1 + 2*k2 + 2*k3 + k4);
        }
        return advectedTrajectory;
    }

    void animate() {
        for (unsigned int pIt; pIt < particles.size(); ++pIt) {
            particles[pIt].advectedTrajectory = RungeKutta_RK4(particles[pIt].pos);
            particles[pIt].animate();

        }
    }

    void draw() {
        for (unsigned int pIt; pIt < particles.size(); ++pIt) {
            particles[pIt].draw();

        }
    }

    void addParticle(particle p) {
        particles.push_back(p);
    }

    bool isEmpty() {
        return curve.control_p.size() > 0;
    }

};


#endif // VECTORFIELD_H
