#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include <omp.h>
#define MAX 10
void *mergesort_pthread(void *);
void mergep(int,int);
void merge_openmp(int arr[], int l, int m, int r);
void mergeSortop(int arr[], int l, int r);
void printArray(int arr[], int size);
void merge_serial(int arr[],int l, int m, int h);
void sort(int l, int h,int arr[]);
int arr[MAX]={9,2,5,1,7,3,8,0,4,6};
struct ArrayIndex
{
 int lowp;
 int highp;
};
void mergep(int lowp,int highp)
{
  int m= (lowp + highp)/2;
  int *left = (int *)malloc(sizeof(int)*(m- lowp + 1));
  int *right = (int *)malloc(sizeof(int)*(highp - m));

  int n1 = m- lowp + 1, n2 = highp - m, i, j;
 
    for (i = 0; i < n1; i++)
        left[i] = arr[i + lowp];
 
    for (i = 0; i < n2; i++)
        right[i] = arr[i + m+ 1];
 
    int k = lowp;
    i = j = 0;
 
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }
 
    while (i < n1) {
        arr[k++] = left[i++];
    }
 
    while (j < n2) {
        arr[k++] = right[j++];
    }

}
void* mergesort_pthread(void* ptr)
{
   struct ArrayIndex *p = (struct ArrayIndex *)ptr;
   struct ArrayIndex a1,a2;

   int m= (p->lowp + p->highp)/2;

   pthread_t t1,t2;
   int retval;

   a1.lowp = p->lowp;
   a1.highp = m;

   a2.lowp = m+ 1;
   a2.highp = p->highp;  

  if(p->lowp >= p->highp)
  {
    return 0;
  } 
   
   retval = pthread_create(&t1,NULL,mergesort_pthread,&a1);
   if(retval) 
   {  
          printf("Thread Creation Failed...!! Return value is %d\n",retval);
	  return 0;

   }

   retval = pthread_create(&t2,NULL,mergesort_pthread,&a2);
   if(retval) 
   {  
          printf("Thread Creation Failed...!! Return value is %d\n",retval);
	  return 0;

   }	


   pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  mergep(p->lowp,p->highp);

  return 0;
}

void merge_openmp(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void mergeSortop(int arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
 
        // Sort first and second halves
        #pragma omp parallel sections num_threads(2) // Two threads are used here
	{
                #pragma omp section 
        	mergeSortop(arr, l, m);
		#pragma omp section
        	mergeSortop(arr, m+1, r);
 	}
        merge_openmp(arr, l, m, r);
    }
}
void merge_serial(int arr[],int l, int m, int r) {
   int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}


void sort(int l, int r,int arr[]) {
 if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        sort( l, m,arr);
        sort( m + 1, r,arr);
 
        merge_serial(arr, l, m, r);
    } 
}
void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
int main()
{
    int i;
    clock_t starop, endop ,starp ,endp,stars,ends;
    double cpu_time_usedop,cpu_time_usedp,cpu_time_useds;
    
     // OPEN MP 
     
    printf("OPENMP");
    starop = clock();
    mergeSortop(arr, 0, MAX - 1);
    printf("\nList after sorting\n");
    printArray(arr, MAX);
    endop = clock();
    cpu_time_usedop = ((double) (endop - starop)) / CLOCKS_PER_SEC * 1000;
    printf("Total time elapsed: %f ms\n\n", cpu_time_usedop);
     
     //PTHREADS
     
     printf("PTHREADS");
     struct ArrayIndex ai;
     ai.lowp = 0;
     ai.highp =  MAX - 1;
     pthread_t t;
     starp = clock();
     int retval;
     retval = pthread_create(&t,NULL,mergesort_pthread,&ai);
  if(retval) 
  {  
          printf("Thread Creation Failed...!! Return value is %d\n",retval);
	  return 0;
  }
  pthread_join(t,NULL);
  printf("\nList after sorting\n");
  printArray(arr, MAX);
  endp = clock();
  cpu_time_usedp = ((double) (endp - starp)) / CLOCKS_PER_SEC * 1000;
  printf("Total time elapsed: %f ms\n\n", cpu_time_usedp);
    
    // SERIALLL
    
     printf("SERIAL");
     stars = clock();
     sort(0, MAX - 1,arr);
     printf("\nList after sorting\n");
     printArray(arr, MAX);
     ends = clock();
     cpu_time_useds = ((double) (ends - stars)) / CLOCKS_PER_SEC * 1000;
     printf("Total time elapsed: %f ms\n\n", cpu_time_useds);
     
     // COMPARISION
     
     if( cpu_time_useds<cpu_time_usedp && cpu_time_useds<cpu_time_usedop)
    {
    printf("In Terms Of Time Complexity SERIAL PROGRAMMING gives the Best    Results In lOWEST POSSIBLE TIME\n");
    }
    else if (cpu_time_usedp<cpu_time_useds && cpu_time_usedp<cpu_time_usedop)
    {
    printf("In Terms Of Time Complexity PTHREAD PROGRAMMING gives the Best Results In lOWEST POSSIBLE TIME\n");
    }
     else if (cpu_time_usedop<cpu_time_useds && cpu_time_usedop<cpu_time_usedp)
    {
    printf("In Terms Of Time Complexity OPEN MP PROGRAMMING gives the Best Results In lOWEST POSSIBLE TIME\n");
    }
    
    return 0;
 
}


