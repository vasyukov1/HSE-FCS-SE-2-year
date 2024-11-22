import math
import numpy as np
import matplotlib.pyplot as plt

def f(x, y, cx, cy, r):
    return (x - cx)**2 + (y - cy)**2 <= r**2

def areaMonteCarlo(N, bounds, circles):
    x_min, x_max, y_min, y_max = bounds
    points = np.random.uniform([x_min, y_min], [x_max, y_max], size=(N, 2))
    inside = np.all([f(points[:, 0], points[:, 1], cx, cy, r) for cx, cy, r in circles], axis=0)
    S_rect = (x_max - x_min) * (y_max - y_min)
    return S_rect * np.sum(inside) / N


circles = [(1, 1, 1), (1.5, 2, 5**0.5 / 2), (2, 1.5, 5**0.5 / 2)]
bounds_1 = (0, 3, 0, 3)
bounds_2 = (2 - 5**0.5 / 2, 2, 2 - 5**0.5 / 2, 2)

exact_area = 0.25 * math.pi + 1.25 * math.asin(0.8) - 1

N_values = range(100, 100001, 500)
areas_1 = []
relative_errors_1 = []
areas_2 = []
relative_errors_2 = []

for N in N_values:
    estimated_area_1 = areaMonteCarlo(N, bounds_1, circles)
    areas_1.append(estimated_area_1)
    relative_error = abs(estimated_area_1 - exact_area) / exact_area * 100
    relative_errors_1.append(relative_error)

    estimated_area_2 = areaMonteCarlo(N, bounds_2, circles)
    areas_2.append(estimated_area_2)
    relative_error = abs(estimated_area_2 - exact_area) / exact_area * 100
    relative_errors_2.append(relative_error)

plt.figure(figsize=(10, 6))
plt.plot(N_values, areas_1, label='Способ 1', color='blue')
plt.plot(N_values, areas_2, label='Способ 2', color='red')
plt.axhline(y=exact_area, color='black', linestyle='--', label='Точная площадь')
plt.xlabel('Количество точек N')
plt.ylabel('Площадь')
plt.title('Зависимость приближенной площади от количества точек')
plt.legend()
plt.grid()
plt.show()

plt.figure(figsize=(12, 6))
plt.plot(N_values, relative_errors_1, label='Способ 1', color='blue')
plt.plot(N_values, relative_errors_2, label='Способ 2', color='red')
plt.xlabel('Количество точек N')
plt.ylabel('Отклонение (%)')
plt.title('Зависимость относительного отклонения от количества точек')
plt.legend()
plt.grid()
plt.show()
