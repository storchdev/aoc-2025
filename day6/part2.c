#include <stdio.h>
#include <stdlib.h>

int iscolclear(char *grid, int rows, int cols, int col) {
  for (int r = 0; r < rows; r++) {
    if (grid[r * cols + col] != ' ') {
      return 0;
    }
  }
  return 1;
}

char getop(char *grid, int rows, int cols, int col) {
  return grid[(rows - 1) * cols + col];
}

long long parsecol(char *grid, int rows, int cols, int col) {
  long long n = 0;

  for (int r = 0; r < (rows - 1); r++) {
    char x = grid[r * cols + col];
    if (x == ' ')
      continue;
    if (!('0' <= x && x <= '9')) {
      printf("Not a digit at (%d,%d)\n", r, col);
      exit(1);
    }

    n = 10 * n + (x - '0');
  }

  // printf("Parsed col %d: %lld\n", col, n);

  return n;
}

int main() {
  char grid[100000];
  int len = 0;

  int rows = 0;
  int cols = 0;

  int x = '0';
  while (x != EOF) {
    x = getchar();

    if (x == '\n') {
      if (rows == 0) {
        cols = len;
      }
      rows++;
    } else if (('0' <= x && x <= '9') || x == '+' || x == '*' || x == ' ') {
      grid[len] = x;
      len++;
    }
  }

  long long partial = 0;
  int first = 1;

  long long ans = 0;
  char op = 'x';

  for (int c = 0; c < cols; c++) {
    if (iscolclear(grid, rows, cols, c)) {
      ans += partial;
      printf("Partial for col %d: %lld\n", c, partial);
      partial = 0;
      first = 1;
      continue;
    }

    if (first) {
      op = getop(grid, rows, cols, c);
      if (op != '*' && op != '+') {
        printf("Unexpected op: %c\n", op);
        exit(1);
      }
      partial = parsecol(grid, rows, cols, c);
      first = 0;
    } else {
      if (op == '*') {
        partial *= parsecol(grid, rows, cols, c);
      } else if (op == '+') {
        partial += parsecol(grid, rows, cols, c);
      } else {
        printf("Uninitialized op\n");
        exit(1);
      }
    }
  }

  ans += partial;

  printf("Len: %d\n", len);
  printf("Dim: %dx%d\n", rows, cols);

  printf("Ans: %lld\n", ans);

  return 0;
}
