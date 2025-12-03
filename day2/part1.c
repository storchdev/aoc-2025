#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void logic(uint64_t low, uint64_t high, uint64_t *ans) {
  for (uint64_t i = low; i <= high; i++) {
    // printf("i=%ld\n", i);
    char buf[100];
    sprintf(buf, "%ld", i);

    if (strlen(buf) % 2 == 1) {
      continue;
    }

    int halflen = strlen(buf) / 2;

    char firsthalf[100];
    char secondhalf[100];

    for (int j = 0; j < halflen; j++) {
      firsthalf[j] = buf[j];
    }
    for (int j = 0; j < halflen; j++) {
      secondhalf[j] = buf[halflen + j];
    }

    // printf("firsthalf=%s, secondhalf=%s\n", firsthalf, secondhalf);

    if (strcmp(firsthalf, secondhalf) == 0) {
      printf("ID %ld invalid\n", i);
      *ans += i;
    }
  }
}

int main() {
  uint64_t ans = 0;

  uint64_t low = 0;
  uint64_t high = 0;

  int x = 'a';
  int islow = 1;

  while (x != EOF) {
    x = getchar();
    if (x == ',') {
      islow = 1;
      // printf("Called logic on %ld-%ld\n", low, high);
      logic(low, high, &ans);
      low = 0;
      high = 0;
      continue;
    }

    if (x == '-') {
      islow = 0;
      continue;
    }

    if ('0' <= x && x <= '9') {
      if (islow) {
        low = low * 10 + (x - '0');
      } else {
        high = high * 10 + (x - '0');
      }
    }
  }

  printf("Answer=%ld\n", ans);
  return 0;
}
