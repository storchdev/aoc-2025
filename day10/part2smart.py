import sys
import re

sys.setrecursionlimit(1000000)


# /


def allsubsets(rows: list[list[int]], target: tuple[int], cache):
    if v := cache.get(target):
        return v

    target_bin = list(map(lambda n: int(n % 2), target))

    allsets = []

    for mask in range(1 << len(rows)):
        mut = [0] * len(target)
        this_set = set()

        for i in range(len(rows)):
            if (1 << i) & mask:
                mut = [a ^ b for a, b in zip(mut, rows[i])]
                this_set.add(i)

        if mut == target_bin:
            allsets.append(this_set)

    cache[target] = allsets
    return allsets


def min_presses(rows, target, cache, cache2):
    if target == [0] * len(target):
        # print(f"f({target}) = {0}")
        return 0

    k = tuple(target)
    if v := cache.get(k):
        # print(f"f({target}) = {v}")
        return v

    minsets = allsubsets(rows, k, cache2)

    minv = float("inf")
    for minset in minsets:
        newtarget = target.copy()

        for i in minset:
            newtarget = [a - b for a, b in zip(newtarget, rows[i])]

        if not all(x >= 0 for x in newtarget):
            continue

        # print(f"target after subtracting: {newtarget}")

        assert all(a % 2 == 0 for a in newtarget)
        newtarget = [x // 2 for x in newtarget]

        v = len(minset) + 2 * min_presses(rows, newtarget, cache, cache2)

        if v < minv:
            minv = v

    cache[k] = minv
    # print(f"f({target}) = {minv}")
    return minv


counter = 0
total = 0
for ln in sys.stdin.read().split("\n"):
    if not ln:
        continue
    if ln[0] != "[":
        continue
    counter += 1

    print("-" * 50)
    print("Machine", counter)
    matches1 = re.findall(r"(\((?:\d|,)+\)\s)", ln)
    matches2 = re.findall(r"\{.+\}", ln)[0].strip("{}")
    matches1 = [str(m).strip("() ") for m in matches1]

    # print(matches1)
    # print(matches2)

    target = [int(x) for x in matches2.split(",")]

    rows = []
    for s in matches1:
        row = [0] * len(target)
        for i in s.split(","):
            row[int(i)] = 1
        rows.append(row)

    # subset = allsubsets(rows, target)
    # print(subset)
    cache = {}
    cache2 = {}
    ans = min_presses(rows, target, cache, cache2)
    print(ans)
    total += ans


print("---")
print(total)
