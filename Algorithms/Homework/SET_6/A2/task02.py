"""
A --> B
/\    |
|     \/
D <-- C
"""

n = 4
dist = [[float('inf') for _ in range(n)] for _ in range(n)]

dist[0][1] = 1 # A -> B = 1
dist[1][2] = 5 # B -> C = 2
dist[2][3] = 4 # C -> D = 4
dist[3][0] = 4 # D -> A = 2

def print_operation(i, j, k):
    print(f"\ndist[{i}][{j}] = min(dist[{i}][{j}], dist[{i}][{k}] + dist[{k}][{j}]) = {dist[i][j]}")

def print_trace():
    print('\tA\tB\tC\tD')
    for i in range(n):
        if i == 0:
            print('A\t', end="")
        elif i == 1:
            print('\nB\t', end="")
        elif i == 2:
            print('\nC\t', end="")
        else:
            print('\nD\t', end="")
        for j in range(n):
            print(f"{dist[i][j]}\t", end="")
    print()

def FloydWarshall():
    for i in range(n):
        for j in range(n):
            for k in range(n):
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
                print_operation(i, j, k)
                print_trace()

FloydWarshall()
