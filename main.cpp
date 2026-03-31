#include <iostream>
#include <cstdio>
using namespace std;

const int MAXN = 500005;

// Min heap for upward requests (smaller floors come first)
struct MinHeap {
    int arr[MAXN];
    int cnt;

    MinHeap() : cnt(0) {}

    void push(int x) {
        arr[++cnt] = x;
        int i = cnt;
        while (i > 1) {
            int j = i >> 1;
            if (arr[j] > arr[i]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                i = j;
            } else {
                break;
            }
        }
    }

    int top() {
        return cnt > 0 ? arr[1] : -1;
    }

    void pop() {
        if (cnt == 0) return;
        arr[1] = arr[cnt--];
        int i = 1;
        while (true) {
            int left = i << 1;
            int right = left + 1;
            int smallest = i;

            if (left <= cnt && arr[left] < arr[smallest]) {
                smallest = left;
            }
            if (right <= cnt && arr[right] < arr[smallest]) {
                smallest = right;
            }

            if (smallest != i) {
                int tmp = arr[i];
                arr[i] = arr[smallest];
                arr[smallest] = tmp;
                i = smallest;
            } else {
                break;
            }
        }
    }

    bool empty() {
        return cnt == 0;
    }
};

// Max heap for downward requests (larger floors come first)
struct MaxHeap {
    int arr[MAXN];
    int cnt;

    MaxHeap() : cnt(0) {}

    void push(int x) {
        arr[++cnt] = x;
        int i = cnt;
        while (i > 1) {
            int j = i >> 1;
            if (arr[j] < arr[i]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                i = j;
            } else {
                break;
            }
        }
    }

    int top() {
        return cnt > 0 ? arr[1] : -1;
    }

    void pop() {
        if (cnt == 0) return;
        arr[1] = arr[cnt--];
        int i = 1;
        while (true) {
            int left = i << 1;
            int right = left + 1;
            int largest = i;

            if (left <= cnt && arr[left] > arr[largest]) {
                largest = left;
            }
            if (right <= cnt && arr[right] > arr[largest]) {
                largest = right;
            }

            if (largest != i) {
                int tmp = arr[i];
                arr[i] = arr[largest];
                arr[largest] = tmp;
                i = largest;
            } else {
                break;
            }
        }
    }

    bool empty() {
        return cnt == 0;
    }
};

// Simple hash map to track cancelled requests
struct HashSet {
    static const int HASH_SIZE = 1000003;

    struct Node {
        int floor;
        Node* next;
    };

    Node* table[HASH_SIZE];
    Node pool[MAXN];
    int pool_ptr;

    HashSet() : pool_ptr(0) {
        for (int i = 0; i < HASH_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    int hash(int x) {
        return ((x % HASH_SIZE) + HASH_SIZE) % HASH_SIZE;
    }

    void insert(int x) {
        int h = hash(x);
        Node* node = &pool[pool_ptr++];
        node->floor = x;
        node->next = table[h];
        table[h] = node;
    }

    bool contains(int x) {
        int h = hash(x);
        Node* curr = table[h];
        while (curr != nullptr) {
            if (curr->floor == x) {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    void remove(int x) {
        int h = hash(x);
        Node* curr = table[h];
        Node* prev = nullptr;

        while (curr != nullptr) {
            if (curr->floor == x) {
                if (prev == nullptr) {
                    table[h] = curr->next;
                } else {
                    prev->next = curr->next;
                }
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }
};

int current_floor = 0;
bool going_up = true;
MinHeap up_heap;
MaxHeap down_heap;
HashSet cancelled;

void add_request(int x) {
    if (x > current_floor) {
        up_heap.push(x);
    } else if (x < current_floor) {
        down_heap.push(x);
    }
}

void cancel_request(int x) {
    cancelled.insert(x);
}

void action() {
    if (going_up) {
        // Skip cancelled requests
        while (!up_heap.empty() && cancelled.contains(up_heap.top())) {
            cancelled.remove(up_heap.top());
            up_heap.pop();
        }

        if (!up_heap.empty()) {
            current_floor = up_heap.top();
            up_heap.pop();
        } else {
            // Skip cancelled requests in down heap
            while (!down_heap.empty() && cancelled.contains(down_heap.top())) {
                cancelled.remove(down_heap.top());
                down_heap.pop();
            }

            if (!down_heap.empty()) {
                going_up = false;
                current_floor = down_heap.top();
                down_heap.pop();
            }
        }
    } else {
        // Skip cancelled requests
        while (!down_heap.empty() && cancelled.contains(down_heap.top())) {
            cancelled.remove(down_heap.top());
            down_heap.pop();
        }

        if (!down_heap.empty()) {
            current_floor = down_heap.top();
            down_heap.pop();
        } else {
            // Skip cancelled requests in up heap
            while (!up_heap.empty() && cancelled.contains(up_heap.top())) {
                cancelled.remove(up_heap.top());
                up_heap.pop();
            }

            if (!up_heap.empty()) {
                going_up = true;
                current_floor = up_heap.top();
                up_heap.pop();
            }
        }
    }
}

int main() {
    int n;
    cin >> n;

    string command;
    for (int i = 0; i < n; i++) {
        cin >> command;

        if (command == "locate") {
            cout << current_floor << endl;
        } else if (command == "add") {
            int x;
            cin >> x;
            add_request(x);
        } else if (command == "cancel") {
            int x;
            cin >> x;
            cancel_request(x);
        } else if (command == "action") {
            action();
        }
    }

    return 0;
}
