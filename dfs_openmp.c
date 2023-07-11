#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <omp.h> 


int num_vertices= 0;
int *visited;

struct Graph {
    int num_vertices;
    int** adj_matrix;
};

// Function to add an edge between two vertices in the graph
void add_edge(struct Graph* graph, int u, int v) {
    graph->adj_matrix[u][v] = 1;
}

void dfs(struct Graph* graph, int start) {
    int i=0;
    #pragma omp critical // Only one thread can access this block at a time
    {
        // Print the visited node
        if(visited[start]!=1){
            printf("%d(%d) ", start, omp_get_thread_num());
            visited[start] = 1;
        }
    }
    // Iterate through the neighbors of the current node
    for (i = start; i < graph->num_vertices;) {
        if (graph->adj_matrix[start][i] == 1 && visited[i] == 0) {
          #pragma omp parallel num_threads(graph->num_vertices) // Start parallel section
      {
        dfs(graph, i); // Recursive call
        }
            
        }
        #pragma omp critical // Only one thread can access this block at a time
        {
            ++i;
        }
    }
}

void dfs_parallel(struct Graph* graph) {
    int start = 0;  // Start traversal from vertex 0
    dfs(graph, start);
  
}

// Main function
int main() {
int vert;
printf("Number of Nodes: ");
scanf("%d",&vert);
int i,j,c;
    visited = (int*)calloc(vert,sizeof(int));
    printf("DFS OpenMP Traversal: ");
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->num_vertices = vert;
    graph->adj_matrix = (int**)malloc(graph->num_vertices * sizeof(int*));
    for (int i = 0; i < graph->num_vertices; i++) {
        graph->adj_matrix[i] = (int*)calloc(graph->num_vertices, sizeof(int));
    }

    // Add edges to the graph
    add_edge(graph, 0, 1);
    add_edge(graph, 0, 2);
    add_edge(graph, 1, 3);
    add_edge(graph, 1, 4);
    for(i=2,j=5,c = 5;c<vert;++i,++j,++c){
      add_edge(graph,i,j);
    }
    clock_t star, end;
    star = clock();
    dfs_parallel(graph);
    end = clock();
    double cpu_time_used = ((double) (end - star)) / CLOCKS_PER_SEC * 1000;
    printf("Total time elapsed: %f ms\n", cpu_time_used);

    
    // Free memory
    for (int i = 0; i < graph->num_vertices; i++) {
        free(graph->adj_matrix[i]);
    }
    free(graph->adj_matrix);
    free(graph);

  
    return 0;
}

