#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <queue>
#include <stack>

using namespace std;

class AdjacencyMatrix {
private:
    vector<vector<int>> matrix;
    vector<string> vertices;

public:
    AdjacencyMatrix(const vector<string>& verts, const vector<vector<int>>& mat)
        : vertices(verts), matrix(mat) {
    }

    vector<pair<string, int>> getNeighbors(const string& vertex) const {
        vector<pair<string, int>> neighbors;
        int index = find(vertices.begin(), vertices.end(), vertex) - vertices.begin();

        if (index < vertices.size()) {
            for (size_t i = 0; i < vertices.size(); ++i) {
                if (matrix[index][i] > 0) {
                    neighbors.push_back({ vertices[i], matrix[index][i] });
                }
            }
        }
        return neighbors;
    }

    const vector<string>& getVertices() const { return vertices; }
};

class AdjacencyList {
private:
    map<string, vector<pair<string, int>>> list;

public:
    void addEdge(const string& u, const string& v, int weight) {
        list[u].push_back({ v, weight });
        list[v].push_back({ u, weight });
    }

    vector<pair<string, int>> getNeighbors(const string& vertex) const {
        auto it = list.find(vertex);
        if (it != list.end()) {
            return it->second;
        }
        return {};
    }

    vector<string> getVertices() const {
        vector<string> vertices;
        for (const auto& pair : list) {
            vertices.push_back(pair.first);
        }
        return vertices;
    }

    void clear() { list.clear(); }
};

class IncidenceMatrix {
private:
    vector<string> vertices;
    vector<vector<int>> matrix;
    vector<tuple<string, string, int>> edges;

public:
    IncidenceMatrix(const vector<string>& verts, const vector<tuple<string, string, int>>& edgesList)
        : vertices(verts) {
        matrix.resize(vertices.size(), vector<int>(edgesList.size(), 0));
        edges = edgesList;

        for (size_t edgeIdx = 0; edgeIdx < edgesList.size(); ++edgeIdx) {
            auto [u, v, w] = edgesList[edgeIdx];
            int uIdx = find(vertices.begin(), vertices.end(), u) - vertices.begin();
            int vIdx = find(vertices.begin(), vertices.end(), v) - vertices.begin();

            if (uIdx < vertices.size()) matrix[uIdx][edgeIdx] = 1;
            if (vIdx < vertices.size()) matrix[vIdx][edgeIdx] = 1;
        }
    }

    void printMatrix() const {
        cout << "Матрица инцидентности:" << endl;
        cout << "Вершины/Ребра: ";
        for (size_t i = 0; i < edges.size(); ++i) {
            auto [u, v, w] = edges[i];
            cout << u << "-" << v << " ";
        }
        cout << endl;

        for (size_t i = 0; i < vertices.size(); ++i) {
            cout << vertices[i] << ": ";
            for (size_t j = 0; j < edges.size(); ++j) {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

void DFS(const AdjacencyList& graph, const string& start, map<string, bool>& visited) {
    stack<string> s;
    s.push(start);

    cout << "DFS обход: ";

    while (!s.empty()) {
        string current = s.top();
        s.pop();

        if (!visited[current]) {
            visited[current] = true;
            cout << current << " ";

            auto neighbors = graph.getNeighbors(current);
            for (const auto& [neighbor, weight] : neighbors) {
                if (!visited[neighbor]) {
                    s.push(neighbor);
                }
            }
        }
    }
    cout << endl;
}

void BFS(const AdjacencyList& graph, const string& start, map<string, bool>& visited) {
    queue<string> q;
    q.push(start);
    visited[start] = true;

    cout << "BFS обход: ";

    while (!q.empty()) {
        string current = q.front();
        q.pop();
        cout << current << " ";

        auto neighbors = graph.getNeighbors(current);
        for (const auto& [neighbor, weight] : neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    cout << endl;
}


class DSU {
private:
    map<string, string> parent;
    map<string, int> rank;

public:
    void makeSet(const string& x) {
        parent[x] = x;
        rank[x] = 0;
    }

    string find(const string& x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(const string& x, const string& y) {
        string xRoot = find(x);
        string yRoot = find(y);

        if (xRoot == yRoot) return;

        if (rank[xRoot] < rank[yRoot]) {
            parent[xRoot] = yRoot;
        }
        else if (rank[xRoot] > rank[yRoot]) {
            parent[yRoot] = xRoot;
        }
        else {
            parent[yRoot] = xRoot;
            rank[xRoot]++;
        }
    }
};


struct Edge {
    string u, v;
    int weight;
    Edge(string u, string v, int w) : u(u), v(v), weight(w) {}

    bool operator<(const Edge& other) const {
        if (u != other.u) return u < other.u;
        return v < other.v;
    }
};

bool compareEdgesByWeight(const Edge& a, const Edge& b) {
    return a.weight < b.weight;
}

vector<Edge> kruskal(const vector<Edge>& edges, const vector<string>& vertices) {
    vector<Edge> sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end(), compareEdgesByWeight);

    DSU dsu;
    for (const auto& v : vertices) {
        dsu.makeSet(v);
    }

    vector<Edge> mst;
    for (const auto& edge : sortedEdges) {
        if (dsu.find(edge.u) != dsu.find(edge.v)) {
            mst.push_back(edge);
            dsu.unite(edge.u, edge.v);
        }
    }

    return mst;
}

vector<Edge> readGraphAndBuildStructures(const string& filename,
    AdjacencyMatrix& adjMatrix,
    AdjacencyList& adjList,
    IncidenceMatrix& incMatrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        exit(1);
    }

    string line;
    getline(file, line);
    stringstream ss(line);
    vector<string> vertices;
    string vertex;

    while (ss >> vertex) {
        vertices.push_back(vertex);
    }

    vector<vector<int>> matrix(vertices.size(), vector<int>(vertices.size()));
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = 0; j < vertices.size(); ++j) {
            file >> matrix[i][j];
        }
    }
    file.close();

    vector<Edge> edges;
    vector<tuple<string, string, int>> edgesForIncMatrix;

    adjList.clear();

    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            if (matrix[i][j] > 0) {
                edges.push_back(Edge(vertices[i], vertices[j], matrix[i][j]));
                edgesForIncMatrix.push_back({ vertices[i], vertices[j], matrix[i][j] });
                adjList.addEdge(vertices[i], vertices[j], matrix[i][j]);
            }
        }
    }

    adjMatrix = AdjacencyMatrix(vertices, matrix);
    incMatrix = IncidenceMatrix(vertices, edgesForIncMatrix);

    return edges;
}

void printResult(const vector<Edge>& mst) {
    vector<Edge> sortedMst = mst;

    for (auto& edge : sortedMst) {
        if (edge.u > edge.v) {
            swap(edge.u, edge.v);
        }
    }

    sort(sortedMst.begin(), sortedMst.end());

    int totalWeight = 0;
    cout << "Минимальное остовное дерево:" << endl;
    for (const auto& edge : sortedMst) {
        cout << edge.u << " " << edge.v << endl;
        totalWeight += edge.weight;
    }
    cout << "Суммарный вес: " << totalWeight << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    string filename;
    cout << "Введите имя файла с матрицей смежности: ";
    cin >> filename;

    AdjacencyMatrix adjMatrix({}, {});
    AdjacencyList adjList;
    IncidenceMatrix incMatrix({}, {});

    vector<Edge> edges = readGraphAndBuildStructures(filename, adjMatrix, adjList, incMatrix);
    vector<string> vertices = adjList.getVertices();

    if (!vertices.empty()) {
        map<string, bool> visitedDFS, visitedBFS;

        for (const auto& v : vertices) visitedDFS[v] = false;
        DFS(adjList, vertices[0], visitedDFS);

        for (const auto& v : vertices) visitedBFS[v] = false;
        BFS(adjList, vertices[0], visitedBFS);
    }

    incMatrix.printMatrix();

    vector<Edge> mst = kruskal(edges, vertices);
    printResult(mst);

    return 0;
}