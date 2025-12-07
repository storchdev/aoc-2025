#include <stdio.h>

int main() {

  int x = '0';
  int rows = 0;
  int cols = 0;
  int this = 0;
  int len = 0;

  int grid[100000];

  char ops[10000];
  int lenops = 0;

  while (x != EOF) {
    x = getchar();

    if (x == ' ' && this != 0) {
      grid[len] = this;
      len++;
      this = 0;
      continue;
    }
    if (x == '\n') {
      if (this != 0) {
        grid[len] = this;
        len++;
      }

      if (rows == 0) {
        cols = len;
      }
      rows++;
      this = 0;
      continue;
    }
    if ('0' <= x && x <= '9') {
      this = this * 10 + (x - '0');
      continue;
    }
    if (x == '*' || x == '+') {
      ops[lenops] = x;
      lenops++;
      continue;
    }
  }

  rows--;

  long long ans = 0;

  // for (int i = 0; i < len; i++) {
  //   printf("%d ", grid[i]);
  // }
  // printf("\n");

  for (int c = 0; c < cols; c++) {
    long long partial = 0;
    if (ops[c] == '+') {
      for (int r = 0; r < rows; r++) {
        int n = grid[r * cols + c];
        // printf("Adding %d\n", n);
        partial += n;
      }
    } else {
      partial = 1;
      for (int r = 0; r < rows; r++) {
        int n = grid[r * cols + c];
        // printf("Multiplying %d\n", n);
        partial *= n;
      }
    }
    ans += partial;
    printf("Partial for %c=%lld\n", ops[c], partial);
  }

  printf("Rows=%d, Cols=%d\n", rows, cols);
  printf("# of nums: %d\n", len);
  printf("# of ops: %d\n", lenops);
  printf("Ans: %lld\n", ans);
  return 0;
}
