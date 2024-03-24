"""
In this program you can solve the system of linear equations, where a coefficient matrix has a special form of 3-Diagonal matrix.
So, you need to instanciate a solver class 'RunThrough' with your coefficient matrix A and bias vector b.
"""

import numpy as np
import pandas as pd


class RunThrough:

    def __init__(self, A, b):
        self._A = A
        self.b = b

    @property
    def A(self):
        return self._A

    @A.setter
    def A(self, value):
        if self.checkRobust():
            self._A = value
        else:
            raise ValueError("Check the robust of matrix A")

    def checkRobust(self):

        n = self.A.shape[0]
        for i in range(n):

            if i < n - 1 and (self.A[i + 1][i] == 0 or self.A[i][i + 1] == 0):
                return 0

            else:
                if i == 0:
                    if abs(self.A[i][i]) < abs(A[i][i + 1]):
                        return 0
                elif i < n - 1 and (abs(self.A[i][i]) < abs(A[i][i + 1]) + abs(A[i][i - 1])):
                    return 0
                elif i == n - 1 and (abs(self.A[n - 1][n - 1]) < abs(A[n - 1][n - 2])):
                    return 0
        return 1

    def runThrough(self):

        # Forward pass
        n = self.A.shape[0]
        P = np.zeros(n)
        Q = np.zeros(n)

        for i in range(n):

            if i == 0:
                P[0] = -self.A[0][1] / self.A[0][0]
                Q[0] = self.b[0] / self.A[0][0]

            elif i == n - 1:
                Q[n - 1] = (self.b[n - 1] - self.A[n - 1][n - 2] * Q[n - 2]) / (
                        self.A[n - 1][n - 1] + self.A[n - 1][n - 2] * P[n - 2])

            else:

                P[i] = -self.A[i][i + 1] / (self.A[i][i] + self.A[i][i - 1] * P[i - 1])
                Q[i] = (self.b[i] - self.A[i][i - 1] * Q[i - 1]) / (self.A[i][i] + self.A[i][i - 1] * P[i - 1])


        # Backward pass
        x = np.zeros(n)
        for i in range(n - 1, -1, -1):

            if i == n - 1:
                x[i] = Q[i]
            else:
                x[i] = P[i] * x[i + 1] + Q[i]

        return x


A = np.array([[-11, -8, 0, 0, 0], [9, -17, 1, 0, 0], [0, -4, 20, 9, 0], [0, 0, -4, -14, 3], [0, 0, 0, -6, 14]], dtype=float)
b = np.array([99, -75, 66, 54, 8], dtype=float)
solve = RunThrough(A, b)
print("X: ", solve.runThrough())
