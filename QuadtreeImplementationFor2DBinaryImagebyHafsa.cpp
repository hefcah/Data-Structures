
#include <iostream>
using namespace std;
struct QuadTreeNode {
    bool isLeaf; // Indicates whether the node is a leaf node 
    bool color; // Color of the node (only valid for leaf nodes)
    QuadTreeNode* children[4]; // Pointers to the four child nodes
};

// Function to build a quadtree from a 2D binary matrix
QuadTreeNode* buildQuadTree(bool** matrix, int size, int startX, int startY) {
    if (size == 1) {
        // Create a leaf node
        QuadTreeNode* node = new QuadTreeNode;
        node->isLeaf = true;
        node->color = matrix[startX][startY];
        for (int i = 0; i < 4; ++i) {
            node->children[i] = nullptr;
        }
        return node;
    }

    // Check if the quadrant contains pixels of different colors
    bool firstColor = matrix[startX][startY];
    bool sameColor = true;
    for (int i = startX; i < startX + size; ++i) {
        for (int j = startY; j < startY + size; ++j) {
            if (matrix[i][j] != firstColor) {
                sameColor = false;
                break;
            }
        }
        if (!sameColor) break;
    }

    if (sameColor) {
        // Create a leaf node
        QuadTreeNode* node = new QuadTreeNode;
        node->isLeaf = true;
        node->color = firstColor;
        for (int i = 0; i < 4; ++i) {
            node->children[i] = nullptr;
        }
        return node;
    }
    else {
        // Create an internal node and recursively build children
        QuadTreeNode* node = new QuadTreeNode;
        node->isLeaf = false;
        for (int i = 0; i < 4; ++i) {
            int childStartX = startX + (i / 2) * size / 2;
            int childStartY = startY + (i % 2) * size / 2;
            node->children[i] = buildQuadTree(matrix, size / 2, childStartX, childStartY);
        }
        return node;
    }
}

// Function to convert a quadtree back into a 2D binary matrix
void quadTreeToMatrix(QuadTreeNode* root, bool** matrix, int size, int startX, int startY) {
    if (root->isLeaf) {
        // Fill the corresponding region with the leaf node's color
        for (int i = startX; i < startX + size; ++i) {
            for (int j = startY; j < startY + size; ++j) {
                matrix[i][j] = root->color;
            }
        }
    }
    else {
        // Recursively convert children
        int newSize = size / 2;
        quadTreeToMatrix(root->children[0], matrix, newSize, startX, startY);
        quadTreeToMatrix(root->children[1], matrix, newSize, startX, startY + newSize);
        quadTreeToMatrix(root->children[2], matrix, newSize, startX + newSize, startY);
        quadTreeToMatrix(root->children[3], matrix, newSize, startX + newSize, startY + newSize);
    }
}

// Function to display the quadtree structure
void displayQuadTree(QuadTreeNode* root, int depth = 0) {
    if (root == nullptr) return;

    for (int i = 0; i < depth; ++i) {
        cout << "  ";
    }
    if (root->isLeaf) {
        cout << "Leaf: " << (root->color ? "Black" : "White") << endl;
    }
    else {
        cout << "Internal Node" << endl;
        for (int i = 0; i < 4; ++i) {
            displayQuadTree(root->children[i], depth + 1);
        }
    }
}

// Function to display the 2D matrix representation of the image
void displayMatrix(bool** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << (matrix[i][j] ? '1' : '0') << " ";
        }
        cout << endl;
    }
}

// Function to take input for the 2D binary matrix from the user
void inputMatrix(bool** matrix, int size) {
    cout << "Enter elements of the 2D binary matrix (0 or 1):" << endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cin >> matrix[i][j];
        }
    }
}

// Main function for testing
int main() {
    int size;
    cout << "Enter the size of the 2D binary matrix: ";
    cin >> size;

    // Allocate memory for the image matrix
    bool** imageMatrix = new bool* [size];
    for (int i = 0; i < size; ++i) {
        imageMatrix[i] = new bool[size];
    }

    // Input elements of the 2D binary matrix from the user
    inputMatrix(imageMatrix, size);

    // Build quadtree from the image matrix
    QuadTreeNode* root = buildQuadTree(imageMatrix, size, 0, 0);

    // Display quadtree structure
    cout << "Quadtree Structure:" << endl;
    displayQuadTree(root);
    cout << endl;

    // Display original image (2D matrix representation)
    cout << "Original Image:" << endl;
    displayMatrix(imageMatrix, size);
    cout << endl;

    // Convert quadtree back into a 2D binary matrix
    bool** reconstructedMatrix = new bool* [size];
    for (int i = 0; i < size; ++i) {
        reconstructedMatrix[i] = new bool[size];
    }
    quadTreeToMatrix(root, reconstructedMatrix, size, 0, 0);

    // Display reconstructed image (2D matrix representation)
    cout << "Reconstructed Image:" << endl;
    displayMatrix(reconstructedMatrix, size);
    cout << endl;

    // Clean up
    for (int i = 0; i < size; ++i) {
        delete[] imageMatrix[i];
        delete[] reconstructedMatrix[i];
    }
    delete[] imageMatrix;
    delete[] reconstructedMatrix;

    return 0;
}