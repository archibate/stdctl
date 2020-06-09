#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rand_xm(char xm[])
{
  char xs[] = "李王张刘陈杨赵黄周吴";
  char mz[] = "一二三四五六七八九十";
  int cw = sizeof("一") - 1;
  int a = rand() % (sizeof(xs) / cw);
  int b = rand() % (sizeof(mz) / cw);
  int c = rand() % (sizeof(mz) / cw * 5 / 4);
  memset(xm, 0, 9);
  for (int i = 0; i < cw; i++) {
    xm[0 * cw + i] = xs[a * cw + i];
    xm[1 * cw + i] = mz[b * cw + i];
    if (c <= 10)
      xm[2 * cw + i] = mz[c * cw + i];
  }
}

int main(int argc, char **argv)
{
  int i, xb, cj, n = 40;
  char xm[9];
  printf("%d\n", n);
  for (i = 0; i < n; i++) {
    rand_xm(xm);
    xb = rand() % 2;
    cj = rand() % 100;
    printf("%d %d %d %s\n", i + 1, xb, cj, xm);
  }
}
