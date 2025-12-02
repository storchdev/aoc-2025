#include <stdio.h>
#include <stdlib.h>

int main() {
  int N = 100000;
  int ans = 0;
  int pos = 50;
  int mult = 1;
  int rot = 0;

  for (int i = 0; i < N; i++) {
    int x = getchar();
    if (x == EOF) {
      break;
    }
    if (i > N - 1) {
      printf("input too large\n");
      exit(1);
    }

    if (x == 'L') {
      mult = -1;
      continue;
    }
    if (x == 'R') {
      mult = 1;
      continue;
    }

    if (x == '\n') {
      pos = (pos + rot * mult) % 100;
      if (pos == 0) {
        printf("0 on i=%d\n", i);
        ans++;
      }
      rot = 0;
      continue;
    }

    // x must be a number
    rot = 10 * rot + (x - '0');
  }

  // printf("%s", buf);
  printf("answer: %d\n", ans);

  return 0;
}
