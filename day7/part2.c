#include <stdio.h>

typedef struct {
  int r;
  int c;
  long long v;
} Pair;

long long getValue(Pair *mem, int len, int r, int c) {
  for (int i = 0; i < len; i++) {
    Pair p = mem[i];
    if (p.r == r && p.c == c) {
      return p.v;
    }
  }
  return -1;
}

typedef long long (*Dpfunc)(char *, int, int, int, int, Pair *, int *);

long long getInc(char *grid, int rows, int cols, int r, int c, Pair *mem,
                 int *memlen, Dpfunc dp) {
  // either computes recursively or from memoized array

  long long cached = getValue(mem, *memlen, r, c);
  long long inc;
  if (cached == -1) {
    inc = dp(grid, rows, cols, r - 1, c, mem, memlen);
    mem[*memlen] = (Pair){r, c, inc};
    (*memlen)++;
    printf("%lld more ways from (%d,%d)\n", inc, r, c);
  } else {
    inc = cached;
  }
  return inc;
}

// long long getInc() {}
// answers "how many ways could a beam have gotten to (initr,initc)?"
long long dp(char *grid, int rows, int cols, int initr, int initc, Pair *mem,
             int *memlen) {
  long long ret = 0;
  for (int r = initr; r >= 0; r--) {
    if (grid[r * cols + initc] == 'S') {
      // 1 way from source
      return 1;
    }

    if (r == 0)
      continue;

    // inductive cases
    if (initc - 1 >= 0 && grid[r * cols + (initc - 1)] == '^') {
      ret += getInc(grid, rows, cols, r, initc - 1, mem, memlen, dp);
    }
    if (initc + 1 < cols && grid[r * cols + (initc + 1)] == '^') {
      ret += getInc(grid, rows, cols, r, initc + 1, mem, memlen, dp);
    }

    // beam cannot come from above ^
    if (grid[r * cols + initc] == '^') {
      break;
    }
  }
  return ret;
}

int main() {
  char grid[100000];
  // int varr[100000];
  // int *v = (int *)varr;

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
  long long ans = 0;

  Pair mem[100000];

  int memlen = 0;

  for (int c = 0; c < cols; c++) {
    ans += dp(grid, rows, cols, rows - 1, c, mem, &memlen);
  }

  printf("Ans: %lld\n", ans);
  return 0;
}
