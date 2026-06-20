import networkx as nx
import numpy as np
import time

start_time = time.time()
G = nx.read_edgelist("../../data/soc-sign-Slashdot081106.txt", comments='#', create_using=nx.DiGraph(), nodetype=int, data=(('sign', int),))
print(f"Grafo carregado em {time.time() - start_time:.2f} segundos.")

# 2. Métricas Básicas
num_nodes = G.number_of_nodes()
num_edges = G.number_of_edges()
density = nx.density(G)

print(f"\n--- Métricas Básicas ---")
print(f"Vértices: {num_nodes}")
print(f"Arestas: {num_edges}")
print(f"Densidade: {density:.8f}")

in_degrees = [d for n, d in G.in_degree()]
out_degrees = [d for n, d in G.out_degree()]
total_degrees = [d for n, d in G.degree()]

print(f"\n--- Graus (Total) ---")
print(f"Grau Mínimo: {min(total_degrees)}")
print(f"Grau Máximo: {max(total_degrees)}")
print(f"Grau Médio: {np.mean(total_degrees):.2f}")


print("\nCalculando Componentes Conexas")

# Usamos weakly connected porque em redes sociais direcionadas é a métrica estrutural mais comum
wcc = list(nx.weakly_connected_components(G))
wcc_sizes = [len(c) for c in wcc]

print(f"\n--- Componentes Conexas (Weakly Connected) ---")
print(f"Número de Componentes: {len(wcc)}")
print(f"Tamanho da Maior Componente (LCC): {max(wcc_sizes)}")
print(f"Tamanho da Menor Componente: {min(wcc_sizes)}")
