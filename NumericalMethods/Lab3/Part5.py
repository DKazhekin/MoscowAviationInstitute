import math


class IntegralCalculation:
    def __init__(self):
        self.x0 = -2
        self.xk = 2
        self.h1 = 1
        self.h2 = 0.5

    def f(self, x):
        return 1 / (x * x + 4)

    def square_method(self):
        res1 = 0
        x_s = self.x0 + self.h1
        while x_s <= self.xk:
            res1 += self.h1 * self.f((x_s - self.h1 + x_s) / 2)
            x_s += self.h1

        res2 = 0
        x_s = self.x0 + self.h2
        while x_s <= self.xk:
            res2 += self.h2 * self.f((x_s - self.h2 + x_s) / 2)
            x_s += self.h2

        return res1, res2

    def trap_method(self):
        res1 = self.h1 * self.f(self.x0) / 2
        x_s = self.x0 + self.h1
        while x_s < self.xk:
            res1 += self.h1 * self.f(x_s)
            x_s += self.h1
        res1 += self.h1 * self.f(self.xk) / 2

        res2 = self.h2 * self.f(self.x0) / 2
        x_s = self.x0 + self.h2
        while x_s < self.xk:
            res2 += self.h2 * self.f(x_s)
            x_s += self.h2
        res2 += self.h2 * self.f(self.xk) / 2

        return res1, res2

    def simpson_method(self):
        res1 = (self.h1 * self.f(self.x0) / 3) + (self.h1 * self.f(self.xk) / 3)
        k = 4
        x_s = self.x0 + self.h1
        while x_s < self.xk:
            res1 += k * self.h1 * self.f(x_s) / 3
            k = 2 if k == 4 else 4
            x_s += self.h1

        res2 = (self.h2 * self.f(self.x0) / 3) + (self.h2 * self.f(self.xk) / 3)
        k = 4
        x_s = self.x0 + self.h2
        while x_s < self.xk:
            res2 += k * self.h2 * self.f(x_s) / 3
            k = 2 if k == 4 else 4
            x_s += self.h2

        return res1, res2

    def runge_romberg_richardson(self, results, n):
        I_h1 = results[0]
        I_h2 = results[1]
        return I_h2 + (I_h2 - I_h1) / (math.pow(2, n) - 1)


if __name__ == "__main__":
    it = IntegralCalculation()

    square_results = it.square_method()
    square_rrr = it.runge_romberg_richardson(square_results, 2)

    trap_results = it.trap_method()
    trap_rrr = it.runge_romberg_richardson(trap_results, 2)

    simpson_results = it.simpson_method()
    simpson_rrr = it.runge_romberg_richardson(simpson_results, 4)

    print(f"Square Method: {square_results[0]}, {square_results[1]} | RRR: {square_rrr}")
    print(f"Trap Method: {trap_results[0]}, {trap_results[1]} | RRR: {trap_rrr}")
    print(f"Simpson Method: {simpson_results[0]}, {simpson_results[1]} | RRR: {simpson_rrr}")
