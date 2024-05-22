import numpy as np
import pandas as pd


# First equation from the system
def f(x, y, z):
    return 4 * x * z - (4 * pow(x, 2) - 3) * y + pow(np.e, pow(x, 2))


# Second equation from the system
def g(x, y, z):
    return z


# Analytic solution
def yf(x):
    return (np.e ** x + np.e ** (-x) - 1) * np.e ** pow(x, 2)


def runge_romberg(y1, y2, r, p):
    k = -10
    for i in range(min(len(y1), len(y2))):
        k = max(k, abs((y1[i] - y2[i]) / (r ** p - 1)))
    return k


# Eurler - Cauchy method with correction operations

# This method making an assumption that the y function, which cross x0, y0 from constraints of the Cauchy problem is slightly differs from the derivative.
# Firstly wemaking replacement Z = Y' and then iteravevely calculate z and the following y values, using correction method of the previous iteration.
def euler(y0, z0, xl, xr, h):
    # X range
    x = [i for i in np.arange(xl, xr + h, h)]

    # Start values for y and z
    y = [y0]
    z = [z0]

    # Arrays for next values of y and z for corrections
    z_next = [0]
    y_next = [0]

    # Main loop
    for i in range(len(x) - 1):
        z_next.append(z[i] + h * f(x[i], y[i], z[i]))
        z.append(z[i] + h * (f(x[i], y[i], z[i]) + f(x[i], y[i], z_next[i])) / 2)
        y_next.append(y[i] + h * g(x[i], y[i], z[i]))
        y.append(y[i] + h * (g(x[i], y[i], z[i]) + g(x[i], y_next[i], z[i])) / 2)
    return x, y, z, y_next, z_next


# This method is a meta method, where Euler is a instance of it.
def runge_kuty(y0, z0, xl, xr, h):
    x = [i for i in np.arange(xl, xr + h, h)]
    y = [y0]
    z = [z0]
    dz = []
    dy = []
    Kz = []
    Ky = []
    for i in range(len(x)):
        # Calculating K and L coefficients
        L1 = h * f(x[i], y[i], z[i])
        K1 = h * g(x[i], y[i], z[i])
        L2 = h * f(x[i] + 0.5 * h, y[i] + 0.5 * K1, z[i] + 0.5 * L1)
        K2 = h * g(x[i] + 0.5 * h, y[i] + 0.5 * K1, z[i] + 0.5 * L1)
        L3 = h * f(x[i] + 0.5 * h, y[i] + 0.5 * K2, z[i] + 0.5 * L2)
        K3 = h * g(x[i] + 0.5 * h, y[i] + 0.5 * K2, z[i] + 0.5 * L2)
        L4 = h * f(x[i] + h, y[i] + K3, z[i] + L3)
        K4 = h * g(x[i] + h, y[i] + K3, z[i] + L3)

        # Storing them and using for next iteration
        Ky.append([K1, K2, K3, K4])
        Kz.append([L1, L2, L3, L4])
        dy.append((K1 + 2 * K2 + 2 * K3 + K4) / 6)
        dz.append((L1 + 2 * L2 + 2 * L3 + L4) / 6)
        y.append(y[i] + dy[i])
        z.append(z[i] + dz[i])

    return x, y, z, dy, dz, Ky, Kz


# Multi-steps method Adams, not as previous ones. We should get first 3 values from Runge-Kutty method and then use them to calculate the next values with Adams method.
def adams(y0, z0, xl, xr, h):
    x, y, z, dy, dz, Ky, Kz = runge_kuty(y0, z0, xl, xl + 3 * h, h)
    x = [i for i in np.arange(xl, xr + h, h)]
    for i in range(4, len(x) - 1):
        z.append(z[i] + h * (
                55 * f(x[i], y[i], z[i]) - 59 * f(x[i - 1], y[i - 1], z[i - 1]) + 37 * f(x[i - 2], y[i - 2],
                                                                                         z[i - 2]) - 9 * f(x[i - 3],
                                                                                                           y[i - 3],
                                                                                                           z[
                                                                                                               i - 3])) / 24
                 )
        y.append(y[i] + h * (
                55 * g(x[i], y[i], z[i]) - 59 * g(x[i - 1], y[i - 1], z[i - 1]) + 37 * g(x[i - 2], y[i - 2],
                                                                                         z[i - 2]) - 9 * g(x[i - 3],
                                                                                                           y[i - 3],
                                                                                                           z[
                                                                                                               i - 3])) / 24
                 )

    return x, y, z


if __name__ == '__main__':
    xl = 0
    xr = 1
    h = 0.1
    y0 = 1
    z0 = 0

    # Euler - Cauchy method output
    print("Euler - Cauchy method output")
    x, y, z, y_next, z_next = euler(y0, z0, xl, xr, h)
    data = []
    for k in range(len(x)):
        data.append([x[k], y[k], y_next[k], z[k], z_next[k], yf(x[k]), abs(yf(x[k]) - y[k])])

    data = pd.DataFrame(data, columns=['x', 'y', 'y_next', 'z', 'z_next', 'yf', 'yf - y'])
    print(data)

    # Runge-Kutty method output
    print("Runge-Kutty method output")
    x, y, z, dy, dz, Ky, Kz = runge_kuty(y0, z0, xl, xr, h)
    data = []
    datak = []
    for k in range(len(x)):
        if k != len(x) - 1:
            datak.append([Ky[k][0], Ky[k][1], Ky[k][2], Ky[k][3], Kz[k][0], Kz[k][1], Kz[k][2], Kz[k][3]])
        data.append([x[k], y[k], dy[k], z[k], dz[k], yf(x[k]), yf(x[k]) - y[k]])

    data = pd.DataFrame(data, columns=["xk", "yk", "dyk", "zk", "dzk", "yf", "yf - y"])
    datak = pd.DataFrame(datak, columns=["K1", "K2", "K3", "K4", "L1", "L2", "L3", "L4"])

    print(data)
    print(datak)

    # Adams method output
    print("Adams method output")
    x, y, z = adams(y0, z0, xl, xr, h)
    data = []
    for k in range(len(x)):
        data.append([x[k], y[k], z[k], yf(x[k]), yf(x[k]) - y[k]])
    data = pd.DataFrame(data, columns=["xk", "yk", "zk", "yf", "errk"])
    print(data)

    # Runge-Romberg method comparison
    print("Runge-Romberg method comparison")
    _, ye2, _, _, _ = euler(y0, z0, xl, xr, 2 * h)
    _, ye1, _, _, z1 = euler(y0, z0, xl, xr, h)

    _, yrk2, _, _, _, _, _ = runge_kuty(y0, z0, xl, xr, 2 * h)
    _, yrk1, _, _, _, _, _ = runge_kuty(y0, z0, xl, xr, h)

    _, ya2, _ = adams(y0, z0, xl, xr, 2 * h)
    _, ya1, _ = adams(y0, z0, xl, xr, h)

    table = ["Эйлера", "Рунге-Кутты", "Адамса"]
    res = ([runge_romberg(ye1, ye2, 2, 2), runge_romberg(yrk1, yrk2, 2, 4), runge_romberg(ya1, ya2, 2, 4)])

    for name, res in zip(table, res):
        print(f"Method {name}: \tLoss: {res}")