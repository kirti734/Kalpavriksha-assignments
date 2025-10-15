#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUBJECT_COUNT 3

enum Grade {A ,B, C, D, F};

struct Student
{
	int rollNumber;
	char studentName[100];
	int marks[SUBJECT_COUNT];
};

void sortRollNumbers(int listofStudents[], int numberofStudents)
{
    if (numberofStudents == 1)
    {
    	return;
	}
	
	int i;
	int rollTemp;
        
    for (i = 0; i < numberofStudents - 1; i++)
    {
        if (listofStudents[i] > listofStudents[i + 1])
        {
            rollTemp = listofStudents[i];
            listofStudents[i] = listofStudents[i + 1];
            listofStudents[i + 1] = rollTemp;
        }
    }
    
    sortRollNumbers(listofStudents, numberofStudents - 1);
}

enum Grade calculateGrade(float averageMarks) 
{
    if (averageMarks >= 85)
    {
    	 return A;
	}
    else if (averageMarks >= 70) 
	{
		return B;
	}
    else if (averageMarks >= 50) 
    {
    	return C;
	}
    else if (averageMarks >= 35) 
    {
    	return D;
	}
    else 
	{
    	return F;
	}
}

char getGrade(enum Grade grade)
{
	switch(grade)
	{
		case A:
			return 'A';
			
		case B:
			return 'B';
			
		case C:
			return 'C';
			
		case D:
			return 'D';
			
		default:
			return 'F';			
	}
}

const char* performanceStars(enum Grade grade) 
{
    switch(grade)
    {
    	case A:
			return "*****";
			
		case B:
			return "****";
			
		case C:
			return "***";
			
		case D:
			return "**";
			
		default:
			return "";	
	}
}

int calculateTotal(int marks[])
{
	int i = 0;
	int marksTotal = 0;
	
	for(i = 0; i < SUBJECT_COUNT; i++)
	{
		marksTotal += marks[i];
	}
	
	return marksTotal;
}

void performanceAnalyzer(struct Student students[] , int numberofStudents)
{
	int i = 0;
	int marksTotal;
	float marksAverage;
	int listofStudents[numberofStudents];
	
	while(i < numberofStudents)
	{
		marksTotal = calculateTotal(students[i].marks);
		
		marksAverage = marksTotal / 3.0;
		listofStudents[i] = students[i].rollNumber;
		
		printf("Roll: %d\n", students[i].rollNumber);
		printf("Name: %s\n", students[i].studentName);
		printf("Total: %d\n", marksTotal);
		printf("Average %0.2f\n", marksAverage);
		
		enum Grade grade = calculateGrade(marksAverage);
		
		printf("Grade: %c\n", getGrade(grade));
		printf("Performance: %s\n", performanceStars(grade));
		
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

void insertStudentDetails(struct Student students[] , int numberofStudents)
{
	int i;
	int j;
	int subjectMarks;
	
	for (i = 0 ;i < numberofStudents ; i++)
	{
		scanf("%d %99s", &students[i].rollNumber, students[i].studentName);
		
		for (j = 0; j < SUBJECT_COUNT; j++)
		{
			if (scanf("%d", &subjectMarks) != 1)
			{
				printf("Marks should be a numeric value");
				return;
			}
			
			if (subjectMarks < 0)
			{
				printf("Marks should be a positive value");
				return;
			}

			students[i].marks[j] = subjectMarks;
		}
	}
}

int main()
{
	int numberofStudents = 0;
	
	if (scanf("%d", & numberofStudents) != 1)
	{
		printf("Number of students should be a numeric value");
		return;
	}

	if (numberofStudents < 0)
	{
		printf("Number of students should be a positive value");
		return;
	}
	
	struct Student students[numberofStudents];
	
	insertStudentDetails(students , numberofStudents);
	performanceAnalyzer(students , numberofStudents);
}
