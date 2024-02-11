import networkx as nx
import matplotlib.pyplot as plt

# Input data
input_data = """
8 9 1
1 2 8
2 7 4
2 3 10
6 7 40
3 6 5
6 8 3
4 8 4
4 5 5
3 4 23

"""

# Parsing the input
lines = input_data.strip().split('\n')
n, m, k = map(int, lines[0].split())  # Number of nodes, edges, and the value of K
edges = [tuple(map(int, line.split())) for line in lines[1:]]

# Create a graph
G = nx.Graph()
G.add_weighted_edges_from(edges)

# Graph Visualization
pos = nx.spring_layout(G)  # positions for all nodes
nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500)
edge_labels = nx.get_edge_attributes(G, 'weight')
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)

plt.title(f"Graph Visualization (n={n}, m={m}, k={k})")
plt.show()
