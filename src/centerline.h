#ifndef CENTERLINE_H
#define CENTERLINE_H

#include <vector>
#include "point3.h"


class centerLine{

public :
    std::vector<std::vector< point3d >> control_p;
    std::vector<float> times;
    unsigned int size;

    void clear() {
        control_p.clear();
        times.clear();
        size = 0;

    }
    void addPointToCurve(point3d p, int n) {
        control_p[n].push_back(p);
    }

    void addCurve (float t) {
        std::vector< point3d > newCurve;
        newCurve.clear();
        control_p.push_back(newCurve);
        times.push_back(t);
        size += 1;
    }

    void make3d () {
        for (unsigned int cIt = 0; cIt < size; ++cIt){
            point3d bb(FLT_MAX,FLT_MAX,FLT_MAX) , BB(-FLT_MAX,-FLT_MAX,-FLT_MAX);
            bool sign = control_p[cIt][1][0] > control_p[cIt][0][0];
            unsigned int start = 0;
            unsigned int end = 1;
            while (end < control_p[cIt].size() - 1) {
                while (end < control_p[cIt].size() - 1 && ((control_p[cIt][end][0] - control_p[cIt][end-1][0] > 0) == sign)) {
                    bb = point3d::min(bb , control_p[cIt][end]);
                    BB = point3d::max(BB , control_p[cIt][end]);
                    end += 1;
                }
                float mid = (BB[0]+bb[0])*0.5f;
                float radius = (BB[0]-bb[0])*0.5f;
                for(unsigned int n = start ; n < end + 1 ; ++n ) {
                    control_p[cIt][n][2] = sqrt(abs(pow(radius, 2) - pow(control_p[cIt][n][0] - mid, 2))) * (sign ? 1 : -1);
                }
                start = end -1;
                sign = !sign;
                bb = point3d(FLT_MAX,FLT_MAX,FLT_MAX);
                BB = point3d(-FLT_MAX,-FLT_MAX,-FLT_MAX);
            }
        }
    }

    void filter () {
//        std::vector< point3d > new_control_p;
//        new_control_p.clear();
//        int last_selected = 0;
//        new_control_p.push_back(control_p[0]);
//        for (unsigned int i = 1; i<control_p[n].size()-1 ; ++i){
//            if ((control_p[i][0] - control_p[i-1][0])*(control_p[i+1][0] - control_p[i][0]) < 0 || last_selected > 1) {
//                new_control_p.push_back(control_p[i]);
//                last_selected = 0;
//            }
//            else {
//                last_selected += 1;
//            }
//        }
//        control_p = new_control_p;
    }

};



#endif // CENTERLINE_H
