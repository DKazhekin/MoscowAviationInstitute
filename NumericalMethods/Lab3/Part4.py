def df(x_test, x, y):
    visited = False
    # Finding the interval
    for interval in range(len(x) - 1):
        if x[interval] <= x_test < x[interval + 1]:
            i = interval
            visited = True
            break

    if not visited or (visited and i > len(x) - 3):
        i = len(x) - 3

    # First accuracy level
    a1 = (y[i + 1] - y[i]) / (x[i + 1] - x[i])

    # Second accuracy level
    a2 = ((y[i + 2] - y[i + 1]) / (x[i + 2] - x[i + 1]) - a1) / (x[i + 2] - x[i]) * (
            2 * x_test - x[i] - x[i + 1])

    # Resulting df
    return a1 + a2


def d2f(x_test, x, y):
    # Finding the interval
    for interval in range(len(x) - 1):
        if x[interval] <= x_test < x[interval + 1]:
            i = interval
            break

    num = (y[i + 2] - y[i + 1]) / (x[i + 2] - x[i + 1]) - (y[i + 1] - y[i]) / (x[i + 1] - x[i])
    return 2 * num / (x[i + 2] - x[i])


if __name__ == '__main__':
    x = [-1.0, 0.0, 1.0, 2.0, 3.0]
    y = [-0.7854, 0.0, 0.7854, 1.1071, 1.249]
    x_test = 1.0

    print('First derivative')
    print(f'df({x_test}) = {df(x_test, x, y)}')

    print('Second derivative')
    print(f'd2f({x_test}) = {d2f(x_test, x, y)}')
