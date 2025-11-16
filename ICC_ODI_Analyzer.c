#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Players_data.h"

#define MIN_ID 1
#define MAX_ID 1500

#define MIN_PLAYERS_IN_TEAM 11
#define MAX_PLAYERS_IN_TEAM 50

#define NAME_LENGTH 50
#define ROLE_NAME_LENGTH 20
#define TEAM_NAME_LENGTH 20

#define MAX_PLAYER_OF_ALL_TEAM_SPECIFIC_ROLE 1000

enum Role {BATSMAN = 1, BOWLER = 2, ALLROUNDER = 3};

struct playerNode
{
	int playerId;
	char playerName[NAME_LENGTH];
	char teamName[TEAM_NAME_LENGTH];
	char playerRole[ROLE_NAME_LENGTH];
	int totalRuns;
	float battingAverage;
	float strikeRate;
	int wickets;
	float economyRate;
	float performanceIndex;
	struct playerNode* next;
};

struct teamNode
{
	int teamId;
	char teamName[TEAM_NAME_LENGTH];
	int totalPlayers;
	float averageBattingStrikeRate;
	
	struct playerNode* batters;
	struct playerNode* bowlers;
	struct playerNode* allRounders;
	
	struct teamNode* left;
	struct teamNode* right;
};

struct teamNode* root = NULL;

struct teamNode* insertTeam(struct teamNode* root, int teamId, const char* teamName)
{
	if(root == NULL)
	{
		struct teamNode* newTeam = (struct teamNode*) malloc (sizeof(struct teamNode));
		if(newTeam == NULL)
		{
			printf("\nMemory allocation failed\n");
			return NULL;
		}
		
		newTeam -> teamId = teamId;
		strcpy(newTeam -> teamName, teamName);
		newTeam -> totalPlayers = 0;
		newTeam -> averageBattingStrikeRate = 0.00;
		newTeam -> batters = NULL;
		newTeam -> bowlers = NULL;
		newTeam -> allRounders = NULL;
		newTeam -> left = NULL;
		newTeam -> right = NULL;
		
		return newTeam;
	}
	
	int comparison = strcmp(teamName, root -> teamName);
	
	if(comparison < 0)
	{
		root -> left = insertTeam(root -> left, teamId, teamName);
	}
	else if (comparison > 0)
	{
		root -> right = insertTeam(root -> right, teamId, teamName);
	}
	else
	{
		return root;
	}
	
	return root;
}

void initializeTeams()
{
	int index = 0;
	for(index = 0; index < teamCount; index++)
	{
		root = insertTeam(root, index + 1, teams[index]);
	}
}

void printTeams()
{
	int index = 0;
	printf("(");
	for(index = 0; index < teamCount; index++)
	{
		printf("%d -- %s\n", (index + 1), teams[index]);
	}
	printf(") :");
}

float calculatePerformanceIndex(struct playerNode* newPlayer)
{
	if(strcmp(newPlayer -> playerRole, "Batsman") == 0)
	{
		return ((newPlayer -> battingAverage * newPlayer -> strikeRate) / 100.00);
	}
	else if(strcmp(newPlayer -> playerRole, "Bowler") == 0)
	{
		return ((newPlayer -> wickets * 2.00) + (100.00 - newPlayer -> economyRate));
	}
	else 
	{
		return (((newPlayer -> battingAverage * newPlayer -> strikeRate) / 100.00) + (newPlayer -> wickets * 2.00));
	}
}

struct teamNode* searchTeamByName(struct teamNode* root, const char* teamName)
{
	if(root == NULL)
	{
		return NULL;
	}
	
	int comparison = strcmp(teamName, root -> teamName);
	
	if(comparison == 0)
	{
		return root;
	}
	else if(comparison < 0)
	{
		return searchTeamByName(root -> left, teamName);
	}
	else
	{
		return searchTeamByName(root -> right, teamName);
	}
}

void calculateAverageBattingStrikeRate(struct teamNode* team)
{
	if(team == NULL)
	{
		return;
	}
	
	float average = 0.00;
	int count = 0;
	
	struct playerNode *temp = team -> batters;
	
	while (temp != NULL)
	{
		average += temp -> strikeRate;
		count++;
		temp = temp -> next;
	}
	
	temp = team -> allRounders;
	
	while (temp != NULL)
	{
		average += temp -> strikeRate;
		count++;
		temp = temp -> next;
	}
	
	if(count > 0)
	{
		team -> averageBattingStrikeRate = (average / count);
	}
	else
	{
		team -> averageBattingStrikeRate = 0.00;
	}
}

void insertIntoTeamList(struct playerNode* newPlayer)
{
	struct teamNode* playerTeam = searchTeamByName(root, newPlayer -> teamName);
	if(playerTeam == NULL)
	{
		printf("\nTeam is not found\n");
		return;
	}
	
	struct playerNode** roleHead = NULL;
	
	if(strcmp(newPlayer -> playerRole, "Batsman") == 0)
	{
		roleHead = &playerTeam -> batters;
	}
	else if(strcmp(newPlayer -> playerRole, "Bowler") == 0)
	{
		roleHead = &playerTeam -> bowlers;
	}
	else 
	{
		roleHead = &playerTeam -> allRounders;
	}
	
	if(*roleHead == NULL)
	{
		newPlayer -> next = NULL;
		*roleHead = newPlayer;
	}
	else if((*roleHead) -> performanceIndex < newPlayer -> performanceIndex)
	{
		newPlayer -> next = *roleHead;
		*roleHead = newPlayer;
	}
	else
	{
		struct playerNode* temp = *roleHead;
		
		while (temp -> next != NULL && temp -> next -> performanceIndex > newPlayer -> performanceIndex)
		{
			temp = temp -> next;
		}
		newPlayer -> next = temp -> next;
		temp -> next = newPlayer;
	}
	
	calculateAverageBattingStrikeRate(playerTeam);
	playerTeam -> totalPlayers++;
}

void addPlayer(int id, const char* name, const char* team, const char* role, int totalRuns, float battingAverage, float strikeRate, int wickets, float economyRate)
{
	struct playerNode* newPlayer = (struct playerNode*) malloc (sizeof(struct playerNode));
	if(newPlayer == NULL)
	{
		printf("\nMemory allocation failed\n");
		return;
	}
	
	newPlayer -> playerId = id;
	strcpy(newPlayer -> playerName, name);
	strcpy(newPlayer -> teamName, team);
	strcpy(newPlayer -> playerRole, role);
	newPlayer -> totalRuns = totalRuns;
	newPlayer -> battingAverage = battingAverage;
	newPlayer -> strikeRate = strikeRate;
	newPlayer -> wickets = wickets;
	newPlayer -> economyRate = economyRate;
	newPlayer -> performanceIndex = calculatePerformanceIndex(newPlayer);
	newPlayer -> next = NULL;
	
	insertIntoTeamList(newPlayer);
}

void initializePlayers()
{
	int index = 0;
	for(index = 0; index < playerCount; index++)
	{
		Player newPlayer = players[index];
		addPlayer(newPlayer.id, newPlayer.name, newPlayer.team, newPlayer.role, newPlayer.totalRuns, newPlayer.battingAverage, newPlayer.strikeRate, newPlayer.wickets, newPlayer.economyRate);
	}
}

struct teamNode* searchTeamById(struct teamNode* root, int teamId)
{
	if(root == NULL)
	{
		return NULL;
	}
	
	if(root -> teamId == teamId)
	{
		return root;
	}
	
	struct teamNode* leftResult = searchTeamById(root -> left, teamId);
	
	if(leftResult != NULL)
	{
		return leftResult;
	}
	
	return searchTeamById(root -> right, teamId);
} 

const char* findRole(enum Role role)
{
	switch(role)
	{
		case BATSMAN:
			return "Batsman";
		case BOWLER:
			return  "Bowler";
		case ALLROUNDER:
			return "All-rounder";
	}
}

int checkUniqueId(struct teamNode* root, int playerId)
{
	if (root == NULL)
	{
		return 0;
	}
	
	if (checkUniqueId(root -> left, playerId))
	{
		return 1;
	}
	
	struct playerNode* temp = root -> batters;
	
	while (temp != NULL)
	{
		if (temp -> playerId == playerId)
		{
			return 1;
		}
		temp = temp -> next;
	}
	
	temp = root -> bowlers;
	
	while (temp != NULL)
	{
		if (temp -> playerId == playerId)
		{
			return 1;
		}
		temp = temp -> next;
	}
	
	temp = root -> allRounders;
	
	while (temp != NULL)
	{
		if (temp -> playerId == playerId)
		{
			return 1;
		}
		temp = temp -> next;
	}
	
	if (checkUniqueId(root -> right, playerId))
	{
		return 1;
	}
	
	return 0;

}

void addNewPlayer()
{
	printf("\nEnter Team ID to add player\n");
	printTeams();
	int teamId = 0;
	
	if(scanf("%d", &teamId) != 1)
	{
		while(getchar() != '\n');
		printf("\nTeam Id should be numeric\n");
		return;
	}
	while(getchar()!='\n');  
	
	if(teamId < MIN_ID || teamId > MAX_ID)
	{
		printf("\nTeam Id should be in between 1 and 1500\n");
		return;
	}
	
	
	char teamName[TEAM_NAME_LENGTH];
	struct teamNode* team = searchTeamById(root, teamId);
	if(team == NULL)
	{
		printf("\nTeam is not found\n");
		return;
	}
	strcpy(teamName, team->teamName);
	
	
	printf("\nEnter Player Details:\n");
	
	printf("\nPlayer ID: ");
	int playerId = 0;
	
	if(scanf("%d", &playerId) != 1)
	{
		while (getchar() != '\n');
		printf("\nPlayer Id should be numeric\n");
		return;
	}
	while(getchar()!='\n');  
	
	if(playerId < MIN_ID || playerId > MAX_ID)
	{
		printf("\nPlayer Id should be in between 1 and 1500\n");
		return;
	}
	
	if(checkUniqueId(root, playerId))
	{
		printf("\nEnter the Unique Player id\n");
		return;
	}
	

	char playerName[NAME_LENGTH];
	printf("\nName: ");
	scanf("%49s", playerName);
	
		
	char playerRole[ROLE_NAME_LENGTH];
	int playerRoleNumber = 0;
	printf("\nRole (1-Batsman, 2-Bowler, 3-All-rounder): ");
	
	if(scanf("%d", &playerRoleNumber) != 1)
	{
		while (getchar() != '\n');
		printf("\nRole should be numeric\n");
		return;
	}
	while(getchar()!='\n'); 
	
	if(playerRoleNumber < 1 || playerRoleNumber > 3)
	{
		printf("\nRole should be in the range of 1 to 3\n");
		return;
	}
	 
	strcpy(playerRole, findRole(playerRoleNumber));


	int totalRuns = 0;
	printf("\nTotal Runs: ");
	if(scanf("%d", &totalRuns) != 1)
	{
		while(getchar() != '\n');
		printf("\nTotal Runs should be numeric\n");
		return;
	}
	while(getchar()!='\n');
	
	
	float battingAverage = 0.00;
	printf("\nBatting Average: ");
	
	if(scanf("%f", &battingAverage) != 1)
	{
		while (getchar() != '\n');
		printf("\nBatting Average should be numeric\n");
		return;
	}
	while(getchar()!='\n');
	
	
	float strikeRate = 0.00;
	printf("\nStrike Rate: ");
	
	if(scanf("%f", &strikeRate) != 1)
	{
		while (getchar() != '\n');
		printf("\nStrike Rate should be numeric\n");
		return;
	}
	while(getchar()!='\n');
	
	
	int wickets = 0;
	printf("\nWickets: ");
	
	if(scanf("%d", &wickets) != 1)
	{
		while (getchar() != '\n');
		printf("\nWickets should be numeric\n");
		return;
	}
	while(getchar()!='\n');
	
	
	float economyRate = 0.00;
	printf("\nEconomy Rate: ");
	
	if(scanf("%f", &economyRate) != 1)
	{
		while (getchar() != '\n');
		printf("\nEconomy Rate should be numeric\n");
		return;
	}
	while(getchar()!='\n');
	
	
	addPlayer(playerId, playerName, teamName, playerRole, totalRuns, battingAverage, strikeRate, wickets, economyRate);
	printf("\nNew Player added\n");
}

void displayPlayersOfteam()
{
	printf("\nEnter Team ID\n");
	printTeams();
	int teamId = 0;
	
	if(scanf("%d", &teamId) != 1)
	{
		while (getchar() != '\n');
		printf("\nTeam Id should be numeric\n");
		return;
	}
	while(getchar()!='\n');
	
	if(teamId < MIN_ID || teamId > MAX_ID)
	{
		printf("\nTeam Id should be in between 1 and 1500\n");
		return;
	}
	
	struct teamNode* team = searchTeamById(root, teamId);
	if(team == NULL)
	{
		printf("\nTeam is not found\n");
		return;
	}
	
	printf("\nPlayers of Team %s:\n",team -> teamName);
	
	printf("\n============================================================================================================\n\n");
	printf("ID Name Role Runs Avg SR Wkts ER Perf.Index\n\n");
	printf("\n============================================================================================================\n\n");
	
	struct playerNode *temp = team -> batters;
	
	while (temp != NULL)
	{
		printf("\n%d %s %s %d %0.2f %0.2f %d %0.2f %0.2f\n", temp -> playerId, temp -> playerName, temp -> playerRole, temp -> totalRuns, temp -> battingAverage, temp -> strikeRate, temp -> wickets, temp -> economyRate, temp -> performanceIndex);
		temp = temp -> next;
	}
	
	temp = team -> bowlers;
	
	while (temp != NULL)
	{
		printf("\n%d %s %s %d %0.2f %0.2f %d %0.2f %0.2f\n", temp -> playerId, temp -> playerName, temp -> playerRole, temp -> totalRuns, temp -> battingAverage, temp -> strikeRate, temp -> wickets, temp -> economyRate, temp -> performanceIndex);
		temp = temp -> next;
	}
	
	temp = team -> allRounders;
	
	while (temp != NULL)
	{
		printf("\n%d %s %s %d %0.2f %0.2f %d %0.2f %0.2f\n", temp -> playerId, temp -> playerName, temp -> playerRole, temp -> totalRuns, temp -> battingAverage, temp -> strikeRate, temp -> wickets, temp -> economyRate, temp -> performanceIndex);
		temp = temp -> next;
	}
	
	printf("\nTotal Players: %d\n",team -> totalPlayers);	
	printf("\nAverage Batting Strike Rate: %0.2f\n", team -> averageBattingStrikeRate);	
}

int compareAverageBattingStrikeRate(const void* a, const void* b)
{
	const struct teamNode* teamA = *(const struct teamNode**)a;
	const struct teamNode* teamB = *(const struct teamNode**)b;
	
	if(teamA -> averageBattingStrikeRate < teamB -> averageBattingStrikeRate)
	{
		return 1;
	}
	else if(teamA -> averageBattingStrikeRate > teamB -> averageBattingStrikeRate)
	{
		return -1;
	}
	
	return 0;
}

void storeTeamArray(struct teamNode* root, struct teamNode** arrayTeam, int* index)
{
	if(root == NULL)
	{
		return;
	}
	
	storeTeamArray(root -> left, arrayTeam, index);
	arrayTeam[(*index)++] = root;
	storeTeamArray(root -> right, arrayTeam, index);	
}

void printTeamDescendingOrder(struct teamNode* root)
{
	if(root == NULL)
	{
		return;
	}
	
	struct teamNode* teamArray[teamCount];
	int index = 0;
	
	storeTeamArray(root, teamArray, &index);
	qsort(teamArray, index, sizeof(struct teamNode*), compareAverageBattingStrikeRate);
	
	index = 0;
	printf("\nTeams Sorted by Average Batting Strike Rate\n\n");
	
	printf("\n========================================================================\n\n");
	printf("ID   Team Name   AvgBatSR   Total Players\n\n");
	printf("\n========================================================================\n\n");
	
	
	for(index = 0; index < teamCount; index++)
	{
		printf("%d  %s %0.2f  %d\n", (index + 1), teamArray[index] -> teamName, teamArray[index] -> averageBattingStrikeRate, teamArray[index] -> totalPlayers);
	}
	printf("\n========================================================================\n\n");
}

void displayTopKPlayers()
{
	printf("\nEnter Team ID\n");
	printTeams();
	int teamId = 0;
	
	if(scanf("%d", &teamId) != 1)
	{
		while (getchar() != '\n');
		printf("\nTeam Id should be numeric\n");
		return;
	}
	while(getchar()!='\n');
	
	if(teamId < MIN_ID || teamId > MAX_ID)
	{
		printf("\nTeam Id should be in between 1 and 1500");
		return;
	}
	
	struct teamNode* team = searchTeamById(root, teamId);
	
	int playerRoleNumber = 0;
	struct playerNode *temp;
	
	printf("\nRole (1-Batsman, 2-Bowler, 3-All-rounder): ");
	if(scanf("%d", &playerRoleNumber) != 1)
	{
		while (getchar() != '\n');
		printf("\nRole should be numeric\n");
		return;
	}
	while(getchar()!='\n');
	
	if(playerRoleNumber < 1 || playerRoleNumber > 3)
	{
		printf("\nRole should be in the range of 1 to 3\n");
		return;
	}
	
	if(playerRoleNumber == BATSMAN)
	{
		temp = team -> batters;
	}
	else if(playerRoleNumber == BOWLER)
	{
		temp = team -> bowlers;
	}
	else
	{
		temp = team -> allRounders;
	}
	
	int numberOfPlayers = 0;
	printf("\nEnter number of players: ");
	
	if(scanf("%d", &numberOfPlayers) != 1)
	{
		while (getchar() != '\n');
		printf("\nNumber of Players should be numeric\n");
		return;
	}
	
	printf("\nTop %d %s of Team %s:\n\n", numberOfPlayers, findRole(playerRoleNumber), team -> teamName);
	
	printf("\n=========================================================================================================\n\n");
	printf("ID Name Role Runs Avg SR Wkts ER Perf.Index\n\n");
	printf("\n=========================================================================================================\n\n");
	
	while (temp != NULL && numberOfPlayers > 0)
	{
		printf("\n%d %s %s %d %0.2f %0.2f %d %0.2f %0.2f\n", temp -> playerId, temp -> playerName, temp -> playerRole, temp -> totalRuns, temp -> battingAverage, temp -> strikeRate, temp -> wickets, temp -> economyRate, temp -> performanceIndex);
		temp = temp -> next;
		numberOfPlayers--;
	}
}

void collectPlayersSpecificRole(struct teamNode* root, struct playerNode** playerArray, int playerRoleNumber, int* index)
{
	if(root == NULL)
	{
		return;
	}
	collectPlayersSpecificRole(root -> left, playerArray, playerRoleNumber, index);
	
	struct playerNode* head = NULL;
	
	if(playerRoleNumber == BATSMAN)
	{
		head = root -> batters;
	}
	else if(playerRoleNumber == BOWLER)
	{
		head = root -> bowlers;
	}
	else
	{
		head = root -> allRounders;
	}
	
	while(head != NULL)
	{
		playerArray[(*index)++] = head;
		head = head -> next;
	}
	
	collectPlayersSpecificRole(root -> right, playerArray, playerRoleNumber, index);
}

int comparePerformanceIndex(const void* a, const void* b)
{
	const struct playerNode* playerA = *(const struct playerNode**)a;
	const struct playerNode* playerB = *(const struct playerNode**)b;
	
	if(playerA->performanceIndex < playerB -> performanceIndex)
	{
		return 1;
	}
	else if(playerA -> performanceIndex > playerB -> performanceIndex)
	{
		return -1;
	}
	
	return 0;
}

void displayPlayersSpecificRole()
{
	int playerRoleNumber = 0;
	
	printf("\nRole (1-Batsman, 2-Bowler, 3-All-rounder): ");
	if(scanf("%d", &playerRoleNumber) != 1)
	{
		while (getchar() != '\n');
		printf("\nRole should be numeric\n");
		return;
	}
	while(getchar()!='\n');
	
	if(playerRoleNumber < 1 || playerRoleNumber > 3)
	{
		printf("\nRole should be in the range of 1 to 3\n");
		return;
	}
	
	int countNumberOfplayers = 0;
	struct playerNode* playerArray[MAX_PLAYER_OF_ALL_TEAM_SPECIFIC_ROLE];
	
	collectPlayersSpecificRole(root,playerArray,playerRoleNumber,&countNumberOfplayers);
	qsort(playerArray, countNumberOfplayers, sizeof(struct playerNode*), comparePerformanceIndex);

	
	printf("\n%s of all teams: \n", findRole(playerRoleNumber));
	
	printf("\n=========================================================================================================\n\n");
	printf("ID Name Team Role Runs Avg SR Wkts ER Perf.Index\n");
	printf("\n=========================================================================================================\n\n");
	int index = 0;
	
	for(index = 0; index < countNumberOfplayers; index++)
	{
		printf("\n%d %s %s %s %d %0.2f %0.2f %d %0.2f %0.2f\n",playerArray[index]->playerId, playerArray[index]->playerName,playerArray[index]->teamName, playerArray[index]->playerRole, playerArray[index]->totalRuns, playerArray[index]->battingAverage, playerArray[index]->strikeRate, playerArray[index]->wickets, playerArray[index]->economyRate, playerArray[index]->performanceIndex);
	}
	printf("\n========================================================================\n\n");
}

void freePlayers(struct playerNode* head)
{
    struct playerNode* temp;
    while (head != NULL)
    {
        temp = head;
        head = head -> next;
        free(temp);
    }
}

void freeTeams(struct teamNode* root)
{
    if (root == NULL)
    {
    	return;
	}

    freeTeams(root -> left);
    freeTeams(root -> right);

    freePlayers(root -> batters);
    freePlayers(root -> bowlers);
    freePlayers(root -> allRounders);

    free(root);
}

int main()
{
	initializeTeams();
	initializePlayers();
	
	int choice = 0;

	while (1)
	{
		printf("\n========================================================================\n\n");
		printf("ICC ODI Player Performance Analyzer\n");
		printf("\n========================================================================\n\n");
		
		printf("\n 1. Add Player to Team \n");
		printf("\n 2. Display Players of a Specific Team \n");
		printf("\n 3. Display Teams by Average Batting Strike Rate \n");
		printf("\n 4. Display Top K Players of a Specific Team by Role \n");
		printf("\n 5. Display all Players of specific role Access All Teams by performance index \n");
		printf("\n 6. Exit \n");
		printf("\nEnter your choice: ");
		if (scanf("%d", &choice) != 1) 
		{
		    printf("\nInvalid input\n");
		    while(getchar() != '\n'); 
		    continue;
		}
		while(getchar()!='\n');  


		switch(choice)
		{
			case 1:
				addNewPlayer();
				break;
			
			case 2:
				displayPlayersOfteam();
				break;
				
			case 3:
				printTeamDescendingOrder(root);
				break;
				
			case 4:
				displayTopKPlayers();
				break;
				
			case 5:
				displayPlayersSpecificRole();
				break;
				
			case 6:
				freeTeams(root);
				printf("Memory is free\n");
				return 0;
			
			default:
				printf("Invalid Choice! Please Try Again\n");				
		}
	}
	return 0;
}
