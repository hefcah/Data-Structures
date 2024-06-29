
#include<iostream>
using namespace std;
class Node {
public:
    int data;
    Node* next;

    Node(int d) {
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

    // Inserts a new node with the given data at the end of the linked list
    void insert(int data) {
        Node* newNode = new Node(data); // Creating a new node with given data
        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* temp = head;
            // Traverseal to the end of the list
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;  // Linking the last node to the new node
        }
    }

    // Concatenating the given linked list to the end of the current list
    void concatenate(linkedlist& L2) {
        if (L2.head == nullptr)
            return;

        Node* temp = head;
        // Traverse to the end of the current list
        while (temp->next != nullptr) {
            temp = temp->next;
        }

        Node* temp2 = L2.head;
        // Traverse the second list and add its elements to the end of the current list
        while (temp2 != nullptr) {
            temp->next = new Node(temp2->data);
            temp = temp->next;
            temp2 = temp2->next;
        }
    }
    void display(linkedlist& L) {
        Node* temp = L.head;
        // Traversal of the list and print each element
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

int main() {
    linkedlist l1, l2;
    int num;
    cout << "Enter elements for linked list l1 (enter -1 to stop): ";
    while (true) {
        cin >> num;
        if (num == -1)
            break;
        l1.insert(num);
    }

    // taking values for l2
    cout << "Enter elements for linked list l2 (enter -1 to stop): ";
    // Loop to take input for linked list l1 
    while (true) {
        cin >> num;
        if (num == -1)
            break;
        l1.insert(num);     // Insertion of the entered element into linked list l1
    }

    l1.concatenate(l2);

    // Displaying final results after concatenation
    cout << "Concatenated Linked List (l1): ";
    l1.display(l1);
    cout << "Concatenated Linked List (l2): ";
    l2.display(l2);

    return 0;
}