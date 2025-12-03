#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_invalid(char *str, int len, int testlen) {
  // assume testlen factor of len
  // printf("len=%d, testlen=%d\n", len, testlen);
  char model[100];
  char cur[100];

  strcpy(model, "");
  strcpy(cur, "");

  for (int i = 0; i < len; i++) {
    int curlen = strlen(cur);
    // printf("curlen=%d\n", curlen);
    cur[curlen] = str[i];
    cur[curlen + 1] = '\0';

    if (curlen + 1 == testlen) {
      if (strcmp(model, "") == 0) {
        strcpy(model, cur);
      }
      if (strcmp(cur, model) != 0) {
        // printf("cur=%s, model=%s\n", cur, model);
        return 0;
      } else {
        // printf("SAME: cur=%s, model=%s\n", cur, model);
      }
      strcpy(cur, "");
      continue;
    }
  }

  return 1;
}

void logic(uint64_t low, uint64_t high, uint64_t *ans) {
  for (uint64_t i = low; i <= high; i++) {
    // printf("i=%ld\n", i);
    char buf[100];
    sprintf(buf, "%ld", i);

    int len = strlen(buf);
    // printf("\n");
    // printf("len=%d, buf=%s\n", len, buf);
    for (int l = 1; l <= len / 2; l++) {
      if (len % l != 0) {
        continue;
      }
      // printf("checked %d/%d\n", len, l);

      if (is_invalid(buf, len, l)) {
        printf("%ld is invalid\n", i);
        *ans += i;
        break;
      }
    }
  }
}

int main() {
  uint64_t ans = 0;

  uint64_t low = 0;
  uint64_t high = 0;

  int x = 'a';
  int islow = 1;

  for (;;) {
    x = getchar();
    if (x == ',' || x == EOF) {
      islow = 1;
      // printf("Called logic on %ld-%ld\n", low, high);
      logic(low, high, &ans);
      low = 0;
      high = 0;

      if (x == EOF)
        break;
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
