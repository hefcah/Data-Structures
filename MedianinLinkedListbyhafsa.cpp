
#include<iostream>
#include<cmath> 
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
class LinkedList {
public:
    Node* head;
    LinkedList()
    {
        head = nullptr;
    }

    // Insertion of new node but in sorted order
    void insert(int num) {
        Node* newNode = new Node(num);
        if (head == nullptr || head->data >= num)
        {
            newNode->next = head;
            head = newNode;
        }
        else
        {
            Node* current = head;
            // Traversing the list in order to find the correct position to insert the new node
            while (current->next != nullptr && current->next->data < num) {
                current = current->next;
            }
            // Insertion of new node after the current node
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    // Finding median 
    double findMedian() {
        if (head == nullptr)
            return 0;

        int length = 0;
        Node* temp = head; // Pointer for traversal

        // Calculation of length
        while (temp) {
            length++;
            temp = temp->next;
        }

        Node* slow = head;
        Node* fast = head;

        while (fast != nullptr && fast->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        if (length % 2 == 1)
        {
            return slow->data;
        }
        else
        {
            return ceil((slow->data + slow->next->data) / 2.0);
        }
    }
};

int main() {
    LinkedList list;
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;
    cout << "Enter the elements: ";
    for (int i = 0; i < n; ++i) {
        int num;
        cin >> num;
        list.insert(num);
    }
    cout << "Median: " << list.findMedian() << endl;

    return 0;
}