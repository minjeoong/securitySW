#include <stdio.h>

typedef unsigned char byte; // GF(2^8) elements

#define MATRIX_SIZE 4

// ====================== 3번 문제 ============================= //

// 유한체 GF(2^8) 의 원소 a,b 를 더하기
byte GF256_add(byte a, byte b) {
    return a ^ b;
}
byte GF256_xtime_simple(byte a){
  return (((a >> 7) & 0x01) == 0) ? (a << 1) : ((a << 1) ^ 0x1b);
}

// GF(2^8)의 (다항식) 곱셈
byte GF256_mul(byte f, byte g) {
  byte h; // 곱셈 결과 저장
  int coef; // 계수

  h = 0x00; // 초기값 h(x) = 0
  for (int i = 7; i >= 0; i--) { // a7, a6, a5, ... ,a1, a0 순서로
      coef = (f >> i) & 0x01; // a7, a6, a5, ... ,a1, a0
      // h = GF256_xtime(h);
      h = GF256_xtime_simple(h);
      if (coef == 1) { // 계수 ai가 1인 경우만 g(x)를 h(x)에 더한다 (+ 아님. xor)
          h = GF256_add(h, g);
      }
  }

  return h;
}

// GF*(2^8)의 역원을 계산하기
byte GF256_inv(byte x) { 


    byte x_inv, term;
    term = x;

    if (x == 0) {
        return 0x00; // 0의 역원은 0
    }

    x_inv = 1; // 곱하기에 대한 초기값
    for (int i = 0; i < 7; i++) { // x^2, x^3, ... , x^7 (1111 1110 에서 1인 것까지만)
        term = GF256_mul(term, term);  // x^2, x^4, x^8, x^16, x^32, x^64, x^128
        x_inv = GF256_mul(x_inv, term); // (1), x^2, x^2*x^4, (x^2*x^4)*x^8, (x^2*x^4*x^8)*x^16, (x^2*x^4*x^8*x^16)*x^32, (x^2*x^4*x^8*x^16*x^32)*x^64, (x^2*x^4*x^8*x^16*x^32*x^64)*x^128
    }
    return x_inv;
}

// 매트리스 출력 함수
void print_augmented_matrix(byte AA[MATRIX_SIZE][2 * MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("[ ");
        for (int j = 0; j < 2 * MATRIX_SIZE; j++) {
            printf("%02x ", AA[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
}

// GF(2^8)에서 행렬의 역행렬을 구하는 함수
void GF256_Matrix_Inverse(byte A[MATRIX_SIZE][MATRIX_SIZE], byte Inv[MATRIX_SIZE][MATRIX_SIZE]) {
    byte AA[MATRIX_SIZE][2 * MATRIX_SIZE]; 

    // 확장된 행렬 [A | I] 초기화
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            AA[i][j] = A[i][j];           // 왼쪽 절반은 A 행렬
            AA[i][j + MATRIX_SIZE] = (i == j) ? 0x01 : 0x00; // 오른쪽 절반은 단위행렬 I
        }
    }

   printf("[A | I]:\n");
    print_augmented_matrix(AA);


    // 가우스-조던 소거법으로 역행렬 계산
    for (int j = 0; j < MATRIX_SIZE; j++) {
        // 원소가 0이면 행 교환
        if (AA[j][j] == 0x00) {
            for (int i = j + 1; i < MATRIX_SIZE; i++) {
                if (AA[i][j] != 0x00) { // 0이 아닌 원소를 찾음
                    // 행을 교환하여 주대각선 원소가 0이 아닌 값으로 만듦
                    for (int k = 0; k < 2 * MATRIX_SIZE; k++) {
                        byte temp = AA[j][k];
                        AA[j][k] = AA[i][k];
                        AA[i][k] = temp;
                    }
                    break;
                }
            }
        }

        // 주대각선 원소의 역원을 계산하여 해당 행의 모든 원소에 곱함
        byte pivot_inv = GF256_inv(AA[j][j]);
        for (int k = 0; k < 2 * MATRIX_SIZE; k++) {
            AA[j][k] = GF256_mul(AA[j][k], pivot_inv);
        }

        // 다른 행들의 해당 열을 0으로 만들기 위해 가우스 소거법 적용
        for (int i = 0; i < MATRIX_SIZE; i++) {
            if (i != j) { // 주대각선이 아닌 다른 행에 대해
                byte factor = AA[i][j];
                for (int k = 0; k < 2 * MATRIX_SIZE; k++) {
                    AA[i][k] = GF256_add(AA[i][k], GF256_mul(factor, AA[j][k]));
                }
            }
        }
        printf("step %d:\n", j + 1);
        print_augmented_matrix(AA);
    }

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            Inv[i][j] = AA[i][j + MATRIX_SIZE];
        }
    }
}

void test_inverse() {
    byte A[MATRIX_SIZE][MATRIX_SIZE] = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x02}
    };

    byte Inv[MATRIX_SIZE][MATRIX_SIZE];

    GF256_Matrix_Inverse(A, Inv);

    printf("Inverse A:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%02x ", Inv[i][j]);
        }
        printf("\n");
    }
}

int main() {
    test_inverse();
    return 0;
}

