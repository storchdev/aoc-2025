#include "common.c"
#include <stdio.h>
#include <stdlib.h>

long long getarea(Coord *a, Coord *b) {
  return (long long)abs(a->b - b->b + 1) * abs(a->a - b->a + 1);
}

int main() {
  int a, b;

  Coord *coords = malloc(10000 * sizeof(Coord));
  int len = 0;

  while (scanf("%d,%d", &a, &b) == 2) {
    coords[len] = (Coord){a, b};
    len++;
  }

  long long maxarea = 0;

  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      long long area = getarea(coords + i, coords + j);
      if (area > maxarea) {
        maxarea = area;
      }
    }
  }

  free(coords);

  printf("Ans: %lld\n", maxarea);
  return 0;
}
