"""
In this program you need to instanciate the solver class 'linalg' with your coefficient matrix A and bias vector b.
Further, it has some useful methods.
You can do LU decomposition, find an inverted matrix A, compute the determinant of A
and solve the system of linear equations.
"""

import numpy as np
import pandas as pd


class Linalg:

    def __init__(self, A: np.ndarray, b: np.ndarray):
        self.A = A
        self.b = b
        self.permutations = 0

    # LU decomposition
    # L - lower triangular matrix of factors
    # U - upper triangular matrix after forward Gauss pass
    def LU(self):
        n = self.A.shape[0]
        P = np.eye(n)
        L = np.zeros((n, n))
        U = self.A.copy()
        for i in range(n - 1):

            # Finding the pivot element with the maximum abs value for performing robust calculations
            index = np.argmax(np.abs(U[i:, i])) + i
            Linalg.swap(P, i, index)
            Linalg.swap(U, i, index)
            Linalg.swap(L, i, index)

            # Zeroing pivot underlyiers
            for j in range(i + 1, n):
                factor = - U[j, i] / U[i, i]
                U[j] = U[i] * factor + U[j]
                L[j, i] = -factor

        for i in range(n):
            L[i, i] = 1

        return P, L, U

    def solveSLQ(self):

        n = self.A.shape[0]
        P, L, U = self.LU()

        # Rotate b according to P
        b = P @ self.b

        # Firstly we need to solve Lz = b -> get z
        z = np.zeros(n)
        for i in range(n):
            z[i] = (b[i] - np.dot(L[i, :i], z[:i])) / (L[i, i])

        # After we need to solve Ux = z -> get x
        x = np.zeros(n)
        for i in range(n - 1, -1, -1):
            x[i] = (z[i] - np.dot(U[i, i:], x[i:])) / (U[i, i])

        return x

    # Determinant function
    def determinant(self):

        n = self.A.shape[0]
        A = self.A.copy()
        permutations = 0

        for i in range(n - 1):

            if A[i, i] == 0:
                index = np.argmax(np.abs(A[i:, i])) + i
                Linalg.swap(A, i, index)

            for j in range(i + 1, n):
                factor = - A[j, i] / A[i, i]
                A[j] = A[i] * factor + A[j]

        determinant = 1
        for i in range(n):
            determinant *= A[i, i]

        return determinant * (-1) ** permutations

    # Finding iverse matrix with Gauss method
    def inverse(self):

        A = self.A.copy()
        n = A.shape[0]
        E = np.eye(n)
        P = np.eye(n)
        for i in range(n - 1):

            index = np.argmax(np.abs(A[i:, i])) + i
            Linalg.swap(A, i, index)
            Linalg.swap(E, i, index)
            Linalg.swap(P, i, index)

            for j in range(i + 1, n):
                factor = - A[j, i] / A[i, i]
                A[j] = A[i] * factor + A[j]
                E[j] = E[i] * factor + E[j]

        X = []
        for i in range(n):
            e = E[:, i]
            x = np.zeros(n)
            for j in range(n - 1, -1, -1):
                x[j] = (e[j] - np.dot(A[j, j:], x[j:])) / (A[j, j])
            X.append(x)
        X = np.vstack(X).transpose()
        return X

    @staticmethod
    def swap(matrix: np.ndarray, curIndex: int, index: int, counter=None):
        tmp = matrix[curIndex].copy()
        matrix[curIndex] = matrix[index]
        matrix[index] = tmp

        if counter:
            counter += 1


if __name__ == "__main__":
    A = np.array([[-4, -9, 4, 3], [2, 7, 9, 8], [4, -4, 0, -2], [-8, 5, 2, 9]], dtype=float)
    b = np.array([-51, 76, 26, -73], dtype=float)
    l = Linalg(A, b)

    print("The original Matrix A: ")
    A = pd.DataFrame(A).to_string(index=False, header=False)
    print(A)

    print()

    det = l.determinant()
    print("Determinant of the matrix A:", det)

    print()

    P, L, U = l.LU()
    P = pd.DataFrame(P).to_string(index=False, header=False)
    L = pd.DataFrame(L).to_string(index=False, header=False)
    U = pd.DataFrame(U).to_string(index=False, header=False)
    print("Rotation matrix P: ")
    print(P)

    print()

    print("Lower triangular matrix L: ")
    print(L)

    print()

    print("Upper triangular matrix U: ")
    print(U)

    print()

    print("Inversed matrix A: ")
    I = l.inverse()
    I = pd.DataFrame(I).to_string(index=False, header=False)
    print(I)

    print()

    print("The solution of the system of linear equations: ")
    S = l.solveSLQ()
    S = pd.DataFrame(S).to_string(index=False, header=False)
    print(S)
