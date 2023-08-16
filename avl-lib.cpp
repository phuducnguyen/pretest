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
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' ||
            c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

bool compareKeys(const vector<char>& keyA, const vector<char>& keyB) {
    int numVowelsA = getNumVowels(keyA);
    int numVowelsB = getNumVowels(keyB);

    if (numVowelsA != numVowelsB) {
        return numVowelsA > numVowelsB;
    }

    // If both keys have the same number of vowels, compare the frequency of characters
    vector<int> freqA(256, 0);
    vector<int> freqB(256, 0);

    for (char c : keyA) {
        freqA[c]++;
    }

    for (char c : keyB) {
        freqB[c]++;
    }

    int maxFreqA = *max_element(freqA.begin(), freqA.end());
    int maxFreqB = *max_element(freqB.begin(), freqB.end());

    if (maxFreqA != maxFreqB) {
        return maxFreqA > maxFreqB;
    }

    return false;
}

// using int height in struct
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
        Node* newNode = new Node();
        newNode->A = key;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    if (compareKeys(key, root->A))
        root->left = insert(root->left, key);
    else if (compareKeys(root->A, key))
        root->right = insert(root->right, key);
    else
        return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getBalanceFactor(root);

    if (balance > 1 && compareKeys(key, root->left->A))
        return rotateRight(root);

    if (balance < -1 && compareKeys(root->right->A, key))
        return rotateLeft(root);

    if (balance > 1 && compareKeys(root->left->A, key)) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && compareKeys(key, root->right->A)) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// Inorder Traversal
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

// Preorder traversal
void preorderTraversal(Node* root) {
    if (root == nullptr)
        return;

    cout << "{";
    for (size_t i = 0; i < root->A.size(); ++i) {
        cout << root->A[i];
        if (i != root->A.size() - 1) {
            cout << ", ";
        }
    }
    cout << "} ";

    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

// Postorder traversal
void postorderTraversal(Node* root) {
    if (root == nullptr)
        return;

    postorderTraversal(root->left);
    postorderTraversal(root->right);

    cout << "{";
    for (size_t i = 0; i < root->A.size(); ++i) {
        cout << root->A[i];
        if (i != root->A.size() - 1) {
            cout << ", ";
        }
    }
    cout << "} ";
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

    // In ra kết quả
    inorderTraversal(root);

    return 0;
}
