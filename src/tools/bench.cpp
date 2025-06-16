#include <iostream>
#include <Eigen/Dense>
#include <x86intrin.h> // for __rdtsc
#include"mat.h"


stdmat4x mat4x4_mul_sse(const stdmat4x& a, const stdmat4x& b) {
    stdmat4x result;
    
    for (int i = 0; i < 4; i++) {
        // Load a's row i into an __m128 vector
        __m128 a_line = _mm_loadu_ps(&a.v[i][0]);

        for (int j = 0; j < 4; j++) {
            // Load column j of b into 4 floats manually
            __m128 b_col = _mm_set_ps(b.v[3][j], b.v[2][j], b.v[1][j], b.v[0][j]);

            // Multiply elementwise
            __m128 mul = _mm_mul_ps(a_line, b_col);

            // Horizontal add to get sum of 4 floats
            __m128 shuf1 = _mm_movehdup_ps(mul);  // (mul3, mul3, mul1, mul1)
            __m128 sum1 = _mm_add_ps(mul, shuf1);
            __m128 shuf2 = _mm_movehl_ps(shuf1, sum1);
            __m128 sum2 = _mm_add_ss(sum1, shuf2);

            // Store lowest float as result[i][j]
            result.v[i][j] = _mm_cvtss_f32(sum2);
        }
    }
    return result;
}








int main() {
unsigned int smalleststrassen=-1;
unsigned int smalleststd=-1;
unsigned int smalleststd2=-1;
unsigned int smalleststdsse=-1;
unsigned int smallesteigen=-1;


    Eigen::Matrix4f A_eigen;A_eigen <<1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16;
    Eigen::Matrix4f B_eigen;B_eigen <<1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16;
    float a[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    float b[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    stdmat4x A;
    A.v[0][0]=1;A.v[0][1]=2;A.v[0][2]=3;A.v[0][3]=4;
    A.v[1][0]=5;A.v[1][1]=6;A.v[1][2]=7;A.v[1][3]=8;
    A.v[2][0]=9;A.v[2][1]=10;A.v[2][2]=11;A.v[2][3]=12;
    A.v[3][0]=13;A.v[3][1]=14;A.v[3][2]=15;A.v[3][3]=16;
    stdmat4x B;
    B.v[0][0]=1;B.v[0][1]=2;B.v[0][2]=3;B.v[0][3]=4;
    B.v[1][0]=5;B.v[1][1]=6;B.v[1][2]=7;B.v[1][3]=8;
    B.v[2][0]=9;B.v[2][1]=10;B.v[2][2]=11;B.v[2][3]=12;
    
unsigned long long start = __rdtsc();
unsigned long long end = __rdtsc();
stdmat4x c;

for (int i=0;i<100000;i++){
/*
start = __rdtsc();
    c=strassen4x4(a,b);
end = __rdtsc();
if (smalleststrassen>=end-start)smalleststrassen=end-start;*/
start = __rdtsc();
    c=std4x4(a,b);
end = __rdtsc();
if (smalleststd>=end-start)smalleststd=end-start;
start = __rdtsc();
    c=eigen4x4(A_eigen,B_eigen);
end = __rdtsc();
if (smallesteigen>=end-start)smallesteigen=end-start;
start = __rdtsc();
    c=A*B;
end = __rdtsc();
if (smalleststd2>=end-start)smalleststd2=end-start;
start = __rdtsc();
    c=mat4x4_mul_sse(a,b);
end = __rdtsc();
if (smalleststdsse>=end-start)smalleststdsse=end-start;
}


std::cout<<"strassen_4x4: "<<smalleststrassen<<std::endl;
std::cout<<"std_sse_4x4: "<<smalleststdsse<<std::endl;
std::cout<<"std_4x4: "<<smalleststd<<std::endl;
std::cout<<"std2_4x4: "<<smalleststd2<<std::endl;
std::cout<<"eigen_4x4: "<<smallesteigen<<std::endl;

return 0;
}
