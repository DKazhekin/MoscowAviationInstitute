import networkx as nx
import matplotlib.pyplot as plt
from tkinter import *
import copy



def get_nodes_number():
    global numberOfNodes
    numberOfNodes = int(numberOfNodes_entry.get())
    window1.destroy()


def get_matrix():
    for i in range(1, numberOfNodes + 1):
        string1 = matrix_entry1.get(str(float(i)), str(i) + '.20')
        matrix_vesov.append(list(map(int, string1.split())))
    window2.destroy()


def get_matrix_propusk():
    for i in range(1, numberOfNodes + 1):
            string2 = matrix_entry2.get(str(float(i)), str(i) + '.20')
            matrix_propusk.append(list(map(int, string2.split())))
    window3.destroy()


window1 = Tk()
window1.title("Количество вершин в графе")
window1.eval('tk::PlaceWindow . center')

numberOfNodes_label = Label(text="Введите количество вершин:")
numberOfNodes_label.pack(expand=1)
numberOfNodes_entry = Entry()
numberOfNodes_entry.pack(expand=1)
numberOfNodes = 0
confirm_button = Button(text="Подтвердить", command=get_nodes_number)
confirm_button.pack(expand=1)
window1.mainloop()

matrix_vesov = []

window2 = Tk()
window2.title("Матрица весов")
window2.eval('tk::PlaceWindow . center')
matrix_label = Label(text="Введите матрицу весов:")
matrix_label.pack(expand=1)
matrix_entry1 = Text(width=25, height=15)
matrix_entry1.pack()

confirm_button2 = Button(text="Подтвердить", command=get_matrix)
confirm_button2.pack()
window2.mainloop()

matrix_propusk = []

window3 = Tk()
window3.title("Матрица пропускных способностей")
window3.eval('tk::PlaceWindow . center')
matrix_label = Label(text="Введите матрицу пропускных способностей:")
matrix_label.pack(expand=1)
matrix_entry2 = Text(width=25, height=15)
matrix_entry2.pack()

confirm_button3 = Button(text="Подтвердить", command=get_matrix_propusk)
confirm_button3.pack()
window2.mainloop()

n = numberOfNodes


final_matrix = []
matrix2 = []

tmp = copy.deepcopy(matrix_propusk)
tmp1 = copy.deepcopy(matrix_vesov)
tmp2 = copy.deepcopy(matrix_vesov)
for i in range(n):
    for j in range(n):
        if tmp1[i][j] != 0:
            tmp1[i][j] = tmp1[i][j] - tmp[i][j]

matrix = copy.deepcopy(tmp1)

for i in range(n):
    for j in range(n):
        if matrix_propusk[i][j] != 0:
            matrix_propusk[i][j] = [matrix_propusk[i][j], matrix_vesov[i][j]]

matrix2 = copy.deepcopy(matrix_propusk)
final_matrix = copy.deepcopy(matrix_propusk)

P = nx.Graph()

nodesz = []
for i in range(n):
    nodesz.append(str(i))

P.add_nodes_from(nodesz)

for i in range(n):
    for j in range(n):
        if tmp2[i][j] != 0:
            P.add_edge(str(i), str(j), weight=float(tmp2[i][j]))

pos = nx.spring_layout(P)
nx.draw(P, pos, with_labels=True)
labels = nx.get_edge_attributes(P, 'weight')
nx.draw_networkx_edge_labels(P, pos, edge_labels=labels)

plt.show()


class Graph:

    def __init__(self, graph):
        self.graph = graph  # residual graph
        self.ROW = len(graph)

    # self.COL = len(gr[0])

    '''Returns true if there is a path from source 's' to sink 't' in
    residual graph. Also fills parent[] to store the path '''

    def BFS(self, s, t, parent):

        # Mark all the vertices as not visited
        visited = [False] * (self.ROW)

        # Create a queue for BFS
        queue = []

        # Mark the source node as visited and enqueue it
        queue.append(s)
        visited[s] = True

        # Standard BFS Loop
        while queue:

            # Dequeue a vertex from queue and print it
            u = queue.pop(0)

            # Get all adjacent vertices of the dequeued vertex u
            # If a adjacent has not been visited, then mark it
            # visited and enqueue it
            for ind, val in enumerate(self.graph[u]):
                if visited[ind] == False and val > 0:
                    # If we find a connection to the sink node,
                    # then there is no point in BFS anymore
                    # We just have to set its parent and can return true
                    queue.append(ind)
                    visited[ind] = True
                    parent[ind] = u
                    if ind == t:
                        return True

        # We didn't reach sink in BFS starting
        # from source, so return false
        return False

    # Returns the maximum flow from s to t in the given graph
    def FordFulkerson(self, source, sink):

        # This array is filled by BFS and to store path
        parent = [-1] * (self.ROW)

        max_flow = 0  # There is no flow initially

        # Augment the flow while there is path from source to sink
        while self.BFS(source, sink, parent):

            # Find minimum residual capacity of the edges along the
            # path filled by BFS. Or we can say find the maximum flow
            # through the path found.
            path_flow = float("Inf")
            s = sink
            while (s != source):
                path_flow = min(path_flow, self.graph[parent[s]][s])
                s = parent[s]

            # Add path flow to overall flow
            max_flow += path_flow

            # update residual capacities of the edges and reverse edges
            # along the path
            v = sink
            while (v != source):
                u = parent[v]
                self.graph[u][v] -= path_flow
                self.graph[v][u] += path_flow
                v = parent[v]

        return max_flow


# Create a graph given in the above diagram


graph = matrix
g = Graph(graph)

source = 0
sink = n-1

max_flow = g.FordFulkerson(source, sink)


for i in range(n):
    for j in range(n):
        if matrix2[i][j] != 0:
            matrix2[i][j][1] = max(matrix2[i][j][1] - max_flow, matrix2[i][j][0])


min_flow = 0



for i in matrix2:
    if i[n-1] != 0:
        min_flow += i[n-1][1]

for i in range(n):
    for j in range(n):
        if matrix2[i][j] != 0:
            matrix2[i][j] = max(matrix2[i][j][1], matrix2[i][j][0])

G = nx.Graph()

nodes = []
for i in range(n):
    nodes.append(str(i))

G.add_nodes_from(nodes)

for i in range(n):
    for j in range(n):
        if matrix2[i][j] != 0:
            G.add_edge(str(i), str(j), weight=float(matrix2[i][j]))

pos = nx.spring_layout(G)
nx.draw(G, pos, with_labels=True)
labels = nx.get_edge_attributes(G, 'weight')
nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)


plt.show()

