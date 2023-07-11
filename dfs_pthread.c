#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t visiting;
pthread_mutex_t l;
pthread_mutex_t g;

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
    pthread_mutex_lock(&visiting);
     // Print the visited node
    if(visited[x->start]!=1){
       printf("%d(%ld) ", x->start,pthread_self());
       visited[x->start] = 1;
    }
    pthread_mutex_unlock(&visiting);
    // Iterate through the neighbors of the current node
    
    for (i = x->start; i < x->graph->num_vertices;) {

        if (x->graph->adj_matrix[x->start][i] == 1 && visited[i] == 0) {
            struct args* y = (struct args*)malloc(sizeof(struct args));
          pthread_mutex_lock(&g);
            y->graph = x->graph;
            y->start = i;
          pthread_mutex_unlock(&g);
          pthread_t p;
            pthread_create(&p,NULL,dfs,(void*)y);
            pthread_join(p,NULL);
            
        }
        pthread_mutex_lock(&l);
        ++i;
        pthread_mutex_unlock(&l);
        
        
        
    }
  
   
}




void dfs_parallel(struct Graph* graph) {

    
    int start = 0;  // Start traversal from vertex 0
    /*
    for(int i=0;i<graph->num_vertices;++i){
      if(graph->adj_matrix[start][i]==1){
        num_vertices++;
      }
    }
    */
    struct args* x = (struct args*)malloc(sizeof(struct args));

    x->graph = graph;
    x->start = start;
    

    pthread_t p;
  
      
      pthread_create(&p,NULL,dfs,(void*)x);
      pthread_join(p,NULL);
    
  

  
    
    

}




// Function to perform Depth-First Search (DFS) traversal


int main() {
int vert;
printf("Number of Nodes: ");
scanf("%d",&vert);
  
    double cpu_time_used;

    // Record the starting time
    
    // Create a sample graph
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

    // Add edges to the graph
    int j,c,i;
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
    cpu_time_used = ((double) (end - star)) / CLOCKS_PER_SEC * 1000;
    printf("Total time elapsed: %f ms\n", cpu_time_used);

    for (int i = 0; i < graph->num_vertices; i++) {
        free(graph->adj_matrix[i]);
    }
    free(graph->adj_matrix);
    free(graph);
    pthread_mutex_destroy(&visiting);
    pthread_mutex_destroy(&l);
    pthread_mutex_destroy(&g);
    
   
    
    

    return 0;
}


