#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include "point3.h"

class Noise {

public :

    std::vector< std::vector< std::vector< point3d > > > gradient;
    unsigned int grid_size;
    float dx = 1;
    point3d grid_bl;

    float scale = 0.05;


    void clear () {
        gradient.clear();
    }

    // Function to linearly interpolate between a0 and a1
    // Weight w should be in the range [0.0, 1.0]
    point3d lerp(point3d a0, point3d a1, float w) {
        return (1.0 - w)*a0 + w*a1;
    }

    float clamp(const float v, const float vmin, const float vmax) {
      if(v<vmin) return vmin;
      if(v>vmax) return vmax;
      return v;
    }

    point3d clamp(const point3d v, const float vmin, const float vmax) {
        return point3d(clamp(v[0],vmin,vmax), clamp(v[1],vmin,vmax), clamp(v[2],vmin,vmax));
    }

    void init(int size, point3d bl) {
        gradient.clear();
        grid_size = size;
        grid_bl = bl;
        gradient.resize(grid_size);
        for (unsigned int i = 0; i < grid_size; ++i) {
            gradient[i].resize(grid_size);
            for (unsigned int j = 0; j < grid_size; ++j) {
                gradient[i][j].resize(grid_size, point3d(0,0,0));
            }
        }

        for (unsigned int i = 0; i < grid_size; ++i) {
            for (unsigned int j = 0; j < grid_size; ++j) {
                for (unsigned int k = 0; k < grid_size; ++k) {
                    gradient[i][j][k] = point3d((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX);
                }
            }
        }
    }

    void update() {
        for (unsigned int i = 0; i < grid_size; ++i) {
            for (unsigned int j = 0; j < grid_size; ++j) {
                for (unsigned int k = 0; k < grid_size; ++k) {
                    gradient[i][j][k] += clamp((point3d(0.5,0.5,0.5) - point3d((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX)) * 0.1, 0, 1);
                }
            }
        }
    }


    point3d gridCoord ( point3d x) {
        point3d coord = (x - grid_bl) / dx;
        return coord;
    }

    point3d sample3d(point3d x) {
        point3d coord = gridCoord(x);
        unsigned int i = clamp(static_cast<int>(coord[0]), 0, grid_size - 1);
        unsigned int j = clamp(static_cast<int>(coord[1]), 0, grid_size - 1);
        unsigned int k = clamp(static_cast<int>(coord[2]), 0, grid_size - 1);
        float alpha = coord[0] - i;
        float beta = coord[1] - j;
        float gamma = coord[2] - k;
        if (i == grid_size - 1) {
            if (j == grid_size -1) {
                if (k == grid_size - 1)
                    return  gradient[i][j][k];
                else
                    return  lerp(gradient[i][j][k], gradient[i][j][k+1], gamma);
            }
            else {
                if (k == grid_size - 1)
                    return  lerp(gradient[i][j][k], gradient[i][j+1][k], beta);
                else
                    return  lerp(
                                    lerp(gradient[i][j][k], gradient[i][j][k+1], gamma),
                                    lerp(gradient[i][j+1][k], gradient[i][j+1][k+1], gamma),
                                beta);
            }
        }
        else if (j == grid_size - 1) {
            if (k == grid_size - 1) {
                return  lerp(gradient[i][j][k], gradient[i+1][j][k], alpha);
            }
            else
                return  lerp(
                            lerp(gradient[i][j][k], gradient[i][j][k+1], gamma),
                            lerp(gradient[i+1][j][k], gradient[i+1][j][k+1], gamma),
                        alpha);
        }
        else if (k == grid_size - 1)
            return  lerp(
                        lerp(gradient[i][j][k], gradient[i][j+1][k], beta),
                        lerp(gradient[i+1][j][k], gradient[i+1][j+1][k], beta),
                    alpha);
        else
            return  lerp(
                        lerp(
                            lerp(gradient[i][j][k], gradient[i][j][k+1], gamma),
                            lerp(gradient[i][j+1][k], gradient[i][j+1][k+1], gamma)
                            , beta),
                        lerp(
                            lerp(gradient[i+1][j][k], gradient[i+1][j][k+1], gamma),
                            lerp(gradient[i+1][j+1][k], gradient[i+1][j+1][k+1], gamma)
                            ,beta),
                        alpha);

    }

    point3d curl(point3d x) {
        float d = 1e-4;
        point3d phi = sample3d(x);
        point3d dphi_dx = (phi-sample3d(x + point3d(d,0,0)))/d;
        point3d dphi_dy = (phi-sample3d(x + point3d(0,d,0)))/d;
        point3d dphi_dz = (phi-sample3d(x + point3d(0,0,d)))/d;

        return point3d(dphi_dy[2] - dphi_dz[1], dphi_dz[0] - dphi_dx[2], dphi_dx[1] - dphi_dy[0]) * scale;

    }

};
#endif // NOISE_H
