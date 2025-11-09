#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define TOTAL_BLOCKS 1024
#define FILE_NAME 50
#define DIRECTORY_NAME 50
#define COMMAND_NAME_SIZE 100
#define MAX_DEPTH 50

struct FreeBlock {
    int blockIndex;
    struct FreeBlock* previous;
    struct FreeBlock* next;
};

struct FileNode {
    char fileName[FILE_NAME];  
    int isDirectory; 
    struct FileNode* parent;
    struct FileNode* next; // for next sibling for circular linked list
    struct FileNode* child; // for first child if directory

    int blockPointer[TOTAL_BLOCKS]; 
    int blockCount;
    int fileSize;
};

char virtualDisk[TOTAL_BLOCKS][BLOCK_SIZE];
struct FreeBlock* freeListHead = NULL;
struct FileNode* root = NULL;
struct FileNode* cwd = NULL;

void initializeFreeList() 
{
    freeListHead = NULL;
    int index;
    
    for (index = 0; index < TOTAL_BLOCKS; index++) 
	{
        struct FreeBlock* newBlock = (struct FreeBlock*) malloc (sizeof(struct FreeBlock));
        if(newBlock == NULL)
        {
        	printf("Memory Allocation failed\n");
        	return;
		}
		
        newBlock -> blockIndex = index;
        newBlock -> previous = NULL;
        newBlock -> next = freeListHead;
        
        if (freeListHead != NULL)
        {
        	freeListHead -> previous = newBlock;
		}
        freeListHead = newBlock;
    }
    return;
}

void initializeRootDirectory() 
{
    root = (struct FileNode*) malloc (sizeof(struct FileNode));
    if(root == NULL)
    {
    	printf("Memory allocation failed\n");
    	return;
	}
	
    strcpy(root -> fileName, "/");
    root -> isDirectory = 1;
    root -> parent = NULL;
    root -> next = root;     
    root -> child = NULL; 
    root -> blockCount = 0;
    root -> fileSize = 0;
    cwd = root;
    
    return;
}

void mkdirCommand (char* directoryName)
{
	if (directoryName == NULL || strcmp(directoryName , "") == 0)
	{
		printf("Directory Name is required to enter\n");
		return;
	}
	
	struct FileNode* currentChild = cwd -> child;
	
	if (currentChild != NULL)
	{
		struct FileNode* start = currentChild;
		do
		{
			if(strcmp(currentChild -> fileName , directoryName) == 0)
			{
				printf("Name already exists in current directory\n");
				return;
			}
			
			currentChild = currentChild -> next;
		} while(currentChild != start);
	}
	
	struct FileNode* newDirectory = (struct FileNode*) malloc (sizeof(struct FileNode));
	if (newDirectory == NULL)
	{
		printf("Memory allocation failed\n");
		return;
	}
	
	strcpy(newDirectory -> fileName , directoryName);
	newDirectory -> isDirectory = 1;
	newDirectory -> parent = cwd;
	newDirectory -> child = NULL;
	newDirectory -> blockCount = 0;
	newDirectory -> fileSize = 0;
	
	if(cwd -> child == NULL)
	{
		cwd -> child = newDirectory;
		newDirectory -> next = newDirectory;
	}
	else
	{
		struct FileNode* head = cwd -> child;
		struct FileNode* tail = head;
		
		while (tail -> next != head)
		{
			tail = tail -> next;
		} 
		
		tail -> next = newDirectory;
		newDirectory -> next = head;
	}
	
	printf("Diretory '%s' created successfully\n", directoryName);
}

void createCommand (char* newFileName)
{
	if (newFileName == NULL || strcmp(newFileName , "") == 0)
	{
		printf("File Name is required to enter\n");
		return;
	}

	
	struct FileNode* currentChild = cwd -> child;
	if (currentChild != NULL)
	{
		struct FileNode* start = currentChild;
		do
		{
			if(strcmp(currentChild -> fileName , newFileName) == 0)
			{
				printf("Name already exists in current directory\n");
				return;
			}
			currentChild = currentChild -> next;
		} while(currentChild != start);
	}
	
	struct FileNode* newFile = (struct FileNode*) malloc (sizeof(struct FileNode));
	if (newFile == NULL)
	{
		printf("Memory allocation failed\n");
		return;
	}
	
	strcpy(newFile -> fileName , newFileName);
	newFile -> isDirectory = 0;
	newFile -> parent = cwd;
	newFile -> child = NULL;
	newFile -> blockCount = 0;
	newFile -> fileSize = 0;
	
	if(cwd -> child == NULL)
	{
		cwd -> child = newFile;
		newFile -> next = newFile;
	}
	else
	{
		struct FileNode* head = cwd -> child;
		struct FileNode* tail = head;
		
		while (tail -> next != head)
		{
			tail = tail -> next;
		} 
		
		tail -> next = newFile;
		newFile -> next = head;
	}

	printf("File '%s' created successfully\n", newFileName);
}

void writeCommand (char* targetFileName, char* content)
{
	if(targetFileName == NULL && content == NULL)
	{
		printf("File name and content required to enter\n");
		return;
	}
	
	if(content == NULL || strlen(targetFileName) == 0)
	{
		printf("File content required to enter\n");
		return;
	}
	
	struct FileNode* currentChild = cwd -> child;
	struct FileNode* targetFile = NULL;
	
	if (currentChild != NULL)
	{
		struct FileNode* start = currentChild;
		do
		{
			if(strcmp(currentChild -> fileName , targetFileName) == 0 && currentChild -> isDirectory == 0)
			{
				targetFile = currentChild;
				break;
			}
			currentChild = currentChild -> next;
		} while(currentChild != start);
	}
	
	if(targetFile == NULL)
	{
		printf("%s is not present in the current directory\n", targetFileName);
		return;
	}
	
	int contentLength = strlen(content);
	int usedBytes = targetFile -> fileSize % BLOCK_SIZE;
	int freeSpaceLastBlock = (usedBytes == 0 && targetFile -> fileSize > 0) ? 0 : BLOCK_SIZE - usedBytes;
	
	int freeBlockCount = 0;
	struct FreeBlock* temp = freeListHead;
	while (temp != NULL)
	{
		freeBlockCount++;
		temp = temp -> next;
	}
	
	int remainingBytes = contentLength - (freeSpaceLastBlock > 0 & targetFile -> blockCount > 0 ? freeSpaceLastBlock : 0);
	int requiredBlocks = (remainingBytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
	
	if(requiredBlocks > freeBlockCount)
	{
		printf("No enough space is present in disk\n");
		return;
	}
	
	int bytesWritten = 0;
	
	if(freeSpaceLastBlock > 0 && targetFile -> blockCount > 0)
	{
		int toWrite = (contentLength < freeSpaceLastBlock) ? contentLength : freeSpaceLastBlock;
        int lastBlockIndex = targetFile->blockPointer[targetFile->blockCount - 1];
        
        memcpy(virtualDisk[lastBlockIndex] + usedBytes, content, toWrite);
        bytesWritten += toWrite;	
	}
	
	while(bytesWritten < contentLength)
	{
		if(freeListHead == NULL)
		{
			printf("No enought space present in disk\n");
			break;
		}
		
		int initialBlockIndex = freeListHead -> blockIndex;
		
		struct FreeBlock* oldHead = freeListHead;
		freeListHead = freeListHead -> next;
		
		if(freeListHead == NULL)
		{
			freeListHead -> previous = NULL;
		}
		free(oldHead);
		
		int remaining = contentLength - bytesWritten;
		int toWrite = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
		
		memcpy(virtualDisk[initialBlockIndex], content + bytesWritten, toWrite);
		bytesWritten += toWrite;
		
		targetFile -> blockPointer[targetFile -> blockCount++] = initialBlockIndex;
	}
	
	targetFile -> fileSize += bytesWritten;
	
	printf("Data written successfully(size = %dbytes)\n",bytesWritten);	
}

void readCommand (char* targetFileName)
{
	if(targetFileName == NULL)
	{
		printf("Target File name required to enter\n");
		return;
	}
	
	struct FileNode* currentChild = cwd -> child;
	struct FileNode* targetFile = NULL;
	
	if (currentChild != NULL)
	{
		struct FileNode* start = currentChild;
		do
		{
			if(strcmp(currentChild -> fileName , targetFileName) == 0 && currentChild -> isDirectory == 0)
			{
				targetFile = currentChild;
				break;
			}
			currentChild = currentChild -> next;
		} while(currentChild != start);
	}
	
	if(targetFile == NULL)
	{
		printf("%s is not present in the current directory\n", targetFileName);
		return;
	}
	
	if(targetFile -> fileSize == 0)
	{
		printf("File is empty\n");
		return;
	}
	
	int dataToWrite = targetFile -> fileSize;
	int index =  0;
	
	for(index = 0; index < targetFile -> blockCount && dataToWrite > 0; index++)
	{
		int blockIndex = targetFile -> blockPointer[index];
		int bytesToRead = (dataToWrite > BLOCK_SIZE) ? BLOCK_SIZE : dataToWrite;
		
		fwrite(virtualDisk[blockIndex], 1, bytesToRead, stdout);
		
		dataToWrite -=  bytesToRead;
	}
	printf("\n");
	fflush(stdout);
}

void deleteCommand (char* targetFileName)
{
	if(targetFileName == NULL)
	{
		printf("Target File name required to enter\n");
		return;
	}
	
	struct FileNode* currentChild = cwd -> child;
	struct FileNode* targetFile = NULL;
	
	if (currentChild != NULL)
	{
		struct FileNode* start = currentChild;
		do
		{
			if(strcmp(currentChild -> fileName , targetFileName) == 0 && currentChild -> isDirectory == 0)
			{
				targetFile = currentChild;
				break;
			}
			currentChild = currentChild -> next;
		} while(currentChild != start);
	}
	
	if(targetFile == NULL)
	{
		printf("%s is not present in the current directory\n", targetFileName);
		return;
	}
	
	int index = 0;
	
	for(index = 0; index < targetFile -> blockCount; index++)
	{
		int blockIndex = targetFile -> blockPointer[index];
		
		struct FreeBlock* newBlock = (struct FreeBlock*) malloc (sizeof(struct FreeBlock));
        if(newBlock == NULL)
        {
        	printf("Memory Allocation failed\n");
        	return;
		}
		
        newBlock -> blockIndex = blockIndex;
        newBlock -> previous = NULL;
        newBlock -> next = NULL;
        
        if (freeListHead == NULL)
        {
        	freeListHead = newBlock;
		}
        else
        {
        	struct FreeBlock* tail = freeListHead;
		
			while (tail -> next != NULL)
			{
				tail = tail -> next;
			} 
			
			tail -> next = newBlock;
			newBlock -> previous  = tail;
		}
	}
	
	struct FileNode *head = cwd -> child;

    if (head == targetFile && head -> next == head)
    {
        cwd -> child = NULL;
    }
    else
    {
        struct FileNode *prev = head;
        struct FileNode *temp = head;

        do {
            if (temp == targetFile)
            {
                if (temp == head)
                {
                	cwd -> child = head -> next;
				}

                prev -> next = temp -> next;
                break;
            }
            prev = temp;
            temp = temp -> next;
        } while (temp != head);
    }
	
	free(targetFile);
	
	printf("File deleted successfully\n");
}

void rmdirCommand(char* directoryName)
{
	if (directoryName == NULL || strcmp(directoryName , "") == 0)
	{
		printf("Directory Name is required to enter\n");
		return;
	}
	
	struct FileNode* currentChild = cwd -> child;
	struct FileNode* targetDirectory = NULL;
	
	if (currentChild != NULL)
	{
		struct FileNode* start = currentChild;
		do
		{
			if(strcmp(currentChild -> fileName , directoryName) == 0 && currentChild -> isDirectory == 1)
			{
				targetDirectory = currentChild;
				break;
			}
			
			currentChild = currentChild -> next;
		} while(currentChild != start);
	}
	
	if (targetDirectory == NULL)
    {
        printf("Directory not found\n");
		return;
    }

    if (targetDirectory -> child != NULL)
    {
        printf("Directory not empty. Remove files first\n");
		return;
    }
	
	struct FileNode* head = cwd -> child;

    if (head == targetDirectory && head -> next == head)
    {
        cwd -> child = NULL;
    }
    else
    {
        struct FileNode *prev = head;
        struct FileNode *temp = head;

        do {
            if (temp == targetDirectory)
            {
                if (temp == head)
                {
                	cwd -> child = head -> next;
				}

                prev -> next = temp -> next;
                break;
            }
            prev = temp;
            temp = temp -> next;
        } while (temp != head);
    }
	
	free(targetDirectory);
	printf("Directory removed successfully\n");
}

void lsCommand()
{
	if(cwd == NULL)
	{
		printf("No current directory\n");
		return;
	}
	
	printf("%s/\n",cwd -> fileName);
	
	struct FileNode* currentChild = cwd -> child;
	
	if(currentChild == NULL)
	{
		printf("(empty)\n");
		return;
	}
	
	struct FileNode* start = currentChild;
	do
	{
		if(currentChild -> isDirectory)
		{
			printf("%s/\n",currentChild -> fileName);
		}
		else
		{
			printf("%s\n",currentChild -> fileName);
		}
		
		currentChild = currentChild -> next;
	} while(currentChild != start);
}

void cdCommand(char* directoryName)
{
	if (directoryName == NULL || strcmp(directoryName , "") == 0)
	{
		printf("Directory Name is required to enter\n");
		return;
	}
	
	if(strcmp(directoryName,"..") == 0)
	{
		if(cwd -> parent != NULL)
		{
			cwd = cwd -> parent;
			printf("Moved to %s\n",cwd -> fileName);
		}
		else
		{
			printf("Already at root directory\n");
		}
		return;
	}
	
	struct FileNode* currentChild = cwd -> child;
	struct FileNode* targetDirectory = NULL;
	
	if (currentChild != NULL)
	{
		struct FileNode* start = currentChild;
		do
		{
			if(strcmp(currentChild -> fileName , directoryName) == 0 && currentChild -> isDirectory == 1)
			{
				targetDirectory = currentChild;
				break;
			}
			currentChild = currentChild -> next;
		} while(currentChild != start);
	}
	
	if(targetDirectory == NULL)
	{
		printf("%s is not present in the current directory\n", directoryName);
		return;
	}
	
	cwd = targetDirectory;
	
	printf("Moved to %s/\n", cwd -> fileName);
}

void pwdCommand()
{
	if(cwd == NULL)
	{
		printf("No current working directory\n");
		return;
	}
	
	char path[MAX_DEPTH][FILE_NAME];
	struct FileNode* temp = cwd;
	int count = 0;
	
	while (temp != NULL)
    {
        strcpy(path[count], temp -> fileName);
        temp = temp -> parent;
        count++;
    }
    
    printf("/");
    
    int index = 0;
	
	for (index = count - 1; index > 0; index--)  
    {
    	 printf("%s/", path[index - 1]);
	}
    printf("\n");
}

void dfCommand()
{
	int freeBlockCount = 0;
	struct FreeBlock *temp = freeListHead;
	
	while(temp != NULL)
	{
		freeBlockCount ++;
		temp = temp -> next;
	}
	
	int usedBlocks = TOTAL_BLOCKS - freeBlockCount;
	float diskUsage = ((usedBlocks) / (TOTAL_BLOCKS)) * 100.00;
	
	printf("Total Blocks:%d\n", TOTAL_BLOCKS);
	printf("Used Blocks: %d\n", usedBlocks);
	printf("Free Blocks: %d\n", freeBlockCount);
	printf("Disk Usage %0.2%%f\n", diskUsage);

}

void freeFileSystem(struct FileNode *rootDirectory)
{
    if (rootDirectory == NULL) 
    {
    	return;
	}

    struct FileNode *child = rootDirectory -> child;
    if (child != NULL)
    {
        struct FileNode *start = child;
        
        do 
		{
            struct FileNode *next = child -> next;
            freeFileSystem(child);
            child = (next == start) ? NULL : next;
            
        } while (child != NULL);
    }

    free(rootDirectory);
}

void exitCommand()
{
    struct FreeBlock *freeBlock = freeListHead;
    while (freeBlock != NULL)
    {
        struct FreeBlock *next = freeBlock->next;
        free(freeBlock);
        freeBlock = next;
    }
    freeListHead == NULL;

    freeFileSystem(root);

    printf("Memory released. Exiting program...\n");
    exit(0);
}

int main()
{
	printf("INITIALIZING VIRTUAL FILE SYSTEM......\n");
	printf("-------------------------------------------\n");
	
	initializeFreeList();
	initializeRootDirectory();
	
	printf("$ ./vfs\nCompact VFS - ready. Type 'exit' to quit.\n\n");
	
	char *command = malloc(COMMAND_NAME_SIZE * sizeof(char));
	if(command == NULL)
	{
		printf("Memory Allocation failed\n");
		return 1;
	}
	
	while(1)
	{
		printf("%s>", cwd -> fileName);
		
		if(fgets(command,COMMAND_NAME_SIZE,stdin) == NULL)
		{
			break;
		}
		
		command[strcspn(command, "\n")] = 0;
		
		if(strncmp(command,"mkdir ",6) == 0)
		{
			mkdirCommand(command + 6);
		}
		else if(strncmp(command,"create ",7) == 0)
		{
			createCommand(command + 7);
		}
		else if(strncmp(command,"write ",6) == 0)
		{
			char* fileName = strtok(command + 6, " ");
			writeCommand(fileName, command + 6 + strlen(fileName) + 1);
		}
		else if(strncmp(command , "read ",5) == 0)
		{
			readCommand(command + 5);
		}
		else if(strncmp(command, "delete ", 7) == 0)
		{
			deleteCommand(command + 7);
		}
		else if(strncmp(command,"rmdir ",6) == 0)
		{
			rmdirCommand(command + 6);
		}
		else if(strncmp(command, "ls",2) == 0)
		{
			lsCommand();
		}
		else if(strncmp(command,"cd ",3) == 0)
		{
			cdCommand(command + 3);
		}
		else if(strncmp(command,"pwd",3) == 0)
		{
			pwdCommand();
		}
		else if(strncmp(command,"df",2) == 0)
		{
			dfCommand();
		}
		else if(strncmp(command,"exit",4) == 0)
		{
			exitCommand();
			break;
		}
		else
		{
			printf("Enter the valid command\n");
		}	
	}
	free(command);
	return 0;
}
