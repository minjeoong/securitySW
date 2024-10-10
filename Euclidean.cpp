


#include <cstdio>

int gcd(int a, int b){
  // a = b*q + r ==> gcd(a, b) = gcd(b, r)
  // 반복하여 수행 ==> b=0 일 때, a 가 gcd 이다. 
  int r, q;

  while (b != 0){
    r = a % b;
    q = a / b;
    printf("%d = %d * %d + %d\n", a,b, q, r)
    a = b;
    b = r;
  }
  return a;
}

int gcd_recursive(int a, int b){
  
  // if (b == 0){
  //   return a;
  // }
  // return gcd_recursive(b, a % b);

  return (b==0) ? a : gcd_recursive(b, a % b);
}

// 확장 유클리드 알고리즘 : ax + by = gcd(a,b) 인 x, y 를 출력
// an = ua*a + va * b, bn = ub*a + vb*b 로 표기를 유지
// 마지막 단계 (b==0) 에서 an = gcd(a,b) = ua*a+ va*b => x=ua, y=va 

// call by reference 로 x, y 를 받아서 값을 변경 (전달 변수의 별명으로 관리, 변수 자체를 받아온 것과 동일) 
int gcd_extended(int a, int b, int &x, int &y ){
  int an, bn; // 각 단계에서의 a,b 값
  int ua, va, ub, vb; // 각 단계에서의 x, y 값
  int new_an, new_bn; // 다음 단계  
  int n_ua, n_va, n_ub, n_vb; // 다음 단계에서의 ua, va, ub, vb 값 값
  int q; // 몫

  an = a; bn = b;
  ua = 1; va = 0; // 초기값 an = 1*a + 0*b
  ub = 0; vb = 1; // 초기값 bn = 0*a + 1*b

  while (bn != 0)
  {
    // an = bn*q + r
    // ==> new_an = bn , new_bn = r = an - bn*q
    // new_an = ub*a + vb*b
    // new_bn = an-bn*q = ua*a + va*b - (ub*a + vb*b)*q 
    //        = (ua-ub*q)*a + (va-vb*q)*b

    q = an / bn;
    new_an = bn;
    new_bn = an - bn*q;

    n_ua = ub; n_va = vb;
    n_ub = ua - ub*q; n_vb = va - vb*q;

    an = new_an; bn = new_bn;
    ua = n_ua; va = n_va;
    ub = n_ub; vb = n_vb;




  }
  // x y 리턴
  x = ua; y = va; // an = gcd(a,b) = x*a + y*b 이므로
  return an;
  
}

int main(){
  int a, b, g;
  a = 90;
  b = 63;

  // g = gcd(a, b);

  // printf("a = %d, b = %d, gcd(%d, %d) = %d/n", a, b,a,b, g);
  // int gr;
  // gr = gcd_recursive(a, b);
  // printf("a = %d, b = %d, gcd_recursive(%d, %d) = %d/n", a, b,a,b, gr);

  int x, y; 
  x=1; y = 1;
  g = ex_gcd(a,b,x,y);

  printf("a = %d, b = %d, ex_gcd(%d, %d) = %d, x = %d, y = %d\n", a, b,a,b, g, x, y);
  printf("gcd(%d, %d) = %d = %d*%d + %d*%d\n", a, b, g, x, a, y, b);

}

