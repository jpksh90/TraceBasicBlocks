#include <stdio.h>
int fib(int n) {
  int a, b, c;
  a = 0;
  b = 1;
  for (int i=1; i<=n; ++i) {
    c = a+b;
    a = b;
    b = c;
  }
  return c;
}

int main() {
  int n;
  scanf("%d", &n);
  if (n < 0) {
    printf("n is negative");
    return -1;
  }
  printf("%d", fib(n));
  return 0;
}
