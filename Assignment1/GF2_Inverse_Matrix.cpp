#include <stdio.h>

#define MATRIX_SIZE 8

// 유한체 GF(2) 의 원소 a,b 를 더하기
int gf2_add(int a, int b) {
    return a ^ b; 
}

// GF(2)의 곱셈
int gf2_mul(int a, int b) {
    return a & b; 
}

// 행렬 출력
void Matrix_print(int A[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("[");
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", A[i][j % MATRIX_SIZE]);
        }
        printf("]\n");
    }
    printf("\n");
}

// GF(2)에서의 역행렬을 구하는 함수
void gf2_inverse(int A[MATRIX_SIZE][MATRIX_SIZE], int Inv[MATRIX_SIZE][MATRIX_SIZE]) {
    int AA[MATRIX_SIZE][MATRIX_SIZE * 2];
    
    // 초기화 [A|I]
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            AA[i][j] = A[i][j];
            AA[i][j + MATRIX_SIZE] = (i == j) ? 1 : 0; 
        }
    }

    
    // 가우스-조던 소거법 적용
    for (int i = 0; i < MATRIX_SIZE; i++) {
        // 피벗 찾기
        if (AA[i][i] == 0) {
            // 피벗이 0이면 아래에서 1인 행 찾기
            for (int j = i + 1; j < MATRIX_SIZE; j++) {
                if (AA[j][i] == 1) {
                    // 행 교환
                    for (int k = 0; k < 2 * MATRIX_SIZE; k++) {
                        int temp = AA[i][k];
                        AA[i][k] = AA[j][k];
                        AA[j][k] = temp;
                    }
                    break;
                }
            }
        }

        // 피벗이 1인 경우, 다른 행들에 대해 해당 열을 0으로 만듦
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (j != i && AA[j][i] == 1) {
                // 행 j에서 행 i를 빼기 (GF(2)에서 덧셈 --> XOR)
                for (int k = 0; k < 2 * MATRIX_SIZE; k++) {
                    AA[j][k] = gf2_add(AA[j][k], AA[i][k]);
                }
            }
        }

    }

    // 역행렬 복사 [A^(-1)]
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            Inv[i][j] = AA[i][j + MATRIX_SIZE];
        }
    }
}



int main() {
    int A[MATRIX_SIZE][MATRIX_SIZE] = {
        {1, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 1}
    };

    int Inv[MATRIX_SIZE][MATRIX_SIZE];

    gf2_inverse(A, Inv);

    printf("A:\n");
    Matrix_print(A);

    printf(" A^(-1):\n");
    Matrix_print(Inv);

    return 0;
}





