"""
This program implements the Simple Iterations and Seidel Iterations methods for solving the system of linear equations.
You need to instanciate a solver class 'Iterations' with your coefficient matrix A and bias vector b
"""

import numpy as np


class Iterations:

    def __init__(self, A, b):
        self.A = A
        self.b = b

    def simpleIterations(self):
        # Jacobi's method as matrix is diagonally dominant
        eps = 1e-3
        n = self.A.shape[0]

        D = np.diagflat(np.diagonal(self.A))
        R = self.A - D

        x_old = np.zeros(n)

        # The first approaching
        x_new = np.linalg.inv(D).dot(b - R.dot(x_old))

        print("Residual Norm: ", np.linalg.norm(x_new - x_old))
        print("X :", x_new)

        # In this method we create fixed Residual Matrix which uses to calculate unit of substracting a linear part
        # of each equation from beta coeficients except diagonal. And then calculating x's analyticaly with invering matrix D
        # wich stores coefficents
        converge = False
        while not converge:
            x_old = x_new.copy()
            x_new = np.linalg.inv(D).dot(b - R.dot(x_old))

            # Swtiching convergences
            if np.linalg.norm(self.A) < 1:
                converge = (np.linalg.norm(self.A) / (1 - np.linalg.norm(self.A))) * np.linalg.norm(
                    x_new - x_old) <= eps
            else:
                converge = np.linalg.norm(x_new - x_old) <= eps
            print("Residual Norm: ", np.linalg.norm(x_new - x_old))
            print("X :", x_new)

    def Seidel(self):

        eps = 1e-3
        n = self.A.shape[0]
        U = np.triu(self.A, 1)
        L = np.tril(self.A, -1)
        D = np.diagflat(np.diagonal(self.A))

        x_old = np.zeros(n)
        x_new = np.zeros(n)
        while True:

            for i in range(n):
                # Calculating x[i] value and using previous values in the newest ones using Upper and Lower triangular matrices
                # Upgraded Simple Iteration metho
                x_new[i] = (1 / D[i][i]) * (self.b[i] - np.dot(U[i], x_new) - np.dot(L[i], x_new))

            if (np.linalg.norm(U) / 1 - np.linalg.norm(self.A)) * np.linalg.norm(x_new - x_old) < eps:
                break
            else:
                x_old = x_new.copy()
                print("Residual Norm: ", np.linalg.norm(x_new - x_old))
                print("X :", x_new)


if __name__ == "__main__":
    A = np.array([[-7, -1, 2, 2], [3, -20, 0, -8], [-9, 1, 18, -6], [-1, 0, -1, -6]],
                 dtype=float)
    b = np.array([-24, -47, 28, -50], dtype=float)

    s = Iterations(A, b)
    print("Simple Iterations")
    s.simpleIterations()
    print()
    print("Seidel Iterations")
    s.Seidel()
