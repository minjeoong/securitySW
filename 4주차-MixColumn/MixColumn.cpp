#include <stdio.h>
#include "GF256.h"

typedef unsigned char byte;

void print_MC_matrix(byte M[4][4], const char *pName) ;


// C = A*B
void MC_Mat_Mul(byte A[4][4], byte B[4][4], byte C[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // C[i][j] = A[i][1]B[1][j] + A[i][2]B[2][j] + A[i][3]B[3][j] + A[i][4]B[4]
            C[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                // C[i][j] = C[i][j] + A[i][k] * B[k][j];
                C[i][j] ^= GF256_mul(A[i][k], B[k][j]);
            }
        }
    }
}


void MixCol (byte in[4], byte out[4]) {
    byte MC[4][4] = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x02}
    };
    byte temp;

    for (int i = 0; i < 4; i++) {
        out[i] = 0;
        for (int j = 0; j < 4; j++) {
          // out[i] = out[i] + MC[i][j] * in[j]
            temp = GF256_mul(MC[i][j], in[j]);
            out[i] = GF256_add(out[i], temp);
        }
    }
    // print_MC_matrix(MC, "MC");
    // print_MC_matrix(MC);
}

void InvMixCol(byte in[4], byte out[4]) {
    byte InvMC[4][4] = {
        {0x0e, 0x0b, 0x0d, 0x09},
        {0x09, 0x0e, 0x0b, 0x0d},
        {0x0d, 0x09, 0x0e, 0x0b},
        {0x0b, 0x0d, 0x09, 0x0e} };

    byte temp;

    for (int i = 0; i < 4; i++) {
        out[i] = 0;
        for (int j = 0; j < 4; j++) {
            //out[i] = out[i] + MC[i][j] * in[j];
            temp = GF256_mul(InvMC[i][j], in[j]);
            out[i] = out[i] ^ temp;
        }
        // out[i] ^= GF256_mul(InvMC[i][j], in[j]);
    }
}

// MC 행렬 출력
void print_MC_matrix(byte M[4][4], const char *pName) {
    
    if (pName != NULL) printf("%s = \n", pName);
    for (int i = 0; i < 4; i++) {
            printf("[");
        for (int j = 0; j < 4; j++) {
            printf("%02x ", M[i][j]);
        }
        printf("]\n");
    }
    printf("]\n");

}

void test_MixCol() {
  byte b[4], d[4], bb[4];
  // [d4, bf, 5d, 30] ---> [04, 66, 81, e5]
  b[0] = 0xd4; b[1] = 0xbf; b[2] = 0x5d; b[3] = 0x30;

  MixCol(b, d); // d = MixCol(b)
  InvMixCol(d, bb); // bb = InvMixCol(d)
  printf("b = ");
  for (int i = 0; i < 4; i++) printf("%02x ", b[i]);
  printf("\nd = ");
  for (int i = 0; i < 4; i++) printf("%02x ", d[i]);
  printf("\n");
  printf("[d4, bf, 5d, 30] ---> [04, 66, 81, e5]\n");
  printf("bb = ");
  for (int i = 0; i < 4; i++) printf("%02x ", bb[i]);
  printf("\n ");
}

void test_Mat_Mul() {
    byte A[4][4] = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x02}
    };
    byte B[4][4] = {
        {0x87, 0x6e, 0x46, 0xa6},
        {0xf2, 0x4c, 0xe4, 0x6c},
        {0x4d, 0x6e, 0x27, 0x4c},
        {0xe5, 0x5f, 0x4b, 0x7b}
    };
    byte C[4][4];
    MC_Mat_Mul(A, B, C);
    print_MC_matrix(A, "MC");
    print_MC_matrix(B, "InvMC");
    print_MC_matrix(C, "MC * InvMC"); // 얘가 단위행렬이면 성공
}

// 제곱, 세제곱, 네제곱
void test_MC_power() {
    byte MC[4][4] = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x02}
    };
    byte MC2[4][4], MC3[4][4], MC4[4][4];

    MC_Mat_Mul(MC, MC, MC2);
    MC_Mat_Mul(MC2, MC, MC3);
    MC_Mat_Mul(MC3, MC, MC4);

    print_MC_matrix(MC, "MC");
    print_MC_matrix(MC2, "MC^2");
    print_MC_matrix(MC3, "MC^3");  // 얘가 MC 의 inv 와 같다
    print_MC_matrix(MC4, "MC^4");  // 단위행렬

}

int main() {
    // test_MixCol();
    // test_Mat_Mul();
    test_MC_power();
}

