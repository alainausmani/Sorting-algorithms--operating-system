#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<time.h>

#define MAX_QUEUE_SIZE 100

// Node structure to represent vertices in the graph
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// Graph structure to store the adjacency list
typedef struct Graph {
    int numVertices;
    Node** adjLists;
    bool* visited;
} Graph;

// Queue structure to implement BFS
typedef struct Queue {
    int items[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

// Function to create a new node
Node* createNode(int vertex) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph with given number of vertices
Graph* createGraph(int numVertices) {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->adjLists = (Node**) malloc(numVertices * sizeof(Node*));
    graph->visited = (bool*) malloc(numVertices * sizeof(bool));

    int i;
    for (i = 0; i < numVertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = false;
    }

    return graph;
}

// Function to add an edge to the graph
void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

// Function to enqueue an item to the queue
void enqueue(Queue* queue, int item) {
    if (queue->rear == MAX_QUEUE_SIZE - 1) {
        printf("Queue is full\n");
        return;
    }
    queue->rear++;
    queue->items[queue->rear] = item;
}

// Function to dequeue an item from the queue
int dequeue(Queue* queue) {
    if (queue->front == queue->rear) {
        printf("Queue is empty\n");
        return -1;
    }
    queue->front++;
    int item = queue->items[queue->front];
    return item;
}

// Function to implement BFS
void bfs(Graph* graph, int startVertex) {
    Queue* queue = (Queue*) malloc(sizeof(Queue));

    queue->front = -1;
    queue->rear = -1;

    graph->visited[startVertex] = true;
    enqueue(queue, startVertex);

    while (queue->front != queue->rear) {
        int currentVertex = dequeue(queue);
        printf("Visited %d\n", currentVertex);

        Node* temp = graph->adjLists[currentVertex];

        while (temp != NULL) {
            int adjVertex = temp->vertex;

            if (!graph->visited[adjVertex]) {
                graph->visited[adjVertex] = true;
                enqueue(queue, adjVertex);
            }

            temp = temp->next;
        }
    }
}

int main() {
    struct Graph* graph = createGraph(5);

 addEdge(graph, 0, 1);
addEdge(graph, 0, 2);
addEdge(graph, 1, 3);
addEdge(graph, 1, 4);
addEdge(graph, 2, 4);


clock_t star, end;
    double cpu_time_used;

    // Record the starting time
    star = clock();
    printf("BFS Traversal starting from vertex 0:\n");
    bfs(graph, 0);
end = clock();

    // Calculate the total time elapsed in milliseconds
    cpu_time_used = ((double) (end - star)) / CLOCKS_PER_SEC * 1000;

    // Print the total time elapsed
    printf("Total time elapsed: %f ms\n", cpu_time_used);
    return 0;
}
