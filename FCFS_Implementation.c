#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 32
#define HASHMAP_SIZE 103

enum ProcessState {NEW, READY, RUNNING, WAITING, TERMINATED} ;

/* --------- Process Control Block (PCB) --------- */

struct PCB 
{
    char name[MAX_NAME_LENGTH];
    int processId;
    int totalBurstTime;
    int ioStartTime;
    int ioDuration;
    int remainingCpuTime;
    int remainingIoTime;
    int cpuExecutedTime;      
    int startTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    enum ProcessState state;
    int killed;
};

/* --------- Kill Event Structure --------- */

struct KillEvent 
{
    int processId;
    int killTime;
    struct KillEvent *next;
};

/* --------- Queue (Linked List) --------- */

struct QueueNode 
{
    struct PCB *process;
    struct QueueNode *next;
};

struct Queue 
{
    struct QueueNode *front;
    struct QueueNode *rear;
};

/* --------- HashMap --------- */

struct HashNode 
{
    int processId;
    struct PCB *process;
    struct HashNode *next;
};

struct HashNode *pcbTable[HASHMAP_SIZE];

/* --------- Global Variables --------- */

struct Queue readyQueue;
struct Queue waitingQueue;
struct Queue terminatedQueue;

struct KillEvent *killEventsHead = NULL;

int currentTime = 0;

/* --------- Queue Function Implementations --------- */

void initQueue(struct Queue *q) 
{
    q->front = q->rear = NULL;
}

int isQueueEmpty(struct Queue *q) 
{
    return (q->front == NULL);
}

void enqueue(struct Queue *q, struct PCB *p) 
{
    
}

struct PCB *dequeue(struct Queue *q) 
{
    struct PCB* p = NULL;
    return p;
}

void removeFromQueue(struct Queue *q, int processId) 
{
    
}

/* --------- HashMap Implementations --------- */

unsigned int hashPid(int processId) 
{
    return processId % HASHMAP_SIZE;
}

void hashmap_insert(struct PCB *p) 
{
    
}

struct PCB *hashmapGet(int processId)
{
    
}

/* --------- Kill Event Implementations --------- */

void addKillEvent(int processId, int killTime) 
{
    
}

void processKillEventsAtCurrentTime() 
{
    
}

/* --------- Input Parsing --------- */

void readProcessInput() 
{
    
}

void readKillEvents() 
{
    
}

/* --------- Scheduler and Simulation --------- */

struct PCB *pickNextProcessFcfs() 
{
    return dequeue(&readyQueue);
}

void handleIoForWaitingQueue() 
{
    
}

void handleProcessIoStart(struct PCB *p) 
{
	
}

void handleProcessCompletion(struct PCB *p) 
{
   
}

void simulate() 
{
    
}

/* --------- Metrics and Output --------- */

void computeMetricsForAll() 
{
    
}

void printFinalResults()
{
    
}

/* --------- main() --------- */

int main() 
{
    return 0;
}
