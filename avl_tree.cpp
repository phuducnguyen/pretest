#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    vector<char> A;
    Node* left;
    Node* right;
    int height;
};

int getNumVowels(const vector<char>& key) {
    int count = 0;
    for (char c : key) {
        if (c == 'A' || c == 'E' || c == 'O' || c == 'U' || c == 'I') {
            count++;
        }
    }
    return count;
}

int getMostFrequentCharCount(const vector<char>& key) {
    int freq[256] = {0};
    for (char c : key) {
        freq[c]++;
    }
    int maxFreq = 0;
    for (int i = 0; i < 256; ++i) {
        maxFreq = max(maxFreq, freq[i]);
    }
    return maxFreq;
}

bool compareKeys(const vector<char>& keyA, const vector<char>& keyB) {
    int vowelsA = getNumVowels(keyA);
    int vowelsB = getNumVowels(keyB);

    // Notice here < or > (based on topic)
    if (vowelsA > vowelsB) {
        return true;
    } else if (vowelsA == vowelsB) {
        return getMostFrequentCharCount(keyA) > getMostFrequentCharCount(keyB);
    }
    return false;
}

int getHeight(Node* node) {
    if (node == nullptr)
        return -1;
    return node->height;
}

int getBalanceFactor(Node* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

Node* insert(Node* root, vector<char>& key) {
    if (root == nullptr) {
        Node* newNode = new Node;
        newNode->A = key;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 0;
        return newNode;
    }

    if (key < root->A) {
        root->left = insert(root->left, key);
    } else if (key > root->A) {
        root->right = insert(root->right, key);
    } else {
        return root;
    }

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getBalanceFactor(root);

    if (balance > 1 && key < root->left->A)
        return rotateRight(root);

    if (balance < -1 && key > root->right->A)
        return rotateLeft(root);

    if (balance > 1 && key > root->left->A) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && key < root->right->A) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

void inorderTraversal(Node* root) {
    if (root == nullptr)
        return;

    inorderTraversal(root->left);

    cout << "{";
    for (size_t i = 0; i < root->A.size(); ++i) {
        cout << root->A[i];
        if (i != root->A.size() - 1) {
            cout << ", ";
        }
    }
    cout << "} ";

    inorderTraversal(root->right);
}

void deleteTree(Node* root) {
    if (root == nullptr)
        return;

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

int main() {
    Node* root = nullptr;
    ifstream inputFile("data.txt");
    string line;

    if (!inputFile) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    // Ignore header line
    string header = "";
    getline(inputFile, line);

    while (getline(inputFile, line)) {
        vector<char> key;
        for (char c : line) {
            if (c != ' ' && c != '\t') {
                key.push_back(c);
            }
        }
        root = insert(root, key);
    }

    inputFile.close();

    // Traversal and printing
    inorderTraversal(root);
    cout << "\n=====\n";

    // Free memory
    deleteTree(root);

    return 0;
}
