"""
This functionality provides methods for solving non-linear equations with Newton and Simple Iteraion numerical methods
"""

import sys
import numpy as np
import matplotlib.pyplot as plt


# For Newton algorithm
def function_newton(x):
    return np.log(x + 1) - 2 * pow(x, 2) + 1


def dv1_newton(x):
    return (1 / (x + 1)) - 4 * x


def dv2_newton(x):
    return -1 * (1 / pow((x + 1), 2)) - 4


# Left part of f(x) = g(x)
def function1_newton(x):
    return np.log(x + 1)


# Right part of f(x) = g(x)
def function2_newton(x):
    return 2 * pow(x, 2) - 1


# For Simple Iteration algorithm x = phi(x)
def function_iter_phi(x):
    return (np.log(x + 1) + 1) / (2 * x)


def phi_dv(x):
    return (1 / (x + 1)) * 2 * x - 2 * (np.log(x + 1) + 1) / (4 * pow(x, 2))


class Plot:

    def __init__(self, function1, function2):
        self.function1 = function1
        self.function2 = function2

    def plot(self):
        x = np.linspace(-0.999, 10)
        y1 = self.function1(x)
        y2 = self.function2(x)

        fig, ax = plt.subplots()
        ax.plot(x, y1)
        ax.plot(x, y2)
        ax.set_title("Origin Function Visualisation")
        ax.set_xlabel("X")
        ax.set_ylabel("Y")
        ax.set_ylim(-5, 5)
        ax.set_xlim(-5, 10)
        ax.axhline(0, color='black', linewidth=0.5)
        ax.axvline(0, color='black', linewidth=0.5)
        plt.show()


class Newton:

    def __init__(self, function, dv1, dv2, x1, x2, eps):
        self.function = function
        self.dv1 = dv1
        self.dv2 = dv2
        self.x1 = x1
        self.x2 = x2
        self.eps = eps

    def get_initial(self):

        # Getting initial value from Theorem
        for x in np.arange(x1, x2, 0.1):
            if self.function(x) * self.dv2(x) > 0:
                return x

    def solve(self):

        x_prev = self.get_initial()
        if x_prev is None:
            raise "There is no initial value"

        while True:

            x_new = x_prev - (self.function(x_prev) / self.dv1(x_prev))
            if abs(x_new - x_prev) < self.eps:
                return x_new
            else:
                x_prev = x_new


class SimpleIter:

    def __init__(self, phi_function, phi_dv, x1, x2, eps):
        self.x1 = x1
        self.x2 = x2
        self.phi_function = phi_function
        self.phi_dv = phi_dv
        self.eps = eps

    def check_constraints(self):

        max_dv = float('-inf')
        for x in np.arange(x1, x2, 0.001):
            # Phi_function(x) for each x should be in the same x1:x2 range
            if not (x1 <= self.phi_function(x) <= x2):
                raise "Constraints are false, check your x1 and x2 values"

            # Finding max_dv of phi to calculate q parameter
            if abs(phi_dv(x)) > max_dv:
                max_dv = abs(phi_dv(x))

            # If max_dv > 1 than theorem constraints are False
            if max_dv >= 1:
                raise "Max derivative value is over 1! Check your x1 and x2 values"

        return max_dv

    def solve(self):

        # Finding q
        q = self.check_constraints()

        # Finding inital value as center of x1:x2 range
        x_prev = (self.x1 + self.x2) / 2
        while True:

            x_new = self.phi_function(x_prev)

            if (q / (1 - q)) * abs(x_new - x_prev) < self.eps:
                return x_new

            x_prev = x_new


if __name__ == '__main__':
    # Plotting section for graphic analysis
    pltObj = Plot(function1_newton, function2_newton)
    pltObj.plot()

    # Getting values
    # 0.7 1.1

    print("Input x range of non negative root: ")
    x1, x2 = tuple(map(float, sys.stdin.readline().split()))
    print("Input epsilon for Newton method: ")
    eps = float(sys.stdin.readline())

    # Newton method
    newtonObj = Newton(function_newton, dv1_newton, dv2_newton, x1, x2, eps)
    print("Newton: ", newtonObj.solve())

    # Simple Iteration method
    simpIterObj = SimpleIter(function_iter_phi, phi_dv, x1, x2, eps)
    print("Simple Iteration: ", simpIterObj.solve())
