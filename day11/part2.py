import sys


g = {}

for line in sys.stdin.read().split("\n"):
    if not line:
        continue

    a, rest = line.split(": ")
    rest = rest.split(" ")

    g[a] = rest


def dfs(node, dac, fft, mem, path):
    if node == "out":
        # print(f"path: {path}")
        if dac and fft:
            print(f"path: {path}")
            return 1
        return 0

    if node == "dac":
        dac = True
    elif node == "fft":
        fft = True

    count = 0

    for n in g[node]:
        inc = dfs(n, dac, fft, mem, path + [n])
        count += inc
        print(f"added {inc} from {n}")

    mem[node] = count
    return count


mem = {}
count = dfs("svr", False, False, mem, ["svr"])

print(count)
