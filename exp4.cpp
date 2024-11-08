#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

class EXP4 {
private:
    vector<vector<int>> graph;              // Adjacency matrix for the original random graph
    vector<vector<int>> connectedMatrix;    // Adjacency matrix for connections
    vector<vector<int>> reachableMatrix;    // Reachability matrix
    vector<int> degrees;                    // Array to store degrees of nodes
    int n;                                  // Total number of nodes
    int edge;                               // Total number of edges
    int sumDegrees;                         // Sum of all degrees
    int has;                                // Flag to indicate if Euler path/circuit is found
    vector<vector<int>> vis;                // Temporary matrix for visited edges
    vector<int> node;                       // Array to store nodes in Euler path
    int count;                              // Number of nodes in the current Euler path

public:
    EXP4(int size)
        : graph(size, vector<int>(size)), connectedMatrix(size, vector<int>(size)),
        reachableMatrix(size, vector<int>(size)), degrees(size, 0), vis(size, vector<int>(size, 0)),
        node(size, 0), n(0), edge(0), sumDegrees(0), has(0), count(0) {}

    // Generates a random adjacency matrix for an undirected graph with n nodes and m edges
    void randGraph(int n, int m) {
        srand(time(0));
        EXP4::n = n;
        graph = vector<vector<int>>(n, vector<int>(n, 0));  // Initialize adjacency matrix with 0s
        int edgesAdded = 0;
        while (edgesAdded < m) {
            int i = rand() % n;
            int j = rand() % n;
            if (i != j && graph[i][j] == 0) {  // Avoid self-loops and duplicate edges
                graph[i][j] = 1;
                graph[j][i] = 1;  // Ensure symmetry for undirected graph
                edgesAdded++;
            }
        }
        connectedMatrix = graph;
    }

    // Matrix addition
    vector<vector<int>> addition(const vector<vector<int>>& m1, const vector<vector<int>>& m2) {
        vector<vector<int>> tmp(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                tmp[i][j] = m1[i][j] + m2[i][j];
            }
        }
        return tmp;
    }

    // Matrix multiplication
    vector<vector<int>> multiplication(const vector<vector<int>>& m1, const vector<vector<int>>& m2) {
        vector<vector<int>> tmp(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    tmp[i][j] += m1[i][k] * m2[k][j];
                }
            }
        }
        return tmp;
    }

    // Calculate the reachability matrix
    void getReachableMatrix() {
        reachableMatrix = graph;
        for (int i = 0; i < n - 1; i++) {
            graph = multiplication(graph, connectedMatrix);
            reachableMatrix = addition(graph, reachableMatrix);
        }
    }

    // Check if the graph is reachable (i.e., all nodes are accessible from any other node)
    bool isReachable() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (reachableMatrix[i][j] == 0) return false;
            }
        }
        return true;
    }

    // Convert the reachability matrix to a binary matrix
    void unitization() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (reachableMatrix[i][j] != 0) reachableMatrix[i][j] = 1;
            }
        }
    }

    // Check if the graph is an Eulerian graph or semi-Eulerian
    int isEulerMap() {
        if (n == 0) return 0;
        if (!isReachable()) return 0;
        int num = 0;
        for (int i = 0; i < n; i++) {
            if (degrees[i] % 2 == 1) num++;
        }
        if (num == 2) return 1;  // Semi-Eulerian
        if (num == 0) return 2;  // Eulerian
        return 0;
    }

    // Calculate the degrees of all nodes
    void getDegrees() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (connectedMatrix[i][j] != 0) degrees[i]++;
            }
        }
        for (int i = 0; i < n; i++) sumDegrees += degrees[i];
        edge = sumDegrees / 2;
    }

    // Print the reachability matrix
    void printReachableMatrix() {
        cout << "Reachable matrix:" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << reachableMatrix[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    // Print the adjacency matrix
    void printConnectedMatrix() {
        cout << "Connected matrix:" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << connectedMatrix[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    // Core algorithm to find the Eulerian path or circuit
    void getEulerMap(int currentIndex) {
        if (has == 1) return;
        if (count == edge + 1) {
            for (int i = 0; i < count; i++) {
                if (i == 0) cout << node[i];
                else cout << " -> " << node[i];
            }
            cout << endl;
            has = 1;
        }
        else {
            for (int i = 0; i < n; i++) {
                if (connectedMatrix[currentIndex][i] == 1 && vis[currentIndex][i] == 0) {
                    vis[i][currentIndex] = vis[currentIndex][i] = 1;
                    node[count++] = i;
                    getEulerMap(i);
                    count--;
                    vis[i][currentIndex] = vis[currentIndex][i] = 0;
                }
            }
        }
    }

    // Print the Eulerian path or circuit
    void printEulerMap() {
        int result = isEulerMap();
        if (result == 1) {
            cout << "Euler road:" << endl;
            for (int i = 0; i < n; i++) {
                int t = 0;
                for (int j = 0; j < n; j++) {
                    if (connectedMatrix[i][j] == 1) t++;
                }
                if (t % 2 == 1) {
                    node[count++] = i;
                    getEulerMap(i);
                    count--;
                    break;
                }
            }
        }
        else if (result == 2) {
            cout << "Euler circuit:" << endl;
            node[count++] = 0;
            getEulerMap(0);
            count--;
        }
        else {
            cout << "Neither Euler road nor Euler circuit is found." << endl;
        }
    }
};

int main() {
    int n, m;
    EXP4 e(100);
    cout << "Number of node: ";
    cin >> n;
    cout << "Number of edge: ";
    cin >> m;
    cout << endl;
    if (n <= 0 || m <= 0 || m * 2 > n * n - n) {  // Validate the relationship between nodes and edges
        cout << "Wrong relationship between n and m!" << endl;
        return -1;
    }
    e.randGraph(n, m);          // Generate random adjacency matrix
    e.printConnectedMatrix();    // Print adjacency matrix
    e.getReachableMatrix();      // Compute reachability matrix
    e.unitization();             // Convert reachability matrix to binary form
    e.printReachableMatrix();    // Print reachability matrix
    cout << "This is " << (e.isReachable() ? "a " : "an un") << "reachable graph." << endl;
    e.getDegrees();              // Calculate degrees of nodes
    e.printEulerMap();           // Print Eulerian path or circuit
    return 0;
}
