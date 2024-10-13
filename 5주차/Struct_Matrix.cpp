#include <stdio.h>

#define MATRIX_MAX_SIZE 20
#define NEARLY_ZERO 1e-10 // 절대값이 이보다 작으면 0

typedef struct { // 행렬을 나타내는 구조체
    double M[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int row;
    int col;
} Matrix;

void Matrix_print(Matrix A) {
    for (int i = 0; i < A.row; i++) { // 각 행에 대하여
        printf("[");
        for (int j = 0; j < A.col; j++) {
            // 거의 0이면,
            if ((A.M[i][j] < NEARLY_ZERO) && (A.M[i][j] > -NEARLY_ZERO)) {
                printf("%7.2f", 0.0);
            } else {
                printf("%7.2f", A.M[i][j]);
            }
        }
        printf("]\n");
    }
    printf("\n");

}



Matrix Matrix_Init() {
    Matrix A;
    A.row = 1; 
    A.col = 1;
    A.M[0][0] = 0.0;

    return A;
}

// C = A + B (행렬의 덧셈)
Matrix Matrix_Add(Matrix A, Matrix B) {
    Matrix result;
    result = Matrix_Init();

    // 행렬의 규격 확인
    if ((A.col != B.col) || (A.row != B.row)) {
        printf("(Matrix_Add) Matrix size error\n");
        return result;
    }

    result.row = A.row;
    result.col = A.col; 
    for (int i = 0; i < result.row; i++)
      for (int j = 0; j < result.col; j++)
          result.M[i][j] = A.M[i][j] + B.M[i][j];

    return result;
}

// C = A * B (행렬 곱) [m,k] * [k,n] = [m,n]
Matrix Matrix_Mul(Matrix A, Matrix B) {
    Matrix AB;
    AB = Matrix_Init();

    // 행렬의 규격 확인
    if (A.col != B.row) {
        printf("(Matrix_Mul) Matrix size error\n");
        return AB;
    }

    AB.row = A.row;
    AB.col = B.col;

    for (int i = 0; i < AB.row; i++) {
        for (int j = 0; j < AB.col; j++) {
            AB.M[i][j] = 0.0;
            for (int k = 0; k < A.col; k++) {
                AB.M[i][j] += A.M[i][k] * B.M[k][j];
            }
        }
    }

    return AB;
}


// 두 행을 바꾸기
void Mat_Exchange_Row(Matrix &A, int row1, int row2) {
    double temp;
    for (int j = 0; j < A.col; j++) {
        // A[row1][j] <--> A[row2][j]
        temp = A.M[row1][j];
        A.M[row1][j] = A.M[row2][j];
        A.M[row2][j] = temp;
    }
}
// 한 행에 상수배 하기
void Mat_Scalar_Mul_Row(Matrix &A, double k, int row) {
    for (int j = 0; j < A.col; j++)
        A.M[row][j] *= k;
}

// 한 행의 상수배를 다른 행에 더하기
void Mat_Row_Add(Matrix &A, double k, int row_src, int row_target) {
    for (int j = 0; j < A.col; j++)
        A.M[row_target][j] += k * A.M[row_src][j];
}

// 역행렬 구하기
Matrix Matrix_Inverse(Matrix A) {
  Matrix InvA;

  InvA = Matrix_Init();

  if (A.row != A.col) { // 정사각행렬인지 확인
      printf("(Matrix Inverse) Non-Square Matrix err");
      return InvA;
  }

  // AA = [A|I]
  Matrix AA;

  AA.row = A.row;
  AA.col = 2 * A.col;
  for (int i = 0; i < A.row; i++) {

    for (int j = 0; j < A.col; j++) {
        AA.M[i][j] = A.M[i][j];
        AA.M[i][j + A.col] = (i == j) ? 1.0 : 0.0;
    }

  }

  Matrix_print(AA); // [A|I]

  // R-REF (Reduced Row Echelon Form) [A|I] ==> [I|A^(-1)]
  int pivot_row; // 각 열을 계산하는 단계에서 '1'이 있는 위치의 행을 기록
  for (int j = 0; j < A.col; j++) { // A의 각 열에 대하여...
      // pivot : AA[0][0], AA[1][1], AA[2][2] ...
      pivot_row = -1; // 초기값
      for (int i = j; i < A.row; i++) { // AA[j][j], AA[j+1][j] ...
          // if (AA.M[i][j] != 0.0) {
          if ((AA.M[i][j] > NEARLY_ZERO) || (AA.M[i][j] < -NEARLY_ZERO)) {
              pivot_row = i;
              break; // for-loop 밖으로
          }
      }
      
      if (pivot_row == -1) { // 해당 열의 모든 원소가 0이면,
          printf("(Matrix Inverse) Not Invertible\n");
          return InvA;
      }
      if (pivot_row != j) {
          Mat_Exchange_Row(AA, j, pivot_row);
      }

      Mat_Scalar_Mul_Row(AA, 1. / AA.M[j][j], j); // 대각원소를 1로 만들기
      for (int i = 0; i < A.row; i++) {
          if (i != j) { // A[j][j]이 포함되지 않은 행에 대해서만...
              Mat_Row_Add(AA, -AA.M[i][j], j, i);
          }
      }
      Matrix_print(AA); // 한줄이 완성될 때 출력
  }


  InvA.row = A.row;
  InvA.col = A.col;
  for (int i = 0; i < InvA.row; i++) 
      for (int j = 0; j < InvA.col; j++) 
          InvA.M[i][j] = AA.M[i][j + A.col];


  return InvA;
}  

// C = kA (행렬의 상수배)
Matrix Matrix_scalar(Matrix A, double k) {
    Matrix kA;
    kA = Matrix_Init();

    kA.row = A.row;
    kA.col = A.col;

    for (int i = 0; i < kA.row; i++)
        for (int j = 0; j < kA.col; j++)
            kA.M[i][j] = k * A.M[i][j];

    return kA;
}


void test_matrix() {
    Matrix A, B, C, D;
    A.row = 4; A.col = 4;
    for (int i = 0; i < A.row; i++)
        for (int j = 0; j < A.col; j++)
            A.M[i][j] = i + j;

    Matrix_print(A);

    int M[4][4] = { {1,0,2,0}, {1,1,0,0}, {1,2,0,1}, {1,1,1,1} };

    B.row = 4; 
    B.col = 4;

    for (int i = 0; i < B.row; i++)
        for (int j = 0; j < B.col; j++)
            B.M[i][j] = M[i][j];

    Matrix_print(B);

    C = Matrix_Add(A, B);
    Matrix_print(C);

    D = Matrix_Mul(A, B);
    Matrix_print(D);

    double k;
    k = 0.5;
    Matrix KD;
    KD = Matrix_scalar(D, k);
    Matrix_print(KD);

    printf("=== Inverse of B\n");
    Matrix InvB;
    InvB = Matrix_Inverse(B);
    Matrix_print(InvB);
}


int main() {
    test_matrix();
    
}









// Matrix Matrix_scalar(Matrix A, double k) {

//     Matrix kA;
//     kA = Matrix_Init();

//     kA.row = A.row;
//     kA.col = A.col;

//     for (int i = 0; i < kA.row; i++)
//         for (int j = 0; j < kA.col; j++)
//             kA.M[i][j] = k * A.M[i][j];

//     return kA;
// }