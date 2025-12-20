#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Edge {
    string u, v;
    int weight;

    Edge() : weight(0) {}
    Edge(const string& u, const string& v, int w) : u(u), v(v), weight(w) {}
};

class EdgeArray {
private:
    Edge* edges;
    int capacity;
    int size;

    void resize() {
        capacity = capacity == 0 ? 1 : capacity * 2;
        Edge* newEdges = new Edge[capacity];
        for (int i = 0; i < size; i++) {
            newEdges[i] = edges[i];
        }
        delete[] edges;
        edges = newEdges;
    }

public:
    EdgeArray() : edges(nullptr), capacity(0), size(0) {}

    ~EdgeArray() {
        delete[] edges;
    }

    void add(const Edge& e) {
        if (size >= capacity) resize();
        edges[size++] = e;
    }

    Edge& operator[](int index) {
        return edges[index];
    }

    const Edge& operator[](int index) const {
        return edges[index];
    }

    int getSize() const { return size; }

    bool isEmpty() const { return size == 0; }
};

class StringArray {
private:
    string* data;
    int capacity;
    int size;

    void resize() {
        capacity = capacity == 0 ? 1 : capacity * 2;
        string* newData = new string[capacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    StringArray() : data(nullptr), capacity(0), size(0) {}

    ~StringArray() {
        delete[] data;
    }

    void add(const string& s) {
        if (size >= capacity) resize();
        data[size++] = s;
    }

    string& operator[](int index) {
        return data[index];
    }

    const string& operator[](int index) const {
        return data[index];
    }

    int getSize() const { return size; }

    bool isEmpty() const { return size == 0; }

    int find(const string& s) const {
        for (int i = 0; i < size; i++) {
            if (data[i] == s) return i;
        }
        return -1;
    }
};

class DSU {
private:
    int* parent;
    int* rank;
    int n;

public:
    DSU(int size) : n(size) {
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    ~DSU() {
        delete[] parent;
        delete[] rank;
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int xRoot = find(x);
        int yRoot = find(y);

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

void sortEdgesByWeight(EdgeArray& edges) {
    for (int i = 0; i < edges.getSize() - 1; i++) {
        for (int j = 0; j < edges.getSize() - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
}

class Stack {
private:
    string* data;
    int capacity;
    int topIndex;

    void resize() {
        capacity = capacity == 0 ? 1 : capacity * 2;
        string* newData = new string[capacity];
        for (int i = 0; i <= topIndex; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    Stack() : data(nullptr), capacity(0), topIndex(-1) {}

    ~Stack() {
        delete[] data;
    }

    void push(const string& s) {
        if (topIndex + 1 >= capacity) resize();
        data[++topIndex] = s;
    }

    string pop() {
        if (topIndex >= 0) {
            return data[topIndex--];
        }
        return "";
    }

    bool isEmpty() const {
        return topIndex < 0;
    }
};

class Queue {
private:
    string* data;
    int capacity;
    int front;
    int rear;
    int count;

    void resize() {
        capacity = capacity == 0 ? 1 : capacity * 2;
        string* newData = new string[capacity];

        for (int i = 0; i < count; i++) {
            newData[i] = data[(front + i) % (capacity / 2)];
        }

        delete[] data;
        data = newData;
        front = 0;
        rear = count;
    }

public:
    Queue() : data(nullptr), capacity(0), front(0), rear(0), count(0) {}

    ~Queue() {
        delete[] data;
    }

    void enqueue(const string& s) {
        if (count >= capacity) resize();
        data[rear] = s;
        rear = (rear + 1) % capacity;
        count++;
    }

    string dequeue() {
        if (count > 0) {
            string s = data[front];
            front = (front + 1) % capacity;
            count--;
            return s;
        }
        return "";
    }

    bool isEmpty() const {
        return count == 0;
    }
};

void DFS(int** matrix, const StringArray& vertices, int start, bool* visited) {
    Stack stack;
    stack.push(vertices[start]);

    cout << "DFS traversal: ";

    while (!stack.isEmpty()) {
        string current = stack.pop();
        int currentIdx = vertices.find(current);

        if (!visited[currentIdx]) {
            visited[currentIdx] = true;
            cout << current << " ";

            for (int i = 0; i < vertices.getSize(); i++) {
                if (matrix[currentIdx][i] > 0 && !visited[i]) {
                    stack.push(vertices[i]);
                }
            }
        }
    }
    cout << endl;
}

void BFS(int** matrix, const StringArray& vertices, int start, bool* visited) {
    Queue queue;
    queue.enqueue(vertices[start]);
    visited[start] = true;

    cout << "BFS traversal: ";

    while (!queue.isEmpty()) {
        string current = queue.dequeue();
        cout << current << " ";

        int currentIdx = vertices.find(current);
        for (int i = 0; i < vertices.getSize(); i++) {
            if (matrix[currentIdx][i] > 0 && !visited[i]) {
                visited[i] = true;
                queue.enqueue(vertices[i]);
            }
        }
    }
    cout << endl;
}

bool readGraph(const string& filename, StringArray& vertices, int**& matrix, EdgeArray& edges) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open file " << filename << endl;
        return false;
    }

    string line;
    if (!getline(file, line)) {
        cerr << "Error: file is empty" << endl;
        return false;
    }

    string current;
    for (char c : line) {
        if (c == ' ' && !current.empty()) {
            vertices.add(current);
            current.clear();
        }
        else if (c != ' ') {
            current += c;
        }
    }
    if (!current.empty()) {
        vertices.add(current);
    }

    int n = vertices.getSize();
    if (n == 0) {
        cerr << "Error: no vertices in file" << endl;
        return false;
    }

    matrix = new int* [n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }

    // Reading adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!(file >> matrix[i][j])) {
                cerr << "Error reading matrix at position [" << i << "," << j << "]" << endl;
                for (int k = 0; k <= i; k++) {
                    delete[] matrix[k];
                }
                delete[] matrix;
                return false;
            }
            if (j > i && matrix[i][j] > 0) {
                edges.add(Edge(vertices[i], vertices[j], matrix[i][j]));
            }
        }
    }

    file.close();
    return true;
}

EdgeArray kruskal(EdgeArray& allEdges, const StringArray& vertices) {
    EdgeArray sortedEdges;

    for (int i = 0; i < allEdges.getSize(); i++) {
        sortedEdges.add(allEdges[i]);
    }

    sortEdgesByWeight(sortedEdges);

    DSU dsu(vertices.getSize());

    EdgeArray mst;

    for (int i = 0; i < sortedEdges.getSize(); i++) {
        const Edge& edge = sortedEdges[i];
        int uIdx = vertices.find(edge.u);
        int vIdx = vertices.find(edge.v);

        if (uIdx != -1 && vIdx != -1 && dsu.find(uIdx) != dsu.find(vIdx)) {
            mst.add(edge);
            dsu.unite(uIdx, vIdx);
        }
    }

    return mst;
}

void printResult(const EdgeArray& mst) {
    if (mst.isEmpty()) {
        cout << "Graph is disconnected or empty" << endl;
        return;
    }

    EdgeArray sortedMst;
    for (int i = 0; i < mst.getSize(); i++) {
        sortedMst.add(mst[i]);
    }

    for (int i = 0; i < sortedMst.getSize() - 1; i++) {
        for (int j = 0; j < sortedMst.getSize() - i - 1; j++) {
            string u1 = sortedMst[j].u;
            string v1 = sortedMst[j].v;
            string u2 = sortedMst[j + 1].u;
            string v2 = sortedMst[j + 1].v;

            if (u1 > v1) {
                string temp = u1;
                u1 = v1;
                v1 = temp;
            }
            if (u2 > v2) {
                string temp = u2;
                u2 = v2;
                v2 = temp;
            }

            if (u1 > u2 || (u1 == u2 && v1 > v2)) {
                Edge temp = sortedMst[j];
                sortedMst[j] = sortedMst[j + 1];
                sortedMst[j + 1] = temp;
            }
        }
    }

    int totalWeight = 0;
    cout << "\nMinimum spanning tree:" << endl;
    for (int i = 0; i < sortedMst.getSize(); i++) {
        cout << sortedMst[i].u << " " << sortedMst[i].v << endl;
        totalWeight += sortedMst[i].weight;
    }
    cout << "Total weight: " << totalWeight << endl;
}

void freeMatrix(int** matrix, int n) {
    if (matrix) {
        for (int i = 0; i < n; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "=== Kruskal's Algorithm for Minimum Spanning Tree ===" << endl;
    cout << "=== Coursework Implementation ===" << endl;

    string filename;
    cout << "\nEnter the filename with the adjacency matrix: ";
    cin >> filename;

    StringArray vertices;
    EdgeArray edges;
    int** matrix = nullptr;

    if (!readGraph(filename, vertices, matrix, edges)) {
        cout << "Error loading graph. Please check the file." << endl;
        return 1;
    }

    int n = vertices.getSize();
    cout << "\nGraph loaded:" << endl;
    cout << "Vertices: " << n << endl;
    cout << "Edges: " << edges.getSize() << endl;

    if (n > 0) {
        bool* visitedDFS = new bool[n];
        bool* visitedBFS = new bool[n];

        for (int i = 0; i < n; i++) {
            visitedDFS[i] = false;
            visitedBFS[i] = false;
        }

        cout << "\n--- Graph traversals ---" << endl;
        DFS(matrix, vertices, 0, visitedDFS);
        BFS(matrix, vertices, 0, visitedBFS);

        delete[] visitedDFS;
        delete[] visitedBFS;
    }

    cout << "\n--- Kruskal's Algorithm ---" << endl;
    EdgeArray mst = kruskal(edges, vertices);
    printResult(mst);

    freeMatrix(matrix, n);

    cout << "\nProgram completed successfully!" << endl;
    cout << "Press Enter to exit...";
    cin.ignore();
    cin.get();

    return 0;
}
