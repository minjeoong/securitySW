#include <stdio.h>
#include "GF256.h"

void print_MC_matrix(byte M[4][4], const char *pName) ;

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

int main() {
    test_MixCol();
}