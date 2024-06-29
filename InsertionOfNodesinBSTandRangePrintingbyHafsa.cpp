
#include <iostream>
using namespace std;
class Node {
public:
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Function to print values in the BST within the range [k1, k2]
void printRange(Node* root, int k1, int k2) {
    if (root == nullptr)
        return;

    // Traverse the left subtree if root's value is greater than k1
    if (k1 < root->data)
        printRange(root->left, k1, k2);

    // If root's value is within the range, print it
    if (k1 <= root->data && k2 >= root->data)
        cout << root->data << " ";

    // Traverse the right subtree if root's value is less than k2
    if (k2 > root->data)
        printRange(root->right, k1, k2);
}

// Function to insert a new node into the BST
Node* insert(Node* root, int data) {
    if (root == nullptr)
        return new Node(data);

    if (data < root->data)
        root->left = insert(root->left, data);
    else if (data > root->data)
        root->right = insert(root->right, data);

    return root;
}

// input validation function
bool isValidInput(int& input) {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(256, '\n');
        cout << "Invalid input. Please enter a valid number: ";
        return false;
    }
    return true;
}

// Function to print the in-order traversal of the tree
void printInOrder(Node* root) {
    if (root == nullptr)
        return;

    printInOrder(root->left);
    cout << root->data << " ";
    printInOrder(root->right);
}

// Test function
int main() {
    // Initialize an empty Binary Search Tree
    Node* root = nullptr;

    // Input k1 and k2
    int k1, k2;
    cout << "Enter the range k1: ";
    cin >> k1;
    while (!isValidInput(k1))
        cin >> k1;

    cout << "Enter the range k2: ";
    cin >> k2;
    while (!isValidInput(k2))
        cin >> k2;

    // Input data to insert into the BST
    int data;
    char choice;
    do {
        cout << "Enter data to insert into BST: ";
        cin >> data;
        while (!isValidInput(data))
            cin >> data;

        root = insert(root, data);

        cout << "Do you want to insert more data? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    // Print the in-order traversal
    cout << "In-order traversal of the Binary Search Tree:" << endl;
    printInOrder(root);
    cout << endl;

    // Print values in the range [k1, k2]
    cout << "\nValues in the range [" << k1 << ", " << k2 << "]: ";
    printRange(root, k1, k2);
    cout << endl;

    return 0;
}