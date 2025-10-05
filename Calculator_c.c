#include <stdio.h>
#include <ctype.h>
#include <string.h>
int evaluateExpression(const char *expression)
{
    int i = 0,number = 0,result = 0,lastTerm = 0;
    char currentOperator = '+';
    int digitFound = 0;

    while(expression[i] != '\0') 
	{
        char ch = expression[i];
        
        if(isspace(ch))
		{
            i++;
            continue;
        }

        if(isdigit(ch))
		{
            number = number * 10 + (ch - '0');
            digitFound = 1;
        }
        
        else
		{
            if(!digitFound && (ch == '+' || ch == '-' || ch == '*' || ch == '/')) 
			{
                printf("Error: Invalid syntax near '%c'.\n", ch);
                return 0;
            }

            if(currentOperator == '+') 
			{
                result += lastTerm;
                lastTerm = number;
            } 
			
			else if(currentOperator == '-') 
			{
                result += lastTerm;
                lastTerm =- number;
            } 
			
			else if(currentOperator == '*') 
            lastTerm = lastTerm * number;
            
			else if(currentOperator == '/') 
			{
                if(number == 0) 
				{
                    printf("Error: Division by zero.\n");
                    return 0;
                }
                
                lastTerm = lastTerm / number;
            }

            if(ch != '+' && ch != '-' && ch != '*' && ch != '/') 
			{
                printf("Error: Invalid operator '%c' in expression.\n", ch);
                return 0;
            }

            currentOperator = ch;
            number = 0;
            digitFound = 0;
        }
        
        i++;
    }

    if(!digitFound) 
	{
        printf("Error: Expression cannot end with an operator.\n");
        return 0;
    }

    if(currentOperator == '+') 
	{
        result += lastTerm;
        lastTerm = number;
    } 
	
	else if(currentOperator == '-') 
	{
        result += lastTerm;
        lastTerm =- number;
    } 
	
	else if(currentOperator == '*')
    lastTerm = lastTerm * number;
        
    else if(currentOperator == '/') 
	{
        if(number == 0) 
		{
            printf("Error: Division by zero.\n");
            return 0;
        }
        
        lastTerm = lastTerm / number;
    }
    result += lastTerm;
    return result;
}

int main() 
{
    char expression[100];

    printf("Enter expression: ");
    if(fgets(expression, sizeof(expression), stdin) == NULL) 
	{
        printf("Error: Failed to read input.\n");
        return 0;
    }

    expression[strcspn(expression, "\n")] = '\0';

    if(strlen(expression) == 0) 
	{
        printf("Error: Empty expression.\n");
        return 0;
    }

    int answer = evaluateExpression(expression);
    printf("Result: %d\n", answer);

    return 0;
}
