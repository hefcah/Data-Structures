
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct Node {
    string word;
    int height;
    Node* left;
    Node* right;

    Node(string value) {
        word = value;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

class AVLTree {
private:
    Node* root;

    int getHeight(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    int getBalanceFactor(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }

    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));
        return newRoot;
    }

    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));
        return newRoot;
    }

    Node* insertRecursive(Node* node, string word) {
        if (node == nullptr) {
            return new Node(word);
        }
        if (word < node->word) {
            node->left = insertRecursive(node->left, word);
        }
        else if (word > node->word) {
            node->right = insertRecursive(node->right, word);
        }
        else {
            return node; // Duplicate words are not allowed
        }
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balanceFactor = getBalanceFactor(node);
        if (balanceFactor > 1) { // Left subtree is taller
            if (word < node->left->word) {
                return rotateRight(node);
            }
            else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        if (balanceFactor < -1) { // Right subtree is taller
            if (word > node->right->word) {
                return rotateLeft(node);
            }
            else {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }
        return node;
    }

    Node* findRecursive(Node* node, string word) {
        if (node == nullptr || node->word == word) {
            return node;
        }
        if (word < node->word) {
            return findRecursive(node->left, word);
        }
        return findRecursive(node->right, word);
    }

    Node* findMinNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    Node* deleteRecursive(Node* node, string word) {
        if (node == nullptr) {
            return node;
        }
        if (word < node->word) {
            node->left = deleteRecursive(node->left, word);
        }
        else if (word > node->word) {
            node->right = deleteRecursive(node->right, word);
        }
        else {
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* minNode = findMinNode(node->right);
            node->word = minNode->word;
            node->right = deleteRecursive(node->right, minNode->word);
        }
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balanceFactor = getBalanceFactor(node);
        if (balanceFactor > 1) {
            if (getBalanceFactor(node->left) >= 0) {
                return rotateRight(node);
            }
            else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        if (balanceFactor < -1) {
            if (getBalanceFactor(node->right) <= 0) {
                return rotateLeft(node);
            }
            else {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }
        return node;
    }

    void displayInOrderRecursive(Node* node) {
        if (node != nullptr) {
            displayInOrderRecursive(node->left);
            cout << node->word << " ";
            displayInOrderRecursive(node->right);
        }
    }

public:
    AVLTree() {
        root = nullptr;
    }

    void insert(string word) {
        root = insertRecursive(root, word);
    }

    void remove(string word) {
        root = deleteRecursive(root, word);
    }

    bool search(string word) {
        Node* result = findRecursive(root, word);
        return result != nullptr;
    }

    void displayInOrder() {
        displayInOrderRecursive(root);
    }
};

int main() {
    AVLTree tree;

    // Read words from file and insert into AVL tree
    ifstream file("wordlist.txt");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file wordlist.txt" << endl;
        return 1;
    }

    string word;
    while (file >> word) {
        tree.insert(word);
    }
    file.close();

    int choice;
    string userWord;

    while (true) {
        cout << "AVL Tree Menu:\n";
        cout << "1. Insert\n";
        cout << "2. Delete\n";
        cout << "3. Search\n";
        cout << "4. Display Inorder\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter a word to insert: ";
            cin >> userWord;
            tree.insert(userWord);
            cout << "Word inserted successfully!\n";
            break;

        case 2:
            cout << "Enter a word to delete: ";
            cin >> userWord;
            tree.remove(userWord);
            cout << "Word deleted successfully!\n";
            break;

        case 3:
            cout << "Enter a word to search: ";
            cin >> userWord;
            if (tree.search(userWord)) {
                cout << "Word found!\n";
            }
            else {
                cout << "Word not found.\n";
            }
            break;

        case 4:
            cout << "Words in inorder traversal: ";
            tree.displayInOrder();
            cout << endl;
            break;

        case 5:
            cout << "Exiting program...\n";
            return 0;

        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }

        cout << endl;
    }

    return 0;
}