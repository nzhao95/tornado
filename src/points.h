#ifndef POINTS_H
#define POINTS_H

#include <vector>
#include "point3.h"


class Points{

public :
    std::vector< point3d > positions;
    unsigned int size;

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
        bool sign = positions[1][0] > positions[0][0];
        unsigned int start = 0;
        unsigned int end = 1;
        while (end < size - 1) {
            while (end < size - 1 && (positions[end][0] - positions[end-1][0] > 0 == sign)) {
                bb = point3d::min(bb , positions[end]);
                BB = point3d::max(BB , positions[end]);
                end += 1;
            }
            float mid = (BB[0]+bb[0])*0.5f;
            float radius = (BB[0]-bb[0])*0.5f;
            for( int n = start ; n < end + 1 ; ++n ) {
                positions[n][2] = sqrt(abs(pow(radius, 2) - pow(positions[n][0] - mid, 2))) * (sign ? 1 : -1);
            }
            start = end -1;
            sign = !sign;
            bb = point3d(FLT_MAX,FLT_MAX,FLT_MAX);
            BB = point3d(-FLT_MAX,-FLT_MAX,-FLT_MAX);
        }
    }

    void filter () {
        std::vector< point3d > new_positions;
        new_positions.clear();
        int last_selected = 0;
        new_positions.push_back(positions[0]);
        for (int i = 1; i<size-1 ; ++i){
            if ((positions[i][0] - positions[i-1][0])*(positions[i+1][0] - positions[i][0]) < 0 || last_selected > 1) {
                new_positions.push_back(positions[i]);
                last_selected = 0;
            }
            else {
                last_selected += 1;
            }
        }
        positions = new_positions;
        size = new_positions.size();
    }

    point3d deCasteljau(float t) {
        unsigned int n = positions.size();
        point3d d[n];
        for( unsigned int r = 0 ; r < n ; ++r )
            d[r] = positions[r];

        for( unsigned int r = 0 ; r < n-1 ; ++r ){
            for( unsigned int j = 0 ; j < n-1-r  ; ++j ) {
                d[j] = (1.0 - t) * d[j] + t * d[j+1];
            }
        }

        return d[0];
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
