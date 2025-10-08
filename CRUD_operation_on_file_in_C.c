#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct User 
{
    char userId[50];     
    char userName[100];
    int userAge;
};

int isNumeric(char str[]) 
{
    int i;
    for (i = 0; str[i] != '\0'; i++) 
	{
        if (str[i] < '0' || str[i] > '9')
        {
            return 0;
		}
    }
    return 1;
}

void createUser() 
{
    FILE *fp;
    struct User user;
    char tempAge[20];

    fp = fopen("users.txt", "a");
    if (fp == NULL) 
	{
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    printf("Enter User ID (letters or numbers allowed): ");
    scanf("%s", user.userId);
    getchar(); 

    printf("Enter Full Name: ");
    fgets(user.userName, sizeof(user.userName), stdin);
    user.userName[strcspn(user.userName, "\n")] = '\0'; 

    if (strlen(user.userName) == 0) 
	{
        printf("Error: Name cannot be empty.\n");
        fclose(fp);
        return;
    }

    printf("Enter Age: ");
    scanf("%s", tempAge);

    if (!isNumeric(tempAge)) 
	{
        printf("Error: Age must be numeric only.\n");
        fclose(fp);
        return;
    }

    user.userAge = atoi(tempAge);

    if (user.userAge < 0) 
	{
        printf("Error: Age cannot be negative.\n");
        fclose(fp);
        return;
    }

    fprintf(fp, "%s|%s|%d\n", user.userId, user.userName, user.userAge);
    fclose(fp);
    printf("? User added successfully.\n");
}

void readUsers() 
{
    FILE *fp;
    struct User user;

    fp = fopen("users.txt", "r");
    if (fp == NULL) 
	{
        printf("?? No records found or unable to open file.\n");
        return;
    }

    printf("\nID\t\tName\t\tAge\n");
    printf("-------------------------------------------\n");

    while (fscanf(fp, "%49[^|]|%99[^|]|%d\n", user.userId, user.userName, &user.userAge) == 3) 
	{
        printf("%s\t\t%s\t\t%d\n", user.userId, user.userName, user.userAge);
    }

    fclose(fp);
}

void updateUser() 
{
    FILE *fp;
	FILE *temp;
    struct User user;
    char updateId[50];
    char tempAge[20];
    int found = 0;

    fp = fopen("users.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) 
	{
        printf("Error: Unable to open file for updating.\n");
        return;
    }

    printf("Enter User ID to update: ");
    scanf("%s", updateId);
    getchar();

    while (fscanf(fp, "%49[^|]|%99[^|]|%d\n", user.userId, user.userName, &user.userAge) == 3) 
	{
        if (strcmp(user.userId, updateId) == 0) 
		{
            found = 1;
            printf("Enter new Full Name: ");
            fgets(user.userName, sizeof(user.userName), stdin);
            user.userName[strcspn(user.userName, "\n")] = '\0';

            printf("Enter new Age: ");
            scanf("%s", tempAge);

            if (!isNumeric(tempAge)) 
			{
                printf("Error: Age must be numeric only.\n");
                fclose(fp);
                fclose(temp);
                return;
            }

            user.userAge = atoi(tempAge);

            if (user.userAge < 0) 
			{
                printf("Error: Age cannot be negative.\n");
                fclose(fp);
                fclose(temp);
                return;
            }
        }

        fprintf(temp, "%s|%s|%d\n", user.userId, user.userName, user.userAge);
    }

    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
    {
    	printf("? User updated successfully.\n");
	}
    else
    {
    	printf("?? User with ID %s not found.\n", updateId);	
	}    
}

void deleteUser() 
{
    FILE *fp;
	FILE *temp;
    struct User user;
    char deleteId[50];
    int isIdFound = 0;

    fp = fopen("users.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) 
	{
        printf("Error: Unable to open file for deleting.\n");
        return;
    }

    printf("Enter User ID to delete: ");
    scanf("%s", deleteId);

    while (fscanf(fp, "%49[^|]|%99[^|]|%d\n", user.userId, user.userName, &user.userAge) == 3) 
	{
        if (strcmp(user.userId, deleteId) != 0)
        {
        	fprintf(temp, "%s|%s|%d\n", user.userId, user.userName, user.userAge);
		}
        else
        {
        	isIdFound = 1;
		}
    }

    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (isIdFound)
    {
    	printf("? User deleted successfully.\n");
	}	
    else
    {
    	printf("?? User with ID %s not found.\n", deleteId);
	}
}

int main() 
{
    int choice;

    printf("\n====== USER MANAGEMENT SYSTEM ======\n");

    while (1) 
	{
        printf("\n 1. Create User \n");
        printf(" 2. Read Users \n");
        printf(" 3. Update User \n");
        printf(" 4. Delete User \n");
        printf(" 5. Exit \n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
		{
            case 1: 
				createUser(); 
				break;
			
            case 2: 
				readUsers(); 
				break;
				
            case 3: 
				updateUser(); 
				break;
				
            case 4: 
				deleteUser(); 
				break;
				
            case 5: 
				printf("Exiting program...\n"); 
				return 0;
				
            default: 
				printf("Invalid choice! Try again.\n");
        }
    }
}
