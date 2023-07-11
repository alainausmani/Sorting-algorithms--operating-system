#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<time.h>
#include<pthread.h>

#define MAX_QUEUE_SIZE 100

struct Node {
    int vertex;
    struct Node* next;
} Node;



struct Graph {
    int numVertices;
    struct Node** adjLists;
    bool* visited;
};


struct Queue {
    int items[MAX_QUEUE_SIZE];
    int front;
    int rear;
};


struct Node* createNode(int vertex) {
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}


struct Graph* createGraph(int numVertices) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;
    graph->adjLists = (struct Node**) malloc(numVertices * sizeof(struct Node*));
    graph->visited = (bool*) malloc(numVertices * sizeof(bool));

    int i;
    for (i = 0; i < numVertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = false;
    }

    return graph;
}


void addEdge(struct Graph* graph, int src, int dest) {
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}


void enqueue(struct Queue* queue, int item) {
    if (queue->rear == MAX_QUEUE_SIZE - 1) {
        printf("Queue is full\n");
        return;
    }
    queue->rear++;
    queue->items[queue->rear] = item;
}


int dequeue(struct Queue* queue) {
    if (queue->front == queue->rear) {
        printf("Queue is empty\n");
        return -1;
    }
    queue->front++;
    int item = queue->items[queue->front];
    return item;
}
struct args{
  struct Graph* g;
  int start;
  
};
struct Queue* queue;
pthread_mutex_t mut1,mut2,mut3,mut4;

void* bfs(void*a) {
    struct args* x  = (struct args*)a;
    struct Graph* graph = x->g;
    int startVertex = x->start;
    pthread_mutex_lock(&mut1);
    graph->visited[startVertex] = true;
    enqueue(queue, startVertex);
    pthread_mutex_unlock(&mut1);
    while (queue->front != queue->rear) {
        pthread_mutex_lock(&mut2);
        int currentVertex = dequeue(queue);
        if (!graph->visited[currentVertex]) {
        printf("Visited %d\n", currentVertex);
        graph->visited[currentVertex] = true;
        }
        struct Node* temp = graph->adjLists[currentVertex];
        pthread_mutex_unlock(&mut2);
        while (temp != NULL) {
        
            int adjVertex = temp->vertex;

            if (!graph->visited[adjVertex]) {

                enqueue(queue, adjVertex);
            }
        pthread_mutex_lock(&mut3);
            temp = temp->next;
        pthread_mutex_unlock(&mut3);
        }
        
    }
}

int main() {
    pthread_mutex_init(&mut1,NULL);
    pthread_mutex_init(&mut2,NULL);
    pthread_mutex_init(&mut3,NULL);
    pthread_mutex_init(&mut4,NULL);
    queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->front = -1;
    queue->rear = -1;
    struct Graph* graph = createGraph(5);
addEdge(graph, 0, 1);
addEdge(graph, 0, 2);
addEdge(graph, 1, 4);
addEdge(graph, 2, 4);
addEdge(graph, 3, 5);
    struct args x; 
    clock_t star, end;
    double cpu_time_used;
    x.g = graph;
    x.start = 0;
    star = clock();
    pthread_t p[5];
    printf("BFS Traversal starting from vertex 0:\n");
    //bfs((void*)x);
    for(int i=0;i<5;++i){
      pthread_create(&p[i],NULL,bfs,(void*)&x);
    }
    for(int i=0;i<5;++i){
      pthread_join(p[i],NULL);
    }
    end = clock();

    cpu_time_used = ((double) (end - star)) / CLOCKS_PER_SEC * 1000;

    printf("Total time elapsed: %f ms\n", cpu_time_used);
    pthread_mutex_destroy(&mut1);
    pthread_mutex_destroy(&mut2);
    pthread_mutex_destroy(&mut3);
    pthread_mutex_destroy(&mut4);
    free(graph);
    free(queue);
    return 0;
}
