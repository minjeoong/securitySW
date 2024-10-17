#include <stdio.h>

typedef unsigned char byte;
typedef unsigned int u32;
typedef unsigned char u8;  // byte와 동일


// 바이트 배열 b[4] --> u32 정수로
#define GETU32(b) (((u32)(b)[0] << 24) ^ ((u32)(b)[1] << 16) \
 ^ ((u32)(b)[2] << 8) ^ ((u32)(b)[3]));

// u32 정수 --> 바이트 배열 b[4]
#define PUTU32(b, x) { \
  (b)[0] = (u8)((x) >> 24); (b)[1] = (u8)((x) >> 16); \
  (b)[2] = (u8)((x) >> 8); (b)[3] = (u8)(x); }

void endian_test_1() {
    byte b[4] = {1, 2, 3, 4};
    u32* pInt;
    u32 x;

    pInt = (u32*)b;
    x = *pInt;
    printf("b = %02x, %02x, %02x, %02x\n", b[0], b[1], b[2], b[3]);
    printf("x = %08x\n", x);

    u32 y;
    // y 결과는 little/big endian 과 무관	
    y = ((u32)b[0] << 24) ^ ((u32)b[1] << 16) ^ ((u32)b[2] << 8) ^ ((u32)b[3]);
    // y = 01000000 ^ 00020000 ^ 00000300 ^ 00000004;
    printf("y = %08x\n", y);
}

void endian_test_2() {
  u32 a;
  byte pt[4];

  a = 0xf1f2f3f4;
  printf("a = %08x\n", a);

  PUTU32(pt, a);
  printf("pt = %02x, %02x, %02x, %02x\n", pt[0], pt[1], pt[2], pt[3]);

  byte b[8];
  for (int i = 0; i < 8; i++) {
    b[i] = (byte)(0xf1 + i);
  }
  printf("b = %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);

  u32 x1, x2;
  x1 = GETU32(b);
  x2 = GETU32(b + 4);  // b + 4는 b[4]의 주소

  printf("x1 = %08x\n", x1);
  printf("x2 = %08x\n", x2);

}

int main() {
    // endian_test_1();
    endian_test_2();
}

