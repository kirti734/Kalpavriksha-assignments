#include <stdio.h>

#define SUBJECT_COUNT 3
#define NAME_LENGTH 100

enum Grade {A ,B, C, D, F};

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
	int i = 0;
	int marksTotal = 0;
	
	for(i = 0; i < SUBJECT_COUNT; i++)
	{
		marksTotal += marks[i];
	}
	
	return marksTotal;
}

float calculateAverage(int totalMarks)
{
	int marksAverage;
	marksAverage = totalMarks / (float)(SUBJECT_COUNT);
	return marksAverage;
}

void performanceAnalyzer(struct Student students[] , int numberofStudents)
{
	int i = 0;
	int totalMarks;
	float marksAverage;
	int listofStudents[numberofStudents];
	
	while(i < numberofStudents)
	{
		totalMarks = calculateTotal(students[i].marks);
		
		marksAverage = calculateAverage(totalMarks);
		listofStudents[i] = students[i].rollNumber;
		
		printf("Roll: %d\n", students[i].rollNumber);
		printf("Name: %s\n", students[i].studentName);
		printf("Total: %d\n", totalMarks);
		printf("Average %0.2f\n", marksAverage);
		
		enum Grade grade = calculateGrade(marksAverage);
		
		printf("Grade: %c\n", getGrade(grade));
		printf("Performance: %s\n", performanceStars(grade));
		
		printf("\n");
		
		i++;	
	}
	
	i = 0;
		
	sortRollNumbers(listofStudents , numberofStudents);
	
	printf("List of Roll Number (via recursion): ");
	printRollNumber(listofStudents , numberofStudents, i);
}

void getStudentSubjectMarks(int marks[])
{
	int i;
	int subjectMarks;
	for (i = 0; i < SUBJECT_COUNT; i++)
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

			marks[i] = subjectMarks;
		}
}

void insertStudentDetails(struct Student students[] , int numberofStudents)
{
	int i;
	
	for (i = 0 ;i < numberofStudents ; i++)
	{
		scanf("%d %99s", &students[i].rollNumber, students[i].studentName);
		getStudentSubjectMarks(students[i].marks);
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
