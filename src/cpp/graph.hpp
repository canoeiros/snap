#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Edge {
    int target;
    int sign;
};

struct Graph {
    unordered_map<int, vector<Edge>> adj;
    vector<int> nodes;
    int num_edges = 0;

    void addEdge(int u, int v, int sign) {
        if (adj.find(u) == adj.end()) nodes.push_back(u);
        if (adj.find(v) == adj.end()) {
            adj[v] = vector<Edge>();
            nodes.push_back(v);
        }
        adj[u].push_back({v, sign});
        num_edges++;
    }

    int numNodes() const {
        return adj.size();
    }
};

static Graph loadGraph(const string& filename) {
    Graph G;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        int u, v, sign;
        stringstream ss(line);

        if (ss >> u >> v >> sign) {
            G.addEdge(u, v, sign);
        }
    }
    file.close();
    return G;
}
