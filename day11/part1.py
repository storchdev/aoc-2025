import sys


g = {}

for line in sys.stdin.read().split("\n"):
    if not line:
        continue

    a, rest = line.split(": ")
    rest = rest.split(" ")

    g[a] = rest

count = 0


def dfs(node):
    global count
    if node == "out":
        count += 1
        return

    for n in g[node]:
        dfs(n)


dfs("you")

print(count)
