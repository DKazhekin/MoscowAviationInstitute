"""
This program uses QR decomposition to find eigenvalues of a matrix.
Don't forget to change the matrix A to the one you want to find the eigenvalues of.
"""


import numpy as np


class Eigen:

    def __init__(self, A: np.ndarray):
        self.A = A

    def QR_decomposition(self, A: np.ndarray):
        """Performs QR decomposition using Householder reflections."""
        n = A.shape[0]
        H_s = []

        for k in range(n - 1):
            v = np.zeros(n)
            for i in range(k, n):
                if i == k:
                    v[i] = A[k, k] + np.sign(A[k, k]) * np.linalg.norm(A[k:, k])
                else:
                    v[i] = A[i, k]
            H = np.eye(n) - 2 * (np.outer(v, v) / np.dot(v, v))
            H_s.append(H)
            A = H @ A

        Q = np.linalg.multi_dot(H_s)
        return Q, A

    def algorithm(self, iterations=1000):
        """Itearatively applies QR decomposition to find eigenvalues of a matrix."""
        Ak = self.A.copy()
        for _ in range(iterations):
            Q, R = self.QR_decomposition(Ak)
            Ak = R @ Q
        return np.diag(Ak)


# Example matrix
A = np.array([[-9, 2, 2],
              [-2, 0, 7],
              [8, 2, 0]], dtype=float)

solution = Eigen(A)
Q, R = solution.QR_decomposition(A)
eigenvalues = solution.algorithm()
print("Eigenvalues: ", eigenvalues)
