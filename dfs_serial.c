#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t visiting;
pthread_mutex_t l;
pthread_mutex_t g;
int num_vertices= 0;
int *visited;
// Definition of a Graph structure
struct Graph {
    int num_vertices;
    int** adj_matrix;
};

// Function to add an edge between two vertices in the graph
void add_edge(struct Graph* graph, int u, int v) {
    graph->adj_matrix[u][v] = 1;
}

struct args{
  struct Graph* graph;
  int start;
  

};
void* dfs(void* a) {
    int i=0;
    struct args* x = (struct args*)a;
    
    
    if(visited[x->start]!=1){
       printf("%d",x->start);
       visited[x->start] = 1;
    }

  
    
    for (i = x->start; i < x->graph->num_vertices;++i) {
        if (x->graph->adj_matrix[x->start][i] == 1 && visited[i] == 0) {
            struct args* y = (struct args*)malloc(sizeof(struct args));
            y->graph = x->graph;
            y->start = i;
            dfs((void*)y); 
        } 
    }
}




void dfs_serial(struct Graph* graph) {    
    int start = 0;
    struct args* x = (struct args*)malloc(sizeof(struct args));
    x->graph = graph;
    x->start = start;
    dfs((void*)x);
}







int main() {
int vert;
printf("Number of Nodes: ");
scanf("%d",&vert);
    int i,j,c;
   visited = (int*)calloc(vert,sizeof(int));
    pthread_mutex_init(&visiting,NULL);
    pthread_mutex_init(&l,NULL);
     pthread_mutex_init(&g,NULL);
    printf("DFS Pthread Traversal: ");
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
     graph->num_vertices = vert;
    graph->adj_matrix = (int**)malloc(graph->num_vertices * sizeof(int*));
    for (int i = 0; i < graph->num_vertices; i++) {
        graph->adj_matrix[i] = (int*)calloc(graph->num_vertices, sizeof(int));
    }

   
    add_edge(graph, 0, 1);
    add_edge(graph, 0, 2);
    add_edge(graph, 1, 3);
    add_edge(graph, 1, 4);
    for(i=2,j=5,c = 5;c<vert;++i,++j,++c){
      add_edge(graph,i,j);
    }
   
    clock_t star, end;
    star = clock();
    dfs_serial(graph);
    end = clock();
    double cpu_time_used = ((double) (end - star)) / CLOCKS_PER_SEC * 1000;
    printf("Total time elapsed: %f ms\n", cpu_time_used);

    for (int i = 0; i < graph->num_vertices; i++) {
        free(graph->adj_matrix[i]);
    }
    free(graph->adj_matrix);
    free(graph);
 
    
   
    
    return 0;
}


