#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

const int M = 100;

void print(long long sum_count, bitset<M> res, int n){
    cout << sum_count << endl;
    for (int i = 0; i < n; ++i) {
        if (res[i]) {
            cout << i + 1 << ' ';
        }
    }
    cout << '\n';
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> w(n);
    vector<long long> c(n);
    for (int i = 0; i < n; ++i) {
        cin >> w[i] >> c[i];
    }

    vector<vector<long long>> ap
            (n + 1, vector<long long>(m + 1));
    vector<vector<bitset<M>>> rp
            (n + 1, vector<bitset<M>>(m + 1));

    long long sum_count = 0;
    bitset<M> res;

    for (int i = 1; i < n + 1; ++i) {
        for (int j = 1; j < m + 1; ++j) {
            ap[i][j] = ap[i - 1][j];
            rp[i][j] = rp[i - 1][j];
            if ((c[i - 1] > ap[i][j]) && (j - w[i - 1] == 0)) {
                ap[i][j] = c[i - 1];
                rp[i][j] = 0;
                rp[i][j][i - 1] = 1;
            }
            if (ap[i][j] > sum_count) {
                sum_count = ap[i][j];
                res = rp[i][j];
            }
        }
    }

    vector<vector<long long>> ac
            (n + 1, vector<long long>(m + 1));
    vector<vector<bitset<M>>> rc
            (n + 1, vector<bitset<M>>(m + 1));

    for (long long i = 2; i < n + 1; ++i) {
        for (int j = 1; j < n + 1; ++j) {
            for (int k = 1; k < m + 1; ++k) {
                ac[j][k] = ac[j - 1][k];
                rc[j][k] = rc[j - 1][k];
                if ((k - w[j - 1] > 0) && (ap[j - 1][k - w[j - 1]] > 0)) {
                    if (i * (c[j - 1] +
                             ap[j - 1][k - w[j - 1]] / (i - 1)) > ac[j][k])
                    {
                        ac[j][k] = i * (c[j - 1] +
                                            ap[j - 1][k - w[j - 1]] / (i - 1));
                        rc[j][k] = rp[j - 1][k - w[j - 1]];
                        rc[j][k][j - 1] = 1;
                    }
                }
                if (ac[j][k] > sum_count) {
                    sum_count = ac[j][k];
                    res = rc[j][k];
                }
            }
        }

        swap(ac, ap);
        swap(rc, rp);
    }

    print(sum_count, res, n);
    return 0;
}
