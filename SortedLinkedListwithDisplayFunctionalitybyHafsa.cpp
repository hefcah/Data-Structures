
#include<iostream>
using namespace std;
class Node {
public:
    int data;
    Node* next;

    Node(int d)
    {
        data = d;
        next = nullptr;
    }
};

class linkedlist {
public:
    Node* head;
    linkedlist() {
        head = nullptr;
    }

    // Inserts a new node with the given data in sorted order
    void insert(int num) {
        Node* newNode = new Node(num);
        if (head == nullptr || head->data >= num)
        {
            newNode->next = head; // New node becomes the new head
            head = newNode;
        }
        else
        {
            Node* current = head;
            // Traversing list to find the correct position to insert the new node
            while (current->next != nullptr && current->next->data < num) {
                current = current->next;
            }
            // Insertion of the new node after the current node
            newNode->next = current->next;
            current->next = newNode;
        }
    }
    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

int main() {
    linkedlist L1;
    int num;
    cout << "Enter elements for linked list L1 (enter -1 to stop): ";
    while (true) {
        cin >> num;
        if (num == -1)
            break;
        L1.insert(num);
    }
    cout << "Enter a number you want to insert: ";
    cin >> num;
    L1.insert(num);
    cout << "Linked list L1 after insertion: ";
    L1.display();

    return 0;
}