#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define Q0 0
#define Q1 1
#define Q2 2
#define Q3 3
#define MAX_SIZE 5  

int timelol = 0;
int n;
int *cputime, *isdone, *start, *finish, *level, *timesofar;
sem_t mutex;

typedef struct 
{
    int *items;
    int front, rear;
    int size;
} Queue;

void initQueue(Queue *q) 
{
    q->front = -1;
    q->rear = -1;
    q->size = 0;
    q->items = (int *) malloc (n*sizeof(int));
}

#define isEmpty(q) ((q)->size == 0)
#define isFull(q) ((q)->size == MAX_SIZE)

#define enqueue(q, value) do { \
    if (!isFull(q)) { \
        (q)->rear = ((q)->rear + 1) % MAX_SIZE; \
        (q)->items[(q)->rear] = (value); \
        (q)->size++; \
        if ((q)->size == 1) (q)->front = (q)->rear; \
    } \
} while(0)

#define dequeue(q) ({ \
    int value = -1; \
    if (!isEmpty(q)) { \
        value = (q)->items[(q)->front]; \
        (q)->front = ((q)->front + 1) % MAX_SIZE; \
        (q)->size--; \
    } \
    value; \
})

#define dequeueback(q) ({ \
    int value = -1; \
    if (!isEmpty(q)) { \
        value = (q)->items[(q)->rear]; \
        (q)->rear = ((q)->rear + MAX_SIZE - 1) % MAX_SIZE; \
        (q)->size--; \
    } \
    value; \
})

Queue *q1, *q2, *q3;

void * fun(void * arg)
{
    int ind = *((int *)arg);
    int robin_time = 0;
    while(timesofar[ind] < cputime[ind])
    {
        sem_wait(&mutex);
        switch(level[ind])
        {
            case Q0:
            {
                int found = 0;
                for(int i=0; i<n; ++i)
                {
                    if(level[i]==Q3 && cputime[i]!=timesofar[i])
                    {
                        found = 1;
                        break;
                    }
                }
                if(found==1)
                {
                    break;
                }
                if(timesofar[ind]==cputime[ind])
                {
                    if(q3->items[q3->front]==ind)
                    {
                        dequeue(q3);
                    }
                    if(q2->items[q2->front]==ind)
                    {
                        dequeue(q2);
                    }
                    level[ind] = Q1;
                    break;
                }
                printf("%d is in Q0.\n", ind+1);
                robin_time += 1;
                timelol += 1;
                timesofar[ind] += 1;
                if(robin_time==5)
                {
                    level[ind] = Q1;
                    robin_time = 0;
                    if(timesofar[ind]!=cputime[ind])
                    {
                        enqueue(q1, ind);
                    }
                }
                if(start[ind]==0)
                {
                    start[ind] = timelol;
                }
                break;
            }
            case Q1:
            {
                int found = 0;
                for(int i=0; i<n; ++i)
                {
                    if(level[i]==Q0 && cputime[i]!=timesofar[i])
                    {
                        found = 1;
                        break;
                    }
                }
                if(found==1)
                {
                    break;
                }
                if(q1->items[q1->front]!=ind)
                {
                    break;
                }
                if(cputime[ind] == timesofar[ind])
                {
                    level[ind] = Q2;
                    if(q3->items[q3->front]==ind)
                    {
                        dequeue(q3);
                    }
                    if(q2->items[q2->front]==ind)
                    {
                        dequeue(q2);
                    }
                    if(q1->items[q1->rear]==ind)
                    {
                        dequeueback(q1);
                    }
                    break;
                }
                printf("%d is in Q1.\n", ind+1);
                if(timesofar[ind] + 5 >= cputime[ind])
                {
                    if(q3->items[q3->front]==ind)
                    {
                        dequeue(q3);
                    }
                    timelol += (cputime[ind] - timesofar[ind]);
                    timesofar[ind] = cputime[ind];
                }
                else
                {
                    timelol += 5;
                    timesofar[ind] += 5;
                }
                level[ind] = Q2;
                dequeue(q1);
                break;
            }
            case Q2:
            {
                int found = 0;
                for(int i=0; i<n; ++i)
                {
                    if((level[i]==Q1) && cputime[i]!=timesofar[i])
                    {
                        found = 1;
                        break;
                    }
                }
                if(found==1)
                {
                    break;
                }
                /*int rem_time[n], index[n];
                for(int i=0; i<n; ++i)
                {
                    rem_time[i] = cputime[i] - timesofar[i];
                    index[i] = i;
                }
                for(int i=0; i<n-1; ++i)
                {
                    int min_id = i;
                    for(int j=i+1; j<n; ++j)
                    {
                        if(rem_time[min_id]>rem_time[j])
                        {
                            min_id = j;
                        }
                    }
                    if(min_id!=i)
                    {
                        int temp = rem_time[min_id];
                        rem_time[min_id] = rem_time[i];
                        rem_time[i] = temp;
                        temp = index[min_id];
                        index[min_id] = index[i];
                        index[i] = temp;
                    }
                }
                for(int i=0; i<n; ++i)
                {
                    if(cputime[index[i]]==timesofar[index[i]])
                    {
                        level[index[i]] = Q3;
                        continue;
                    }
                    printf("%d is in Q2.\n", index[i] + 1);
                    if(rem_time[index[i]] <= 5)
                    {
                        timesofar[index[i]] += rem_time[index[i]];
                        timelol += rem_time[index[i]];
                    }
                    timesofar[index[i]] += 5;
                    timelol += 5;
                    level[index[i]] = Q3;
                }*/
                if(q2->items[q2->front]!=ind)
                {
                    if(cputime[q2->items[q2->front]] == timesofar[q2->items[q2->front]])
                    {
                        dequeue(q2);
                        break;
                    }                
                    break;
                }
                if(cputime[ind] == timesofar[ind])
                {
                    dequeue(q2);
                    break;
                }
                dequeue(q2);
                printf("%d is in Q2.\n", ind+1);
                if(timesofar[ind] + 5 >= cputime[ind])
                {
                    timelol += (cputime[ind] - timesofar[ind]);
                    timesofar[ind] = cputime[ind];
                    level[ind] = Q0;
                    if(q3->items[q3->front]==ind)
                    {
                        dequeue(q3);
                    }
                    break;
                }
                else
                {
                    timelol += 5;
                    timesofar[ind] += 5;
                    enqueue(q2, ind);
                }
                level[ind] = Q3;
                break;
            }
            case Q3:
            {
                int found = 0;
                for(int i=0; i<n; ++i)
                {
                    if((level[i]==Q2) && cputime[i]!=timesofar[i])
                    {
                        found = 1;
                        break;
                    }
                }
                if(found==1)
                {
                    break;
                }
                if(q3->items[q3->front]!=ind)
                {
                    if(cputime[q3->items[q3->front]] == timesofar[q3->items[q3->front]])
                    {
                        dequeue(q3);
                        break;
                    }                
                    break;
                }
                if(cputime[ind] == timesofar[ind])
                {
                    dequeue(q3);
                    break;
                }
                dequeue(q3);
                printf("%d is in Q3.\n", ind+1);
                if(timesofar[ind] + 5 >= cputime[ind])
                {
                    timelol += (cputime[ind] - timesofar[ind]);
                    timesofar[ind] = cputime[ind];
                    level[ind] = Q0;
                    if(q2->items[q2->front]==ind)
                    {
                        dequeue(q2);
                    }
                    break;
                }
                else
                {
                    timelol += 5;
                    timesofar[ind] += 5;
                    enqueue(q3, ind);
                }
                level[ind] = Q0;
                break;
            }
        }
        if(timesofar[ind]==cputime[ind])
        {
            finish[ind] = timelol;
        }
        sem_post(&mutex);
    }
    printf("%d started at %d and ended at %d with processor time %d.\n", ind + 1, start[ind], finish[ind], timesofar[ind]);
    free(arg);
}

int cmpIndices(const void *a, const void *b) 
{
    int index1 = *(const int *)a;
    int index2 = *(const int *)b;
    return cputime[index1] - cputime[index2];
}

int main()
{
    scanf("%d", &n);
    cputime = (int *) malloc (n * sizeof(int));
    isdone = (int *) malloc (n * sizeof(int));
    start = (int *) malloc (n * sizeof(int));
    finish = (int *) malloc (n * sizeof(int));
    level = (int *) malloc (n * sizeof(int));
    timesofar = (int *) malloc (n * sizeof(int));
    q1 = malloc(sizeof(Queue));
    q2 = malloc(sizeof(Queue));
    q3 = malloc(sizeof(Queue));
    initQueue(q1);
    initQueue(q2);
    initQueue(q3);
    for(int i=0; i<n; ++i)
    {
        scanf("%d", &cputime[i]);
        isdone[i] = 0;
        start[i] = 0;
        finish[i] = 0;
        level[i] = Q0;
        timesofar[i] = 0;
        enqueue(q2, i);
        enqueue(q3, i);
    }
    qsort(q2->items, n, sizeof(int), cmpIndices);
    qsort(q3->items, n, sizeof(int), cmpIndices);
    sem_init(&mutex, 0, 1);
    pthread_t th[n];
    for(int i=0; i<n; ++i)
    {
        int *arg = malloc(sizeof(int));
        *arg = i;
        if(pthread_create(&th[i], NULL, fun, (void*)arg)!=0)
        {
            perror("pthread_create failed");
            free(arg);
            exit(1);
        }
    }
    for(int i=0; i<n; ++i)
    {
        if(pthread_join(th[i], NULL))
        {
            perror("pthread_join failed");
        }
    }
    sem_destroy(&mutex);
    free(cputime);
    free(isdone);
    free(start);
    free(finish);
    free(timesofar);
    free(level);
    free(q1->items);
    free(q2->items);
    free(q3->items);
    free(q1);
    free(q2);
    free(q3);
    return 0;
}