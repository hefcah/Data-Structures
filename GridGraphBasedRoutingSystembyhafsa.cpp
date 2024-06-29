#include <iostream>
#include <fstream>
#include <string>
#include <climits>
using namespace std;

// Struct to represent pairs of integers
struct Pair {
    int first; // First integer
    int second; // Second integer
};

// Class representing a node in a graph
class Node {
public:
    int data; // Data stored in the node
    Node* right; // Pointer to the node on the right
    Node* left; // Pointer to the node on the left
    Node* up; // Pointer to the node above
    Node* down; // Pointer to the node below
    int distance; // Distance of the node from a source node
    bool visited; // Flag to mark whether the node has been visited or not

    // Constructor to initialize a node with given value
    Node(int val) {
        data = val;
        right = left = up = down = nullptr; // Initialize pointers to null
        distance = INT_MAX; // Initialize distance to infinity
        visited = false; // Initialize visited flag to false
    }
};

// Class representing a grid graph
class GridGraph {
public:
    int n; // Size of the grid
    Node*** nodes; // 2D array to store nodes of the grid

    // Constructor to initialize a grid graph with given size
    GridGraph(int size) {
        n = size;
        nodes = new Node * *[n]; // Allocate memory for rows
        for (int i = 0; i < n; ++i) {
            nodes[i] = new Node * [n]; // Allocate memory for columns
            for (int j = 0; j < n; ++j) {
                nodes[i][j] = new Node(i * n + j + 1); // Create a node with unique data value
                if (j > 0) {
                    nodes[i][j]->left = nodes[i][j - 1]; // Set left pointer
                    nodes[i][j - 1]->right = nodes[i][j]; // Set right pointer of the adjacent node
                }
                if (i > 0) {
                    nodes[i][j]->up = nodes[i - 1][j]; // Set up pointer
                    nodes[i - 1][j]->down = nodes[i][j]; // Set down pointer of the adjacent node
                }
            }
        }
    }

    // Destructor to deallocate memory
    ~GridGraph() {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                delete nodes[i][j]; // Delete each node
            }
            delete[] nodes[i]; // Delete array of nodes
        }
        delete[] nodes; // Delete array of arrays of nodes
    }

    // Function to calculate distance between two nodes in the grid
    int x(int startNode, int endNode) {
        int gridSize = n;
        // Reset distance and visited flags for all nodes
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                nodes[i][j]->distance = INT_MAX;
                nodes[i][j]->visited = false;
            }
        }

        // Calculate row and column indices of start and end nodes
        int startRow = (startNode - 1) / gridSize;
        int startCol = (startNode - 1) % gridSize;
        int endRow = (endNode - 1) / gridSize;
        int endCol = (endNode - 1) % gridSize;

        nodes[startRow][startCol]->distance = 0; // Set distance of start node to 0

        // Dijkstra's algorithm to find shortest path
        while (true) {
            Node* minNode = nullptr;
            int minDistance = INT_MAX;

            // Find the unvisited node with minimum distance
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (!nodes[i][j]->visited && nodes[i][j]->distance < minDistance) {
                        minDistance = nodes[i][j]->distance;
                        minNode = nodes[i][j];
                    }
                }
            }

            // If no unvisited node found, break
            if (!minNode) {
                break;
            }

            minNode->visited = true; // Mark node as visited

            // Update distances of adjacent nodes if shorter path found
            if (minNode->right && !minNode->right->visited) {
                if (minNode->right->distance > minNode->distance + 1) {
                    minNode->right->distance = minNode->distance + 1;
                }
            }

            if (minNode->left && !minNode->left->visited) {
                if (minNode->left->distance > minNode->distance + 1) {
                    minNode->left->distance = minNode->distance + 1;
                }
            }

            if (minNode->up && !minNode->up->visited) {
                if (minNode->up->distance > minNode->distance + 1) {
                    minNode->up->distance = minNode->distance + 1;
                }
            }

            if (minNode->down && !minNode->down->visited) {
                if (minNode->down->distance > minNode->distance + 1) {
                    minNode->down->distance = minNode->distance + 1;
                }
            }
        }

        // Check if end node is out of bounds
        if (endRow < 0 || endRow >= gridSize || endCol < 0 || endCol >= gridSize) {
            return -1; // Return -1 if out of bounds
        }

        return nodes[endRow][endCol]->distance; // Return distance of end node
    }

};

// Class representing an order
class Order {
public:
    string name; // Name of the order
    int nodeNumber; // Node number representing delivery location
    int deliveryTime; // Time required for delivery
};

// Class representing a restaurant
class Restaurant {
public:
    string name; // Name of the restaurant
    int nodeNumber; // Node number representing restaurant location
    int numOrders; // Number of orders
    Order* orders; // Array of orders

    // Constructor to initialize a restaurant
    Restaurant() : name(""), nodeNumber(0), numOrders(0), orders(nullptr) {}

    // Destructor to deallocate memory
    ~Restaurant() {
        delete[] orders; // Delete array of orders
    }
};

// Main function
int main() {
    ifstream inputFile("input.txt"); // Open input file
    if (!inputFile.is_open()) {
        cerr << "Error opening input file." << endl; // Display error message if file cannot be opened
        return 1;
    }

    int numTestCases;
    inputFile >> numTestCases; // Read number of test cases

    // Loop through each test case
    for (int testCase = 1; testCase <= numTestCases; ++testCase) {
        int gridSize, numRiders, numRestaurants;
        inputFile >> gridSize >> numRiders >> numRestaurants; // Read grid size, number of riders, and number of restaurants

        cout << "Test Case " << testCase << ":\n";
        cout << "Grid Size: " << gridSize << ", Num Riders: " << numRiders << ", Num Restaurants: " << numRestaurants << endl;

        // Create an array to store restaurants
        Restaurant* restaurants = new Restaurant[numRestaurants];
        // Loop through each restaurant
        for (int i = 0; i < numRestaurants; ++i) {
            Restaurant& restaurant = restaurants[i];
            inputFile >> restaurant.name >> restaurant.nodeNumber >> restaurant.numOrders; // Read restaurant details

            // If restaurant has no orders, set orders to nullptr and continue to next restaurant
            if (restaurant.numOrders <= 0) {
                restaurant.orders = nullptr;
                continue;
            }

            // Create an array to store orders for the restaurant
            restaurant.orders = new Order[restaurant.numOrders];
            // Loop through each order
            for (int j = 0; j < restaurant.numOrders; ++j) {
                inputFile >> restaurant.orders[j].name >> restaurant.orders[j].nodeNumber >> restaurant.orders[j].deliveryTime; // Read order details
            }
        }

        // Create a grid graph with given size
        GridGraph graph(gridSize);

        // Create an array to store routes for riders
        int* riderRoutes = new int[numRiders];
        for (int i = 0; i < numRiders; ++i) {
            riderRoutes[i] = -1; // Initialize routes to -1 (indicating rider is not assigned)
        }

        cout << "Optimal Routes and Time Units:\n";

        int totalTime = 0; // Total time for all deliveries
        // Loop through each restaurant
        for (int i = 0; i < numRestaurants; ++i) {
            const Restaurant& restaurant = restaurants[i];

            // Loop through each order in the restaurant
            for (int j = 0; j < restaurant.numOrders; ++j) {
                const Order& order = restaurant.orders[j];
                int assignedRider = -1; // Index of assigned rider
                int minDistance = INT_MAX; // Minimum distance to deliver the order

                // Find the rider with shortest distance to deliver the order
                for (int k = 0; k < numRiders; ++k) {
                    int riderLocation = riderRoutes[k] == -1 ? restaurant.nodeNumber : riderRoutes[k];
                    int distance = graph.x(riderLocation, order.nodeNumber);
                    if (distance < minDistance) {
                        minDistance = distance;
                        assignedRider = k;
                    }
                }

                // If a rider is assigned
                if (assignedRider != -1) {
                    riderRoutes[assignedRider] = order.nodeNumber; // Update rider's route
                    totalTime += minDistance; // Update total time
                    // Output route information
                    cout << "Rider " << assignedRider + 1 << ": "
                        << "Start at " << riderRoutes[assignedRider]
                        << " and travel " << minDistance << " units to deliver " << order.name
                        << "\n";
                }
            }
        }

        // Output total delivery time for the test case
        cout << "Total delivery time for Test Case " << testCase << ": " << totalTime << " units.\n";

        // Deallocate memory for restaurants and rider routes
        delete[] restaurants;
        delete[] riderRoutes;
    }

    inputFile.close(); // Close input file

    return 0; // Return 0 to indicate successful execution
}
