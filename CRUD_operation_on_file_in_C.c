#include<stdio.h>
void create() 
{
    FILE *fp;
    int id,age;
    char name[50];
    fp=fopen("users.txt","a");
    
    if(fp==NULL) 
    {
        printf("File error!\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d",&id);
    printf("Enter Name: ");
    scanf("%s",name);
    printf("Enter Age: ");
    scanf("%d",&age);

    fprintf(fp,"%d %s %d\n",id,name,age);
    fclose(fp);
    printf("User added.\n");
}

void read() 
{
    FILE *fp;
    int id,age;
    char name[50];
    fp=fopen("users.txt", "r");
    
    if(fp==NULL) 
    {
        printf("No records found.\n");
        return;
    }

    printf("ID\tName\tAge\n");
    while(fscanf(fp,"%d %s %d",&id,name,&age)!=EOF)
    printf("%d\t%s\t%d\n", id, name, age);
    
    fclose(fp);
}

void update() 
{
    FILE *fp,*temp;
    int id,age,updateid;
    char name[50];

    fp=fopen("users.txt","r");
    temp=fopen("temp.txt","a");
    if(fp==NULL || temp==NULL) 
    {
        printf("File error!\n");
        return;
    }

    printf("Enter ID to update: ");
    scanf("%d",&updateid);

    while(fscanf(fp,"%d %s %d",&id,name,&age)!=EOF)
    {
        if(id==updateid) 
        {
            printf("Enter new Name: ");
            scanf("%s",name);
            printf("Enter new Age: ");
            scanf("%d",&age);
        }
        
        fprintf(temp,"%d %s %d\n",id,name,age);
    }

    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt", "users.txt");
    printf("Update done.\n");
}

void deleteuser()
{
    FILE *fp,*temp;
    int id,age,deleteid;
    char name[50];

    fp=fopen("users.txt","r");
    temp=fopen("temp.txt","a");
    if(fp==NULL || temp == NULL) 
    {
        printf("File error!\n");
        return;
    }

    printf("Enter ID to delete: ");
    scanf("%d",&deletedid);

    while(fscanf(fp,"%d %s %d",&id,name,&age)!=EOF)
    {
        if(id!=deleteid) 
        fprintf(temp,"%d %s %d\n",id,name,age);
    }

    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt","users.txt");
    printf("Delete done.\n");
}

int main() 
{
    create();
    create();
    read();
    update();
    deleteuser();
    return 0;
}
