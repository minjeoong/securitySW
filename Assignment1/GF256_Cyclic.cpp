#include <stdio.h>

typedef unsigned char byte; // GF(2^8) elements
#define TABLE_SIZE 256


byte GF256_xtime_simple(byte a){
  return (((a >> 7) & 0x01) == 0) ? (a << 1) : ((a << 1) ^ 0x1b);
}

// 유한체 GF(2^8) 의 원소 a,b 를 더하기
byte GF256_add(byte a, byte b) {
    return a ^ b;
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

// GF(2^8)의 원소의 위수(order) 구하기
int GF256_ord(byte x) {
    byte temp;
    int ord;
    if (x == 0) return -1; // 0은 곱셈군의 원소가 아님. 의미없음.

    temp = x;
    ord = 1;

    while (temp != 0x01) {
        temp = GF256_mul(temp, x);
        ord++;
    }
    return ord;
}



// ==================== 5 - (a) ======================== //


// GF(2^8)의 원소의 위수(order) 테스트
void test_order() {
    int ord;
    int count_full_order;
    byte generators[256];

    count_full_order = 0;

    for(int i = 0; i < 256; i++){
        ord = GF256_ord(i);
        printf("ord(%02x) = %d\n", i, ord);
        if( ord == 255){
            generators[count_full_order++] = (byte)i;
        }
    }

    // ord(x) = 255 인 x의 개수
    // printf("The number of x s.t. ord(x) = 255 is %d\n", count_full_order);
    printf("\n생성자가 될 수 있는 x 의 개수: %d\n", count_full_order);
    printf("\n");

}
// =================================================== //




// ==================== 5 - (b) ======================== //


void find_generators() {
    int order;
    int generator_count = 0;
    byte generators[256]; // 생성자 배열

    printf("Order of elements in GF(2^8):\n");
    for (int x = 1; x < 256; x++) {  
        order = GF256_ord((byte)x); 

        // 원소가 생성자이면 (위수가 255이면)
        if (order == 255) {
            generators[generator_count++] = (byte)x;
        }
    }

    printf("\n생성자가 될 수 있는 x 의 개수: %d\n", generator_count);

    printf("\n");
}

// 생성자의 거듭제곱 출력 함수
void print_generator_powers(byte alpha) {
    byte result = 0x01; // α^0 = 1
    printf("\nPowers of generator 0x%02x:\n", alpha);
    printf("α^0 = 0x%02x\n", result);

    // 테이블을 만드.....는 건 
    // for (int i = 1; i < 255; i++) {
    //     printf("α^%d ", i);
    // }

    for (int i = 1; i < 255; i++) {
        result = GF256_mul(result, alpha);
        printf("α^%d = 0x%02x  ", i, result);
        if (i % 10 == 0) {
            printf("\n");
        }
    }
}

// 5번  - (a), (b) 문제 print
void test_generators() {
    test_order(); // (a) 문제 출력

    // 생성자 0x03
    print_generator_powers(0x03);      // 이거 주석처리 하면 -> 다른 생성자에 대한 출력만 나옴.
    
    // 다른 생성자
    print_generator_powers(0x05);
}
// =================================================== //




// ==================== 5 - (d) ======================== //

// 지수 테이블(ExpTable)과 로그 테이블(LogTable) 생성
void generate_tables(byte alpha, byte ExpTable[TABLE_SIZE], byte LogTable[TABLE_SIZE]) {
    byte result = 0x01;  // α^0 = 1
    ExpTable[0] = 0x01; 

    // ExpTable과 LogTable 생성
    for (int i = 1; i < 255; i++) {
        result = GF256_mul(result, alpha);
        ExpTable[i] = result;
        LogTable[result] = i;
    }

    // 특수한 경우임.  α^255 = α^0 = 1, LogTable[0x01] = 0
    ExpTable[255] = ExpTable[0];  
    LogTable[0x01] = 0;
}

// 지수 테이블(ExpTable) 출력
void print_exp_table(byte ExpTable[TABLE_SIZE]) {
    printf("\nExpTable (k -> α^k):\n");
    for (int i = 0; i < 256; i++) {
        printf("ExpTable[%d] = 0x%02x   ", i, ExpTable[i]);
        if (i % 5 == 0) {
            printf("\n");
        }
    }
}

// 로그 테이블(LogTable) 출력
void print_log_table(byte LogTable[TABLE_SIZE]) {
    printf("\nLogTable (α^k -> k):\n");
    for (int i = 0; i < 256; i++) {
        printf("LogTable[0x%02x] = %d    ", i, LogTable[i]);
        if (i % 5 == 0) {
            printf("\n");
        }
    }
}

void test_tables() {
    byte ExpTable[TABLE_SIZE];
    byte LogTable[TABLE_SIZE];

    byte alpha = 0x03;

    // 테이블 생성
    generate_tables(alpha, ExpTable, LogTable);

    // 테이블 출력
    print_exp_table(ExpTable);
    print_log_table(LogTable);
}

// =================================================== //



// ==================== 5 - (e) ======================== //

// 테이블 사용해서 두 요소 곱하는 함수
byte multiply_using_tables(byte x, byte y, byte ExpTable[TABLE_SIZE], byte LogTable[TABLE_SIZE]) {
    if (x == 0 || y == 0) {
        return 0x00; 
    }

    // 요소에 해당하는 k 값 가져오기 (α^k -> k)
    // LogTable[0xA1] = 12, LogTable[0x63] = 195 
    int log_x = LogTable[x];
    int log_y = LogTable[y];

    // 두 k 값 더하기 ( mode 255 )
    int log_result = (log_x + log_y) % 255;

    // k 값에 해당하는 요소 가져오기 (k -> α^k)
    // ExpTable[12+195] = ExpTable[207] = 0xCA = 0xA1 * 0x63
    return ExpTable[log_result];
}

void test_multiplication() {
    byte ExpTable[TABLE_SIZE];
    byte LogTable[TABLE_SIZE];
    byte alpha = 0x03; 

    // 테이블 만들고
    generate_tables(alpha, ExpTable, LogTable);

    // 두 원소를 곱하고
    byte x = 0xA1;
    byte y = 0x63;
    byte result = multiply_using_tables(x, y, ExpTable, LogTable);

    printf("0x%02x * 0x%02x = 0x%02x\n", x, y, result);
}



int main() {
    // test_generators(); // 5번 - (a), (b) 문제 print
    test_tables(); // 5번 - (d) 문제 print
    // test_multiplication(); // 5번 - (e) 문제 print
    return 0;
}