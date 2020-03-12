#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H


#include <vector>


template< class val_t >
struct bezierCurve{
    std::vector< val_t > control_p;

    // may be useful someday...
    static double Bernstein(int i, int n, double u){
        double temp[n+1];

        for (int j = 0; j <= n; ++j) {
            temp[j]=0.0;
        }

        temp[n-i]=1.0;
        double u1=1.0-u;

        for (int k = 1; k <= n; ++k) {
            for (int j = n; j >=k; --j) {
                temp[j]=u1*temp[j]+u*temp[j-1];
            }
        }

        return temp[n];
    }

    // based on de Casteljau :
    val_t getValue( double t ) {
        unsigned int n = control_p.size();
        val_t d[n];
        for( unsigned int r = 0 ; r < n ; ++r )
            d[r] = control_p[r];

        for( unsigned int r = 0 ; r < n-1 ; ++r ){
            for( unsigned int j = 0 ; j < n-1-r  ; ++j ) {
                d[j] = (1.0 - t) * d[j] + t * d[j+1];
            }
        }

        return d[0];
    }

    val_t getDerivative( double t ) {
        unsigned int n = control_p.size()-1;
        val_t d[n];
        for( unsigned int r = 0 ; r < n ; ++r )
            d[r] = control_p[r+1] - control_p[r];

        for( unsigned int r = 0 ; r < n-1 ; ++r ){
            for( unsigned int j = 0 ; j < n-1-r  ; ++j ) {
                d[j] = (1.0 - t) * d[j] + t * d[j+1];
            }
        }

        return (double)((n+1))*d[0];
    }

    val_t getSecondDerivative( double t ) {
        unsigned int n = control_p.size()-1;
        val_t d[n];
        for( unsigned int r = 0 ; r < n ; ++r )
            d[r] = control_p[r+1] - control_p[r];

        --n;
        for( unsigned int r = 0 ; r < n ; ++r )
            d[r] = d[r+1] - d[r];

        for( unsigned int r = 0 ; r < n-1 ; ++r ){
            for( unsigned int j = 0 ; j < n-1-r  ; ++j ) {
                d[j] = (1.0 - t) * d[j] + t * d[j+1];
            }
        }

        return (double)((n+1)*(n+2))*d[0];
    }
};



#endif // BEZIERCURVE_H
