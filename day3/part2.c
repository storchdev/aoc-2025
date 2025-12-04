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

void logic(char *line, long long *ans) {
  long long inc = 0;
  int prev_pos = 0;
  for (int i = 11; i >= 0; i--) {
    Maxthing t = getmax(line, prev_pos, strlen(line) - i);
    if (t.pos == -1)
      printf("oh no\n");

    prev_pos = t.pos + 1;
    inc = 10 * inc + t.max;
  }
  printf("Added %lld\n", inc);
  *ans += inc;
}

int main() {
  int x = '0';
  char line[1000];

  long long ans = 0;

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

  printf("Ans=%lld\n", ans);

  return 0;
}
