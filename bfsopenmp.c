#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include<time.h>
#define MAX_NODES 100

int visited[MAX_NODES];
int queue[MAX_NODES];
int front = 0;
int rear = 0;

void bfs(int graph[MAX_NODES][MAX_NODES], int start, int num_nodes) {
    int current_node;

    queue[rear++] = start;
    visited[start] = 1;

    #pragma omp parallel shared(graph, visited, queue, front, rear) private(current_node)
    {
        while (front < rear) {
            #pragma omp for schedule(dynamic)
            for (int i = front; i < rear; i++) {
                current_node = queue[i];
                printf("%d ", current_node);

                #pragma omp for schedule(static)
                for (int j = 0; j < num_nodes; j++) {
                    if (graph[current_node][j] == 1 && !visited[j]) {
                        #pragma omp critical
                        {
                            visited[j] = 1;
                            queue[rear++] = j;
                        }
                    }
                }
            }

            #pragma omp barrier
            #pragma omp single
            {
                front = rear;
            }
        }
    }
}

int main() {
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes=5, start_node;

   graph[0][1] = graph[1][0] = 1;
graph[0][2] = graph[2][0] = 1;
graph[1][3] = graph[3][1] = 1;
graph[1][4] = graph[4][1] = 1;
graph[2][4] = graph[4][2] = 1;

    printf("BFS Traversal: ");
    clock_t star, end;
    double cpu_time_used;

    star = clock();
    bfs(graph,0, num_nodes);
end = clock();

     cpu_time_used = ((double) (end - star)) / CLOCKS_PER_SEC * 1000;

    printf("Total time elapsed: %f ms\n", cpu_time_used);
    return 0;
}
