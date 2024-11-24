import numpy as np
import matplotlib.pyplot as plt

def load_data(filename):
    data = np.loadtxt(filename)
    size = data[:, 0]
    time = data[:, 1] 
    return size, time

def plot_merge_all(file1, file2, file3, name):
    size1, time1 = load_data(file1)
    size2, time2 = load_data(file2)
    size3, time3 = load_data(file3)

    plt.figure(figsize=(10, 6))
    plt.plot(size1, time1, label='Random Array', linestyle='-')
    plt.plot(size2, time2, label='Reversed Array', linestyle='-')
    plt.plot(size3, time3, label='Nearly Sorted Array', linestyle='-')

    plt.title(f"{name} Sort Performance", fontsize=16)
    plt.xlabel("Array Size", fontsize=14)
    plt.ylabel("Execution Time (ms)", fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True)
    plt.tight_layout()

    plt.show()

def plot_tracks_one(file, name, array_name):
    size1, time1 = load_data(file)

    plt.figure(figsize=(10, 6))
    plt.plot(size1, time1, label=f"{array_name} Array", linestyle='-')

    plt.title(f"{name} Sort Performance", fontsize=16)
    plt.xlabel("Array Size", fontsize=14)
    plt.ylabel("Execution Time (ms)", fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True)
    plt.tight_layout()

    plt.show()

def load_big_data(filename):
    data = np.loadtxt(filename)
    size = data[:, 0]
    time1 = data[:, 1]
    time2 = data[:, 2]
    time3 = data[:, 3]
    time4 = data[:, 4]
    time5 = data[:, 5]
    return size, time1, time2, time3, time4, time5

def plot_hybrid(file, name, array_name):
    size1, time1, time2, time3, time4, time5 = load_big_data(file)

    plt.figure(figsize=(10, 6))
    plt.plot(size1, time1, label="Threshold 5", linestyle='-')
    plt.plot(size1, time2, label="Threshold 10", linestyle='-')
    plt.plot(size1, time3, label="Threshold 20", linestyle='-')
    plt.plot(size1, time4, label="Threshold 30", linestyle='-')
    plt.plot(size1, time5, label="Threshold 50", linestyle='-')

    plt.title(f"{name} Sort Performance\n{array_name} Array", fontsize=16)
    plt.xlabel("Array Size", fontsize=14)
    plt.ylabel("Execution Time (ms)", fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True)
    plt.tight_layout()

    plt.show()

file1 = 'merge_sort_random.txt'
file2 = 'merge_sort_reversed.txt'
file3 = 'merge_sort_nearly_sorted.txt'

file4 = 'hybrid_sort_random.txt'
file5 = 'hybrid_sort_reversed.txt'
file6 = 'hybrid_sort_nearly_sorted.txt'

# plot_merge_all(file1, file2, file3, "Merge")

# plot_tracks_one(file1, "Merge", "Random")
# plot_tracks_one(file2, "Merge", "Reversed")
# plot_tracks_one(file3, "Merge", "Nearly Sorted")

plot_hybrid(file4, "Merge + Insertion", "Random")
plot_hybrid(file5, "Merge + Insertion", "Reversed")
plot_hybrid(file6, "Merge + Insertion", "Nearly Sorted")