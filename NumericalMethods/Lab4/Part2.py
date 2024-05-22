import numpy as np
import pandas as pd


# Equal form of origin function
def f(x, y, z):
    return np.tan(x) * z - 2 * y


def g(x, y, z):
    return z


# The first coeficent before z
def p(x):
    return -np.tan(x)


# The second coeficent before y
def q(x):
    return 2


# The right part of the equation
def f2(x):
    return 0


# Analytic solution
def yf(x):
    return np.sin(x) + 2 - np.sin(x) * np.log((1 + np.sin(x)) / (1 - np.sin(x)))


# Shooting method
def runge_kuty(y0, z0, xl, xr, h):
    x = [i for i in np.arange(xl, xr + h, h)]
    y = [y0]
    z = [z0]
    dz = []
    dy = []
    Kz = []
    Ky = []
    for i in range(len(x) - 1):
        L1 = h * f(x[i], y[i], z[i])
        K1 = h * g(x[i], y[i], z[i])
        L2 = h * f(x[i] + 0.5 * h, y[i] + 0.5 * K1, z[i] + 0.5 * L1)
        K2 = h * g(x[i] + 0.5 * h, y[i] + 0.5 * K1, z[i] + 0.5 * L1)
        L3 = h * f(x[i] + 0.5 * h, y[i] + 0.5 * K2, z[i] + 0.5 * L2)
        K3 = h * g(x[i] + 0.5 * h, y[i] + 0.5 * K2, z[i] + 0.5 * L2)
        L4 = h * f(x[i] + h, y[i] + K3, z[i] + L3)
        K4 = h * g(x[i] + h, y[i] + K3, z[i] + L3)
        Ky.append([K1, K2, K3, K4])
        Kz.append([L1, L2, L3, L4])
        dy.append((K1 + 2 * K2 + 2 * K3 + K4) / 6)
        dz.append((L1 + 2 * L2 + 2 * L3 + L4) / 6)
        y.append(y[i] + dy[i])
        z.append(z[i] + dz[i])

    return x, y, z


def shooting(x0, y0, x1, y1, h):
    eps = 0.0001
    tt = [1.0, 0.8]

    # Scoping
    xn1, yn1, zn1 = runge_kuty(y0, tt[0], x0, x1, h)
    xn2, yn2, zn2 = runge_kuty(y0, tt[1], x0, x1, h)
    y = [yn1[-1], yn2[-1]]

    # Accuracy
    Phi = [abs(yn1[-1] - y1), abs(yn2[-1] - y1)]
    i = 1
    # Shooting
    while abs(Phi[i]) > eps:
        i += 1

        # Method secushix
        tt.append(tt[i - 1] - ((tt[i - 1] - tt[i - 2]) / (Phi[i - 1] - Phi[i - 2])) * Phi[i - 1])
        xni, yni, zni = runge_kuty(y0, tt[i], x0, x1, h)
        Phi.append(abs(yni[-1] - y1))
        y.append(yni[-1])
    return tt, y, Phi, xni, yni


# Finite differences
def progon(a, b):
    n = len(a)
    P = np.zeros(n)
    Q = np.zeros(n)
    P[0] = -a[0][1] / a[0][0]
    Q[0] = b[0] / a[0][0]
    for i in range(1, n):
        denominator = a[i][i] + a[i][i - 1] * P[i - 1]
        if i == n - 1:
            P[i] = 0
        else:
            P[i] = -a[i][i + 1] / denominator
        Q[i] = (b[i] - a[i][i - 1] * Q[i - 1]) / denominator
    x = np.zeros(n)
    x[n - 1] = Q[n - 1]
    for i in range(n - 2, -1, -1):
        x[i] = P[i] * x[i + 1] + Q[i]
    return x


# Dividing our space on intervals and creating a system via derivative approximation to solve with a progon method
def finite_difference(x0, x1, y0, y1, h):
    x = [i for i in np.arange(x0, x1 + h, h)]
    A = []
    b = []
    for i in range(1, len(x)):
        if i == 1:
            a = []
            a.append(-2 + h * h * q(x[i]))
            a.append(1 + p(x[i]) * h / 2)
            for j in range(2, len(x)):
                a.append(0)
            A.append(a)
            b.append(h * h * f2(x[i]) - (1 - p(x[i]) * h / 2) * y0)
        elif i == len(x) - 1:
            a = []
            for j in range(1, len(x) - 2):
                a.append(0)
            a.append(1 - p(x[i]) * h / 2)
            a.append(-2 + h * h * q(x[i]))
            A.append(a)
            b.append(h * h * f2(x[i]) - (1 + p(x[i]) * h / 2) * y1)
        else:
            a = []
            for j in range(1, i - 1):
                a.append(0)
            a.append(1 - p(x[i]) * h / 2)
            a.append(-2 + h * h * q(x[i]))
            a.append(1 + p(x[i]) * h / 2)
            for j in range(i + 2, len(x)):
                a.append(0)
            A.append(a)
            b.append(h * h * f2(x[i]))
    y = progon(A, b)
    return x, y


def runge_rom(y1, y2, r, p):
    k = -10
    for i in range(min(len(y1), len(y2))):
        k = max(k, abs((y1[i] - y2[i]) / (r ** p - 1)))
    return k


if __name__ == '__main__':
    x0 = 0
    y0 = 2
    x1 = np.pi / 6
    y1 = 2.5 - 0.5 * np.log(3)
    h = (x1 - x0) / 5

    # Shooting method
    tt, y, Phi, xni, yni = shooting(x0, y0, x1, y1, h)

    data = []
    data1 = []
    for i in range(len(Phi)):
        data.append([tt[i], y[i], Phi[i]])
    for i in range(len(xni)):
        data1.append([xni[i], yni[i], yf([xni[i]][0])])

    data = pd.DataFrame(data, columns=["этта", "y(b, y0, tt)", "Phi"])
    data1 = pd.DataFrame(data1, columns=["xk", "yk", "yf"])

    print("Shooting method")
    print(data)
    print(data1)

    # Finite differences
    x, y = finite_difference(x0, x1, y0, y1, h)
    table = [["k", "xk", "yk", "yf"]]
    data = []
    data.append([x0, y0, yf([x[0]])])
    for i in range(0, len(y)):
        data.append([x[i + 1], y[i], yf(x[i + 1])])

    print("Finite differences")
    data = pd.DataFrame(data, columns=["xk", "yk", "yf"])
    print(data)

    # Runge-Romberg method
    tt, y, Phi, xni, yni2 = shooting(x0, y0, x1, y1, 2 * h)
    tt, y, Phi, xni, yni1 = shooting(x0, y0, x1, y1, h)

    x, yk1 = finite_difference(x0, x1, y0, y1, h / 2)
    x, yk2 = finite_difference(x0, x1, y0, y1, h)

    methods = ["Стрельбы", "Конечно-Разностный"]
    data = [runge_rom(yni1, yni2, 2, 4), runge_rom(yk1, yk2, 2, 1)]
    for method, res in zip(methods, data):
        print(f"Метод: {method}\t - : {res}")
