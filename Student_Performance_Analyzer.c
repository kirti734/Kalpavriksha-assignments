#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student
{
	int rollNumber;
	char studentName[100];
	int marks1;
	int marks2;
	int marks3;
};

void sortRollNumbers(int listofStudents[], int numberofStudents)
{
    if (numberofStudents == 1)
    {
    	return;
	}
	int i;
        
    for (i = 0; i < numberofStudents - 1; i++)
    {
        if (listofStudents[i] > listofStudents[i + 1])
        {
            int temp = listofStudents[i];
            listofStudents[i] = listofStudents[i + 1];
            listofStudents[i + 1] = temp;
        }
    }
    
    recursiveSort(listofStudents, numberofStudents - 1);
}

void performanceAnalyzer(struct Student students[] , int numberofStudents)
{
	int i = 0;
	int marksTotal = 0;
	float marksAverage;
	int listofStudents[numberofStudents];
	
	while(i < numberofStudents)
	{
		marksTotal = students[i].marks1 + students[i].marks2 + students[i].marks3;
		marksAverage = marksTotal / numberofStudents;
		listofStudents[i] = students[i].rollNumber;
		
		printf("Roll: %d\n", students[i].rollNumber);
		printf("Name: %s\n", students[i].studentName);
		printf("Total: %d\n", marksTotal);
		printf("Average %f\n", marksAverage);
		
		if (marksAverage >= 85)
		{
		    printf("Grade: A\n");
		    printf("Performance: *****\n");
		}
		else if (marksAverage >= 70)
		{
		    printf("Grade: B\n");
		    printf("Performance: ****\n");
		}
		else if (marksAverage >= 50)
		{
		    printf("Grade: C\n");
		    printf("Performance: ***\n");
		}
		else if (marksAverage >= 35)
		{
		    printf("Grade: D\n");
		    printf("Performance: **\n");
		}
		else
		{
		    printf("Grade: F\n");
		}
		
		printf("\n");
		i++;
	}
	
	i = 0;
	printf("List of Roll Number (via recursion): ");
		
	sortRollNumbers(listofStudents , numberofStudents);
		
	while (i < numberofStudents)
	{
		printf("%d ", listofStudents[i]);
		i++;
	}
	
}

int main()
{
	int numberofStudents = 0;
	scanf("%d", & numberofStudents);
	
	struct Student students[numberofStudents];
	int i;
	
	for (i = 0 ;i < numberofStudents ; i++)
	{
		scanf("%d %s %d %d %d", &students[i].rollNumber, students[i].studentName, &students[i].marks1, &students[i].marks2, &students[i].marks3);
	}
	
	performanceAnalyzer(students , numberofStudents);
}
