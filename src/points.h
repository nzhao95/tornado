#ifndef POINTS_H
#define POINTS_H

#include <vector>
#include "point3.h"


class Points{

public :
    std::vector< point3d > positions;
    int size;

    void clear() {
        positions.clear();
        size = 0;

    }
    void add(point3d p) {
        positions.push_back(p);
        size += 1;
    }

    void make3d () {
        point3d bb(FLT_MAX,FLT_MAX,FLT_MAX) , BB(-FLT_MAX,-FLT_MAX,-FLT_MAX);
        for( unsigned int n = 0 ; n < positions.size() ; ++n ) {
            bb = point3d::min(bb , positions[n]);
            BB = point3d::max(BB , positions[n]);
        }
        float radius = (BB[0]-bb[0])*0.5f;
        bool sign = positions[0][0] < BB[0] - radius;
        for( unsigned int n = 0 ; n < positions.size() ; ++n ) {
            if (n > 1) {
                sign = positions[n][0] > positions[n-1][0];
            }
            positions[n][2] = sqrt(pow(radius, 2) - pow(positions[n][0], 2)) * (sign ? 1 : -1);
        }
    }

    point3d deCasteljau(unsigned int n, float u) {
        std::vector< point3d > q;
        q.clear();
        if (n < positions.size() - 3) {
            q.push_back(positions[n]); q.push_back(positions[n+1]); q.push_back(positions[n+2]); q.push_back(positions[n+3]);
        }
        else if (n == positions.size() - 3){
            q.push_back(positions[n]); q.push_back(positions[n+1]); q.push_back(positions[n+2]);
        }
        else if (n == positions.size() - 2){
            q.push_back(positions[n]); q.push_back(positions[n+1]);
        }
        else {
            return positions[n];
        }
        for (unsigned int k = 1; k < q.size(); ++k) {
            for (unsigned int i = 0; i < q.size()-k; ++i){
                q[i] = (1-u)*q[i]+u*q[i+1];
            }
        }
        return q[0];
    }
};



#endif // POINTS_H
