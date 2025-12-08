#include <stdio.h>

void beamsim(char *grid, int rows, int cols, int initr, int initc, int **v,
             int *vlen, int *ans) {
  for (int r = initr; r < rows; r++) {
    if (grid[r * cols + initc] == '^') {
      for (int i = 0; i < *vlen; i++) {
        if (r * cols + initc == (*v)[i]) {
          return;
        }
      }

      (*v)[*vlen] = r * cols + initc;
      (*vlen)++;

      printf("Split at (%d,%d)\n", r, initc);
      (*ans)++;
      if (initc - 1 >= 0) {
        beamsim(grid, rows, cols, r, initc - 1, v, vlen, ans);
      }
      if (initc + 1 < cols) {
        beamsim(grid, rows, cols, r, initc + 1, v, vlen, ans);
      }
      break;
    }
  }
}

int main() {
  char grid[100000];
  int varr[100000];
  int *v = (int *)varr;

  int len = 0;
  int rows = 0;
  int cols = 0;

  int x = '0';

  int scol = 0;

  while (x != EOF) {
    x = getchar();

    if (x == 'S') {
      scol = len;
    }
    if (x == '\n') {
      if (rows == 0) {
        cols = len;
      }
      rows++;
    } else {
      grid[len] = x;
      len++;
    }
  }

  printf("%dx%d, len=%d, scol=%d\n", rows, cols, len, scol);
  int ans = 0;

  int vlen = 0;
  beamsim(grid, rows, cols, 0, scol, &v, &vlen, &ans);

  printf("Ans: %d\n", ans);
  return 0;
}
