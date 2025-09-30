#include<stdio.h>
struct user
{
    int id;
    char name[50];
    int age;
};

void create(){
    FILE *fp;
    struct user u;
    fp=fopen("users.txt","a");  
    if(fp==NULL)
    {
        printf("File error!\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d",&u.id);
    printf("Enter Name: ");
    scanf(" %s",u.name);
    printf("Enter Age: ");
    scanf("%d",&u.age);

    fprintf(fp,"%d %s %d\n",u.id,u.name,u.age);
    fclose(fp);
    printf("User added.\n");
    
}

void read()
{
    FILE *fp;
    struct user u;
    fp=fopen("users.txt","r");  
    if(fp==NULL)
    {
        printf("No records found.\n");
        return;
    }

    printf("ID\tName\tAge\n");
    while(fscanf(fp,"%d %s %d",&u.id,u.name,&u.age)!=EOF)
    printf("%d\t%s\t%d\n",u.id,u.name,u.age);

    fclose(fp);
}

void update()
{
    FILE *fp,*temp;
    struct user u;
    int id;

    fp=fopen("users.txt","r");
    temp =fopen("temp.txt","w");
    if(fp==NULL || temp==NULL) 
    {
        printf("File error!\n");
        return;
    }

    printf("Enter ID to update: ");
    scanf("%d",&id);

    while(fscanf(fp,"%d %s %d",&u.id,u.name,&u.age)!=EOF)
    {
        if(u.id==id) 
        {
            printf("Enter new Name: ");
            scanf(" %s",u.name);
            printf("Enter new Age: ");
            scanf("%d",&u.age);
        }
        
        fprintf(temp,"%d %s %d\n",u.id,u.name,u.age);
    }

    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt","users.txt");
    printf("Update done.\n");
}

void deleteuser() 
{
    FILE *fp, *temp;
    struct user u;
    int id;

    fp=fopen("users.txt","r");
    temp=fopen("temp.txt","w");
    
    if(fp==NULL || temp==NULL) 
    {
        printf("File error!\n");
        return;
    }
    
    printf("Enter ID to delete: ");
    scanf("%d",&id);

    while(fscanf(fp,"%d %s %d",&u.id,u.name,&u.age)!=EOF)
    {
        if(u.id!=id)  
        fprintf(temp,"%d %s %d\n",u.id,u.name,u.age);
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
