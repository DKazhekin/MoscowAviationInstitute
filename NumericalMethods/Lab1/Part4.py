"""
This program implements the Jacobi rotation method to find the eigenvalues and eigenvectors of a symmetric matrix.
You need to provide the matrix A and the tolerance value eps. The program will return the eigenvalues and eigenvectors
Don't forget to instanciate the Eigen class with the matrix A and the tolerance value eps. Then, call the method
"""

import numpy as np


class Eigen:

    def __init__(self, A: np.ndarray, eps: float):
        self.A = A
        self.eps = eps

    @property
    def A(self):
        return self._A

    # Checking matrix A to be symmetric
    @A.setter
    def A(self, matrix: np.ndarray):
        for i in range(matrix.shape[0]):
            for j in range(i + 1, matrix.shape[1]):
                if matrix[i, j] != matrix[j, i]:
                    raise ValueError("Matrix must be symmetric")

        self._A = matrix

    def jacobi_rotation(self):

        A = self.A.copy()
        U_s = []
        n = len(A)

        while True:

            # Find the maximum off-diagonal element
            max_off_diag = float('-inf')
            p, q = 0, 0
            for i in range(n):
                for j in range(i + 1, n):
                    if abs(A[i, j]) > max_off_diag:
                        max_off_diag = abs(A[i, j])
                        p, q = i, j

            # Compute the rotation angle
            theta = 0.5 * np.arctan(2 * A[p, q] / (A[p, p] - A[q, q])) if A[p, p] != A[q, q] else np.pi / 4

            # Compute the rotation matrix
            c = np.cos(theta)
            s = np.sin(theta)
            U = np.eye(n)
            U[p, p] = c
            U[q, q] = c
            U[p, q] = -s
            U[q, p] = s

            # Update A and U
            A = U.T @ A @ U
            U_s.append(U)

            # Check the converegence as Frobenius upper triangular matrix, as it symmetric
            conv = 0
            for i in range(n):
                for j in range(i + 1, n):
                    conv += A[i, j] ** 2
            conv = np.sqrt(conv)

            if conv < self.eps:
                # Returning Eigenvalues, Eigenvectors
                return np.diag(A), np.linalg.multi_dot(U_s)


if __name__ == "__main__":
    A = np.array([[2, -2, 3],
                  [-2, 5, 4],
                  [3, 4, 2]])

    solver = Eigen(A, 0.001)
    eigenvalues, eigenvectors = solver.jacobi_rotation()

    print("Eigenvalues:", eigenvalues)
    print("Eigenvectors:\n", eigenvectors)
