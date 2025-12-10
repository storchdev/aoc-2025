import sys

lns = sys.stdin.read()
lns = lns.split("\n")[:-1]


class Vec:
    def __init__(self, a, b, c):
        self.a = a
        self.b = b
        self.c = c

    def __eq__(self, o):
        return o.a == self.a and o.b == self.b and o.c == self.c

    def dist(self, o):
        return sum(map(lambda x: x * x, [self.a - o.a, self.b - o.b, self.c - o.c]))

    def __repr__(self):
        return f"<{self.a},{self.b},{self.c}>"

    def __hash__(self):
        return hash(self.a) + hash(self.b) + hash(self.c)


lns = [Vec(*(map(int, ln.split(",")))) for ln in lns]

cs = [{v} for v in lns]
vis = set()

dists = []
n = len(lns)
for i in range(n - 1):
    for j in range(i, n):
        dists.append((lns[i], lns[j]))

dists.sort(reverse=False, key=lambda t: t[0].dist(t[1]))

# di = 0
i = 0
while i < 10:
    v1, v2 = dists[i]
    # if (v1,v2) in vis:
    #     continue
    # vis.add((v1,v2))

    i1 = 0
    i2 = 0
    for i, c in enumerate(cs):
        if v1 in c:
            i1 = i
        if v2 in c:
            i2 = i

    if i1 == i2:
        # i += 1
        continue

    cs[i1] |= cs[i2]
    cs.pop(i2)

    i += 1

cs.sort(key=lambda c: len(c), reverse=True)

print(cs)

print(len(cs[0]) * len(cs[1]) * len(cs[2]))
