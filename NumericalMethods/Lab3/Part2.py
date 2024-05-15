"""
Spline interpolation. It better for interpolation of large amount of points. It uses cubic polynoms for interpolation.
Cubic spline is a piecewise-defined function consisting of polynomial pieces for each pair of the nearest x's, each of which has degree 3.
"""
import numpy as np
import matplotlib.pyplot as plt
from NumericalMethods.Lab1.Part2 import RunThrough


def s(a, b, c, d, x):
    return a + b * x + c * x ** 2 + d * x ** 3


def spline_interpolation(x, y, x_test):
    n = len(x)

    # Store the differences between x[i] and x[i-1] to create tridiagonal matrix
    h = [x[i] - x[i - 1] for i in range(1, len(x))]
    A = [[0 for _ in range(len(h) - 1)] for _ in range(len(h) - 1)]
    A[0][0] = 2 * (h[0] + h[1])
    A[0][1] = h[1]
    for i in range(1, len(A) - 1):
        A[i][i - 1] = h[i - 1]
        A[i][i] = 2 * (h[i - 1] + h[i])
        A[i][i + 1] = h[i]
    A[-1][-2] = h[-2]
    A[-1][-1] = 2 * (h[-2] + h[-1])

    # Getting the vector of m coefs to solve tridiagonal system
    m = [3.0 * ((y[i + 1] - y[i]) / h[i] - (y[i] - y[i - 1]) / h[i - 1]) for i in range(1, len(h))]

    # Solution of tridiagonal system
    c = [0] + RunThrough(np.array(A), np.array(m)).runThrough().tolist()

    # Restoring coeficients with special formulas
    a = [y[i - 1] for i in range(1, n)]
    b = [(y[i] - y[i - 1]) / h[i - 1] - (h[i - 1] / 3.0) * (2.0 * c[i - 1] + c[i]) for i in range(1, len(h))]
    b.append((y[-1] - y[-2]) / h[-1] - (2.0 * h[-1] * c[-1]) / 3.0)
    d = [(c[i] - c[i - 1]) / (3.0 * h[i - 1]) for i in range(1, len(h))]
    d.append(-c[-1] / (3.0 * h[-1]))

    #  Estimating which interval x_test belongs to
    for interval in range(len(x) - 1):
        if x[interval] <= x_test < x[interval + 1]:
            i = interval
            break
    # Finding the y_test value
    y_test = s(a[i + 1], b[i + 1], c[i + 1], d[i + 1], x_test - x[i])
    return a, b, c, d, y_test


def draw_plot(x_original, y_original, a, b, c, d):
    """
    Plot spline and original points
    """
    x, y = [], []
    for i in range(len(x_original) - 1):
        x1 = np.linspace(x_original[i], x_original[i + 1], 10)
        y1 = [s(a[i], b[i], c[i], d[i], j - x_original[i]) for j in x1]
        x.append(x1)
        y.append(y1)

    plt.scatter(x_original, y_original, color='r')
    for i in range(len(x_original) - 1):
        plt.plot(x[i], y[i], color='b')
    plt.show()


if __name__ == '__main__':
    x = [-0.4, -0.1, 0.2, 0.5, 0.8]
    y = [-0.41152, -0.10017, 0.20136, 0.52360, 0.92730]
    x_test = 0.1

    a, b, c, d, y_test = spline_interpolation(x, y, x_test)
    for i in range(len(x) - 1):
        print(f'[{x[i]}; {x[i + 1]})')
        print(f's(x) = {a[i]} + {b[i]}(x - {x[i]}) + {c[i]}(x - {x[i]})^2 + {d[i]}(x - {x[i]})^3')
    print(f's(x_test) = s({x_test}) = {y_test}')
    draw_plot(x, y, a, b, c, d)
