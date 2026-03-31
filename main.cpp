#include <iostream>
#include <cstdio>
using namespace std;

const int MAXN = 500005;

// Simple structure to track requests with a flag
struct Request {
    int floor;
    bool active;
};

Request requests[MAXN];
int request_count = 0;

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

    void clear() {
        cnt = 0;
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

    void clear() {
        cnt = 0;
    }
};

int current_floor = 0;
bool going_up = true;
MinHeap up_heap;
MaxHeap down_heap;

void rebuild_heaps() {
    up_heap.clear();
    down_heap.clear();

    for (int i = 0; i < request_count; i++) {
        if (requests[i].active) {
            if (requests[i].floor > current_floor) {
                up_heap.push(requests[i].floor);
            } else if (requests[i].floor < current_floor) {
                down_heap.push(requests[i].floor);
            }
        }
    }
}

void add_request(int x) {
    requests[request_count].floor = x;
    requests[request_count].active = true;
    request_count++;

    if (x > current_floor) {
        up_heap.push(x);
    } else if (x < current_floor) {
        down_heap.push(x);
    }
}

void cancel_request(int x) {
    for (int i = 0; i < request_count; i++) {
        if (requests[i].active && requests[i].floor == x) {
            requests[i].active = false;
            break;
        }
    }
    rebuild_heaps();
}

void action() {
    if (going_up) {
        if (!up_heap.empty()) {
            current_floor = up_heap.top();
            up_heap.pop();
            // Remove this request from active list
            for (int i = 0; i < request_count; i++) {
                if (requests[i].active && requests[i].floor == current_floor) {
                    requests[i].active = false;
                    break;
                }
            }
        } else if (!down_heap.empty()) {
            going_up = false;
            current_floor = down_heap.top();
            down_heap.pop();
            // Remove this request from active list
            for (int i = 0; i < request_count; i++) {
                if (requests[i].active && requests[i].floor == current_floor) {
                    requests[i].active = false;
                    break;
                }
            }
        }
    } else {
        if (!down_heap.empty()) {
            current_floor = down_heap.top();
            down_heap.pop();
            // Remove this request from active list
            for (int i = 0; i < request_count; i++) {
                if (requests[i].active && requests[i].floor == current_floor) {
                    requests[i].active = false;
                    break;
                }
            }
        } else if (!up_heap.empty()) {
            going_up = true;
            current_floor = up_heap.top();
            up_heap.pop();
            // Remove this request from active list
            for (int i = 0; i < request_count; i++) {
                if (requests[i].active && requests[i].floor == current_floor) {
                    requests[i].active = false;
                    break;
                }
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
