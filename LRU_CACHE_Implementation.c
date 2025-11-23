#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VALUE_LENGTH 50 
#define MIN_CACHE_CAPACITY 1
#define MAX_CACHE_CAPACITY 1000

struct queueNode
{
	int key;
	char value[VALUE_LENGTH];
	
	struct queueNode* prev;
	struct queueNode* next;
		
	struct queueNode* left;
	struct queueNode* right;
};

struct queueNode* front = NULL;
struct queueNode* rear = NULL;
struct queueNode** lruCache;
int hashMapCapacity;
int top;

void createCache(int capacity)
{
	hashMapCapacity = capacity;
	top = 0;
	front = NULL;
	rear = NULL;
	
	lruCache = (struct queueNode**) malloc (capacity * sizeof(struct queueNode*));	
	if (lruCache == NULL)
	{
		printf("Memory allocation failed\n");
		return;
	}
	
	int index = 0;
	for (index = 0; index < capacity; index++)
	{
		lruCache[index] = NULL;
	}
}

struct queueNode* searchIndexNode(struct queueNode* lruCacheIndexPointer, int key)
{	
	while (lruCacheIndexPointer != NULL)
	{
		if (lruCacheIndexPointer -> key == key)
		{
			return lruCacheIndexPointer;
		}
		
		if (lruCacheIndexPointer -> key > key)
		{
			lruCacheIndexPointer = lruCacheIndexPointer -> left;
		}
		else
		{
			lruCacheIndexPointer = lruCacheIndexPointer -> right;
		}
	}
	
	return NULL;
}

struct queueNode* insertIntoHashmapIndexTree (struct queueNode* newCache, struct queueNode* lruCacheIndexPointer)
{
	if (lruCacheIndexPointer == NULL)
	{
		return newCache;
	}	
	
	if (lruCacheIndexPointer -> key > newCache -> key)
	{
		lruCacheIndexPointer -> left = insertIntoHashmapIndexTree(newCache, lruCacheIndexPointer -> left);
	}
	else if (lruCacheIndexPointer -> key < newCache -> key)
	{
		lruCacheIndexPointer -> right = insertIntoHashmapIndexTree(newCache, lruCacheIndexPointer -> right);
	}
	
	return lruCacheIndexPointer;
}

struct queueNode* removeFromHashmap(struct queueNode* lruCacheIndexPointer, int key)
{
	if (lruCacheIndexPointer == NULL)
	{
		return NULL;
	}
	
	if (lruCacheIndexPointer -> key > key)
	{
		lruCacheIndexPointer -> left = removeFromHashmap(lruCacheIndexPointer -> left, key);
	}
	else if (lruCacheIndexPointer -> key < key)
	{
		lruCacheIndexPointer -> right = removeFromHashmap(lruCacheIndexPointer -> right, key);
	}
	else
	{
		if (lruCacheIndexPointer -> left == NULL)
		{
			struct queueNode* temp = lruCacheIndexPointer -> right;
			return temp;
		}
		
		if (lruCacheIndexPointer -> right == NULL)
		{
			struct queueNode* temp = lruCacheIndexPointer -> left;
			return temp;
		}
		
		struct queueNode* temp = lruCacheIndexPointer -> right;
		while (temp -> left != NULL)
		{
			temp = temp -> left;
		}
		
		lruCacheIndexPointer -> key = temp -> key;
		strcpy(lruCacheIndexPointer -> value , temp -> value);
		
		lruCacheIndexPointer -> right = removeFromHashmap(lruCacheIndexPointer -> right, temp -> key);
	}
	return lruCacheIndexPointer;
}

void insertIntoQueue(struct queueNode* newCache)
{
	if (top == hashMapCapacity)
	{
		struct queueNode* temp = rear;
		int index = temp -> key % hashMapCapacity;
		
		lruCache[index] = removeFromHashmap(lruCache[index], temp -> key);
		
		if (rear -> prev != NULL)
		{
			rear = rear -> prev;
			rear -> next = NULL;
		}
		else
		{
			rear = NULL;
			front = NULL;
		}
		
		free(temp);
		top--;
	}
	
	newCache -> next = front;
	newCache -> prev = NULL;
	
	if (front != NULL)
	{
		front -> prev = newCache;
	}
	
	front = newCache;
	
	if (rear == NULL)
	{
		rear = newCache;
	}
	top++;
}

void removeFromQueue(struct queueNode* temp)
{
	if (temp -> prev != NULL)
	{
		temp -> prev -> next = temp -> next;
	}
	else
	{
		front = temp -> next;
	}
	
	if (temp -> next != NULL)
	{
		temp -> next -> prev = temp -> prev;
	}
	else 
	{
		rear = temp -> prev;
	}
	
	temp -> next = NULL;
	temp -> prev = NULL;
	top--;
}

void put(int key, const char* value)
{
	int index = key % hashMapCapacity;
	
	struct queueNode* existingNode = searchIndexNode(lruCache[index], key);
	
	if (existingNode != NULL)
	{
		strcpy(existingNode -> value, value);
		removeFromQueue(existingNode);
		insertIntoQueue(existingNode);
		
		return;
	}
	
	struct queueNode* newCache = malloc (sizeof(struct queueNode));
	if (newCache == NULL)
	{
		printf("Memory allocation failed\n");
		return;
	}
	
	newCache -> key = key;
	strcpy(newCache -> value, value);
	
	newCache -> next = NULL;
	newCache -> prev = NULL;
	
	newCache -> left = NULL;
	newCache -> right = NULL;
	
	insertIntoQueue(newCache);
	
	if (lruCache[index] == NULL)
	{
		lruCache[index] = newCache;
	}
	else
	{
		lruCache[index] = insertIntoHashmapIndexTree(newCache, lruCache[index]);
	}
}

void insertPut()
{
	int key = 0;
	printf("\nEnter the key: ");
	if (scanf("%d", &key) != 1)
	{
		printf("\nKey should be numeric\n");
		while(getchar() != '\n');
		return;
	}
	while(getchar() != '\n');
	
	char value[VALUE_LENGTH];
	printf("Enter the value: ");
	if(fgets(value, VALUE_LENGTH, stdin) == NULL)
	{
		printf("\nFailed to read value\n");
		return;
	}
	value[strcspn(value, "\n")] = '\0';
	
	if(strlen(value) == 0)
	{
		printf("\nPlease enter the value\n");
		return;
	}
	
	put(key, value);
}

char* get(int key)
{
	int hashValue = key % hashMapCapacity;
	
	struct queueNode* existingNode = searchIndexNode(lruCache[hashValue], key);
	
	if (existingNode != NULL)
	{
		removeFromQueue(existingNode);
		insertIntoQueue(existingNode);
		
		return existingNode -> value;
	}
	
	return "NULL"; 
}

void insertGet()
{
	int key = 0;
	printf("\nEnter the key: ");
	if (scanf("%d", &key) != 1)
	{
		printf("\nKey should be numeric\n");
		while(getchar() != '\n');
		return;
	}
	while(getchar() != '\n');
	
	printf("\n%s\n", get(key));
}

void freeMemory()
{
	while (front != NULL)
	{
		struct queueNode* temp = front;
		front = front -> next;
		free(temp);
	}
	
	free(lruCache);
}

int main()
{
	int capacity = 0;
	printf("createCache ");
	if (scanf("%d", &capacity) != 1)
	{
		printf("Capacity should be numeric value\n");
		while(getchar() != '\n');
		return 0;
	}
	while(getchar() != '\n');
	
	if (capacity < MIN_CACHE_CAPACITY || capacity > MAX_CACHE_CAPACITY)
	{
		printf("Capacity should be in between 1 to 1000\n");
		return 0;
	}
	
	createCache(capacity);
	
	while (1)
	{
		printf("\n1. for put\n");
		printf("\n2. for get\n");
		printf("\n3. for exit\n");
		
		int choice = 0;
		printf("\nEnter choice: ");
		
		if (scanf("%d", &choice) != 1)
		{
			printf("\nChoice should be numeric value\n");
			while(getchar() != '\n');
			continue;
		}
		while(getchar() != '\n');
		
		if (choice < 1 || choice > 3)
		{
			printf("\nChoice should be in range of 1 to 3\n");
			continue;
		}
	
		switch (choice)
		{
			case 1:
				insertPut();
				break;
			
			case 2:
				insertGet();
				break;
			
			case 3:
				freeMemory();
				printf("\nexit");				
				return 0;
			
			default:
				printf("\nEnter the valid choice\n");
		}
	}
	
	return 0;
}
