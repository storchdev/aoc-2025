import sys
import re

sys.setrecursionlimit(1000000)


# inspired by https://www.reddit.com/r/adventofcode/comments/1pk87hl/2025_day_10_part_2_bifurcate_your_way_to_victory/


def list2int(v: list[int]) -> int:
    rv = 0
    for i in v:
        rv = (rv << 1) | (i & 1)

    return rv


def allsubsets(rows: list[list[int]], target: list[int], cache) -> list[int]:
    target_bin = list2int(target)

    if len(cache) > 0:
        if v := cache.get(target_bin):
            return v

        return []

    rows_bin = [list2int(x) for x in rows]

    for mask in range(0, 1 << len(rows)):
        mut = 0
        for i in range(len(rows)):
            if (1 << i) & mask:
                mut = mut ^ rows_bin[i]

        if mut not in cache:
            cache[mut] = [mask]
        else:
            cache[mut].append(mask)

    return cache.get(target_bin, [])


def min_presses(rows, target, cache, cache2):
    if sum(target) == 0:
        return 0

    k = tuple(target)
    if v := cache.get(k):
        return v

    allsets = allsubsets(rows, target, cache2)

    minv = float("inf")
    for minset in allsets:
        newtarget = target.copy()

        c = 0
        for i in range(len(rows)):
            if (1 << i) & minset:
                newtarget = [a - b for a, b in zip(newtarget, rows[i])]
                c += 1

        if not all(x >= 0 for x in newtarget):
            continue

        newtarget = [x // 2 for x in newtarget]

        v = c + 2 * min_presses(rows, newtarget, cache, cache2)

        if v < minv:
            minv = v

    cache[k] = minv
    return minv


counter = 0
total = 0
for ln in sys.stdin.read().split("\n"):
    if not ln:
        continue
    if ln[0] != "[":
        continue
    counter += 1

    matches1 = re.findall(r"(\((?:\d|,)+\)\s)", ln)
    matches2 = re.findall(r"\{.+\}", ln)[0].strip("{}")
    matches1 = [str(m).strip("() ") for m in matches1]

    target = [int(x) for x in matches2.split(",")]

    rows = []
    for s in matches1:
        row = [0] * len(target)
        for i in s.split(","):
            row[int(i)] = 1
        rows.append(row)

    cache = {}
    cache2 = {}
    ans = min_presses(rows, target, cache, cache2)
    # print("-" * 50)
    # print("Machine", counter)
    # print(ans)
    total += ans


print("---")
print(total)
