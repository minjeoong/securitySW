#ifndef GF256_H
#define GF256_H

typedef unsigned char byte; // byte는 GF(2^8) 유한체의 원소를 저장하는 데이터타입

byte GF256_add(byte a, byte b);
void GF256_print_bin(byte x);
void GF256_print_poly(byte a);
byte GF256_xtime(byte a);
byte GF256_mul(byte f, byte g);
byte GF256_inv(byte x);

#endif