import random
import numpy as np
import matplotlib.pyplot as plt

class HashTable:
    def __init__(self, size, probing, c1 = 1, c2 = 1, c3 = 1):
        self.size = size
        self.table = [None] * size
        self.probing = probing
        self.c1 = c1
        self.c2 = c2
        self.c3 = c3

    def hash(self, key):
        return key % self.size
    
    def probe(self, key, i):
        if self.probing == 'quadratic':
            return (key + self.c1 * i + self.c2 * i**2) % self.size
        elif self.probing == 'cubic':
            return (key + self.c1 * i * self.c2 * i**2 + self.c3 * i**3) % self.size
        
    def insert(self, key):
        for i in range(self.size):
            index = self.probe(key, i)
            if self.table[index] is None:
                self.table[index] = key
                return i + 1
        return -1
    
    def get_cluster_sizes(self):
        clusters = []
        cluster_size = 0

        for cell in self.table:
            if cell is None:
                clusters.append(cluster_size)
                cluster_size = 0
            else:
                cluster_size += 1
        
        if cluster_size > 0:
            clusters.append(cluster_size)

        return clusters
    
def work(size, numbers_count, method):
    table = HashTable(size, method)
    keys = random.sample(range(10 * size), numbers_count)
    attempts = [table.insert(k) for k in keys]
    clusters = table.get_cluster_sizes()
    return np.mean(attempts), np.mean(clusters), np.max(clusters)

def graphic(values1, values2, title):
    plt.figure(figsize=(8, 5))
    plt.plot(sizes, values1, marker='o', label=f'Quadratic {title}')
    plt.plot(sizes, values2, marker='s', label=f'Cubic {title}')
    plt.xlabel("Hash Table Size")
    plt.ylabel(f"{title} per Insert")
    plt.title(f"Comparison of Quadratic and Cubic {title}")
    plt.legend()
    plt.grid()
    plt.show()

if __name__ == "__main__":  
    sizes = [151, 209, 523, 599, 1001, 2003, 5003]
    numbers_count = [int(s * 0.75) for s in sizes]

    quad_attempts_list = []
    cubic_attempts_list = []

    quad_clusters_list = []
    cubic_clusters_list = []

    quad_max_cluster_list = []
    cubic_max_cluster_list = []

    for size, nk in zip(sizes, numbers_count):
        quad_attempts, quad_clusters, quad_max_cluster = work(size, nk, 'quadratic')
        cubic_attempts, cubic_clusters, cubic_max_cluster = work(size, nk, 'cubic')
        print(f"Table size {size}")
        print(f"\tQuadratic: Avg attempts\t {quad_attempts:.2f}\t Avg cluster {quad_clusters:.4f}\t Max cluster {quad_max_cluster:.4f}")
        print(f"\tCubic: Avg attempts\t {cubic_attempts:.2f}\t Avg cluster {cubic_clusters:.4f}\t Max cluster {cubic_max_cluster:.4f}\n")

        quad_attempts_list.append(quad_attempts)
        cubic_attempts_list.append(cubic_attempts)
        quad_clusters_list.append(quad_clusters)
        cubic_clusters_list.append(cubic_clusters)
        quad_max_cluster_list.append(quad_max_cluster)
        cubic_max_cluster_list.append(cubic_max_cluster)
        
    graphic(quad_attempts_list, cubic_attempts_list, "Probing")
    graphic(quad_clusters_list, cubic_clusters_list, "Cluster Size")
    graphic(quad_max_cluster_list, cubic_max_cluster_list, "Max Cluster")
