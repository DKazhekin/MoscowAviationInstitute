#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

struct suffix_tree {
    using table = map<char, int>;
    using item = pair<char, int>;

    const int INF = 1e9;

    struct node {
        table go;
        int l;
        int len;
        int link = 0;
        int type = 0;

        node(int _l, int _len) {
            l = _l;
            len = _len;
        }

        int r(int str_size) {
            return min(str_size, l + len);
        }
    };

    vector<node> data;
    string s;

    int create_node(int _l, int _len) {
        data.push_back(node(_l, _len));
        ++nodes;
        return nodes;
    }

    bool can_go(int id, char c) {
        return data[id].go.count(c);
    }

    int go(int id, char c) {
        return data[id].go[c];
    }

    int size;
    int nodes;

    int u = 0;
    int rem = 0;

    void go_edge() {
        while (1) {
            char c = s[size - rem];
            if (!can_go(u, c)) {
                return;
            }
            int v = go(u, c);
            if (data[v].len < rem) {
                rem -= data[v].len;
                u = v;
            } else {
                return;
            }
        }
    }

    void extend(char c) {
        ++size;
        ++rem;
        int last = 0;
        while (rem) {
            go_edge();
            char a = s[size - rem];
            if (!can_go(u, a)) {
                data[u].go[a] = create_node(size - rem, INF);
                data[last].link = u;
                last = u;
            } else {
                int v = go(u, a);
                int good_len = rem - 1;
                int edge_left = data[v].l;
                char t = s[edge_left + good_len];
                if (t != c) {
                    int split = create_node(edge_left, good_len);
                    int new_v = create_node(size - 1, INF);
                    data[split].go[t] = v;
                    data[split].go[c] = new_v;
                    data[u].go[a] = split;

                    data[v].len -= good_len;
                    data[v].l += good_len;

                    data[last].link = split;
                    last = split;
                } else {
                    data[last].link = u;
                    return;
                }
            }
            if (u == 0) {
                --rem;
            } else {
                u = data[u].link;
            }
        }
    }

    void build() {
        size = 0;
        nodes = 0;

        u = 0;
        rem = 0;
        for (char c: s) {
            extend(c);
        }
    }

    suffix_tree(const string &_s) : s(_s) {
        size = 0;
        nodes = 0;
        create_node(0, INF);
        build();
    }

    int dfs(int u, int h) {
        if (u) {
            cout << h << ' ' << data[u].l + 1 << ' ' << data[u].r(size) << ' ' << data[u].type << "|" << data[u].l << " " << data[u].r(size) << endl;
            for (int i = data[u].l; i < data[u].r(size); ++i) {
                cout << s[i];
            }
            cout << '\n';
        }
        for (item elem : data[u].go) {
            int v = elem.second;
            dfs(v, h + 1);
        }
    }

    int max = 0;
    vector<int> v;
    int sharp_ind = s.find("#");

    int fill_all(int u, int d, const string& str){

        if (data[u].go.empty()){
            if (data[u].l > sharp_ind) {
                data[u].type = 2;
            } else {
                data[u].type = 1;
            }
        }

        int result = data[u].type;  // Инициализируем результат текущим типом u

        if (!data[u].go.empty()) {
            for (const auto& i : data[u].go) {
                int v = i.second;
                int k = data[v].r(size) - data[v].l;
                result |= fill_all(v, d + k, str);  // Обновляем результат побитовым "или" с результатами рекурсивных вызовов
            }
        }

        if (result == 3){
            if (max == d){
                v.push_back(data[u].r(size));
            }
            else if (d > max){
                max = d;
                v.resize(0);
                v.push_back(data[u].r(size));
            }
        }

        data[u].type = result;  // Присваиваем обновленное значение обратно в data[u].type


        return result;  // Возвращаем итоговый результат
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    string s1, s2, s;
    cin >> s1 >> s2;
    auto start = chrono::high_resolution_clock::now();
    s = s1 + "#" + s2 + "$";
    suffix_tree st(s);
    st.fill_all(0, 0, st.s);
    cout << st.max << '\n';
    for (auto i : st.v){
        for (int j = i - st.max; j < i; j++){
            cout << st.s[j];
        }
        cout << '\n';
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    cout << "Время работы программы: " << duration.count() << " ms";
}
