#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 64
#define MAX_COLS 64
#define HASH_SIZE 100003 // Prime number for hash table

// --- Structures ---

// Struct to store cached results for min_presses
// Key: Array of target values, Value: Result cost
typedef struct MemoEntry {
  long long *key;
  int len;
  long long val;
  struct MemoEntry *next;
} MemoEntry;

// Struct to store the XOR subset cache
// Key: Parity mask (int), Value: List of row masks (ints)
typedef struct SubsetNode {
  unsigned long long mask;
  struct SubsetNode *next;
} SubsetNode;

typedef struct SubsetEntry {
  unsigned long long key;
  SubsetNode *values; // Linked list of valid masks
  struct SubsetEntry *next;
} SubsetEntry;

// --- Globals ---

int rows[MAX_ROWS][MAX_COLS];
int num_rows = 0;
int num_cols = 0;

MemoEntry *memo_table[HASH_SIZE];
SubsetEntry *subset_table[HASH_SIZE];

// --- Helper Functions ---

unsigned long long list2int(long long *v, int len) {
  unsigned long long rv = 0;
  for (int i = 0; i < len; i++) {
    rv = (rv << 1) | (v[i] & 1);
  }
  return rv;
}

// DJB2 Hash for array
unsigned int hash_array(long long *arr, int len) {
  unsigned int hash = 5381;
  for (int i = 0; i < len; i++) {
    // Mix the long long value into the hash
    unsigned int val =
        (unsigned int)arr[i]; // simple cast is usually enough for distribution
    hash = ((hash << 5) + hash) + val;
  }
  return hash % HASH_SIZE;
}

// DJB2 Hash for int key
unsigned int hash_int(unsigned long long key) { return key % HASH_SIZE; }

// --- Memoization (Cache) Logic ---

// Get value from memo_table
// Returns -1 if not found (since costs are >= 0)
long long memo_get(long long *target, int len) {
  unsigned int h = hash_array(target, len);
  MemoEntry *e = memo_table[h];
  while (e) {
    if (e->len == len && memcmp(e->key, target, len * sizeof(long long)) == 0) {
      return e->val;
    }
    e = e->next;
  }
  return -1;
}

// Put value into memo_table
void memo_put(long long *target, int len, long long val) {
  unsigned int h = hash_array(target, len);
  MemoEntry *e = malloc(sizeof(MemoEntry));
  e->key = malloc(len * sizeof(long long));
  memcpy(e->key, target, len * sizeof(long long));
  e->len = len;
  e->val = val;
  e->next = memo_table[h];
  memo_table[h] = e;
}

// Clear memo table
void clear_memo() {
  for (int i = 0; i < HASH_SIZE; i++) {
    MemoEntry *e = memo_table[i];
    while (e) {
      MemoEntry *temp = e;
      e = e->next;
      free(temp->key);
      free(temp);
    }
    memo_table[i] = NULL;
  }
}

// --- Subset Cache Logic (Cache2) ---

void subset_put(unsigned long long key, unsigned long long mask) {
  unsigned int h = hash_int(key);

  // Find bucket
  SubsetEntry *e = subset_table[h];
  while (e) {
    if (e->key == key) {
      // Key exists, append to list
      SubsetNode *n = malloc(sizeof(SubsetNode));
      n->mask = mask;
      n->next = e->values;
      e->values = n;
      return;
    }
    e = e->next;
  }

  // Key does not exist, create entry
  e = malloc(sizeof(SubsetEntry));
  e->key = key;
  e->next = subset_table[h];
  subset_table[h] = e;

  // Create value list
  SubsetNode *n = malloc(sizeof(SubsetNode));
  n->mask = mask;
  n->next = NULL;
  e->values = n;
}

SubsetNode *subset_get(unsigned long long key) {
  unsigned int h = hash_int(key);
  SubsetEntry *e = subset_table[h];
  while (e) {
    if (e->key == key)
      return e->values;
    e = e->next;
  }
  return NULL;
}

void clear_subset_cache() {
  for (int i = 0; i < HASH_SIZE; i++) {
    SubsetEntry *e = subset_table[i];
    while (e) {
      SubsetEntry *tempE = e;
      e = e->next;

      // Free the value list
      SubsetNode *n = tempE->values;
      while (n) {
        SubsetNode *tempN = n;
        n = n->next;
        free(tempN);
      }
      free(tempE);
    }
    subset_table[i] = NULL;
  }
}

// --- Core Logic ---

void build_allsubsets() {
  // This populates subset_table (cache2 in python)
  // rows_bin equivalent
  unsigned long long rows_bin[MAX_ROWS];
  for (int r = 0; r < num_rows; r++) {
    unsigned long long rv = 0;
    for (int c = 0; c < num_cols; c++) {
      rv = (rv << 1) | (rows[r][c] & 1);
    }
    rows_bin[r] = rv;
  }

  // 1 << len(rows)
  unsigned long long limit = (1ULL << num_rows);
  for (unsigned long long mask = 0; mask < limit; mask++) {
    unsigned long long mut = 0;
    for (int i = 0; i < num_rows; i++) {
      if ((1ULL << i) & mask) {
        mut = mut ^ rows_bin[i];
      }
    }
    subset_put(mut, mask);
  }
}

long long min_presses(long long *target) {
  // Check if sum is 0
  long long s = 0;
  for (int i = 0; i < num_cols; i++)
    s += target[i];
  if (s == 0)
    return 0;

  // Check Cache
  long long cached = memo_get(target, num_cols);
  if (cached != -1)
    return cached;

  // Get subsets matching parity
  unsigned long long target_bin = list2int(target, num_cols);
  SubsetNode *minsets = subset_get(target_bin);

  long long minv = LLONG_MAX;

  // Iterate over valid masks
  while (minsets) {
    unsigned long long mask = minsets->mask;
    minsets = minsets->next;

    // Create new target
    long long newtarget[MAX_COLS];
    int c_presses = 0;
    int valid = 1;

    // Apply subtraction
    for (int c = 0; c < num_cols; c++)
      newtarget[c] = target[c];

    for (int i = 0; i < num_rows; i++) {
      if ((1ULL << i) & mask) {
        for (int c = 0; c < num_cols; c++) {
          newtarget[c] -= rows[i][c];
        }
        c_presses++;
      }
    }

    // Check non-negative
    for (int c = 0; c < num_cols; c++) {
      if (newtarget[c] < 0) {
        valid = 0;
        break;
      }
    }

    if (!valid)
      continue;

    // Divide by 2
    for (int c = 0; c < num_cols; c++) {
      newtarget[c] /= 2;
    }

    // Recurse
    long long res = min_presses(newtarget);

    // Avoid overflow if sub-result is infinite (though logic prevents this
    // usually)
    if (res != LLONG_MAX) {
      long long v = c_presses + 2 * res;
      if (v < minv)
        minv = v;
    }
  }

  memo_put(target, num_cols, minv);
  return minv;
}

// --- Parsing and Main ---

int main() {
  char line[4096];
  long long total = 0;
  int counter = 0;

  while (fgets(line, sizeof(line), stdin)) {
    if (line[0] != '[')
      continue;
    counter++;

    // Reset Globals
    clear_memo();
    clear_subset_cache();
    num_rows = 0;
    num_cols = 0;
    memset(rows, 0, sizeof(rows));

    // 1. Parse Target inside {}
    char *brace_start = strchr(line, '{');
    char *brace_end = strchr(line, '}');
    if (!brace_start || !brace_end)
      continue;

    long long initial_target[MAX_COLS];
    char *ptr = brace_start + 1;
    while (ptr < brace_end) {
      initial_target[num_cols++] = strtoll(ptr, &ptr, 10);
      while (*ptr == ',' || *ptr == ' ')
        ptr++;
    }

    // 2. Parse Rows inside ()
    ptr = line;
    while (ptr < brace_start) {
      char *open_p = strchr(ptr, '(');
      if (!open_p || open_p >= brace_start)
        break;

      char *close_p = strchr(open_p, ')');

      char *curr = open_p + 1;
      while (curr < close_p) {
        int idx = (int)strtol(curr, &curr, 10);
        if (idx < MAX_COLS) {
          rows[num_rows][idx] = 1;
        }
        while (*curr == ',' || *curr == ' ')
          curr++;
      }
      num_rows++;
      ptr = close_p + 1;
    }

    // 3. Precompute subsets (cache2)
    build_allsubsets();

    // 4. Solve
    long long ans = min_presses(initial_target);
    total += ans;
  }

  printf("---\n");
  printf("%lld\n", total);

  // Cleanup leftovers
  clear_memo();
  clear_subset_cache();

  return 0;
}
