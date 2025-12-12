#include "common.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

long long getarea(Coord *c1, Coord *c2) {
  int x1 = c1->a;
  int y1 = c1->b;
  int x2 = c2->a;
  int y2 = c2->b;

  return (long long)(abs(x1 - x2) + 1) * (abs(y1 - y2) + 1);
}

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int isinside(Coord *coord, Seg *segs, int seglen) {
  int x = coord->a;
  int y = coord->b;

  int rctr = 0;

  for (int i = 0; i < seglen; i++) {
    int x1 = segs[i].c1->a;
    int y1 = segs[i].c1->b;
    int x2 = segs[i].c2->a;
    int y2 = segs[i].c2->b;

    if (x1 > x2)
      swap(&x1, &x2);

    if (y1 > y2)
      swap(&y1, &y2);

    if (x1 == x2) {
      // vertical seg
      if (y1 <= y && y <= y2 && x == x1)
        return 1;

      if (y1 <= y && y < y2 && x < x1) {
        rctr++;
      }
    } else {
      // horizontal segment
      if (x1 <= x && x <= x2 && y == y1) {
        return 1;
      }
    }
  }
  return rctr % 2;
}

int coordseq(Coord *c1, Coord *c2) { return c1->a == c2->a && c1->b == c2->b; }

int isvert(Seg *s) { return s->c1->a == s->c2->a; }

int arraymin(int *arr, int len) {
  int min = INT32_MAX;
  for (int i = 0; i < len; i++) {
    if (arr[i] < min) {
      min = arr[i];
    }
  }
  return min;
}

int arraymax(int *arr, int len) {
  int max = INT32_MIN;
  for (int i = 0; i < len; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }
  return max;
}

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

int doesseginteresect(Seg *s1, Seg *s2) {
  int s1x1 = s1->c1->a;
  int s1y1 = s1->c1->b;
  int s1x2 = s1->c2->a;
  int s1y2 = s1->c2->b;
  int s2x1 = s2->c1->a;
  int s2y1 = s2->c1->b;
  int s2x2 = s2->c2->a;
  int s2y2 = s2->c2->b;

  if (isvert(s1) == isvert(s2))
    return 0;

  if (isvert(s1)) {
    int miny = min(s1y1, s1y2);
    int maxy = max(s1y1, s1y2);

    int minx = min(s2x1, s2x2);
    int maxx = max(s2x1, s2x2);

    if (miny < s2y1 && s2y1 < maxy && minx < s1x1 && s1x1 < maxx) {
      return 1;
    }
  } else {
    int miny = min(s2y1, s2y2);
    int maxy = max(s2y1, s2y2);

    int minx = min(s1x1, s1x2);
    int maxx = max(s1x1, s1x2);

    if (miny < s1y1 && s1y1 < maxy && minx < s2x1 && s2x1 < maxx) {
      return 1;
    }
  }
  return 0;
}

int doesintersect(Coord *pcnr1, Coord *pcnr2, Seg *polygon, int len) {
  Coord cnr3 = {pcnr1->a, pcnr2->b};
  Coord cnr4 = {pcnr2->a, pcnr1->b};

  Seg sides[4] = {(Seg){pcnr1, &cnr3}, (Seg){pcnr1, &cnr4}, (Seg){pcnr2, &cnr3},
                  (Seg){pcnr2, &cnr4}};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < len; j++) {
      // if (doesseginteresect(sides + i, polygon + j)) {
      if (doesseginteresect(sides + i, polygon + j)) {
        return 1;
      }
    }
  }
  return 0;
}

int main() {
  int a, b;

  Coord *coords = malloc(1000 * sizeof(Coord));
  Seg *segs = malloc(1000 * sizeof(Seg));

  int len = 0;

  while (scanf("%d,%d", &a, &b) == 2) {
    coords[len] = (Coord){a, b};
    len++;
    if (len == 1000) {
      printf("len is 1000!\n");
      return -1;
    }
  }

  int seglen = 0;

  for (int i = 0; i < len; i++) {
    int j;
    if (i == len - 1) {
      j = 0;
    } else {
      j = i + 1;
    }
    segs[seglen] = (Seg){coords + i, coords + j};
    seglen++;
  }

  printf("len=%d, seglen=%d\n", len, seglen);

  long long maxarea = 0;

  for (int i = 0; i < len - 1; i++) {
    for (int j = i + 1; j < len; j++) {
      int x1 = coords[i].a;
      int y1 = coords[i].b;
      int x2 = coords[j].a;
      int y2 = coords[j].b;

      Coord c3 = {x1, y2};
      Coord c4 = {x2, y1};

      Coord corners[2] = {
          c3,
          c4,
      };
      int allinside = 1;

      for (int k = 0; k < 2; k++) {
        if (!isinside(corners + k, segs, seglen)) {
          allinside = 0;
          break;
        }
      }

      if (!allinside)
        continue;

      if (doesintersect(coords + i, coords + j, segs, seglen)) {
        continue;
      }

      long long area = getarea(coords + i, coords + j);
      if (area > maxarea) {
        printf("%d,%d,%d,%d\n", coords[i].a, coords[i].b, coords[j].a,
               coords[j].b);
        maxarea = area;
      }
    }
  }

  free(coords);
  free(segs);

  printf("Ans: %lld\n", maxarea);
}
