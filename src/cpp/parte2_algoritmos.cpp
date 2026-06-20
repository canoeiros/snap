#include "graph.hpp"
#include "algoritmos.hpp"

#include <chrono>
#include <cmath>
#include <random>
#include <functional>
#include <iostream>

using namespace std;

void printStats(const string& name, const vector<double>& times) {
    double sum = 0;
    for (double t : times) sum += t;
    double mean = sum / times.size();

    double var_sum = 0;
    for (double t : times) var_sum += (t - mean) * (t - mean);
    double std_dev = sqrt(var_sum / times.size());

    double ci_margin = 1.96 * (std_dev / sqrt(times.size()));

    cout << "\n=== Resultados: " << name << " ===" << endl;
    cout << "Iterações: " << times.size() << endl;
    cout << "Média de Tempo: " << mean << " ms" << endl;
    cout << "Desvio Padrão: " << std_dev << " ms" << endl;
    cout << "Intervalo de Confiança: [" << mean - ci_margin << " ms, " << mean + ci_margin << " ms]" << endl;
}

void testBattery(const Graph &G, int runs, vector<double> &data, const std::function<void(int)>& algorithm) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, G.nodes.size() - 1);

    for (int i = 0; i < runs; ++i) {
        int source = G.nodes[dis(gen)];
        auto start = chrono::high_resolution_clock::now();
        algorithm(source);
        auto end = chrono::high_resolution_clock::now();

        data.push_back(chrono::duration<double, milli>(end - start).count());
    }
}

int main() {
    auto start_load = chrono::high_resolution_clock::now();
    Graph G = loadGraph("../../data/soc-sign-Slashdot081106.txt");
    auto end_load = chrono::high_resolution_clock::now();
    
    cout << "Grafo carregado. Vértices: " << G.numNodes() << " | Arestas: " << G.num_edges << endl;
    cout << "Tempo de carregamento (ms): " << chrono::duration<double, milli>(end_load - start_load).count() << endl;

    int num_tests = 30;
    
    vector<double> times_dijkstra;
    testBattery(G, num_tests, times_dijkstra, [&](int source) { dijkstra(G, source); });
    printStats("Dijkstra", times_dijkstra);

    // vector<double> times_bellman;
    // testBattery(G, num_tests, times_bellman, [&](int source) { bellmanFord(G, source); });
    // printStats("Bellman-Ford", times_bellman);

    vector<double> times_tarjan;
    TarjanSCC tarjan; 

    testBattery(G, num_tests, times_tarjan, [&](int source) { 
        tarjan.run(G); 
    });

    printStats("Algoritmo de Tarjan (Global)", times_tarjan);
    cout << "Total de Componentes Fortemente Conexas (SCC): " << tarjan.getSccCount() << endl;
    cout << "Tamanho da Maior SCC: " << tarjan.getMaxSccSize() << " nós." << endl;

    vector<double> times_bfs;
    testBattery(G, num_tests, times_bfs, [&](int source) { 
        runBFS(G, source); 
    });
    printStats("Busca em Largura (BFS)", times_bfs);

    vector<double> times_dfs;
    testBattery(G, num_tests, times_dfs, [&](int source) { 
        runDFS(G, source); 
    });
    printStats("Busca em Profundidade (DFS)", times_dfs);

    calculateSampledMetrics(G, 300);
    calculateTrianglesAndClustering(G);
    return 0;
}