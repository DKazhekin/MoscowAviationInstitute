#include <iostream>
#include <vector>
#include <cmath>

std::vector<long long> greedy_coin_change(long long N, long long target, long long p) {

    std::vector<long long> coin_counts(N, 0);  // Инициализируем массив нулями

    // Начинаем с наибольшего номинала p^(N-1)
    for (auto i = static_cast<long long>(N - 1); i >= 0; --i) {

        auto coin_value = static_cast<long long>(pow(p, i));

        // Количество монет данного номинала для размена
        auto count = static_cast<long long>(target / coin_value);
        target -= count * coin_value;

        coin_counts[i] = count;
    }

    return coin_counts;
}

int main() {
    long long N, p;
    std::cin >> N >> p;

    long long target;
    std::cin >> target;

    std::vector<long long> coin_counts = greedy_coin_change(N, target, p);

    for (long long coin_count : coin_counts) {
        std::cout << coin_count << '\n';
    }

    return 0;
}
