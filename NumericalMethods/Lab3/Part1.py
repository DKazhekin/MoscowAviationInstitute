"""
Lagrange and Newton interpolation methods are used to approximate the value of the function at the test point.
"""
import math


def f(x):
    return math.asin(x)


def lagrange_interpolation(x, y, test_point):
    polynom_str = 'L(x) ='
    polynom_test_value = 0
    for i in range(len(x)):
        cur_num_str = ''
        num = 1
        denom = 1

        # Calculating numerator and denominator, skipping i == j case as this multiplyer eliminates with division
        # Page 5
        for j in range(len(x)):
            if i == j:
                continue
            cur_num_str += f'(x - {x[j]:.2f})'
            num *= (test_point[0] - x[j])
            denom *= (x[i] - x[j])

        polynom_str += f' + {(y[i] / denom):.2f}*' + cur_num_str

        # Calculating polynom value for test point like in formula
        polynom_test_value += y[i] * num / denom
    return polynom_str, abs(polynom_test_value - test_point[1])


def newton_interpolation(x, y, test_point):

    # Calculating division residuals coefficents
    n = len(x)
    coefs = [y[i] for i in range(n)]
    for i in range(1, n):
        for j in range(n - 1, i - 1, -1):
            coefs[j] = float(coefs[j] - coefs[j - 1]) / float(x[j] - x[j - i])

    # Get polynom
    polynom_str = 'P(x) = '
    polynom_test_value = 0

    cur_multipliers_str = ''
    cur_multipliers = 1
    for i in range(n):
        polynom_test_value += cur_multipliers * coefs[i]

        # The first value is a simple function value
        if i == 0:
            polynom_str += f'{coefs[i]:.2f}'
        # Else using the formula
        else:
            polynom_str += ' + ' + cur_multipliers_str + '*' + f'{coefs[i]:.2f}'

        cur_multipliers *= (test_point[0] - x[i])
        cur_multipliers_str += f'(x-{x[i]:.2f})'
    return polynom_str, abs(polynom_test_value - test_point[1])


if __name__ == '__main__':
    x_a = [-0.4, -0.1, 0.2, 0.5]
    x_b = [-0.4, 0, 0.2, 0.5]
    y_a = [f(x) for x in x_a]
    y_b = [f(x) for x in x_b]

    x_test = 0.1
    y_test = f(x_test)

    print('Lagrange interpolation')
    print('Points A')
    lagrange_polynom_a, lagrange_error_a = lagrange_interpolation(x_a, y_a, (x_test, y_test))
    print('Polynom')
    print(lagrange_polynom_a)
    print('error for test point =', lagrange_error_a)

    print('Points B')
    lagrange_polynom_b, lagrange_error_b = lagrange_interpolation(x_b, y_b, (x_test, y_test))
    print('Polynom')
    print(lagrange_polynom_b)
    print('error for test point =', lagrange_error_b)
    print()

    print('Newton interpolation')
    print('Points A')
    newton_polynom_a, newton_error_a = newton_interpolation(x_a, y_a, (x_test, y_test))
    print('Polynom')
    print(newton_polynom_a)
    print('error for test point =', newton_error_a)

    print('Points B')
    newton_polynom_b, newton_error_b = newton_interpolation(x_b, y_b, (x_test, y_test))
    print('Polynom')
    print(newton_polynom_b)
    print('error for test point =', newton_error_b)