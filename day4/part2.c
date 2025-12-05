#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getnbrs(char *grid, int rows, int cols, int i, int j) {
  int nbrs = 0;
  for (int x = i - 1; x <= i + 1; x++) {
    for (int y = j - 1; y <= j + 1; y++) {
      if (i == x && j == y)
        continue;
      if (x < 0 || x > rows - 1)
        continue;
      if (y < 0 || y > cols - 1)
        continue;

      if (grid[x * rows + y] == '@')
        nbrs++;
    }
  }
  return nbrs;
}

int rmrolls(char *grid, int rows, int cols) {
  int num = 0;

  int tormi[10000];
  int tormj[10000];

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (grid[rows * i + j] != '@') {
        continue;
      }
      if (getnbrs(grid, rows, cols, i, j) < 4) {
        tormi[num] = i;
        tormj[num] = j;
        num++;
      }
    }
  }

  for (int k = 0; k < num; k++) {
    int i = tormi[k];
    int j = tormj[k];
    grid[rows * i + j] = '.';
  }

  return num;
}

int main() {
  int rows = 0;
  int cols = 0;

  char grid[1000000];

  int x = '0';

  int i = 0;
  while (x != EOF) {
    x = getchar();
    if (x == '\n' && cols == 0) {
      cols = i;
    }
    if (x == '\n') {
      rows++;
    }

    if (x == '.' || x == '@') {
      grid[i] = x;
      i++;
    }
  }

  int ans = 0;

  // for (int i = 0; i < rows; i++) {
  //   for (int j = 0; j < cols; j++) {
  //     printf("%c", grid[rows * i + j]);
  //   }
  //   printf("\n");
  // }
  for (;;) {
    int rm = rmrolls(grid, rows, cols);
    if (rm == 0)
      break;
    ans += rm;
  }

  printf("Cols: %d\n", cols);
  printf("Rows: %d\n", rows);
  printf("Ans: %d\n", ans);

  return 0;
}
