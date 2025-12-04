#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int DEBUG = 1;

// void dbgargs(char *str, va_list args) {
//   if (DEBUG) {
//     vprintf(str, args);
//   }
// }
// void dbg(char *str) {
//   if (DEBUG) {
//     printf("%s", str);
//   }
// }

typedef struct {
  int max;
  int pos;
} Maxthing;

Maxthing getmax(char *line, int a, int b) {
  // get max digit in the range [a,b)

  int max = -1;
  int pos = -1;
  for (int i = a; i < b; i++) {
    if (line[i] < '0' || line[i] > '9') {
      printf("what %c\n", line[i]);
      continue;
    }
    if (line[i] - '0' > max) {
      max = line[i] - '0';
      pos = i;
    }
  }

  Maxthing ret = {max, pos};

  return ret;
}

void logic(char *line, int *ans) {
  Maxthing d1 = getmax(line, 0, strlen(line) - 1);
  if (d1.pos == -1)
    printf("oh no\n");

  Maxthing d2 = getmax(line, d1.pos + 1, strlen(line));
  if (d2.pos == -1)
    printf("oh no\n");

  int inc = 10 * d1.max + d2.max;
  printf("Added %d\n", inc);
  *ans += inc;
}

int main() {
  int x = '0';
  char line[1000];

  int ans = 0;

  while (x != EOF) {
    x = getchar();

    if (x == '\n') {
      logic(line, &ans);
      strcpy(line, "");
      continue;
    }

    int len = strlen(line);
    line[len] = x;
    line[len + 1] = '\0';
  }

  printf("Ans=%d\n", ans);

  return 0;
}
