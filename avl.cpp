#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

int treeHeight = -1;

struct Node {
    vector<char> A;
    Node* left;
    Node* right;
};

int getNumVowels(const vector<char>& key) {
    int count = 0;
    for (char c : key) {
        if (c == 'a' || c == 'e' || c == 'o' || c == 'u' || c == 'i' || 
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
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
    
    // A > B (output is not like expected)
    if (vowelsA > vowelsB) {
        return true;
    } else if (vowelsA == vowelsB) {
         // make sure that nodes with the same priority 1, 2 will be sorted lexicographically
        int freqA = getMostFrequentCharCount(keyA);
        int freqB = getMostFrequentCharCount(keyB);
        if (freqA > freqB) {
            return true;
        } else if (freqA == freqB) {
            return keyA < keyB; // Dictionary Ordered
        }
    }

    return false;
}

// recursive
int getHeight(Node* node) {
    if (node == nullptr)
        return -1;
    
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    
    return 1 + max(leftHeight, rightHeight);
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

    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

Node* insert(Node* root, vector<char>& key) {
    if (root == nullptr) {
        Node* newNode = new Node;
        newNode->A = key;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }

    if (compareKeys(key, root->A)) {
        root->left = insert(root->left, key);
    } else {
        root->right = insert(root->right, key);
    }

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

    // If needed
    // treeHeight = 1 + max(getHeight(root->left), getHeight(root->right));

    return root;
}

Node* findMax(Node* node) {
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}

Node* deleteNode(Node* root, const vector<char>& key) {
    if (root == nullptr) {
        return root;
    }

    if (compareKeys(key, root->A)) {
        root->left = deleteNode(root->left, key);
    } else if (compareKeys(root->A, key)) {
        root->right = deleteNode(root->right, key);
    } else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = findMax(root->left);
        root->A = temp->A;
        root->left = deleteNode(root->left, temp->A);
    }

    int balance = getBalanceFactor(root);

    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rotateRight(root);

    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return rotateLeft(root);

    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    // Tính toán lại độ cao của nút
    // treeHeight = 1 + max(getHeight(root->left), getHeight(root->right));

    return root;
}

void inorderTraversal(Node* root) {
    if (root == nullptr)
        return;

    inorderTraversal(root->left);

    cout << "{";
    for (size_t i = 0; i < root->A.size(); i++) {
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

void deleteTree(Node* root) {
    if (root == nullptr)
        return;

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

void getNodesToDelete(Node* root, vector<vector<char>>& nodesToDelete) {
    if (root == nullptr) {
        return;
    }

    getNodesToDelete(root->left, nodesToDelete);

    if (getNumVowels(root->A) == 2) {
        nodesToDelete.push_back(root->A);
    }

    getNodesToDelete(root->right, nodesToDelete);
}

int main() {
    Node* root = nullptr;
    ifstream inputFile("data.txt");
    string line;

    if (!inputFile) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    // Ignore first line
    string ignore = "";
    getline(inputFile, ignore);

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

    // LNR
    // Q4: Print LNR

    // Duyệt và in cây AVL theo thứ tự LNR (Inorder) trước khi xóa
    inorderTraversal(root);
    cout << endl;
    cout << "\n=====\n";

    // Thu thập danh sách các nút cần xóa
    vector<vector<char>> nodesToDelete;
    getNodesToDelete(root, nodesToDelete);

    // // In danh sách các nút cần xóa
    // cout << "Nodes to delete:" << endl;
    // for (const vector<char>& key : nodesToDelete) {
    //     for (char c : key) {
    //         cout << c;
    //     }
    //     cout << endl;
    // }

    // Xóa các nút có số lượng kí tự nguyên âm bằng 2
    for (const vector<char>& key : nodesToDelete) {
        root = deleteNode(root, key);
    }

    // Duyệt và in cây AVL theo thứ tự LNR (Inorder) sau khi xóa
    inorderTraversal(root);
    cout << endl;

    // treeHeight = getHeight(root);
    // cout << "Tree height: " << treeHeight << endl;

    // // Q5: Delete node
    // for (int i = 0; i < 256; ++i) {
    //     vector<char> key(1, char(i));
    //     if (getNumVowels(key) == 2) {
    //         root = deleteNode(root, key);
    //     }
    // }

    // inorderTraversal(root);
    // cout << endl;

    // Free memory
    deleteTree(root);

    return 0;
}