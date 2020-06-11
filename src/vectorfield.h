#ifndef VECTORFIELD_H
#define VECTORFIELD_H


#include <vector>
#include "point3.h"
#include "centerline.h"
#include "beziercurve.h"
#include "polygonise.h"
#include "particle.h"
#include "noise.h"

class vectorField{

public :

    //curve stuff
    std::vector<bezierCurve<point3d>> curves;
    std::vector<float> times;
    float curve_step = 0.05;


    //grid stuff
    std::vector< std::vector< std::vector< point3d >>> grid;
    unsigned int grid_size;
    float grid_step = 0.5f;
    point3d grid_bl;

    //kelvinlets stuff
    float a = 20;
    float epsilon_a = 1;
    float b = 0.2;
    float epsilon = 1;
    float epsilon2 = 7;

    Noise vortices;

    //polygon stuff

    std::vector<std::vector<point3d>> triangles;

    //particle stuff

    std::vector<particle> particles;
    float current_time = 0;
    float time_step = 0.5;

    float curve_speed = 0.01;

    void clear() {
        curves.clear();
        grid.clear();
        triangles.clear();
        particles.clear();
        vortices.clear();
    }

    /// Curve stuff

    void addCurve(bezierCurve<point3d> curve, float t ) {
        curves.push_back(curve);
        times.push_back(t);
        initVortices();
    }


    point3d curveLinearInterpValue(float t, float s) {
        t = t * curve_speed;
        if (curves.size() == 0)
            return point3d(0,0,0);
        if (curves.size() == 1)
            return curves[0].getValue(s);
        int start = 0;
        int end = times.back();
        float max_t = *std::max_element(times.begin(), times.end());

        for (unsigned int n = 0; n < curves.size(); ++n) {
            if (times[n] < t && times[n] > times[start] && t < max_t)
                start = n;
            else if (times[n] > t && times[n] < times[end])
                end = n;
        }
        float a = (t - times[start])/(times[end] - times[start]);
        return (1-a) * curves[start].getValue(s) + a * curves[end].getValue(s);
    }

    point3d curveLinearInterpDerivative(float t, float s) {
        t = t * curve_speed;
        if (curves.size() == 0)
            return point3d(0,0,0);
        if (curves.size() == 1)
            return curves[0].getDerivative(s);
        int start = 0;
        int end = times.back();
        float max_t = *std::max_element(times.begin(), times.end());
        for (unsigned int n = 0; n < curves.size(); ++n) {
            if (times[n] < t && times[n] > times[start] && t < max_t)
                start = n;
            else if (times[n] > t && times[n] < times[end])
                end = n;
        }
        float a = (t - times[start])/(times[end] - times[start]);
        return (1-a) * curves[start].getDerivative(s) + a * curves[end].getDerivative(s);
    }

    void initVortices() {
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
        vortices.init(grid_size, grid_bl);

    }

    void updateVortices() {
        vortices.update();
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
                   grid[i][j][k] = computeVelocity(current_time, x);
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
        point3d result = point3d(0,0,0);
        if (isEmpty()){
            return point3d(0, 0, 0);
        }
        float s = curve_step;
        while (s < 1) {
            point3d r = x - curveLinearInterpValue(t, s);
            point3d q = curveLinearInterpDerivative(t, s);
            point3d c = point3d::cross(q, r);
            float r_ea = sqrt(r.sqrnorm() + pow(epsilon_a, 2));
            float r_ea2 = sqrt(r.sqrnorm() + pow(epsilon_a * 2, 2));
            float r_e = sqrt(r.sqrnorm() + pow(epsilon, 2));
            float r_e2 = sqrt(r.sqrnorm() + pow(epsilon2, 2));
            result += (- a * (1/pow(r_ea,3)+ 3*pow(epsilon_a, 3) / (2*pow(r_ea, 5))) * c) * curve_step;
//            result += (a * (1/pow(r_ea2,3)+ 3*pow(epsilon_a * 2, 3) / (2*pow(r_ea2, 5))) * c) * curve_step;
            result += (b/r_e + b * 0.5 * pow(epsilon, 2) / pow(r_e, 3)) * q * curve_step;
            result += (-b/r_e2 - b * 0.5 * pow(epsilon2, 2) / pow(r_e2, 3)) * q * curve_step;
            result += point3d(0, -0.01, 0); //gravity
            result += vortices.curl(x);
            s += curve_step;
        }
        return result;
    }

    point3d RungeKutta_RK4(point3d pos) {
        point3d advectedTrajectory = point3d(0,0,0);
        unsigned int nSteps = min(max((int) (computeVelocity(current_time, pos).norm() / 0.5), 1), 5);
        double timeStep = 1.0 / (double)(nSteps);
        for( unsigned int s = 0 ; s < nSteps ; ++s ){
            point3d xN = pos + advectedTrajectory;
            point3d k1 = computeVelocity(current_time + timeStep * s, xN);
            point3d k2 = computeVelocity(current_time + timeStep * s, xN + (timeStep/2)*k1);
            point3d k3 = computeVelocity(current_time + timeStep * s, xN + (timeStep/2)*k2);
            point3d k4 = computeVelocity(current_time + timeStep * s, xN + (timeStep)*k3);
            advectedTrajectory += (timeStep/6) * (k1 + 2*k2 + 2*k3 + k4);
        }
        return advectedTrajectory;
    }

    void particleAnimation() {
        if (particles.size() < 50) {
            particle p;
            float x = rand();
            float y = (float) rand()/RAND_MAX;
            p.pos = curveLinearInterpValue(current_time, 0.1) + point3d(y * 10 * cos(x), y , y * 10* sin(x));
            p.color = point3d(cos(x), sin(x), cos(x + 0.5));
            addParticle(p);
        }
        for (unsigned int pIt; pIt < particles.size(); ++pIt) {
            particles[pIt].advectedTrajectory = RungeKutta_RK4(particles[pIt].pos) * time_step;
            particles[pIt].animate();
            if (particles[pIt].pos[1] > max(curveLinearInterpValue(current_time, 1)[1], curveLinearInterpValue(current_time, 0)[1])
                    || particles[pIt].pos[1] < min(curveLinearInterpValue(current_time, 1)[1], curveLinearInterpValue(current_time, 0)[1]))
                deleteParticle(pIt);

        }
        current_time += time_step;
    }

    void pathAnimation() {
        for (unsigned int pIt; pIt < particles.size(); ++pIt) {
            particles[pIt].advectedTrajectory = RungeKutta_RK4(particles[pIt].pos);
            particles[pIt].animate();

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
