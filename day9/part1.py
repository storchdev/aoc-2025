import sys


lns = sys.stdin.read().split("\n")[:-1]

coords = []
for ln in lns:
    a, b = ln.split(",")
    coords.append((int(a), int(b)))


areas = []
for a, b in coords:
    for c, d in coords:
        areas.append(abs(c - a + 1) * abs(d - b))

print(coords)
print(max(areas))
