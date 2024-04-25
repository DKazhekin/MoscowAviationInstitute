#include <iostream>
#include <vector>
#include <climits>
#include <queue>

using namespace std;

struct Edge {
    long int from, to, weight;
};

void input(long int edges_count, vector<Edge> *edges);
bool ford_bellman(long int vertex_count, const vector<Edge> &edges, vector<long int> &dist);
void make_reachable(long int vertex_count, vector<Edge> *edges);
void improve_weights(vector<Edge> &edges, vector<long int> &potentials);
vector<vector<long int>> dijkstraAll(long int V, const vector<Edge> &edges);
vector<long int> dijkstraSimple(long int V, const vector<Edge> &edges, long int start);

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long int edges_count, vertex_count;
    cin >> vertex_count >> edges_count;

    vector<Edge> edges;
    input(edges_count, &edges);

//  Добавим нулевую вершину, из которой будут достижимы все вершины
    make_reachable(vertex_count, &edges);

//  Проверяем на отрицательный цикл и получаем вектор потенциалов
    vector<long int> potentials(vertex_count + 1, LONG_MAX);
    potentials[0] = 0;
    if (ford_bellman(vertex_count + 1, edges, potentials)) {
        cout << "Negative cycle";
        return 0;
    }

//  Делаем веса на ребрах неотрицательными, пользуясь потенциалами
    improve_weights(edges, potentials);


    vector<vector<long int>> shortest_paths = dijkstraAll(vertex_count + 1, edges);

// Корректируем расстояния обратно с помощью потенциалов.
    for (long int i = 1; i <= vertex_count; ++i) {
        for (long int j = 1; j <= vertex_count; ++j) {
            if (shortest_paths[i][j] != LONG_MAX) {
                shortest_paths[i][j] += potentials[j] - potentials[i];
            }
        }
    }

// Выводим результаты в соответствии с форматом задания.
    for (long int i = 1; i <= vertex_count; ++i) {
        for (long int j = 1; j <= vertex_count; ++j) {
            if (shortest_paths[i][j] == LONG_MAX) {
                cout << "inf ";
            } else {
                cout << shortest_paths[i][j] << " ";
            }
        }
        if (i != vertex_count) cout << endl;
    }

    return 0;
}

void input(long int edges_count, vector<Edge> *edges) {
    for (long int i = 0; i < edges_count; i++) {
        Edge input = {};
        cin >> input.from >> input.to >> input.weight;
        edges->push_back(input);
    }
}

bool ford_bellman(long int vertex_count, const vector<Edge> &edges, vector<long int> &dist) {

    bool relaxed;
    // Релаксация всех рёбер vertex_count раз
    for (long int i = 0; i < vertex_count - 1; ++i) {
        relaxed = false;
        for (const Edge &e: edges) {
            if (dist[e.from] < LONG_MAX && dist[e.from] + e.weight < dist[e.to]) {
                dist[e.to] = dist[e.from] + e.weight;
                relaxed = true;
            }
        }

        if (!relaxed) {
            break;
        }
    }

    // Проверка на отрицательные циклы
    for (const Edge &e: edges) {
        if (dist[e.from] < LONG_MAX && dist[e.from] + e.weight < dist[e.to]) {
            return true; // Обнаружен отрицательный цикл
        }
    }

    return false; // Отрицательные циклы отсутствуют
}

void make_reachable(long int vertex_count, vector<Edge> *edges) {
    for (long int i = 0; i < vertex_count; i++) {
        Edge addition = {0, i, 0};
        edges->push_back(addition);
    }
}

void improve_weights(vector<Edge> &edges, vector<long int> &potentials) {
    for (auto &edge: edges) {
        if (edge.from != 0) {
            edge.weight += potentials[edge.from] - potentials[edge.to];
        }
    }
}

vector<long int> dijkstraSimple(long int V, const vector<Edge> &edges, long int start) {
    vector<long int> dist(V, LONG_MAX);
    priority_queue<pair<long int, long int>, vector<pair<long int, long int>>, greater<pair<long int, long int>>> pq;

    // Инициализация начальной вершины
    dist[start] = 0;
    pq.push({0, start}); // пара (расстояние, вершина)

    while (!pq.empty()) {
        // Извлечение вершины с минимальным расстоянием
        long int u = pq.top().second;
        pq.pop();

        // Перебор всех соседей u
        for (const auto &edge : edges) {
            if (edge.from == u) {
                long int v = edge.to;
                long int weight = edge.weight;

                // Релаксация ребра
                if (dist[u] != LONG_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    return dist;
}

vector<vector<long int>> dijkstraAll(long int V, const vector<Edge> &edges) {
    vector<vector<long int>> allDistances(V);
    for (long int i = 0; i < V; ++i) {
        allDistances[i] = dijkstraSimple(V, edges, i);
    }
    return allDistances;
}
