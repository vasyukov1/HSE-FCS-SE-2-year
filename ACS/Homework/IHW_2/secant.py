def f(x):
    return x**5 - x - 0.2

def secant(start_left, start_right, eps):
    left = start_left
    right = start_right

    while abs(right - left) > eps:
        c = left - (f(left) * (right - left)) / (f(right) - f(left))
        f_c = f(c)
        if abs(f_c) < eps:
            return c

        if f_c > 0:
            right = c
        else:
            left = c

    return c
        
def main():
    eps = 0.001
    left = 1.0
    right = 1.1

    if f(left) * f(right) >= 0:
        print(f"There is not the solution between {left} and {right}")
        return

    for i in range(6):
        res = secant(left, right, eps)
        print(f"Solution for {eps:.10f}: {res:.10f}")
        eps /= 10
        

if __name__ == "__main__":
    main()


