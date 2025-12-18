#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int n;
  bool *target;
  int schemlen;
  bool **schems;
} Machine;

void delmachine(Machine *m) {
  free(m->target);

  for (int i = 0; i < m->schemlen; i++) {
    free(m->schems[i]);
  }
  free(m->schems);
}

void printmachine(Machine *m) {
  printf("<Machine n=%d schemlen=%d\ntarget=[", m->n, m->schemlen);

  for (int i = 0; i < m->n; i++) {
    printf("%d", m->target[i]);
    if (i != m->n - 1) {
      printf(",");
    }
  }

  printf("]\nschems=\n[\n");

  for (int i = 0; i < m->schemlen; i++) {
    printf("\t[");
    for (int j = 0; j < m->n; j++) {
      printf("%d", m->schems[i][j]);
    }
    printf("],\n");
  }
  printf("]>\n");
}

void arrxor(bool *dst, bool *a1, bool *a2, int len) {
  for (int i = 0; i < len; i++) {
    dst[i] = a1[i] ^ a2[i];
  }
}

int arrcmp(bool *a1, bool *a2, int len) {
  for (int i = 0; i < len; i++) {
    if (a1[i] != a2[i])
      return 1;
  }

  return 0;
}

void searchmachine(Machine *m, int *ans) {
  int minc = 1000;
  for (int mask = 1; mask < 1 << m->schemlen; mask++) {
    bool testarr[100] = {0};

    int c = 0;
    for (int i = 0; i < m->schemlen; i++) {
      if ((mask & (1 << i)) != 0) {
        arrxor(testarr, testarr, m->schems[i], m->n);
        c++;
      }
    }

    if (arrcmp(testarr, m->target, m->n) == 0) {
      if (c < minc)
        minc = c;
    }
  }

  if (minc == 1000) {
    printf("Didn't find anything for machine\n");
  } else {
    // printf("Added %d for machine\n", minc);
    *ans += minc;
  }
}

int main() {

  Machine *machines = malloc(1000 * sizeof(Machine));
  int mlen = 0;

  int x = 0;

  for (;;) {
    x = getchar();

    if (x == EOF)
      break;

    char targetstr[20];
    int i = 0;
    for (;;) {
      x = getchar();
      if (x == ']')
        break;
      targetstr[i] = x;
      i++;
    }
    targetstr[i] = '\0';
    int n = strlen(targetstr);

    bool *target = malloc(sizeof(bool) * (n + 1));
    for (int i = 0; i < n; i++) {
      if (targetstr[i] == '.') {
        target[i] = false;
      } else {
        target[i] = true;
      }
    }

    int schemlen = 0;
    bool **schems = malloc(20 * sizeof(bool *));
    // bool *schem;
    int d = 0;

    for (;;) {
      x = getchar();
      if (x == '{')
        break;
      if (x == ' ')
        continue;
      if (x == '(') {
        d = 0;
        schems[schemlen] = calloc(n, sizeof(bool));
        schemlen++;
      } else if (x == ',') {
        schems[schemlen - 1][d] = true;
        d = 0;
      } else if ('0' <= x && x <= '9') {
        d = 10 * d + (x - '0');
      } else if (x == ')') {
        if (d >= 0 && d < n) {
          schems[schemlen - 1][d] = true;
        } else {
          printf("d overflow: %d\n", d);
          exit(1);
        }
        d = 0;
      }
    }

    machines[mlen] = (Machine){n, target, schemlen, schems};
    mlen++;

    while (x != '\n') {
      x = getchar();
    }
  }

  int ans = 0;

  for (int i = 0; i < mlen; i++) {
    // printmachine(machines + i);
    searchmachine(machines + i, &ans);
  }

  for (int i = 0; i < mlen; i++) {
    delmachine(machines + i);
  }

  free(machines);

  printf("Ans: %d\n", ans);
  return 0;
}
