import networkx as nx
import matplotlib.pyplot as plt

def get_graph_input():
    nodes = input("Enter the nodes of the graph (comma-separated): ").split(',')
    nodes = [node.strip() for node in nodes]
    
    G = nx.Graph()
    G.add_nodes_from(nodes)
    
    while True:
        edge = input("Enter an edge (node1 node2), or press Enter to finish: ")
        if edge == "":
            break
        node1, node2 = edge.split()
        if node1 in nodes and node2 in nodes:
            G.add_edge(node1, node2)
        else:
            print("Invalid nodes. Please try again.")
    
    return G

def visualize_graph(G):
    pos = nx.spring_layout(G)
    plt.figure(figsize=(10, 8))
    nx.draw(G, pos, with_labels=True, node_color='lightblue', 
            node_size=500, font_size=16, font_weight='bold')
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    plt.title("Graph Visualization")
    plt.axis('off')
    plt.show()

def main():
    print("Welcome to the Graph Input and Visualization Program!")
    G = get_graph_input()
    visualize_graph(G)

if __name__ == "__main__":
    main()
