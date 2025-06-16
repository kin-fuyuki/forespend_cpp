#include "mat.h"

inline void add(float A[2][2], float B[2][2], float C[2][2], bool subtract = false) {
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            C[i][j] = A[i][j] + (subtract ? -B[i][j] : B[i][j]);
}

inline void strassen2x2(float A[2][2], float B[2][2], float C[2][2]) {
    float M1 = (A[0][0] + A[1][1]) * (B[0][0] + B[1][1]);
    float M2 = (A[1][0] + A[1][1]) * B[0][0];
    float M3 = A[0][0] * (B[0][1] - B[1][1]);
    float M4 = A[1][1] * (B[1][0] - B[0][0]);
    float M5 = (A[0][0] + A[0][1]) * B[1][1];
    float M6 = (A[1][0] - A[0][0]) * (B[0][0] + B[0][1]);
    float M7 = (A[0][1] - A[1][1]) * (B[1][0] + B[1][1]);

    C[0][0] = M1 + M4 - M5 + M7;
    C[0][1] = M3 + M5;
    C[1][0] = M2 + M4;
    C[1][1] = M1 - M2 + M3 + M6;
}

 stdmat4x strassen4x4(float A[4][4], float B[4][4]) {
    // Divide into 2x2 blocks
    float C[4][4];
    float A11[2][2], A12[2][2], A21[2][2], A22[2][2];
    float B11[2][2], B12[2][2], B21[2][2], B22[2][2];
    float C11[2][2], C12[2][2], C21[2][2], C22[2][2];

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j+2];
            A21[i][j] = A[i+2][j];
            A22[i][j] = A[i+2][j+2];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j+2];
            B21[i][j] = B[i+2][j];
            B22[i][j] = B[i+2][j+2];
        }

    // Compute 7 products
    float M1[2][2], M2[2][2], M3[2][2], M4[2][2], M5[2][2], M6[2][2], M7[2][2];

    float T1[2][2], T2[2][2];

    add(A11, A22, T1);
    add(B11, B22, T2);
    strassen2x2(T1, T2, M1);

    add(A21, A22, T1);
    strassen2x2(T1, B11, M2);

    add(B12, B22, T2, true);
    strassen2x2(A11, T2, M3);

    add(B21, B11, T2, true);
    strassen2x2(A22, T2, M4);

    add(A11, A12, T1);
    strassen2x2(T1, B22, M5);

    add(A21, A11, T1, true);
    add(B11, B12, T2);
    strassen2x2(T1, T2, M6);

    add(A12, A22, T1, true);
    add(B21, B22, T2);
    strassen2x2(T1, T2, M7);

    // Combine results
    for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j) {
        C11[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
        C12[i][j] = M3[i][j] + M5[i][j];
        C21[i][j] = M2[i][j] + M4[i][j];
        C22[i][j] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
    }

    // Reassemble final 4x4 matrix
    for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j) {
        C[i][j]       = C11[i][j];
        C[i][j+2]     = C12[i][j];
        C[i+2][j]     = C21[i][j];
        C[i+2][j+2]   = C22[i][j];
    }

    return C;
}; 
 stdmat4x std4x4( const float a[4][4], const float b[4][4]) {
    float out[4][4];
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            out[row][col] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                out[row][col] += a[row][k] * b[k][col];
            }
        }
    }
    return out;
}
 stdmat4x eigen4x4( const Eigen::Matrix4f a, const Eigen::Matrix4f b){
    Eigen::Matrix4f out;
    out = a * b;
    return out;
}