import networkx as nx
import matplotlib.pyplot as plt
from collections import Counter

G = nx.read_edgelist("/home/caio/Documents/!ufba/2026.1/teo_grafos/trabalho/snap/data/soc-sign-Slashdot081106.txt", comments='#', create_using=nx.DiGraph(), nodetype=int, data=(('sign', int),))

graus = [grau for no, grau in G.degree()]
contagem_graus = Counter(graus)

x = sorted(contagem_graus.keys())
y = [contagem_graus[k] for k in x]

plt.figure(figsize=(10, 6))
plt.scatter(x, y, color='purple', alpha=0.6, s=15, edgecolor='black', linewidth=0.5)


plt.title("Distribuição Bruta de Graus (Escala Linear) - Slashdot", fontsize=14)
plt.xlabel("Grau do Vértice (k)", fontsize=12)
plt.ylabel("Frequência (Número de Vértices)", fontsize=12)

plt.grid(True, linestyle='--', alpha=0.5)

plt.xlim(left=0) 
plt.ylim(bottom=0)

plt.tight_layout()

caminho_arquivo = "/home/caio/Documents/!ufba/2026.1/teo_grafos/trabalho/snap/results/grafos_plotados/distribuicao_bruta.png"
plt.savefig(caminho_arquivo, dpi=300)
print(f"Gráfico bruto salvo com sucesso em: {caminho_arquivo}")

plt.close()