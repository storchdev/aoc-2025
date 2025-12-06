#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  long long a;
  long long b;
} Range;

int inRange(Range *r, int len, long long id) {
  for (int i = 0; i < len; i++) {
    if (r[i].a <= id && id <= r[i].b) {
      return 1;
    }
  }

  return 0;
}

int main() {
  int xp = '1';
  int x = '0';

  int side = 'a';
  long long a = 0;
  long long b = 0;

  Range ranges[1000];
  int i = 0;

  while (x != EOF) {
    x = getchar();

    if (x == '\n' && xp == '\n') {
      break;
    }

    xp = x;

    if (x == '\n') {
      Range r = (Range){a, b};
      ranges[i] = r;
      i++;
      side = 'a';
      a = 0;
      b = 0;
      continue;
    }

    if (x == '-') {
      side = 'b';
      continue;
    }

    if (x >= '0' && x <= '9') {
      if (side == 'a') {
        a = 10 * a + (x - '0');
      } else {
        b = 10 * b + (x - '0');
      }
    }
  }

  int len = i;

  for (int i = 0; i < len; i++) {
    printf("(%lld,%lld)\n", ranges[i].a, ranges[i].b);
  }

  int ans = 0;

  long long id = 0;

  while (x != EOF) {
    x = getchar();

    if (x == '\n') {
      if (inRange(ranges, len, id)) {
        ans++;
      }
      id = 0;
      continue;
    }

    if (x >= '0' && x <= '9') {
      id = id * 10 + (x - '0');
    }
  }

  printf("Ans: %d\n", ans);

  return 0;
}
