#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <climits> 

using namespace std;

class Node {
public:
    int data;
    Node* right;
    Node* left;
    Node* up;
    Node* down;
    int distance; // Distance from the source node
    bool visited; // Indicates if the node has been visited in Dijkstra's algorithm

    Node(int val) {
        data = val;
        right = nullptr;
        left = nullptr;
        up = nullptr;
        down = nullptr;
        distance = -1; // Initially set distance to -1
        visited = false;
    }
};

class Order {
public:
    string name;
    int nodeNumber; // Location (node number) for delivery
    int deliveryTime;

    Order() : name(""), nodeNumber(0), deliveryTime(0) {}
};

class Restaurant {
public:
    string name;
    int nodeNumber;
    int numOrders;
    Order* orders;

    Restaurant() : name(""), nodeNumber(0), numOrders(0), orders(nullptr) {}

    ~Restaurant() {
        delete[] orders;
    }
};

class GridGraph {
public:
    int n;
    Node*** nodes;

    GridGraph(int size) {
        n = size;
        nodes = new Node * *[n];
        for (int i = 0; i < n; ++i) {
            nodes[i] = new Node * [n];
            for (int j = 0; j < n; ++j) {
                nodes[i][j] = new Node(i * n + j);
                if (j > 0) {
                    nodes[i][j]->left = nodes[i][j - 1];
                    nodes[i][j - 1]->right = nodes[i][j];
                }
                if (i > 0) {
                    nodes[i][j]->up = nodes[i - 1][j];
                    nodes[i - 1][j]->down = nodes[i][j];
                }
            }
        }
    }

    ~GridGraph() {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                delete nodes[i][j];
            }
            delete[] nodes[i];
        }
        delete[] nodes;
    }

    // Custom queue implementation for Dijkstra's algorithm
    struct QueueItem {
        int distance;
        int row;
        int col;

        QueueItem(int dist, int r, int c) : distance(dist), row(r), col(c) {}
    };

    void dijkstra(int startRow, int startCol, int** distances) {
        // Reset distances and visited flags
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                nodes[i][j]->distance = INT_MAX; // Reset distance to infinity
                nodes[i][j]->visited = false;
            }
        }

        // Initialize distance of start node to 0
        nodes[startRow][startCol]->distance = 0;

        // Dijkstra's algorithm
        queue<QueueItem> q;
        q.push(QueueItem(0, startRow, startCol));

        while (!q.empty()) {
            QueueItem current = q.front();
            q.pop();

            int currRow = current.row;
            int currCol = current.col;

            if (nodes[currRow][currCol]->visited)
                continue;

            nodes[currRow][currCol]->visited = true;

            // Update distances of adjacent nodes
            if (currCol < n - 1 && !nodes[currRow][currCol + 1]->visited && current.distance + 1 < nodes[currRow][currCol + 1]->distance) {
                nodes[currRow][currCol + 1]->distance = current.distance + 1;
                q.push(QueueItem(current.distance + 1, currRow, currCol + 1));
            }
            if (currCol > 0 && !nodes[currRow][currCol - 1]->visited && current.distance + 1 < nodes[currRow][currCol - 1]->distance) {
                nodes[currRow][currCol - 1]->distance = current.distance + 1;
                q.push(QueueItem(current.distance + 1, currRow, currCol - 1));
            }
            if (currRow > 0 && !nodes[currRow - 1][currCol]->visited && current.distance + 1 < nodes[currRow - 1][currCol]->distance) {
                nodes[currRow - 1][currCol]->distance = current.distance + 1;
                q.push(QueueItem(current.distance + 1, currRow - 1, currCol));
            }
            if (currRow < n - 1 && !nodes[currRow + 1][currCol]->visited && current.distance + 1 < nodes[currRow + 1][currCol]->distance) {
                nodes[currRow + 1][currCol]->distance = current.distance + 1;
                q.push(QueueItem(current.distance + 1, currRow + 1, currCol));
            }
        }

        // Store distances in the given array
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                distances[i][j] = nodes[i][j]->distance;
            }
        }
    }
};

class Coordinates {
public:
    int nodeNumber; // Location (node number)
    int row;
    int col;

    Coordinates(int num, int r, int c) : nodeNumber(num), row(r), col(c) {}

    Coordinates() : nodeNumber(0), row(0), col(0) {} // Default constructor
};

Coordinates getNodeCoordinates(int nodeNumber, int gridSize) {
    int row = (nodeNumber - 1) / gridSize;
    int col = (nodeNumber - 1) % gridSize;
    return Coordinates(nodeNumber, row, col);
}

int main() {
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    int numTestCases;
    inputFile >> numTestCases;

    for (int testCase = 1; testCase <= numTestCases; ++testCase) {
        int gridSize, numRiders, numRestaurants;
        inputFile >> gridSize >> numRiders >> numRestaurants;

        cout << "Test Case " << testCase << ":\n";
        cout << "Grid Size: " << gridSize << ", Num Riders: " << numRiders << ", Num Restaurants: " << numRestaurants << endl;

        // Process restaurants
        Restaurant* restaurants = new Restaurant[numRestaurants];
        Coordinates* startNodes = new Coordinates[numRestaurants]; // Stores starting nodes (restaurant locations)
        Coordinates* lastDeliveryPoints = new Coordinates[numRiders]; // Stores the last delivery points for each rider
        for (int i = 0; i < numRestaurants; ++i) {
            Restaurant& restaurant = restaurants[i];
            inputFile >> restaurant.name >> restaurant.nodeNumber >> restaurant.numOrders;
            Coordinates restaurantCoordinates = getNodeCoordinates(restaurant.nodeNumber, gridSize);
            cout << "Restaurant: " << restaurant.name << ", Node Number: " << restaurant.nodeNumber << ", Grid Coordinates: (" << restaurantCoordinates.row << ", " << restaurantCoordinates.col << "), Num Orders: " << restaurant.numOrders << "\n";
            startNodes[i] = restaurantCoordinates;
            restaurant.orders = new Order[restaurant.numOrders];
            for (int j = 0; j < restaurant.numOrders; ++j) {
                Order& order = restaurant.orders[j];
                inputFile >> order.name >> order.nodeNumber >> order.deliveryTime;
                Coordinates orderCoordinates = getNodeCoordinates(order.nodeNumber, gridSize);
                cout << "    Order: " << order.name << ", Location (Grid Coordinates): (" << orderCoordinates.row << ", " << orderCoordinates.col << "), Node Number: " << order.nodeNumber << ", Delivery Time: " << order.deliveryTime << "\n";
            }
        }

        // Calculate delivery routes for each rider and find the maximum delivery time
        GridGraph graph(gridSize);
        int*** distances = new int** [numRestaurants]; // Stores distances for each restaurant
        for (int i = 0; i < numRestaurants; ++i) {
            distances[i] = new int* [gridSize];
            for (int j = 0; j < gridSize; ++j) {
                distances[i][j] = new int[gridSize];
            }
            graph.dijkstra(startNodes[i].row, startNodes[i].col, distances[i]);
        }

        int totalDeliveryTime = 0; // Total delivery time for this test case
        for (int i = 0; i < numRestaurants; ++i) {
            const Restaurant& restaurant = restaurants[i];

            // Output processed data for this test case
            for (int j = 0; j < restaurant.numOrders; ++j) {
                const Order& order = restaurant.orders[j];
                Coordinates orderCoordinates = getNodeCoordinates(order.nodeNumber, gridSize);
                Coordinates startCoordinates;
                if (j == 0) // First delivery, start from the restaurant
                    startCoordinates = startNodes[i];
                else // Subsequent delivery, start from the last delivery point
                    startCoordinates = lastDeliveryPoints[i];
                int deliveryTime = distances[i][orderCoordinates.row][orderCoordinates.col];
                cout << "Rider " << i + 1 << ": ";
                cout << startCoordinates.nodeNumber << " -> " << order.nodeNumber << " = " << deliveryTime << " time units\n";
                totalDeliveryTime += deliveryTime;
                // Update the last delivery point for this rider
                lastDeliveryPoints[i] = Coordinates(order.nodeNumber, orderCoordinates.row, orderCoordinates.col);
            }
        }

        cout << "Total delivery time for Test Case " << testCase << ": " << totalDeliveryTime << " units.\n";

        // Deallocate memory
        for (int i = 0; i < numRestaurants; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                delete[] distances[i][j];
            }
            delete[] distances[i];
        }
        delete[] distances;
        delete[] restaurants;
        delete[] startNodes;
        delete[] lastDeliveryPoints;
    }

    inputFile.close();

    return 0;
}