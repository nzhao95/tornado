#ifndef DENSITYFIELD_H
#define DENSITYFIELD_H

#include <math.h>
#include <QGLViewer/qglviewer.h>

#include "point3.h"
#include "vectorfield.h"

using namespace qglviewer;

class densityfield {

public :
    std::vector< std::vector< std::vector< float > > > grid;
    std::vector< std::vector< std::vector< point3d > > > grid3d;
    std::vector< std::vector< std::vector< float > > > newgrid;
    std::vector< std::vector< std::vector< point3d > > > newgrid3d;
    unsigned int grid_size;
    float dx = 0.5f;
    point3d grid_bl;

    float current_time = 0;
    float dt = 0.5;

    int mode = 0;  // 0 : 1D, 1 : 3D
    vectorField vector_field;

    std::vector< std::vector<point3d> > triangles;

    float clamp(const float v, const float vmin, const float vmax) {
      if(v<vmin) return vmin;
      if(v>vmax) return vmax;
      return v;
    }

    void init() {
        triangles.clear();
        grid.clear();
        grid3d.clear();
        newgrid.clear();
        newgrid3d.clear();
        float s = 0;
        point3d bb(FLT_MAX,FLT_MAX,FLT_MAX) , BB(-FLT_MAX,-FLT_MAX,-FLT_MAX);
        while (s<1) {
            bb = point3d::min(bb , vector_field.curveLinearInterpValue(current_time, s));
            BB = point3d::max(BB , vector_field.curveLinearInterpValue(current_time, s));
            s += vector_field.curve_step;
        }
        point3d center = (BB + bb) * 0.5;
        float length = std::max(BB[0] - bb[0], std::max(BB[1] - bb[1], BB[2] - bb[2]));
        grid_size = (unsigned int) ((length) / dx);
        grid_bl = center - point3d(length, length, length) * 0.5;
        if (mode == 0) {
            grid.resize(grid_size);
            newgrid.resize(grid_size);
            for (unsigned int i = 0; i < grid_size; ++i) {
                grid[i].resize(grid_size);
                newgrid[i].resize(grid_size);
                for (unsigned int j = 0; j < grid_size; ++j) {
                    grid[i][j].resize(grid_size, 0);
                    newgrid[i][j].resize(grid_size, 0);
                }
            }

            for (unsigned int i = 0; i < grid_size; ++i) {
                for (unsigned int j = 0; j < grid_size * 0.1; ++j) {
                    for (unsigned int k = 0; k < grid_size; ++k) {
                        grid[i][j][k] = (float) rand()/RAND_MAX;
                    }
                }
            }
        }
        else {
            grid3d.resize(grid_size);
            newgrid3d.resize(grid_size);
            for (unsigned int i = 0; i < grid_size; ++i) {
                grid3d[i].resize(grid_size);
                newgrid3d[i].resize(grid_size);
                for (unsigned int j = 0; j < grid_size; ++j) {
                    grid3d[i][j].resize(grid_size, point3d(0,0,0));
                    newgrid3d[i][j].resize(grid_size, point3d(0,0,0));
                }
            }

            for (unsigned int i = 0; i < grid_size; ++i) {
                for (unsigned int j = 0; j < grid_size * 0.1; ++j) {
                    for (unsigned int k = 0; k < grid_size; ++k) {
                        grid3d[i][j][k] = point3d(i>grid_size / 2 ? 1 : 0, 0, i>grid_size / 2 ? 0 : 1);
                    }
                }
            }
        }
    }


    point3d worldCoord (int i, int j, int k) {
        return point3d(i*dx, j*dx, k*dx) + grid_bl;
    }

    point3d gridCoord ( point3d x) {
        point3d coord = (x - grid_bl) / dx;
        return coord;
    }

    float sampleAt(point3d x) {
        point3d coord = gridCoord(x);
        unsigned int i = clamp(static_cast<int>(coord[0]), 0, grid_size - 1);
        unsigned int j = clamp(static_cast<int>(coord[1]), 0, grid_size - 1);
        unsigned int k = clamp(static_cast<int>(coord[2]), 0, grid_size - 1);
        float alpha = coord[0] - i;
        float beta = coord[1] - j;
        float gamma = coord[2] - k;
        float result;
        if (i == grid_size - 1) {
            if (j == grid_size -1) {
                if (k == grid_size - 1)
                    result =  grid[i][j][k];
                else
                    result =  (1-gamma) * grid[i][j][k] + gamma * grid[i][j][k+1];
            }
            else {
                if (k == grid_size - 1)
                    result =  (1-beta) * grid[i][j][k] + beta* grid[i][j+1][k];
                else
                    result =  ( (1 - beta) * ((1 - gamma) * grid[i][j][k] + gamma * grid[i][j][k+1])
                        + beta * ((1 - gamma) * grid[i][j+1][k] + gamma * grid[i][j+1][k+1]));
            }
        }
        else if (j == grid_size - 1) {
            if (k == grid_size - 1) {
                result =  (1-alpha) * grid[i][j][k] + alpha * grid[i+1][j][k];
            }
            else
                result =  ( (1 - alpha) * ((1 - gamma) * grid[i][j][k] + gamma * grid[i][j][k+1])
                        + alpha * ((1 - gamma) * grid[i+1][j][k] + gamma * grid[i+1][j][k+1]));
        }
        else if (k == grid_size - 1)
            result =  ( (1 - alpha) * ((1 - beta) * grid[i][j][k] + beta * grid[i][j+1][k])
                    + alpha * ((1 - beta) * grid[i+1][j][k] + beta* grid[i+1][j+1][k]));
        else
            result =  (1 - alpha) * ( (1 - beta) * ((1 - gamma) * grid[i][j][k] + gamma * grid[i][j][k+1])
                + beta * ((1 - gamma) * grid[i][j+1][k] + gamma * grid[i][j+1][k+1]))
                + alpha * ( (1 - beta) * ((1 - gamma) * grid[i+1][j][k] + gamma * grid[i+1][j][k+1])
                + beta * ((1 - gamma) * grid[i+1][j+1][k] + gamma * grid[i+1][j+1][k+1]));
        return clamp(result, 0, 1);
    }

    point3d sample3d(point3d x) {
        point3d coord = gridCoord(x);
        unsigned int i = clamp(static_cast<int>(coord[0]), 0, grid_size - 1);
        unsigned int j = clamp(static_cast<int>(coord[1]), 0, grid_size - 1);
        unsigned int k = clamp(static_cast<int>(coord[2]), 0, grid_size - 1);
        float alpha = coord[0] - i;
        float beta = coord[1] - j;
        float gamma = coord[2] - k;
        point3d result;
        if (i == grid_size - 1) {
            if (j == grid_size -1) {
                newgrid3d.resize(grid_size);
                newgrid.resize(grid_size);
                if (k == grid_size - 1)
                    result =  grid3d[i][j][k];
                else
                    result =  (1-gamma) * grid3d[i][j][k] + gamma * grid3d[i][j][k+1];
            }
            else {
                if (k == grid_size - 1)
                    result =  (1-beta) * grid3d[i][j][k] + beta* grid3d[i][j+1][k];
                else
                    result =  ( (1 - beta) * ((1 - gamma) * grid3d[i][j][k] + gamma * grid3d[i][j][k+1])
                        + beta * ((1 - gamma) * grid3d[i][j+1][k] + gamma * grid3d[i][j+1][k+1]));
            }
        }
        else if (j == grid_size - 1) {
            if (k == grid_size - 1) {
                result =  (1-alpha) * grid3d[i][j][k] + alpha * grid3d[i+1][j][k];
            }
            else
                result =  ( (1 - alpha) * ((1 - gamma) * grid3d[i][j][k] + gamma * grid3d[i][j][k+1])
                        + alpha * ((1 - gamma) * grid3d[i+1][j][k] + gamma * grid3d[i+1][j][k+1]));
        }
        else if (k == grid_size - 1)
            result =  ( (1 - alpha) * ((1 - beta) * grid3d[i][j][k] + beta * grid3d[i][j+1][k])
                    + alpha * ((1 - beta) * grid3d[i+1][j][k] + beta* grid3d[i+1][j+1][k]));
        else
            result =  (1 - alpha) * ( (1 - beta) * ((1 - gamma) * grid3d[i][j][k] + gamma * grid3d[i][j][k+1])
                + beta * ((1 - gamma) * grid3d[i][j+1][k] + gamma * grid3d[i][j+1][k+1]))
                + alpha * ( (1 - beta) * ((1 - gamma) * grid3d[i+1][j][k] + gamma * grid3d[i+1][j][k+1])
                + beta * ((1 - gamma) * grid3d[i+1][j+1][k] + gamma * grid3d[i+1][j+1][k+1]));
        return point3d(clamp(result[0], 0, 1), clamp(result[1], 0, 1), clamp(result[2], 0, 1));
    }

    void updateDensity() {
        vector_field.updateVortices();
        for (unsigned int i = 0; i < grid_size; ++i) {
            for (unsigned int j = 0; j < grid_size; ++j) {
                for (unsigned int k = 0; k < grid_size; ++k) {
                    point3d sample_point = worldCoord(i,j,k) - vector_field.computeVelocity(current_time, worldCoord(i, j, k)) * dt;
                    if (mode == 0)
                        newgrid[i][j][k] = sampleAt(sample_point);
                    else
                        newgrid3d[i][j][k] = sample3d(sample_point);
                }
            }
        }

        //copy all values

        for (unsigned int i = 0; i < grid_size; ++i) {
            for (unsigned int j = 0; j < grid_size; ++j) {
                for (unsigned int k = 0; k < grid_size; ++k) {
                    if (mode == 0){
                        grid[i][j][k] = newgrid[i][j][k];
                        newgrid[i][j][k] = 0;
                    }
                    else {
                        grid3d[i][j][k] = newgrid3d[i][j][k];
                        newgrid3d[i][j][k] = point3d (0,0,0);
                    }
                }
            }
        }
//        if (mode == 1) {
//            for (unsigned int i = 0; i < grid_size; ++i) {
//                for (unsigned int j = 0; j < grid_size * 0.1; ++j) {
//                    for (unsigned int k = 0; k < grid_size; ++k) {
//                        grid3d[i][j][k] = point3d(i>grid_size / 2 ? 1 : 0, 0, i>grid_size / 2 ? 0 : 1);
//                    }
//                }
//            }
//        }
//        for (unsigned int i = 0; i < grid_size; ++i) {
//            for (unsigned int j = 0; j < grid_size * 0.1; ++j) {
//                for (unsigned int k = 0; k < grid_size; ++k) {
//                    grid[i][j][k] = 0;
//                }
//            }
//        }
//        for (unsigned int i = 0; i < grid_size * 0.5; ++i) {
//            for (unsigned int j = 0; j < grid_size * 0.1; ++j) {
//                grid[i * 2][j][(int) grid_size * 0.5] = (float) rand()/RAND_MAX;
//            }
//        }
        current_time += dt;
    }

    void computePolygon(float isovalue) {
        std::vector<point3d> polygon;
        polygon.clear();
        for (unsigned int i = 0; i < grid_size-1; ++i) {
            for (unsigned int j = 0; j < grid_size-1; ++j) {
                for (unsigned int k = 0; k < grid_size-1; ++k) {
                    point3d x = worldCoord(i,j,k);
                    IsoSurfacePolygonizer::GRIDCELL gridcell;
                    point3d vertex_order[8] = {point3d(0, 0, 0), point3d(1, 0, 0), point3d(1, 0, 1), point3d(0, 0, 1),
                                               point3d(0, 1, 0), point3d(1, 1, 0), point3d(1, 1, 1), point3d(0, 1, 1) };
                    for (int n = 0; n < 8; ++n){
                        gridcell.p[n] = x + vertex_order[n] * dx;
                        gridcell.val[n] = grid[i+vertex_order[n][0]][j+vertex_order[n][1]][k+vertex_order[n][2]];
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

    void saveVDB();
};

#endif // DENSITYFIELD_H
