# Trabalho Análise SNAP

Análise estrutural e de algoritmos do grafo de sinais do Slashdot (dataset SNAP, Stanford).

## Pré-requisitos

- **C++:** `g++` com suporte a C++11
- **Python:** 3.14 (instalado no `venv/`)
- **Dados:** `data/soc-sign-Slashdot081106.txt`

---

## 1. Preparo do Ambiente

```bash
# Ativar o ambiente virtual Python
source venv/bin/activate
```

Se o `venv` não existir ou precisar recriar:

```bash
python3.14 -m venv venv
source venv/bin/activate
pip install networkx matplotlib numpy
```

---

## 2. C++ (Parte II) — Algoritmos

Todos os arquivos C++ estão em `src/cpp/`.

### Compilar

```bash
cd src/cpp
g++ -std=c++11 -O2 parte2_algoritmos.cpp -o parte2_algoritmos
```

> No VS Code, abra `parte2_algoritmos.cpp` e pressione `Ctrl+Shift+B` (task em `.vscode/tasks.json`).

### Executar

```bash
./parte2_algoritmos
```

O binário:
1. Carrega `../../data/soc-sign-Slashdot081106.txt`
2. Executa 30 rodadas de **Dijkstra**, **Tarjan (SCC)**, **BFS** e **DFS** com nós iniciais aleatórios
3. Calcula média, desvio padrão e intervalo de confiança (95%) de cada algoritmo
4. Estima **diâmetro**, **raio** e **caminho médio** via amostragem (300 nós)
5. Calcula **triângulos** e **coeficiente de clusterização** médio

Saída no terminal. Resultados salvos em `results/metricas_parte2.txt`.

### Arquivos C++

| Arquivo | Função |
|---|---|
| `graph.hpp` | Estrutura do grafo (`Graph`, `Edge`) e carregamento (`loadGraph`) |
| `algoritmos.hpp` | Implementação de Dijkstra, Bellman-Ford, Tarjan, BFS, DFS, amostragem, triângulos |
| `parte2_algoritmos.cpp` | `main()` — cronometragem e estatísticas |

---

## 3. Python (Partes I e III) — Análise e Plots

Todos em `src/python/`. Ative o `venv` antes:

```bash
source venv/bin/activate
cd src/python
```

### 3.1 Métricas Estruturais

```bash
python3 parte1_estrutural.py
```

Imprime: vértices, arestas, densidade, grau min/max/médio (total/in/out), componentes fracamente conexas.

### 3.2 Análise Avançada (Parte III)

```bash
python3 parte3_analise.py
```

- Plota distribuição de graus (lei de potência) → `results/grafos_plotados/lei_de_potencia.png`
- Teste de robustez: remove 5% aleatório e 5% hubs, mede LCC
- Estima parâmetros small-world teóricos
- Salva resultados em `results/metricas_parte3.txt`

### 3.3 Distribuição Bruta

```bash
python3 distribuicao.py
```

Plota distribuição de graus em escala linear → `results/grafos_plotados/distribuicao_bruta.png`.

### 3.4 Visualização do Grafo


```bash
python3 'visualizacao.py'
```

Extrai 100 nós via snowball sampling, desenha arestas verdes (sinal +1) e vermelhas (sinal -1) → `results/grafos_plotados/visualizacao_100_nos.png`.

---

## 4. Ordem de Execução Recomendada

1. `parte1_estrutural.py` — métricas básicas
2. `parte2_algoritmos` (C++) — algoritmos + estatísticas
3. `distribuicao.py` — plot da distribuição bruta
4. `parte3_analise.py` — lei de potência, robustez, small-world
5. `visualizacao.py` — visualização da rede

---

## 5. Limpeza

```bash
rm -f src/cpp/parte2_algoritmos
```

---

## Estrutura do Projeto

```
snap/
├── data/
│   └── soc-sign-Slashdot081106.txt      # Dataset SNAP (~24 MB)
├── src/
│   ├── cpp/
│   │   ├── graph.hpp                    # Estrutura do grafo + leitura
│   │   ├── algoritmos.hpp               # Implementação dos algoritmos
│   │   └── parte2_algoritmos.cpp        # Main (cronometragem)
│   └── python/
│       ├── parte1_estrutural.py         # Métricas da Parte I
│       ├── parte3_analise.py            # Análise da Parte III
│       ├── distribuicao.py              # Plot da distribuição de graus
│       └── visualizacao.py              # Visualização amostrada
├── results/
│   ├── metricas_parte1.txt              # Saída da Parte I
│   ├── metricas_parte2.txt              # Saída da Parte II (C++)
│   ├── metricas_parte3.txt              # Saída da Parte III
│   └── grafos_plotados/                 # Imagens geradas
├── venv/                                # Ambiente virtual Python
├── orientacoes.md                       # Enunciado do trabalho
└── README.md
```
