#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define TOTAL_BLOCKS 1024
#define FILE_NAME 50
#define DIRECTORY_NAME 50
#define COMMAND_NAME_SIZE 100

struct FreeBlock {
    int blockIndex;
    struct FreeBlock* previous;
    struct FreeBlock* next;
};

unsigned char virtualDisk[TOTAL_BLOCKS][BLOCK_SIZE];
struct FreeBlock* freeListHead = NULL;

struct FileNode {
    char name[51];  
    int isDirectory; 
    struct FileNode* parent;
    struct FileNode* next; // for next sibling for circular linked list
    struct FileNode* children; // for first child if directory

    int blockPointer[TOTAL_BLOCKS]; 
    int blockCount;
};

struct FileNode* root = NULL;
struct FileNode* cwd = NULL;

void initializeFreeList() 
{
    freeListHead = NULL;
    int index;
    
    for (index = 0; index < TOTAL_BLOCKS; index++) 
	{
        struct FreeBlock* newBlock = (struct FreeBlock*) malloc (sizeof(struct FreeBlock));
        
        newBlock->blockIndex = index;
        newBlock->previous = NULL;
        newBlock->next = freeListHead;
        
        if (freeListHead != NULL)
        {
        	freeListHead->previous = newBlock;
		}
        freeListHead = newBlock;
    }
}

void initializeRoot() 
{
    root = (struct FileNode*) malloc (sizeof(struct FileNode));
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->parent = NULL;
    root->next = root;     
    root->children = NULL; 
    root->blockCount = 0;
    cwd = root;
}

void mkdirCommand (char* diretoryName)
{
	printf("diretory created: %s\n", diretoryName);
}

void createCommand (char* fileName)
{
	printf("file created: %s\n", fileName);
}

void writeCommand (char* fileName, char* data)
{
	printf("filename = %s\n", fileName);
	printf("content = %s\n", data);	
}

void readCommand (char* fileName)
{
	printf("filename read: %s\n", fileName);
}

void deleteCommand (char* fileName)
{
	printf("filename deleted: %s\n", fileName);
}

void rmdirCommand(char* diretoryName)
{
	printf("dirname deleted: %s\n", diretoryName);
}

void lsCommand()
{
	printf("list of files and directories\n");
}

void cdCommand(char* diretoryName)
{
	printf("changed the current directory\n");
}

void pwdCommand()
{
	printf("displays the path\n");
}

void dfCommand()
{
	printf("displays the block usage\n");
}
void exitCommand()
{
	printf("File system exit");
}

int main()
{
	printf("INITIALIZING VIRTUAL FILE SYSTEM......\n");
	printf("-------------------------------------------\n");
	
	initializeFreeList();
	initializeRoot();
	
	printf("Compact VFS - ready. Type 'exit' to quit.\n\n");
	
	char command[COMMAND_NAME_SIZE];
	
	while(1)
	{
		printf("/>");
		
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
	return 0;
}
