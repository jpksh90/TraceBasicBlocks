#include <stdio.h>
int fib(int n) {
  int a=0, b=1, c=0;
  for (int i=1; i<=n; ++i) {
    c = a+b;
    a = b;
    b = c;
  }
  return c;
}

int main() {
  int n;
  printf("Enter a number: ");
  scanf("%d", &n);
  if (n < 0) {
    printf("n is negative");
    return -1;
  }
  printf("fib(%d)=%d", n, fib(n));
  return 0;
}
