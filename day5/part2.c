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

int inRange2(Range *r, int len, long long id, Range *found) {
  for (int i = 0; i < len; i++) {
    if (r[i].a <= id && id <= r[i].b) {
      *found = r[i];
      return 1;
    }
  }

  return 0;
}

int isbetw(long long n, Range betw) { return betw.a <= n && n <= betw.b; }

void cleanRanges(Range *ranges, int *len, Range betw) {
  Range newranges[1000];
  int newlen = 0;
  for (int i = 0; i < *len; i++) {
    Range r = ranges[i];
    if (isbetw(r.a, betw) || isbetw(r.b, betw))
      continue;

    // printf("adding %d-%d\n", r.a, r.b);
    newranges[newlen] = r;
    newlen++;
  }

  memcpy(ranges, newranges, newlen * sizeof(Range));
  // *ranges = *newranges;
  *len = newlen;
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

  long long ans = 0;

  Range newranges[1000];
  int newlen = 0;

  for (int i = 0; i < len; i++) {
    Range r = ranges[i];
    Range afound;
    Range bfound;

    int ain = inRange2(newranges, newlen, r.a, &afound);
    int bin = inRange2(newranges, newlen, r.b, &bfound);

    long long a;
    long long b;
    if (ain && bin) {
      a = afound.a;
      b = bfound.b;
    } else if (!ain && bin) {
      a = r.a;
      b = bfound.b;
    } else if (ain && !bin) {
      a = afound.a;
      b = r.b;
    } else {
      a = r.a;
      b = r.b;
    }

    Range newr = {a, b};
    cleanRanges(newranges, &newlen, newr);

    newranges[newlen] = newr;
    newlen++;
  }

  for (int i = 0; i < newlen; i++) {
    Range r = newranges[i];
    ans += r.b - r.a + 1;
  }

  for (int i = 0; i < newlen; i++) {
    for (int j = 0; j < newlen; j++) {
      if (i == j)
        continue;
      if (isbetw(newranges[j].a, newranges[i]) ||
          isbetw(newranges[j].b, newranges[i])) {
        printf("Overlap detected: (%lld,%lld) in (%lld,%lld)\n", newranges[j].a,
               newranges[j].b, newranges[i].a, newranges[i].b);
      }
    }
  }

  printf("---\n");
  for (int i = 0; i < newlen; i++) {
    printf("(%lld,%lld)\n", newranges[i].a, newranges[i].b);
  }

  // long long id = 0;
  //
  // while (x != EOF) {
  //   x = getchar();
  //
  //   if (x == '\n') {
  //     if (inRange(ranges, len, id)) {
  //       ans++;
  //     }
  //     id = 0;
  //     continue;
  //   }
  //
  //   if (x >= '0' && x <= '9') {
  //     id = id * 10 + (x - '0');
  //   }
  // }

  printf("Ans: %lld\n", ans);

  return 0;
}
