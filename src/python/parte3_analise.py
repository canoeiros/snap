import networkx as nx
import matplotlib.pyplot as plt
import random
import math
import time


G = nx.read_edgelist("../../data/soc-sign-Slashdot081106.txt", comments='#', create_using=nx.DiGraph(), nodetype=int, data=(('sign', int),))

N = G.number_of_nodes()
E = G.number_of_edges()
print(f"Grafo carregado: {N} nós e {E} arestas.")


print("\nPlotando a Distribuição de Graus (Lei de Potência)...")
degrees = [d for n, d in G.degree()]
degree_counts = {}
for d in degrees:
    degree_counts[d] = degree_counts.get(d, 0) + 1

x = list(degree_counts.keys())
y = list(degree_counts.values())

plt.figure(figsize=(8, 6))
plt.scatter(x, y, color='blue', alpha=0.5, s=10)
plt.xscale('log')
plt.yscale('log')
plt.title("Distribuição de Graus (Escala Log-Log) - Slashdot")
plt.xlabel("Grau (k)")
plt.ylabel("Frequência P(k)")
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.savefig("../../results/grafos_plotados/lei_de_potencia.png")
plt.close()


print("\nIniciando Testes de Robustez...")

def get_lcc_size(graph):
    # Retorna o tamanho da maior componente fracamente conexa
    wcc = nx.weakly_connected_components(graph)
    return len(max(wcc, key=len))

tamanho_original = get_lcc_size(G)
print(f"Tamanho da Maior Componente Original (LCC): {tamanho_original} nós")

num_to_remove = int(0.05 * N)

# --- 3.1. Remoção Aleatória (5%) ---
G_random = G.copy()
random_nodes = random.sample(list(G_random.nodes()), num_to_remove)
G_random.remove_nodes_from(random_nodes)
lcc_random = get_lcc_size(G_random)
print(f"LCC após remover 5% dos vértices aleatoriamente: {lcc_random} nós ({(lcc_random/tamanho_original)*100:.2f}%)")

# --- 3.2. Remoção dos 5% Mais Centrais (Ataque a Hubs) ---
G_targeted = G.copy()
# Ordena os nós pelo grau de forma decrescente
sorted_nodes = sorted(G_targeted.degree, key=lambda x: x[1], reverse=True)
top_5_percent_nodes = [node for node, degree in sorted_nodes[:num_to_remove]]
G_targeted.remove_nodes_from(top_5_percent_nodes)
lcc_targeted = get_lcc_size(G_targeted)
print(f"LCC após remover os 5% maiores Hubs: {lcc_targeted} nós ({(lcc_targeted/tamanho_original)*100:.2f}%)")



# ==========================================
# 4. SMALL-WORLD (CÁLCULO MATEMÁTICO MANUAL)
# ==========================================
print("\nCálculo Teórico de Small-World (Para comparar com seu C++)...")
k_avg = (2 * E) / N  # Grau médio

# Fórmulas de Erdős-Rényi para grafos aleatórios equivalentes
C_rand = k_avg / N
L_rand = math.log(N) / math.log(k_avg)

print(f"Grau Médio <k>: {k_avg:.2f}")
print(f"Clusterização Aleatória Esperada (C_rand): {C_rand:.6f}")
print(f"Comprimento Médio Aleatório Esperado (L_rand): {L_rand:.2f}")