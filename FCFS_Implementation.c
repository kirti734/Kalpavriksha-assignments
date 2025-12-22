#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NAME_LENGTH 100
#define HASHMAP_SIZE 103

enum ProcessState {READY, RUNNING, WAITING, TERMINATED} ;

struct PCB 
{
    int processId;
    char processName[MAX_NAME_LENGTH];
    
    int burstTime;
    int ioStartTime;
    int ioDuration;
    
    int remainingCPUTime;
    int remainingIOTime;
    int executedTime;     
 
    int completionTime;
    
    enum ProcessState state;
    int isKilled;
    
    struct PCB *next;
};

struct KillEvent 
{
    int processId;
    int killTime;
    struct KillEvent *next;
};

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

struct PCB **pcbTable;

struct Queue readyQueue;
struct Queue waitingQueue;
struct Queue terminatedQueue;

struct KillEvent *killEventsHead;

int currentTime;

void initializeQueue(struct Queue *queue)
{
	queue -> front = NULL;
	queue -> rear = NULL;
}

int isQueueEmpty(struct Queue *queue)
{
	return (queue -> front == NULL);
}

void initialize()
{
	initializeQueue(&readyQueue);
	initializeQueue(&waitingQueue);
	initializeQueue(&terminatedQueue);
	
	pcbTable = (struct PCB**) calloc (HASHMAP_SIZE, sizeof(struct PCB*));
	
	if (pcbTable == NULL)
	{
		printf("Memory allocation failed\n");
		return;
	}
	
	int index = 0;
	
	for (index = 0; index < HASHMAP_SIZE; index++)
	{
		pcbTable[index] = NULL;
	}
	
	killEventsHead = NULL;
	currentTime = 0;
}

void enqueue(struct Queue *queue, struct PCB *process)
{
	if (process == NULL)
	{
		return;
	}
	
	struct QueueNode *newNode = malloc (sizeof(struct QueueNode));
	newNode -> process = process;
	newNode -> next = NULL;
	
	if (queue -> rear == NULL)
	{
		queue -> rear = newNode;
		queue -> front = newNode;
		return;
	}
		
	queue -> rear -> next = newNode;
	queue -> rear = newNode;
}

struct PCB* dequeue(struct Queue *queue)
{
	if (isQueueEmpty(queue))
	{
		return NULL;
	}
	
	struct QueueNode *temp = queue -> front;
	struct PCB *tempProcess = temp -> process;
	
	queue -> front = queue -> front -> next;
	
	if (queue -> front == NULL)
	{
		queue -> rear = NULL;
	}
	
	free(temp);
	return tempProcess;
}

void removeFromQueueByProcessId(struct Queue *queue, int deleteProcessId)
{
	if (isQueueEmpty(queue))
	{
		return;
	}
	
	struct QueueNode *current = queue -> front;
	struct QueueNode *prev = NULL;
	
	while (current != NULL)
	{
		if (current -> process -> processId == deleteProcessId)
		{
			if (prev == NULL)
			{
				queue -> front = current -> next;
				
				if (queue -> front == NULL)
				{
					queue -> rear = NULL;
				}
			}
			else
			{
				prev -> next = current -> next;
				
				if (current == queue -> rear)
				{
					queue -> rear = prev;
				}
			}
			
			free(current);
			return;
		}
		
		prev = current;
		current = current -> next;
	}
}

struct PCB* getProcessByProcessId(int searchProcessId)
{
	int key = searchProcessId % HASHMAP_SIZE;
	
	struct PCB *current = pcbTable[key];
	
	while (current != NULL)
	{
		if (current -> processId == searchProcessId)
		{
			return current;
		}
		
		current = current -> next;
	}
	
	return NULL;
}

void createProcess(int processId, const char* processName, int burstTime, int ioStartTime, int ioDuration)
{
	struct PCB* newProcess = malloc (sizeof(struct PCB));
	
	if (newProcess == NULL)
	{
		printf("\nMemory allocation is failed\n");
		return;
	} 
	
	newProcess -> processId = processId;
	strcpy(newProcess -> processName, processName);
	newProcess -> burstTime = burstTime;
	newProcess -> ioStartTime = ioStartTime;
	newProcess -> ioDuration = ioDuration;
	
	newProcess -> remainingCPUTime = burstTime;
	newProcess -> remainingIOTime = 0;
	newProcess -> executedTime = 0;
	newProcess -> completionTime = 0;
	
	newProcess -> state = READY;
	newProcess -> isKilled = 0;
	newProcess -> next = NULL;
	
	int key = processId % HASHMAP_SIZE;
	
	if (pcbTable[key] == NULL)
	{
		pcbTable[key] = newProcess;
	}
	else
	{
		newProcess -> next = pcbTable[key];
		pcbTable[key] = newProcess;
	}
	
	enqueue(&readyQueue, newProcess);
}

void insertProcessDetails()
{
	printf("Enter the Process Id: ");
	int processId = 0;
	if (scanf("%d", &processId) != 1)
	{
		printf("\nProcess id should be numeric\n");
		while (getchar() != '\n');
		return;
	}
	while (getchar() != '\n');
	
	if (processId < 0)
	{
		printf("\nProcess Id should be positive\n");
		return;
	}
	
	if (getProcessByProcessId(processId) != NULL)
	{
		printf("\nProcess Id should be Unique\n");
		return;
	}
	
	
	char processName[MAX_NAME_LENGTH];
	printf("Enter the Process Name: ");
	
	if (fgets(processName, MAX_NAME_LENGTH, stdin) == NULL)
	{
		printf("\nFailed to read value\n");
		return;
	}
	processName[strcspn(processName, "\n")] = '\0';
	
	if (strlen(processName) == 0)
	{
		printf("\nPlease enter the process name\n");
		return;
	}
	
	
	printf("Enter the Burst Time: ");
	int burstTime = 0;
	if (scanf("%d", &burstTime) != 1)
	{
		printf("\nBurst Time should be numeric\n");
		while (getchar() != '\n');
		return;
	}
	while (getchar() != '\n');
	
	if (burstTime < 0)
	{
		printf("\nBurst Time should be positive\n");
		return;
	}
	
	
	printf("Enter the I/O Start Time: ");
	int ioStartTime = 0;
	if (scanf("%d", &ioStartTime) != 1)
	{
		printf("\nI/O Start Time should be numeric\n");
		while (getchar() != '\n');
		return;
	}
	while (getchar() != '\n');
	
	if (ioStartTime < 0)
	{
		printf("\nI/O Start Time should be positive\n");
		return;
	}
	
	
	printf("Enter the I/O Duration: ");
	int ioDuration = 0;
	if (scanf("%d", &ioDuration) != 1)
	{
		printf("\nI/O Duration should be numeric\n");
		while (getchar() != '\n');
		return;
	}
	while (getchar() != '\n');
	
	if (ioDuration < 0)
	{
		printf("\nI/O Duration should be positive\n");
		return;
	}
	
	createProcess(processId, processName, burstTime, ioStartTime, ioDuration);
}

void addKillEvent(int processId, int killTime)
{
	struct KillEvent *newKillEvent = malloc (sizeof(struct KillEvent));
	
	if (newKillEvent == NULL)
	{
		printf("Memory allocation failed\n");
		return;
	}
	
	newKillEvent -> killTime = killTime;
	newKillEvent -> processId = processId;
	newKillEvent -> next = NULL;
	
	if (killEventsHead == NULL)
	{
		killEventsHead = newKillEvent;
		return;
	}
	struct KillEvent *current = killEventsHead;
	while (current -> next != NULL)
	{
		current = current -> next;
	}
	
	current -> next = newKillEvent;
}

void insertKillProcessDetails()
{
	printf("Enter the Process Id to Kill: ");
	int processId = 0;
	if (scanf("%d", &processId) != 1)
	{
		printf("\nProcess id should be numeric\n");
		while (getchar() != '\n');
		return;
	}
	while (getchar() != '\n');
	
	if (processId < 0)
	{
		printf("\nProcess Id should be positive\n");
		return;
	}
	
	if (getProcessByProcessId(processId) == NULL)
	{
		printf("\nProcess should be present in PCB Table\n");
		return;
	}	
	
	
	printf("Enter the Kill Time: ");
	int killTime = 0;
	if (scanf("%d", &killTime) != 1)
	{
		printf("\nKill Time should be numeric\n");
		while (getchar() != '\n');
		return;
	}
	while (getchar() != '\n');
	
	if (killTime < 0)
	{
		printf("\nKill Time should be positive\n");
		return;
	}
	
	addKillEvent(processId, killTime);
}

void processKillEventForCurrentTime()
{
	struct KillEvent *current = killEventsHead;
	struct KillEvent *prev = NULL;
	
	while (current != NULL)
	{
		if (current -> killTime == currentTime)
		{
			struct PCB *process = getProcessByProcessId(current -> processId);
			
			if (process != NULL && process -> state != TERMINATED)
			{
				process -> isKilled = 1;
				process -> completionTime = currentTime;
				
				if(process -> state == READY || process -> state == WAITING)
				{
					process -> state = TERMINATED;
					enqueue(&terminatedQueue, process);
				}
			
				removeFromQueueByProcessId(&readyQueue, process -> processId);
				removeFromQueueByProcessId(&waitingQueue, process -> processId);
				
			}
			
			struct KillEvent *toFree = current;
		
			if (prev == NULL)
			{
				killEventsHead = current -> next;
				current = killEventsHead ;
			}
			else
			{
				prev -> next = current -> next;
				current = current -> next;
			}
			
			free(toFree);
			continue;
		}
		
		prev = current;	
		current = current -> next;
	}
}

void handleIOprocess()
{
	if (isQueueEmpty(&waitingQueue))
	{
		return;
	}
	
	struct QueueNode *current = waitingQueue.front;
	struct QueueNode *prev = NULL;
	
	while (current != NULL)
	{
		struct PCB *process = current -> process;
		
		if (process -> remainingIOTime > 0)
		{
		    process -> remainingIOTime--;
		}
		
		if (process -> remainingIOTime == 0)
		{
			if(!process -> isKilled && process -> state != TERMINATED)
			{
				process -> state = READY;
			    enqueue(&readyQueue, process);	
			}		
			
			struct QueueNode *toRemove = current;
			
			if (prev == NULL)
			{
				waitingQueue.front = current -> next;
				
				if (waitingQueue.front == NULL)
				{
					waitingQueue.rear = NULL;
				}
				
				current = waitingQueue.front;
			}
			else
			{
				prev -> next = current -> next;
				
				if (current == waitingQueue.rear)
				{
					waitingQueue.rear = prev;
				}
				current = current -> next;
			}
			
			free(toRemove);
		}
		else
		{
			prev = current;
			current = current -> next;
		}
	}
}

void processScheduling()
{
	struct PCB *runningProcess = NULL;
	
	while (1)
	{
		processKillEventForCurrentTime();
		
		if (runningProcess != NULL && runningProcess -> isKilled)
		{
			runningProcess -> state = TERMINATED;
			enqueue(&terminatedQueue, runningProcess);
			runningProcess = NULL;
		}
		
		if (runningProcess == NULL)
		{
			runningProcess = dequeue(&readyQueue);
			
			if (runningProcess != NULL)
			{
				if (!runningProcess -> isKilled)
	            {
	                runningProcess -> state = RUNNING;
	            }
	            else if (runningProcess -> isKilled)
	            {
	                runningProcess = NULL;
	            }
			}
		}
		
		if (runningProcess != NULL)
		{
			runningProcess -> remainingCPUTime--;
			runningProcess -> executedTime++;
			
			if (runningProcess -> ioStartTime > 0 && runningProcess -> executedTime == runningProcess -> ioStartTime && runningProcess -> ioDuration > 0)
			{
				runningProcess -> state = WAITING;
				runningProcess -> remainingIOTime = runningProcess -> ioDuration;
				
				enqueue(&waitingQueue, runningProcess);
				runningProcess = NULL;
			}
			else if (runningProcess -> remainingCPUTime == 0)
			{
				runningProcess -> state = TERMINATED;
				runningProcess -> completionTime = currentTime + 1;
				enqueue(&terminatedQueue, runningProcess);
				runningProcess = NULL;
			}
		}
		
		handleIOprocess();
		
		currentTime++;
		
		if (runningProcess == NULL && isQueueEmpty(&readyQueue) && isQueueEmpty(&waitingQueue))
		{
			break;
		}
	}
}

void displayProcessDetailsOfTerminationQueue()
{
	struct QueueNode *currentProcess = terminatedQueue.front;
	
	printf("\nProcess Id | Process Name | Burst Time | I/O Time | Turnaround Time | Waiting Time\n");
	printf("------------------------------------------------------------------------------------------------\n");
	
	while (currentProcess != NULL)
	{
		struct PCB *process = currentProcess -> process;
		
		if (process -> isKilled)
		{
			printf("\n%d | %s | %d | %d | killed at - %d\n", process -> processId, process -> processName, process -> burstTime, process -> ioDuration, process -> completionTime );
		}
		else
		{
			int turnAroundTime = process -> completionTime;
			int waitingTime = turnAroundTime - process -> burstTime - process -> ioDuration;
			
			printf("\n%d | %s | %d | %d | %d | %d \n", process -> processId, process -> processName, process -> burstTime, process -> ioDuration, turnAroundTime, waitingTime);			
		}
		currentProcess = currentProcess -> next;
	}	
}

void freeMemory()
{
	int index = 0;
	for (index = 0; index < HASHMAP_SIZE; index++)
	{
		struct PCB *current = pcbTable[index];
		
		while (current != NULL)
		{
			struct PCB *temp = current;
			current = current -> next;
			free(temp);
		}
	}
	
	free(pcbTable);
	
	struct KillEvent *killCurrent = killEventsHead;
	
	while (killCurrent != NULL)
	{
		struct KillEvent *temp = killCurrent;
		killCurrent = killCurrent -> next;;
		free(temp);
	}
	
	while (!isQueueEmpty(&readyQueue))
	{
		dequeue(&readyQueue);
	}
	
	while (!isQueueEmpty(&waitingQueue))
	{
		dequeue(&waitingQueue);
	}
	
	while (!isQueueEmpty(&terminatedQueue))
	{
		dequeue(&terminatedQueue);
	}
}

int main() 
{
	initialize();
	
	while (1)
	{
		printf("\n1. Enter the process details\n");
		printf("\n2. Enter the kill process details\n");
		printf("\n3. Start Simulation\n");
		printf("\n4. for exit\n");
		
		int choice = 0;
		printf("\nEnter choice: ");
		if (scanf("%d", &choice) != 1)
		{
			printf("\nChoice should be a numeric value\n");
			while (getchar() != '\n');
			continue;
		}	
		
		while (getchar() != '\n');
		
		switch (choice)
		{
			case 1:
				insertProcessDetails();
				break;
			
			case 2:
				insertKillProcessDetails();
				break;
				
			case 3:
				processScheduling();
                displayProcessDetailsOfTerminationQueue();
                break;
			
			case 4:
				freeMemory();
				printf("\nExit\n");
				return 0;
			
			default:
				printf("\nEnter the valid choice\n");
		}
	}
    return 0;
}
