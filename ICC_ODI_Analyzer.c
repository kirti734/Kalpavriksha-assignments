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

float calculatePerformanceIndex(struct playerNode* newPlayer)
{
	if(strcmp(newPlayer -> playerRole, "Batsman") == 0)
	{
		return ((newPlayer -> battingAverage * newPlayer -> strikeRate) / 100.00);
	}
	else if(strcmp(newPlayer -> playerRole, "Bowlers") == 0)
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

void addNewPlayer()
{
	printf("\nEnter Team ID to add player: ");
	int teamId = 0;
	
	if(scanf("%d", &teamId) != 1)
	{
		printf("\nTeam Id should be numeric\n");
		return;
	}
	
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
		printf("\nPlayer Id should be numeric\n");
		return;
	}
	
	if(playerId < MIN_ID || playerId > MAX_ID)
	{
		printf("\nPlayer Id should be in between 1 and 1500\n");
		return;
	}
	
	while(getchar() != '\n');
	
	
	char playerName[NAME_LENGTH];
	printf("\nName: ");
	
	fgets(playerName, NAME_LENGTH, stdin);
	playerName[strcspn(playerName, "\n")] = '\0';
	
		
	char playerRole[ROLE_NAME_LENGTH];
	int playerRoleNumber = 0;
	printf("\nRole (1-Batsman, 2-Bowler, 3-All-rounder): ");
	
	if(scanf("%d", &playerRoleNumber) != 1)
	{
		printf("\nRole should be numeric\n");
		return;
	}
	
	if(playerRoleNumber == 1)
	{
		strcpy(playerRole, "Batsman");
	}
	else if(playerRoleNumber == 2)
	{
		strcpy(playerRole, "Bowler");
	}
	else
	{
		strcpy(playerRole, "All-rounder");
	}
	
	int totalRuns = 0;
	printf("\nTotal Runs: ");
	if(scanf("%d", &totalRuns) != 1)
	{
		printf("\nTotal Runs should be numeric\n");
		return;
	}
	
	float battingAverage = 0.00;
	printf("\nBatting Average: ");
	
	if(scanf("%f", &battingAverage) != 1)
	{
		printf("\nBatting Average should be numeric\n");
		return;
	}
	
	
	float strikeRate = 0.00;
	printf("\nStrike Rate: ");
	
	if(scanf("%f", &strikeRate) != 1)
	{
		printf("\nStrike Rate should be numeric\n");
		return;
	}
	
	int wickets = 0;
	printf("\nWickets: ");
	
	if(scanf("%d", &wickets) != 1)
	{
		printf("\nWickets should be numeric\n");
		return;
	}
	
	float economyRate = 0.00;
	printf("\nEconomy Rate: ");
	
	if(scanf("%f", &economyRate) != 1)
	{
		printf("\nEconomy Rate should be numeric\n");
		return;
	}
	
	addPlayer(playerId, playerName, teamName, playerRole, totalRuns, battingAverage, strikeRate, wickets, economyRate);
}

void displayPlayersOfteam()
{
	printf("\nEnter Team ID: ");
	int teamId = 0;
	
	if(scanf("%d", &teamId) != 1)
	{
		printf("\nTeam Id should be numeric\n");
		return;
	}
	
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
		printf("%d  %s           %0.2f  %d\n", (index + 1), teamArray[index] -> teamName, teamArray[index] -> averageBattingStrikeRate, teamArray[index] -> totalPlayers);
	}
	printf("\n========================================================================\n\n");
}

void displayTopKPlayers()
{
	printf("\nEnter Team ID: ");
	int teamId = 0;
	
	if(scanf("%d", &teamId) != 1)
	{
		printf("\nTeam Id should be numeric\n");
		return;
	}
	
	if(teamId < MIN_ID || teamId > MAX_ID)
	{
		printf("\nTeam Id should be in between 1 and 1500");
		return;
	}
	
	struct teamNode* team = searchTeamById(root, teamId);
	
	char playerRole[ROLE_NAME_LENGTH];
	int playerRoleNumber = 0;
	struct playerNode *temp;
	
	printf("\nRole (1-Batsman, 2-Bowler, 3-All-rounder): ");
	if(scanf("%d", &playerRoleNumber) != 1)
	{
		printf("\nRole should be numeric\n");
		return;
	}

	if(playerRoleNumber == 1)
	{
		strcpy(playerRole, "Batsman");
		temp = team -> batters;
	}
	else if(playerRoleNumber == 2)
	{
		strcpy(playerRole, "Bowler");
		temp = team -> bowlers;
	}
	else
	{
		strcpy(playerRole, "All-rounder");
		temp = team -> allRounders;
	}
	
	int numberOfPlayers = 0;
	printf("\nEnter number of players: ");
	
	if(scanf("%d", &numberOfPlayers) != 1)
	{
		printf("\nNumber of Players should be numeric\n");
		return;
	}
	
	printf("\nTop %d %s of Team %s:\n\n", numberOfPlayers, playerRole, team -> teamName);
	
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

void displayPlayers(struct teamNode* root, const char* playerRole)
{
	if(root == NULL)
	{
		return;
	}
	
	displayPlayers(root -> left, playerRole);
	
	struct playerNode *temp = NULL;
	
	if (strcmp(playerRole, "Batsman") == 0)
	{
		temp = root -> batters;
	}
	else if(strcmp(playerRole, "Bowler") == 0)
	{
		temp = root -> bowlers;
	}
	else
	{
		temp = root -> allRounders;
	}
	
	struct playerNode* playerHead = temp;
	
	while (playerHead != NULL)
	{
		printf("\n%d %s %s %d %0.2f %0.2f %d %0.2f %0.2f\n", playerHead -> playerId, playerHead -> playerName, playerHead -> playerRole, playerHead -> totalRuns, playerHead -> battingAverage, playerHead -> strikeRate, playerHead -> wickets, playerHead -> economyRate, playerHead -> performanceIndex);
		playerHead = playerHead -> next;
	}
	
	displayPlayers(root -> right, playerRole);	
}

void displayPlayersSpecificRole()
{
	char playerRole[ROLE_NAME_LENGTH];
	int playerRoleNumber = 0;
	
	printf("\nRole (1-Batsman, 2-Bowler, 3-All-rounder): ");
	if(scanf("%d", &playerRoleNumber) != 1)
	{
		printf("\nRole should be numeric\n");
		return;
	}

	if(playerRoleNumber == 1)
	{
		strcpy(playerRole, "Batsman");
	}
	else if(playerRoleNumber == 2)
	{
		strcpy(playerRole, "Bowler");
	}
	else
	{
		strcpy(playerRole, "All-rounder");
	}
	
	printf("\n%s of all teams: \n", playerRole);
	
	printf("\n=========================================================================================================\n\n");
	printf("ID Name Role Runs Avg SR Wkts ER Perf.Index\n\n");
	printf("\n=========================================================================================================\n\n");
	displayPlayers(root ,playerRole);
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

void freeTeams()
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
		printf("ICC ODI Player Performance Analyzer\n\n");
		printf("\n========================================================================\n\n");
		
		printf("\n 1. Add Player to Team \n");
		printf("\n 2. Display Players of a Specific Team \n");
		printf("\n 3. Display Teams by Average Batting Strike Rate \n");
		printf("\n 4. Display Top K Players of a Specific Team by Role \n");
		printf("\n 5. Display all Players of specific role Access All Teams by performance index \n");
		printf("\n 6. Exit \n");
		printf("\nEnter your choice: ");
		scanf("%d", &choice);
		
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
				freeTeams();
				return 0;
				
			default:
				printf("\nInvalid Choice! Try Again\n");
		}
	}
	return 0;
}
