#include <stdio.h>
#include <ctype.h>
#include <string.h>

int applyOperation(char currentOperator, int result, int *lastTerm, int number)
{
    if (currentOperator == '+')
    {
        result += *lastTerm;
        *lastTerm = number;
    }
    else if (currentOperator == '-')
    {
        result += *lastTerm;
        *lastTerm = -number;
    }
    else if (currentOperator == '*')
    {
        *lastTerm = (*lastTerm) * number;
    }
    else if (currentOperator == '/')
    {
        if (number == 0)
        {
            printf("Error: Division by zero.\n");
            return 0; 
        }
        *lastTerm = (*lastTerm) / number;
    }
    else
    {
        printf("Error: Invalid operator '%c'.\n", operator_);
        return 0;
    }
    return result;
}

int evaluateExpression(const char *expression)
{
    int i = 0;
	int number = 0;
	int result = 0;
	int lastTerm = 0;
    char currentOperator = '+';
    int hasDigit = 0;

    while (expression[i] != '\0')
    {
        char currentExpression = expression[i];

        if (isspace(currentExpression))
        {
            i++;
            continue;
        }
		
        if (isdigit(currentExpression))
        {
            number = number * 10 + (currentExpression - '0');
            hasDigit = 1;
        }
        else
        {
            if (!hasDigit && (currentExpression == '+' || currentExpression == '-' || currentExpression == '*' || currentExpression == '/'))
            {
                printf("Error: Invalid syntax near '%c'.\n", currentExpression);
                return 0;
            }

            result = applyOperation(currentOperator, result, &lastTerm, number);
            if (currentOperator == '/' && number == 0)
            {
            	return 0;
			} 

            if (currentExpression != '+' && currentExpression != '-' && currentExpression != '*' && currentExpression != '/')
            {
                printf("Error: Invalid operator '%c' in expression.\n", currentExpression1);
                return 0;
            }

            currentOperator = currentExpression;
            number = 0;
            hasDigit = 0;
        }

        i++;
    }

    if (!hasDigit)
    {
        printf("Error: Expression cannot end with an operator.\n");
        return 0;
    }

    result = applyOperation(currentOperator, result, &lastTerm, number);

    result += lastTerm;
    return result;
}

int main()
{
    char expression[100];

    printf("Enter expression: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = '\0';

    if (strlen(expression) == 0)
    {
        printf("Error: Empty expression.\n");
        return 0;
    }

    int answer = evaluateExpression(expression);
    printf("Result: %d\n", answer);

    return 0;
}
