import networkx as nx
import matplotlib.pyplot as plt
import time


start_time = time.time()
G = nx.read_edgelist("../../data/soc-sign-Slashdot081106.txt", comments='#', create_using=nx.DiGraph(), nodetype=int, data=(('sign', int),))
print(f"Grafo carregado em {time.time() - start_time:.2f} segundos.")

print("\nGerando Visualização Reduzida (Amostra de 100 nós)...")

# Técnica: Snowball Sampling centrada no maior Hub para garantir conectividade visual
hubs = sorted(G.degree, key=lambda x: x[1], reverse=True)
start_node = hubs[0][0] # Pega o nó com o maior número de conexões

sampled_nodes = set([start_node])
queue = [start_node]

# Busca em Largura (BFS) até coletar 100 nós conectados
while queue and len(sampled_nodes) < 100:
    current = queue.pop(0)
    # Pega tanto quem ele segue quanto quem segue ele
    neighbors = list(G.successors(current)) + list(G.predecessors(current))
    
    for neighbor in neighbors:
        if neighbor not in sampled_nodes:
            sampled_nodes.add(neighbor)
            queue.append(neighbor)
        if len(sampled_nodes) == 100:
            break

# Cria o subgrafo apenas com esses 100 nós
H = G.subgraph(sampled_nodes)

# Separa as arestas pelo sinal para colorir
pos_edges = [(u, v) for u, v, d in H.edges(data=True) if d.get('sign', 1) == 1]
neg_edges = [(u, v) for u, v, d in H.edges(data=True) if d.get('sign', 1) == -1]

# Configuração do Plot
plt.figure(figsize=(10, 8))
# spring_layout simula física de molas, afastando nós desconectados e aproximando conectados
pos = nx.spring_layout(H, seed=42, k=0.15) 

# Desenha os Vértices
nx.draw_networkx_nodes(H, pos, node_size=60, node_color='#1f78b4', edgecolors='black', linewidths=0.5)

# Desenha as Arestas (+1 = Verde, -1 = Vermelho)
nx.draw_networkx_edges(H, pos, edgelist=pos_edges, edge_color='green', alpha=0.5, arrows=False)
nx.draw_networkx_edges(H, pos, edgelist=neg_edges, edge_color='red', alpha=0.5, arrows=False)

plt.title("Visualização Reduzida da Rede Slashdot (Amostra de 100 Nós)\nVerde: Confiança (+1) | Vermelho: Desconfiança (-1)", fontsize=14)
plt.axis("off") # Esconde os eixos do gráfico
plt.tight_layout()
plt.savefig("../../results/grafos_plotados/visualizacao_100_nos.png", dpi=300)
print("Visualização salva em 'results/grafos_plotados/visualizacao_100_nos.png'.")
plt.close()
