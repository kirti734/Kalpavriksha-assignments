#include <stdio.h>
int main()
{
    char expr[100];
    int i,num=0,result=0,last=0;
    char op='+';
    
    printf("Enter expression: ");
    scanf("%[^\n]",expr);

    for(i=0;expr[i]!='\0';i++)
    {
        if(expr[i]==' ')
        continue; 
        
        if(expr[i]>='0' && expr[i]<='9')
        num=num*10+(expr[i]-'0');
        
        else 
        {
            if(op=='+')
            {
                result+=last;
                last=num;
            }
            
            else if(op=='-')
            {
                result+=last;
                last=-num;
            }
            
            else if(op=='*')
            last=last*num;
            
            else if(op=='/')
            {
                if(num==0)
                {
                    printf("Error: Division by zero.\n");
                    return 0;
                }
                last=last/num;
            }
            
            else
            {
                printf("Error: Invalid expression.\n");
                return 0;
            }
            op=expr[i];
            num=0;
        }
    }

    if(op=='+')
    {
        result+=last;
        last=num;
    }
    
    else if(op=='-')
    {
        result+=last;
        last=-num;
    }
    
    else if(op=='*')
    last=last*num;
    
    else if(op=='/')
    {
        if(num==0)
        {
            printf("Error: Division by zero.\n");
            return 0;
        }
        last=last/num;
    }
    
    result+=last;

    printf("%d", result);
    return 0;
}
