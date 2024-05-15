"""
Approximating a set of points with a polynomial of degree 1 and 2 using the least squares method.
"""

import matplotlib.pyplot as plt
import numpy as np
from NumericalMethods.Lab1.Part1 import Linalg


def least_squares(x, y, n):
    A = []
    b = []

    # Defining matrix A and vector b for solving system of linear equations
    for k in range(n + 1):
        A.append([sum(map(lambda x: x ** (i + k), x)) for i in range(n + 1)])
        b.append(sum(map(lambda x: x[0] * x[1] ** k, zip(y, x))))

    # Solving system of linear equations
    return Linalg(np.array(A), np.array(b)).solveSLQ()


# Polynomial function
def P(coefs, x):
    return sum([c * x ** i for i, c in enumerate(coefs)])


# Calculating the sum of squared errors
def sum_squared_errors(x, y, ls_coefs):
    y_ls = [P(ls_coefs, x_i) for x_i in x]
    return sum((y_i - y_ls_i) ** 2 for y_i, y_ls_i in zip(y, y_ls))


if __name__ == '__main__':
    x = [-0.7, -0.4, -0.1, 0.2, 0.5, 0.8]
    y = [-0.7754, -0.41152, -0.10017, 0.20136, 0.5236, 0.9273]
    plt.scatter(x, y, color='r')
    plt.plot(x, y, color='c', label='original')

    print('Least squares method, degree = 1')
    ls1 = least_squares(x, y, 1)
    print(f'P(x) = {ls1[0]} + {ls1[1]}x')
    plt.plot(x, [P(ls1, x_i) for x_i in x], color='b', label='degree = 1')
    print(f'Sum of squared errors = {sum_squared_errors(x, y, ls1)}')

    print('Least squares method, degree = 2')
    ls2 = least_squares(x, y, 2)
    print(f'P(x) = {ls2[0]} + {ls2[1]}x + {ls2[2]}x^2')
    plt.plot(x, [P(ls2, x_i) for x_i in x], color='g', label='degree = 2')
    print(f'Sum of squared errors = {sum_squared_errors(x, y, ls2)}')

    plt.legend()
    plt.show()
