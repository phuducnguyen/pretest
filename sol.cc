#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

void readMatrix(vector<vector<int>>& maxtrix, const string& filename, int& n);
void readAdjList(vector<vector<int>>& adjList, const string& filename, int& n);
vector<vector<int>> matrixToList(const vector<vector<int>>& matrix);
vector<vector<int>> listToMatrix(const vector<vector<int>>& adjList);

void printList(const vector<vector<int>>& list);
void printMatrix(const vector<vector<int>>& matrix);
bool isDirectedGraph(const vector<vector<int>>& graph);
int countEdges(const vector<vector<int>>& graph);
int countVertices(const vector<vector<int>>& graph);

int main() {
	ifstream inputFile("graph2.txt");
    if (!inputFile) {
        cerr << "Error: Cannot open file graph2.txt" << endl;
        return 1;
    }

    int n;
    inputFile >> n;

    vector<vector<int>> adjacencyList(n);
    for (int i = 0; i < n; ++i) {
        int count;
        inputFile >> count;
        adjacencyList[i].resize(count);
        for (int j = 0; j < count; ++j) {
            inputFile >> adjacencyList[i][j];
        }
    }

    printList(adjacencyList);

    vector<vector<int>> adjacencyMatrix = listToMatrix(adjacencyList);
    printMatrix(adjacencyMatrix);

    inputFile.close();
    return 0;
    // vector<vector<int>> matrix;
    // int n;
    // readMatrix(matrix, "graph1.txt", n);

    // printMatrix(matrix, n);

    // int edgeCount = countEdges(matrix);
    // int vertexCount = countVertices(matrix);

    // cout << "Number of Edges: " << edgeCount << endl;
    // cout << "Number of Vertices: " << vertexCount << endl;

    // vector<vector<int>> adjList = matrixToList(matrix);
    // printList(adjList);

    // cout << (isDirectedGraph(matrix) ? "YES" : "NO");

    // cout << endl;
    // return 0;
}

void readMatrix(vector<vector<int>>& matrix, const string& filename, int& n) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    inputFile >> n;
    matrix.resize(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inputFile >> matrix[i][j];
        }
    }

    inputFile.close();
}

void readAdjList(vector<vector<int>>& adjList, const string& filename, int& n) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    inputFile >> n;
    vector<vector<int>> adj(n);

    for (int i = 0; i < n; ++i) {
    	int count;
    	inputFile >> count;
    	adj[i].resize(count);
        for (int j = 0; j < count; ++j) {
            inputFile >> adj[i][j];
        }
    }
    
    inputFile.close();
}

vector<vector<int>> matrixToList(const vector<vector<int>>& matrix) {
	int n = matrix.size();
	vector<vector<int>> adjList(n);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (matrix[i][j] == 1) {
				adjList[i].push_back(j);
				// adjList[j].push_back(i); // Undirected graph, add the reverse edge
			}
		}
	}

	return adjList;
}

vector<vector<int>> listToMatrix(const vector<vector<int>>& adjList) {
    int n = adjList.size();
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j : adjList[i]) {
            adjMatrix[i][j] = 1;
        }
    }

    return adjMatrix;
}

void printMatrix(const vector<vector<int>>& matrix) {
	int n = matrix.size();
    cout << "Adjacency Matrix:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void printList(const vector<vector<int>>& adjList) {
    int n = adjList.size();
    for (int i = 0; i < n; ++i) {
        // cout << "Vertex " << i << ": ";
        for (int j = 0; j < adjList[i].size(); ++j) {
            cout << adjList[i][j] << " ";
        }
        cout << endl;
    }
}


bool isDirectedGraph(const vector<vector<int>>& graph) {
    for (int i = 0; i < graph.size(); ++i) {
        for (int j = 0; j < graph.size(); ++j) {
            if (graph[i][j] == 1 && graph[j][i] == 0) {
                return true;
            }
        }
    }
    return false;
}

int countEdges(const vector<vector<int>>& graph) {
	int n = graph.size();
	int edgeCount = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			edgeCount += graph[i][j];
		}
	}
	return edgeCount;
}

int countVertices(const vector<vector<int>>& graph) {
	return graph.size();
}