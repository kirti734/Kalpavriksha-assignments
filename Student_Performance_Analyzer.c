#include <stdio.h>
#include <stdlib.h>

#define SUBJECT_COUNT 3
#define NAME_LENGTH 100

enum Grade {A, B, C, D, F};

struct Student
{
	int rollNumber;
	char studentName[NAME_LENGTH];
	int marks[SUBJECT_COUNT];
};

void sortRollNumbers(int listofStudents[], int numberofStudents)
{
    if (numberofStudents == 1)
    {
    	return;
	}
	
	int index;
	int tempRollNumber;
        
    for (index = 0; index < numberofStudents - 1; index++)
    {
        if (listofStudents[index] > listofStudents[index + 1])
        {
            tempRollNumber = listofStudents[index];
            listofStudents[index] = listofStudents[index + 1];
            listofStudents[index + 1] = tempRollNumber;
        }
    }
    
    sortRollNumbers(listofStudents, numberofStudents - 1);
}

void printRollNumber(int listofStudents[], int numberofStudents, int index)
{
	if(index >= numberofStudents)
	{
		return;
	}
	
	printf("%d ", listofStudents[index]);
	
	printRollNumber(listofStudents, numberofStudents, index+1);
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
	int index = 0;
	int marksTotal = 0;
	
	for(index = 0; index < SUBJECT_COUNT; index++)
	{
		marksTotal += marks[index];
	}
	
	return marksTotal;
}

float calculateAverage(int totalMarks)
{
	float marksAverage;
	marksAverage = totalMarks / (float)(SUBJECT_COUNT);
	return marksAverage;
}

void performanceAnalyzer(struct Student students[] , int numberofStudents)
{
	int index = 0;
	int totalMarks;
	float marksAverage;
	int *listofStudents = malloc(numberofStudents * sizeof(int));
	
	if(listofStudents == NULL)
	{
		printf("Memory allocation is failed\n");
		return;
	}
	
	while(index < numberofStudents)
	{
		totalMarks = calculateTotal(students[index].marks);
		
		marksAverage = calculateAverage(totalMarks);
		listofStudents[index] = students[index].rollNumber;
		
		printf("Roll: %d\n", students[index].rollNumber);
		printf("Name: %s\n", students[index].studentName);
		printf("Total: %d\n", totalMarks);
		printf("Average: %0.2f\n", marksAverage);
		
		enum Grade grade = calculateGrade(marksAverage);
		
		printf("Grade: %c\n", getGrade(grade));
		printf("Performance: %s\n", performanceStars(grade));
		
		printf("\n");
		
		index++;	
	}
	
	index = 0;
		
	sortRollNumbers(listofStudents , numberofStudents);
	
	printf("List of Roll Number (via recursion): ");
	printRollNumber(listofStudents , numberofStudents, index);
	
	free(listofStudents);
}

void getStudentSubjectMarks(int marks[])
{
	int index;
	int subjectMarks;
	for (index = 0; index < SUBJECT_COUNT; index++)
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

			marks[index] = subjectMarks;
		}
}

void insertStudentDetails(struct Student students[] , int numberofStudents)
{
	int index;
	
	for (index = 0; index < numberofStudents; index++)
	{
		if (scanf("%d ", & students[index].rollNumber) != 1)
		{
			printf("Roll Number should be a numeric value");
			return;
		}
	
		if (students[index].rollNumber < 0)
		{
			printf("Roll Number should be a positive value");
			return;
		}
		
		scanf("%99s", students[index].studentName);
		getStudentSubjectMarks(students[index].marks);
	}
}

int main()
{
	int numberofStudents = 0;
	
	if (scanf("%d", & numberofStudents) != 1)
	{
		printf("Number of students should be a numeric value");
		return 1;
	}

	if (numberofStudents < 0)
	{
		printf("Number of students should be a positive value");
		return 1;
	}
	
	struct Student *students = malloc(numberofStudents * sizeof(struct Student));
	if(students == NULL)
	{
		printf("Memory allocation is failed\n");
		return 1;
	}
	
	insertStudentDetails(students , numberofStudents);
	performanceAnalyzer(students , numberofStudents);
	
	free(students);
}
