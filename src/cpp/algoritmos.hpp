#pragma once

#include "graph.hpp"
#include <queue>
#include <limits>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <numeric>
#include <random>

using namespace std;

void dijkstra(const Graph &G, int source) {
    unordered_map<int, int> dist;
    for (int node : G.nodes) dist[node] = numeric_limits<int>::max();
    dist[source] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        auto it = G.adj.find(u);
        if (it != G.adj.end()) {
            for (const auto& edge : it->second) {
                if (edge.sign == 1) {
                    int v = edge.target;
                    int weight = 1;
                    if (dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        pq.push({dist[v], v});
                    }
                }
            }
        }
    }
}

bool negativeCycles(const Graph &G) {
    unordered_map<int, int> dist;
    for (int node : G.nodes) dist[node] = numeric_limits<int>::max() / 2;
    for (const auto& [u, edges] : G.adj) {
        if (dist[u] == numeric_limits<int>::max() / 2) continue;
        for (const auto& edge : edges) {
            int v = edge.target;
            int weight = edge.sign;
            if (dist[u] + weight < dist[v]) {
                return true;
            }
        }
    }
    return false;
}

bool bellmanFord(const Graph& G, int source) {
    unordered_map<int, int> dist;
    for (int node : G.nodes) dist[node] = numeric_limits<int>::max() / 2;
    dist[source] = 0;

    int V = G.numNodes();

    if (negativeCycles(G)) {return false;}

    for (int i = 1; i <= V - 1; ++i) {
        bool any_updated = false;
        for (const auto& [u, edges] : G.adj) {
            if (dist[u] == numeric_limits<int>::max() / 2) continue;
            for (const auto& edge : edges) {
                int v = edge.target;
                int weight = edge.sign;
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    any_updated = true;
                }
            }
        }
        if (!any_updated) break;
    }
    return true;
}


class TarjanSCC {
private:
    int id_counter;
    int scc_count;
    int max_scc_size;
    unordered_map<int, int> ids, low;
    unordered_map<int, bool> onStack;
    vector<int> st;

    void dfs(int at, const Graph& G) {
        st.push_back(at);
        onStack[at] = true;
        ids[at] = low[at] = id_counter++;

        auto it = G.adj.find(at);
        if (it != G.adj.end()) {
            for (const auto& edge : it->second) {
                int to = edge.target;
                
                if (ids.find(to) == ids.end()) {
                    dfs(to, G);
                    low[at] = min(low[at], low[to]);
                } else if (onStack[to]) {
                    low[at] = min(low[at], ids[to]);
                }
            }
        }

        if (ids[at] == low[at]) {
            int scc_size = 0;
            for (int node = st.back(); ; node = st.back()) {
                st.pop_back();
                onStack[node] = false;
                scc_size++;
                if (node == at) break;
            }
            scc_count++;
            max_scc_size = max(max_scc_size, scc_size);
        }
    }

public:
    void run(const Graph& G) {
        id_counter = 0;
        scc_count = 0;
        max_scc_size = 0;
        ids.clear();
        low.clear();
        onStack.clear();
        st.clear();

        for (int node : G.nodes) {
            if (ids.find(node) == ids.end()) {
                dfs(node, G);
            }
        }
    }

    int getSccCount() const { return scc_count; }
    int getMaxSccSize() const { return max_scc_size; }
};

inline void runBFS(const Graph& G, int source) {
    unordered_map<int, bool> visited;
    queue<int> q;

    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        auto it = G.adj.find(u);
        if (it != G.adj.end()) {
            for (const auto& edge : it->second) {
                int v = edge.target;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
}


inline void runDFS(const Graph& G, int source) {
    unordered_map<int, bool> visited;
    vector<int> stack; 

    stack.push_back(source);

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();

        if (!visited[u]) {
            visited[u] = true;

            auto it = G.adj.find(u);
            if (it != G.adj.end()) {
                for (const auto& edge : it->second) {
                    int v = edge.target;
                    if (!visited[v]) {
                        stack.push_back(v);
                    }
                }
            }
        }
    }
}

inline void calculateSampledMetrics(const Graph& G, int sample_size) {
    cout << "\nCalculando Diâmetro, Raio e Caminho Médio por amostragem (" << sample_size << " nós)..." << endl;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, G.nodes.size() - 1);

    long long total_paths = 0;
    long long total_distance = 0;
    int estimated_diameter = 0;
    int estimated_radius = numeric_limits<int>::max();

    for (int i = 0; i < sample_size; ++i) {
        int source = G.nodes[dis(gen)];
        
        unordered_map<int, int> dist;
        queue<int> q;
        
        q.push(source);
        dist[source] = 0;
        int max_dist_from_source = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            auto it = G.adj.find(u);
            if (it != G.adj.end()) {
                for (const auto& edge : it->second) {
                    int v = edge.target;
                    if (dist.find(v) == dist.end()) {
                        dist[v] = dist[u] + 1;
                        max_dist_from_source = max(max_dist_from_source, dist[v]);
                        total_distance += dist[v];
                        total_paths++;
                        q.push(v);
                    }
                }
            }
        }
        
        estimated_diameter = max(estimated_diameter, max_dist_from_source);
        if (max_dist_from_source > 0) {
            estimated_radius = min(estimated_radius, max_dist_from_source);
        }
    }

    double avg_path_length = (double)total_distance / total_paths;

    cout << "--- Resultados da Amostragem ---" << endl;
    cout << "Diâmetro Estimado: " << estimated_diameter << endl;
    cout << "Raio Estimado: " << estimated_radius << endl;
    cout << "Comprimento Médio dos Caminhos: " << avg_path_length << endl;
}


inline void calculateTrianglesAndClustering(const Graph& G) {
    cout << "\nCalculando Triângulos e Clusterização..." << endl;
    unordered_map<int, unordered_set<int>> undirected_adj;
    for (const auto& pair : G.adj) {
        int u = pair.first;
        for (const auto& edge : pair.second) {
            int v = edge.target;
            if (u != v) {
                undirected_adj[u].insert(v);
                undirected_adj[v].insert(u);
            }
        }
    }

    long long total_triangles = 0;
    double total_clustering = 0.0;
    int valid_nodes = 0;

    for (const auto& pair : undirected_adj) {
        int u = pair.first;
        const auto& neighbors = pair.second;
        int degree = neighbors.size();
        
        int node_triangles = 0;
        
        for (auto it1 = neighbors.begin(); it1 != neighbors.end(); ++it1) {
            auto it2 = it1;
            ++it2;
            for (; it2 != neighbors.end(); ++it2) {
                int v = *it1;
                int w = *it2;
                if (undirected_adj[v].count(w)) {
                    node_triangles++;
                }
            }
        }
        
        total_triangles += node_triangles;

        if (degree > 1) {
            double possible_triangles = (double)(degree * (degree - 1)) / 2.0;
            total_clustering += (double)node_triangles / possible_triangles;
            valid_nodes++;
        }
    }

    total_triangles /= 3;
    double avg_clustering = (valid_nodes > 0) ? (total_clustering / valid_nodes) : 0;

    cout << "--- Resultados da Clusterização ---" << endl;
    cout << "Número Total de Triângulos: " << total_triangles << endl;
    cout << "Coeficiente de Clusterização Médio: " << avg_clustering << endl;
}