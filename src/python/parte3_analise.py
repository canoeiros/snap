import networkx as nx
import matplotlib.pyplot as plt
import random
import math
import time


G = nx.read_edgelist("../../data/soc-sign-Slashdot081106.txt", comments='#', create_using=nx.DiGraph(), nodetype=int, data=(('sign', int),))

N = G.number_of_nodes()
E = G.number_of_edges()
print(f"Grafo carregado: {N} nós e {E} arestas.")

