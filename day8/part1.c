#include "common.c"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int steps = 10;
  if (argc > 1) {
    char *eptr = NULL;
    steps = strtol(argv[1], &eptr, 10);

    if (eptr == argv[1]) {
      printf("Invalid # of steps\n");
      exit(1);
    }
  }

  printf("Steps: %d\n\n", steps);

  int x = '0';
  int temp[3];
  int tempi = 0;

  int v = 0;

  Circuits circuits = newcircuits();
  Box *boxes = malloc(10000 * sizeof(Box));
  int blen = 0;
  Conn *conns = malloc(1000000 * sizeof(Conn));
  int conlen = 0;

  while (x != EOF) {
    x = getchar();
    if (x == '\n') {
      temp[tempi] = v;
      v = 0;

      Circuit c = newcircuit(circuits.len);
      insertcircuit(&circuits, c);

      Box b = (Box){temp[0], temp[1], temp[2], -1};
      insertbox(&circuits, circuits.len - 1, b);
      boxes[blen] = b;
      blen++;

      tempi = 0;
      continue;
    }

    if (x == ',') {
      temp[tempi] = v;
      tempi++;
      v = 0;
      continue;
    }

    if ('0' <= x && x <= '9') {
      v = 10 * v + (x - '0');
    }
  }

  makeconns(boxes, blen, conns, &conlen);

  for (int a = 0; a < steps; a++) {
    Conn *conn = conns + a;

    // these boxes do not contain ci info
    Box *b1 = boxes + conn->a;
    Box *b2 = boxes + conn->b;

    int ci1 = -1;
    int ci2 = -1;

    for (int i = 0; i < circuits.len; i++) {
      for (int j = 0; j < circuits.circuits[i].len; j++) {
        Box *b = circuits.circuits[i].boxes + j;
        if (boxeq(b, b1)) {
          ci1 = b->ci;
        }
        if (boxeq(b, b2)) {
          ci2 = b->ci;
        }
      }

      if (ci1 != -1 && ci2 != -1) {
        break;
      }
    }

    if (ci1 == ci2)
      continue;

    // printf("%d,%d\n", c1->len, c2->len);
    // printf("combining %d and %d\n", ci1, ci2);
    combine(&circuits, ci1, ci2);
    // printcircuits(&circuits);
  }

  qsort(circuits.circuits, circuits.len, sizeof(Circuit), compcir);

  int x0 = circuits.circuits[0].len;
  int x1 = circuits.circuits[1].len;
  int x2 = circuits.circuits[2].len;

  // printcircuits(&circuits);
  delcircuits(&circuits);

  free(boxes);
  free(conns);

  printf("Ans: %d\n", x0 * x1 * x2);

  return 0;
}
