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

long long sq(long long x) { return x * x; }

long long dist(Box *a, Box *b) {
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

int compcir(const void *a, const void *b) {
  const Circuit *c1 = a;
  const Circuit *c2 = b;

  return c2->len - c1->len;
}

int boxeq(Box *b1, Box *b2) {
  return (b1->x == b2->x && b1->y == b2->y && b1->z == b2->z);
}

typedef struct {
  int a;
  int b;
  Box *boxes;
} Conn;

int comppair(const void *a, const void *b) {
  const Conn *conn1 = a;
  const Conn *conn2 = b;

  Box *bs1 = conn1->boxes;
  Box *bs2 = conn2->boxes;

  return dist(bs1 + conn1->a, bs1 + conn1->b) >
         dist(bs2 + conn2->a, bs2 + conn2->b);
}

void makeconns(Box *boxes, int blen, Conn *pairs, int *plen) {
  int len = 0;

  for (int i = 0; i < blen - 1; i++) {
    for (int j = i + 1; j < blen; j++) {
      pairs[len] = (Conn){i, j, boxes};
      len++;
    }
  }

  *plen = len;
  qsort(pairs, *plen, sizeof(Conn), comppair);
}
