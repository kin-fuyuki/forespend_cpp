
#include <iostream>
#include <vector>
#include <complex>
#include <iomanip>
#include<Eigen/Dense>
#include <Eigen/Geometry>
#define complex_num const std::complex<double>
using ComplexMatrix4x4 = std::vector<std::vector<std::complex<double>>>;

struct stdmat4x
{
    float v[4][4];
    stdmat4x() {for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) v[i][j] = 0;}
    stdmat4x(float a[4][4]) {for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) v[i][j] = a[i][j];}
    stdmat4x(ComplexMatrix4x4 A) {for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) v[i][j] = A[i][j].real();}
    stdmat4x(Eigen::Matrix4f A) {for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) v[i][j] = A(i,j);}
    operator float*() {return &v[0][0];}
    stdmat4x operator*(const stdmat4x r) const{
        stdmat4x result;
        result.v[0][0]=v[0][0]*r.v[0][0]+v[0][1]*r.v[1][0]+v[0][2]*r.v[2][0]+v[0][3]*r.v[3][0];
        result.v[0][1]=v[0][0]*r.v[0][1]+v[0][1]*r.v[1][1]+v[0][2]*r.v[2][1]+v[0][3]*r.v[3][1];
        result.v[0][2]=v[0][0]*r.v[0][2]+v[0][1]*r.v[1][2]+v[0][2]*r.v[2][2]+v[0][3]*r.v[3][2];
        result.v[0][3]=v[0][0]*r.v[0][3]+v[0][1]*r.v[1][3]+v[0][2]*r.v[2][3]+v[0][3]*r.v[3][3];
        result.v[1][0]=v[1][0]*r.v[0][0]+v[1][1]*r.v[1][0]+v[1][2]*r.v[2][0]+v[1][3]*r.v[3][0];
        result.v[1][1]=v[1][0]*r.v[0][1]+v[1][1]*r.v[1][1]+v[1][2]*r.v[2][1]+v[1][3]*r.v[3][1];
        result.v[1][2]=v[1][0]*r.v[0][2]+v[1][1]*r.v[1][2]+v[1][2]*r.v[2][2]+v[1][3]*r.v[3][2];
        result.v[1][3]=v[1][0]*r.v[0][3]+v[1][1]*r.v[1][3]+v[1][2]*r.v[2][3]+v[1][3]*r.v[3][3];
        result.v[2][0]=v[2][0]*r.v[0][0]+v[2][1]*r.v[1][0]+v[2][2]*r.v[2][0]+v[2][3]*r.v[3][0];
        result.v[2][1]=v[2][0]*r.v[0][1]+v[2][1]*r.v[1][1]+v[2][2]*r.v[2][1]+v[2][3]*r.v[3][1];
        result.v[2][2]=v[2][0]*r.v[0][2]+v[2][1]*r.v[1][2]+v[2][2]*r.v[2][2]+v[2][3]*r.v[3][2];
        result.v[2][3]=v[2][0]*r.v[0][3]+v[2][1]*r.v[1][3]+v[2][2]*r.v[2][3]+v[2][3]*r.v[3][3];
        result.v[3][0]=v[3][0]*r.v[0][0]+v[3][1]*r.v[1][0]+v[3][2]*r.v[2][0]+v[3][3]*r.v[3][0];
        result.v[3][1]=v[3][0]*r.v[0][1]+v[3][1]*r.v[1][1]+v[3][2]*r.v[2][1]+v[3][3]*r.v[3][1];
        result.v[3][2]=v[3][0]*r.v[0][2]+v[3][1]*r.v[1][2]+v[3][2]*r.v[2][2]+v[3][3]*r.v[3][2];
        result.v[3][3]=v[3][0]*r.v[0][3]+v[3][1]*r.v[1][3]+v[3][2]*r.v[2][3]+v[3][3]*r.v[3][3];
        return result;
    }
};

stdmat4x strassen4x4( const float a[4][4], const float b[4][4]);
stdmat4x std4x4( const float a[4][4], const float b[4][4]);
stdmat4x eigen4x4( const Eigen::Matrix4f a, const Eigen::Matrix4f b);
