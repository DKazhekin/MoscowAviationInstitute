"""
This functionality provides methods for solving non-linear systems of equations with Newton and Simple Iteraion numerical methods
"""

import sys
import numpy as np
import matplotlib.pyplot as plt
from NumericalMethods.Lab1.Part1 import Linalg


# Newton functions
def function1(x1, x2):
    return pow(x1, 2) + pow(x2, 2) - 9


def function2(x1, x2):
    return x1 - pow(np.e, pow(x2, 2)) + 3


# Simple iteration phi functions for finding x1_new and x2_new
def phi1(x2):
    return np.sqrt(9 - x2 ** 2)


def phi2(x1):
    return np.sqrt(np.log(x1 + 3))


class Newton:

    def __init__(self, function1, function2, x1_0, x1_1, x2_0, x2_1, eps):
        self.function1 = function1
        self.function2 = function2
        self.x1_0 = x1_0
        self.x1_1 = x1_1
        self.x2_0 = x2_0
        self.x2_1 = x2_1
        self.eps = eps
        self.iterations = None

    # Jacobi matrix instantiating for derivatives on each function
    @staticmethod
    def Jacobi(x1, x2):
        Jacobi = np.array([[2 * x1, 2 * x2], [1, -(pow(np.e, x2 ** 2)) * 2 * x2]])
        return Jacobi

    # Checking non-null determinant for convergence
    def check_constraints(self, x1, x2):
        if Linalg(self.Jacobi(x1, x2), None).determinant() == 0:
            raise "Error! J has zero determinant!"
        return True

    # Solving function
    def solve(self):

        # Count Iterations
        self.iterations = 0

        # Initial value
        x_prev = np.array([(x1_0 + x1_1) / 2, (x2_0 + x2_1) / 2]).reshape(-1, 1)

        # Checking constraints
        self.check_constraints(x_prev[0][0], x_prev[1][0])
        while True:

            # New value calculating
            x_new = x_prev - np.matmul(Linalg(self.Jacobi(x_prev[0][0], x_prev[1][0]), None).inverse(),
                                       np.array([[self.function1(x_prev[0][0], x_prev[1][0])],
                                                 [self.function2(x_prev[0][0], x_prev[1][0])]]).reshape(-1, 1))

            self.iterations += 1
            # Checking constraints
            self.check_constraints(x_new[0][0], x_new[1][0])

            # If convergence -> then return answer
            if np.linalg.norm(x_new - x_prev) < self.eps:
                return x_new

            # Updating answer
            x_prev = x_new


class SimpleIter:

    def __init__(self, phi1, phi2, x1_0, x1_1, x2_0, x2_1, eps):
        self.phi1 = phi1
        self.phi2 = phi2
        self.x1_0 = x1_0
        self.x1_1 = x1_1
        self.x2_0 = x2_0
        self.x2_1 = x2_1
        self.eps = eps
        self.iterations = None

    # Function to get norm of matrix of all derivatives
    @staticmethod
    def Jacobian(x1, x2):
        J = np.array([[0, -x2 / np.sqrt(9 - x2 ** 2)], [1 / (np.sqrt(np.log(x1 + 3)) * (2 * x1 + 6)), 0]])
        return np.linalg.norm(J)

    # Function to check Theorem: answer exists if for every dot in space grid Jacobian less than 1
    # Also we calculate maximum_Jacobian to take it for q parameter
    def check_constraints(self):
        X1, X2 = np.meshgrid(np.arange(self.x1_0, self.x1_1, 0.01), np.arange(self.x2_1, self.x2_0, 0.01))
        maximum_jacobian = float('-inf')

        for pair in np.array(list(zip(X1.flatten(), X2.flatten()))):
            x1, x2 = pair
            Jacobian = self.Jacobian(x1, x2)
            if Jacobian > maximum_jacobian:
                maximum_jacobian = Jacobian

            if Jacobian >= 1:
                raise "Jacobian value should be less than 1!"

        return (maximum_jacobian + 1) / 2

    def solve(self):

        # Count iterations
        self.iterations = 0

        # Q parameter initialization + checking theorem
        q = self.check_constraints()
        x1_prev, x2_prev = ((self.x1_0 + self.x1_1) / 2), ((self.x2_0 + self.x2_1) / 2)
        while True:

            x1_new, x2_new = phi1(x2_prev), -phi2(x1_prev)
            self.iterations += 1
            # print(q)
            if (q / (1 - q)) * np.linalg.norm(
                    np.array([x1_new, x2_new]).reshape(-1, 1) - np.array([x1_prev, x2_prev]).reshape(-1, 1)) < self.eps:
                return x1_new, x2_new

            x1_prev, x2_prev = x1_new, x2_new


if __name__ == '__main__':
    # Getting values:
    # 0 3
    # 0 -2
    # 0.00001

    print("Input x1 range of non negative root: ")
    x1_0, x1_1 = tuple(map(float, sys.stdin.readline().split()))

    print("Input x2 range of non negative root: ")
    x2_0, x2_1 = tuple(map(float, sys.stdin.readline().split()))

    print("Input epsilon for Newton and Simple Iter methods: ")
    eps = float(sys.stdin.readline())

    # Newton
    newtonObj = Newton(function1, function2, x1_0, x1_1, x2_0, x2_1, eps)
    newtonAnswer = newtonObj.solve()

    print("Newton: ", (newtonAnswer[0][0], newtonAnswer[1][0]))
    print("F1 = ", function1(newtonAnswer[0][0], newtonAnswer[1][0]))
    print("F2 = ", function2(newtonAnswer[0][0], newtonAnswer[1][0]))

    # Simple Iteration
    simIterObj = SimpleIter(phi1, phi2, x1_0, x1_1, x2_0, x2_1, eps)
    simpIterAnswer = simIterObj.solve()
    print("SimpIter: ", simpIterAnswer)
    print("F1 = ", function1(simpIterAnswer[0], simpIterAnswer[1]))
    print("F2 = ", function2(simpIterAnswer[0], simpIterAnswer[1]))
