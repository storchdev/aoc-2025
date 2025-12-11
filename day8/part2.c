#include "common.c"
#include <stdio.h>
#include <stdlib.h>

int main() {
  Circuits circuits = newcircuits();
  Box *boxes = malloc(10000 * sizeof(Box));
  int blen = 0;
  Conn *conns = malloc(1000000 * sizeof(Conn));
  int conlen = 0;

  int x, y, z;
  while (scanf("%d,%d,%d", &x, &y, &z) == 3) {
    Circuit c = newcircuit(circuits.len);
    insertcircuit(&circuits, c);

    Box b = (Box){x, y, z, -1};
    insertbox(&circuits, circuits.len - 1, b);
    boxes[blen] = b;
    blen++;
  }

  makeconns(boxes, blen, conns, &conlen);

  int a = 0;
  Box *last1;
  Box *last2;

  while (circuits.len > 1) {
    Conn *conn = conns + a;
    a++;

    // these boxes do not contain ci info
    Box *b1 = boxes + conn->a;
    Box *b2 = boxes + conn->b;

    last1 = b1;
    last2 = b2;

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

  delcircuits(&circuits);

  long long ans = (long long)last1->x * (long long)last2->x;

  free(boxes);
  free(conns);

  printf("Ans: %lld\n", ans);

  return 0;
}
