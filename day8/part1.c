#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _Circuit Circuit;

typedef struct {
  int x;
  int y;
  int z;
  int ci;
} Box;

struct _Circuit {
  Box *boxes;
  int len;
  int id;
};

typedef struct {
  Circuit *circuits;
  int len;
} Circuits;

Circuit newcircuit(int id) {
  return (Circuit){malloc(10000 * sizeof(Box)), 0, id};
}

Circuits newcircuits() {
  return (Circuits){malloc(10000 * sizeof(Circuit)), 0};
}

void insertbox(Circuits *cs, int i, Box b) {
  b.ci = i;
  cs->circuits[i].boxes[cs->circuits[i].len] = b;
  cs->circuits[i].len++;
}

void insertcircuit(Circuits *cs, Circuit c) {
  cs->circuits[cs->len] = c;
  cs->len++;
}

void delcircuit(Circuit *c) { free(c->boxes); }

void removecircuit(Circuits *cs, int i) {
  delcircuit(cs->circuits + i);

  // cs -> circuits[i] = NULL;
  for (int j = i; j < cs->len - 1; j++) {
    cs->circuits[j] = cs->circuits[j + 1];
    for (int k = 0; k < cs->circuits[j].len; k++) {
      cs->circuits[j].boxes[k].ci--;
    }
  }

  cs->len--;
}

void delcircuits(Circuits *cs) {
  for (int i = 0; i < cs->len; i++) {
    delcircuit(cs->circuits + i);
  }
  free(cs->circuits);
}

void printbox(Box *b) { printf("<Box (%d,%d,%d)>", b->x, b->y, b->z); }

void printcircuit(Circuit *c) {
  printf("<Circuit id=%d len=%d boxes=[", c->id, c->len);
  for (int i = 0; i < c->len; i++) {
    Box b = c->boxes[i];
    printbox(&b);
    printf(", ");
  }
  printf("]>");
}

void printcircuits(Circuits *cs) {
  printf("<Circuits len=%d circuits=[\n", cs->len);

  for (int i = 0; i < cs->len; i++) {
    Circuit c = cs->circuits[i];
    printf("\t");
    printcircuit(&c);
    printf("\n");
  }
  printf("]>");
  printf("\n");
}

int sq(int x) { return x * x; }

int dist(Box *a, Box *b) {
  return sq(a->x - b->x) + sq(a->y - b->y) + sq(a->z - b->z);
}

void combine(Circuits *circuits, int i1, int i2) {
  Circuit *cs = circuits->circuits;

  if (cs[i1].len < cs[i2].len) {
    int tmp = i1;
    i1 = i2;
    i2 = tmp;
  }

  Circuit *c1 = cs + i1;
  Circuit *c2 = cs + i2;

  // copy from c2 into end of c1
  for (int i = c1->len; i < c1->len + c2->len; i++) {
    int j = i - c1->len;
    c1->boxes[i] = c2->boxes[j];
    c1->boxes[i].ci = i1;
  }

  c1->len += c2->len;

  removecircuit(circuits, i2);

  // delcircuit(c2)
}

Box *allboxes(Circuits *cs, int *len) {
  Box *boxes = malloc(sizeof(Box) * 10000);

  int i = 0;

  for (int j = 0; j < cs->len; j++) {
    for (int k = 0; k < cs->circuits[j].len; k++) {
      Circuit c = cs->circuits[j];
      Box b = c.boxes[k];
      boxes[i] = b;
      i++;
    }
  }

  *len = i;
  return boxes;
}

int comp(const void *a, const void *b) {
  const Circuit *c1 = a;
  const Circuit *c2 = b;

  return c2->len - c1->len;
}

int boxeq(Box *b1, Box *b2) {
  return (b1->x == b2->x && b1->y == b2->y && b1->z == b2->z);
}

int main() {

  int x = '0';
  int temp[3];
  int tempi = 0;

  int v = 0;

  Circuits circuits = newcircuits();

  Box *v1 = malloc(10000 * sizeof(Box));
  Box *v2 = malloc(10000 * sizeof(Box));
  int vlen = 0;

  while (x != EOF) {
    x = getchar();
    if (x == '\n') {
      temp[tempi] = v;
      v = 0;

      Circuit c = newcircuit(circuits.len);
      insertcircuit(&circuits, c);

      Box b = (Box){temp[0], temp[1], temp[2], -1};
      insertbox(&circuits, circuits.len - 1, b);

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

  int blen = 0;
  for (int a = 0; a < 1000; a++) {
    Box *boxes = allboxes(&circuits, &blen);
    int mindist = INT32_MAX;

    int ci1 = -1;
    int ci2 = -1;

    // int go = 1;

    Box b1, b2;

    for (int i = 0; i < blen - 1; i++) {
      for (int j = i + 1; j < blen; j++) {

        int cont = 0;
        for (int k = 0; k < vlen; k++) {
          if (boxeq(v1 + k, boxes + i) && boxeq(v2 + k, boxes + j)) {
            cont = 1;
            break;
          }
          if (boxeq(v1 + k, boxes + j) && boxeq(v2 + k, boxes + i)) {
            cont = 1;
            break;
          }
        }

        if (cont)
          continue;

        int d = dist(boxes + i, boxes + j);
        if (d < mindist) {
          mindist = d;
          b1 = boxes[i];
          b2 = boxes[j];
          ci1 = b1.ci;
          ci2 = b2.ci;
        }
      }

      // if (!go)
      //   break;
    }
    v1[vlen] = b1;
    v2[vlen] = b2;
    vlen++;
    if (b1.ci == b2.ci) {
      free(boxes);
      continue;
    }

    if (ci1 == -1 || ci2 == -1) {
      printf("c1 or c2 NULL\n");
      exit(1);
    }

    // printf("%d,%d\n", c1->len, c2->len);
    printf("combining %d and %d\n", ci1, ci2);
    combine(&circuits, ci1, ci2);
    printcircuits(&circuits);

    free(boxes);
  }

  qsort(circuits.circuits, circuits.len, sizeof(Circuit), comp);

  int x0 = circuits.circuits[0].len;
  int x1 = circuits.circuits[1].len;
  int x2 = circuits.circuits[2].len;

  printcircuits(&circuits);
  delcircuits(&circuits);

  free(v1);
  free(v2);

  printf("Ans: %d\n", x0 * x1 * x2);

  return 0;
}
