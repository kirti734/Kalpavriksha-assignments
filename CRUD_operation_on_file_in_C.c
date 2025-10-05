#include<stdio.h>
#include<string.h>
struct User
{
    int userId;
    char userName[100];
    int userAge;
};

void createUser()
{
    FILE *fp;
    struct User user;

    fp=fopen("users.txt","a");
    if(fp==NULL)
	{
        printf("Error opening file for writing");  
        return;
    }

    printf("Enter User ID: ");
    scanf("%d", &user.userId);
    getchar();

    printf("Enter Full Name: ");
    fgets(user.userName, sizeof(user.userName), stdin);

    int len = strlen(user.userName);
    if(len>0 && user.userName[len-1] == '\n')
	{
        user.userName[len-1] = '\0';
    }

    if(strlen(user.userName) >= sizeof(user.userName)-1)
	{
        printf("Error: Name too long.\n");
        fclose(fp);
        return;
    }

    printf("Enter Age: ");
    scanf("%d", &user.userAge);

    if(user.userAge < 0){
        printf("Error: Age cannot be negative.\n");
        fclose(fp);
        return;
    }

    fprintf(fp, "%d|%s|%d\n", user.userId, user.userName, user.userAge);
    fclose(fp);
    printf("? User added successfully.\n");
}

void readUser()
{
    FILE *fp;
    struct User user;

    fp = fopen("users.txt","r");
    if(fp == NULL)
	{
        printf("?? No records found or unable to open file.\n");
        return;
    }

    printf("\nID\tName\t\tAge\n");
    printf("---------------------------------------\n");

    while(fscanf(fp,"%d|%99[^|]|%d\n", &user.userId, user.userName, &user.userAge)==3)
	{
        printf("%d\t%s\t%d\n", user.userId, user.userName, user.userAge);
    }

    fclose(fp);
}

void updateUser()
{
    FILE *fp,*temp;
    struct User user;
    int updateId, found=0;

    fp=fopen("users.txt","r");
    temp=fopen("temp.txt","w");

    if(fp == NULL || temp == NULL)
	{
        printf("Error opening file for updating");
        return;
    }

    printf("Enter User ID to update: ");
    scanf("%d", &updateId);
    getchar(); 

    while(fscanf(fp,"%d|%99[^|]|%d\n", &user.userId, user.userName, &user.userAge)==3)
	{
        if(user.userId == updateId)
		{
            found=1;
            printf("Enter new Full Name: ");
            fgets(user.userName,sizeof(user.userName), stdin);
            user.userName[strcspn(user.userName,"\n")] = '\0'; 
            printf("Enter new Age: ");
            scanf("%d", &user.userAge);

            if(user.userAge<0)
			{
                printf("Error: Age cannot be negative.\n");
                fclose(fp);
                fclose(temp);
                return;
            }
        }
        fprintf(temp,"%d|%s|%d\n", user.userId, user.userName, user.userAge);
    }

    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt","users.txt");

    if(found)
        printf("? User updated successfully.\n");
    else
        printf("?? User with ID %d not found.\n", updateId);
}

void deleteUser()
{
    FILE *fp,*temp;
    struct User user;
    int deleteId,found = 0;

    fp=fopen("users.txt","r");
    temp=fopen("temp.txt","w");

    if(fp == NULL || temp == NULL)
	{
        printf("Error opening file for deleting");
        return;
    }

    printf("Enter User ID to delete: ");
    scanf("%d", &deleteId);

    while(fscanf(fp,"%d|%99[^|]|%d\n", &user.userId, user.userName, &user.userAge)==3){
        if(user.userId != deleteId){
            fprintf(temp,"%d|%s|%d\n", user.userId, user.userName, user.userAge);
        } 
		else{
            found=1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt","users.txt");

    if(found)
        printf("? User deleted successfully.\n");
    else
        printf("?? User with ID %d not found.\n", deleteId);
}

int main(){
    int choice;

    printf("\n====== USER MANAGEMENT SYSTEM ======\n");

    while(1){
		
        printf("\n1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);

        if(choice == 1)
            createUser();
        else if(choice == 2)
            readUsers();
        else if(choice == 3)
            updateUser();
        else if(choice == 4)
            deleteUser();
        else if(choice == 5){
            printf("Exiting program...\n");
            break;
        } else
            printf("Invalid choice! Try again.\n");
    }
    return 0;
}
