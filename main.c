#include <stdio.h>
#include <stdlib.h>

// Cấu trúc để lưu trữ một node của cây nhị phân
struct Node {
    int key;
    struct Node *left, *right;
};

// Cấu trúc để lưu trữ một phần tử của hàng đợi
struct QueueNode {
    struct Node* node;  // Con trỏ đến node trong cây
    int dist;           // Khoảng cách ngang
    int level;          // Cấp độ
    struct QueueNode* next;
};

// Cấu trúc để lưu trữ cặp giá trị (giá trị node và cấp độ)
struct Pair {
    int value;
    int level;
};

// Cấu trúc để lưu trữ bản đồ (map)
struct Map {
    int dist;               // Khoảng cách ngang
    struct Pair pair;       // Cặp (giá trị node và cấp độ)
    struct Map* next;
};

// Hàm tạo một node mới cho cây
struct Node* createNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

// Hàm tạo một phần tử mới cho hàng đợi
struct QueueNode* createQueueNode(struct Node* node, int dist, int level) {
    struct QueueNode* qNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    qNode->node = node;
    qNode->dist = dist;
    qNode->level = level;
    qNode->next = NULL;
    return qNode;
}

// Hàm thêm hoặc cập nhật giá trị trong bản đồ
void updateMap(struct Map** head, int dist, int value, int level) {
    struct Map* temp = *head;

    // Tìm khoảng cách ngang trong bản đồ
    while (temp != NULL) {
        if (temp->dist == dist) {
            // Nếu cấp độ hiện tại >= cấp độ đã lưu, cập nhật giá trị
            if (level >= temp->pair.level) {
                temp->pair.value = value;
                temp->pair.level = level;
            }
            return;
        }
        temp = temp->next;
    }

    // Nếu khoảng cách ngang chưa tồn tại, thêm mới
    struct Map* newNode = (struct Map*)malloc(sizeof(struct Map));
    newNode->dist = dist;
    newNode->pair.value = value;
    newNode->pair.level = level;
    newNode->next = *head;
    *head = newNode;
}

// Hàm thêm một node vào hàng đợi
void enqueue(struct QueueNode** front, struct QueueNode** rear, struct Node* node, int dist, int level) {
    struct QueueNode* qNode = createQueueNode(node, dist, level);
    if (*rear == NULL) {
        *front = *rear = qNode;  // Nếu hàng đợi rỗng, cập nhật cả front và rear
    } else {
        (*rear)->next = qNode;   // Thêm phần tử mới vào cuối hàng đợi
        *rear = qNode;           // Cập nhật rear
    }
}

// Hàm lấy một node ra khỏi hàng đợi
struct QueueNode* dequeue(struct QueueNode** front, struct QueueNode** rear) {
    if (*front == NULL) return NULL;

    struct QueueNode* temp = *front;
    *front = (*front)->next;

    if (*front == NULL) {  // Nếu hàng đợi rỗng, cập nhật rear
        *rear = NULL;
    }

    return temp;
}

// Hàm kiểm tra hàng đợi rỗng
int isEmpty(struct QueueNode* front) {
    return front == NULL;
}

// Hàm sắp xếp và in kết quả từ bản đồ
void printMap(struct Map* head) {
    int count = 0;
    struct Map* temp = head;

    // Đếm số lượng phần tử trong bản đồ
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    // Lưu các phần tử bản đồ vào mảng để sắp xếp
    struct Map** array = (struct Map**)malloc(count * sizeof(struct Map*));
    temp = head;
    for (int i = 0; i < count; i++) {
        array[i] = temp;
        temp = temp->next;
    }

    // Sắp xếp mảng theo khoảng cách ngang
    for (int i = 1; i < count; i++) {
        struct Map* key = array[i];
        int j = i - 1;
        while (j >= 0 && array[j]->dist > key->dist) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }

    // In các giá trị từ bản đồ đã sắp xếp
    for (int i = 0; i < count; i++) {
        printf("%d ", array[i]->pair.value);
    }
    printf("\n");

    free(array);
}

// Hàm duyệt cây nhị phân và in hàng dưới cùng (sử dụng BFS)
void printBottomView(struct Node* root) {
    if (root == NULL) return;

    struct QueueNode *front = NULL, *rear = NULL;
    struct Map* map = NULL;

    // Thêm gốc cây vào hàng đợi
    enqueue(&front, &rear, root, 0, 0);

    while (!isEmpty(front)) {
        // Lấy phần tử khỏi hàng đợi
        struct QueueNode* qNode = dequeue(&front, &rear);
        struct Node* curr = qNode->node;
        int dist = qNode->dist;
        int level = qNode->level;

        // Cập nhật bản đồ
        updateMap(&map, dist, curr->key, level);

        // Thêm con trái và con phải vào hàng đợi
        if (curr->left != NULL) {
            enqueue(&front, &rear, curr->left, dist - 1, level + 1);
        }
        if (curr->right != NULL) {
            enqueue(&front, &rear, curr->right, dist + 1, level + 1);
        }

        free(qNode);  // Giải phóng bộ nhớ cho node trong hàng đợi
    }

    // In kết quả
    printMap(map);

    // Giải phóng bộ nhớ của bản đồ
    while (map != NULL) {
        struct Map* temp = map;
        map = map->next;
        free(temp);
    }
}

// Hàm main
int main() {
    struct Node* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->right = createNode(4);
    root->right->left = createNode(5);
    root->right->right = createNode(6);
    root->right->left->left = createNode(7);
    root->right->left->right = createNode(8);

    printBottomView(root);

    return 0;
}
