#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include "Players_data.h"


typedef struct PlayerNode {
    int playerID;
    char *playerName;
    char *teamName;
    char *role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float perfoemanceIndex;
    struct PlayerNode *next;
} PlayerNode;

typedef struct Team {
    int teamID;
    char *teamName;
    int totalPlayers;
    float averageBattingStrikeRate;
    struct Team *next;
}Team;


bool checkPlayerIdExist(PlayerNode *listHead, int id)
{
    PlayerNode *checkPtr = listHead;
    while (checkPtr != NULL)
    {
        if(checkPtr->playerID == id) return false;
        checkPtr = checkPtr->next;
    }
    return true;
}


float calculatePerformanceIndex(PlayerNode *node)
{
    if (strcmp(node->role, "Batsman") == 0)
    {
        return (node->battingAverage * node->strikeRate) / 100;
    }
    if(strcmp(node->role, "Bowler") == 0)
    {
        return (node->wickets * 2) + (100 - node->economyRate);
    }
    if(strcmp(node->role, "All-rounder") == 0)
    {
        return ((node->battingAverage * node->strikeRate) / 100) + (node->wickets * 2);
    }
    return 0.00;
}

PlayerNode *createPlayersList()
{
    PlayerNode *head = NULL;
    PlayerNode *tail = NULL;
    for (int i = 0; i < playerCount; i++)
    {
        PlayerNode *newNode = (PlayerNode*)malloc(sizeof(PlayerNode));

        if(newNode == NULL)
        {
            printf("Memory allocation failed");
            return head;
        }

        newNode->playerID = (rand() % 1000) + 1;
        while (!checkPlayerIdExist(head, newNode->playerID))
        {
            newNode->playerID = rand() % 1000 + 1;
        }

        newNode->playerName = malloc(50 * sizeof(char));
        newNode->teamName = malloc(50 * sizeof(char));
        newNode->role = malloc(50 * sizeof(char));

        strcpy(newNode->playerName, players[i].name);
        strcpy(newNode->teamName, players[i].team);
        strcpy(newNode->role, players[i].role);

        newNode->totalRuns = players[i].totalRuns;
        newNode->battingAverage = players[i].battingAverage;
        newNode->strikeRate = players[i].strikeRate;
        newNode->wickets = players[i].wickets;
        newNode->economyRate = players[i].economyRate;

        newNode->perfoemanceIndex = calculatePerformanceIndex(newNode);
        newNode->next = NULL;

        if(head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

Team *createTeamList(PlayerNode *playerListHead)
{
    Team *head = NULL;
    Team *tail = NULL;
    for (int i = 0; i < teamCount; i++)
    {
        Team *newNode = (Team*)malloc(sizeof(Team));

        if(newNode == NULL)
        {
            printf("Memory allocation failed\n");
            return head;
        }

        newNode->teamID = i + 1;
        
        newNode->teamName = malloc(50 * sizeof(char));

        strcpy(newNode->teamName, teams[i]);

        newNode->totalPlayers = 0;
        newNode->averageBattingStrikeRate = 0.00;
        newNode->next = NULL;

        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }

    Team *teamPtr = head;

    while(teamPtr != NULL)
    {
        float sumOfStrikeRate = 0;
        int countOfBatsman = 0;
        PlayerNode *playerPtr = playerListHead;
        while(playerPtr != NULL)
        {
            if(strcmp(teamPtr->teamName, playerPtr->teamName) == 0)
            {
                if(strcmp(playerPtr->role, "Batsman") == 0 || strcmp(playerPtr->role, "All-rounder") == 0)
                {
                    sumOfStrikeRate += playerPtr->strikeRate;
                    countOfBatsman++;
                }
                teamPtr->totalPlayers++;
            }
            playerPtr = playerPtr->next;
        }
        if(countOfBatsman == 0)
        {
            teamPtr->averageBattingStrikeRate = 0;
        }
        else{
        teamPtr->averageBattingStrikeRate = sumOfStrikeRate / countOfBatsman;
        }

        teamPtr = teamPtr->next;
    }

    return head;

}

void displayPlayersOfTeam(PlayerNode *playerListHead, Team *teamListHead)
{
    int teamID;
    printf("Enter Team ID: ");
    if(scanf("%d", &teamID) != 1)
    {
        printf("Invalid input, ID must be an integer");
        while(getchar()!='\n');
        return;
    }

    Team *teamPtr = teamListHead;
    while (teamPtr != NULL)
    {
        if(teamPtr->teamID == teamID)
            break;
        teamPtr = teamPtr->next;
    }

    if(teamPtr == NULL)
    {
        printf("Team ID not found\n");
        return;
    }
    
    printf("Players of Team %s:\n", teamPtr->teamName);
    printf("====================================================================================\n");
    printf("ID\tName\t\tRole\t\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n");
    printf("====================================================================================\n");
    PlayerNode *playerPtr = playerListHead;
    while(playerPtr != NULL)
    {
        if(strcmp(teamPtr->teamName, playerPtr->teamName) == 0)
        {
            printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n", playerPtr->playerID, playerPtr->playerName, playerPtr->role,
        playerPtr->totalRuns, playerPtr->battingAverage, playerPtr->strikeRate, playerPtr->wickets, playerPtr->economyRate, playerPtr->perfoemanceIndex);
        }
        playerPtr = playerPtr->next;
    }
    printf("====================================================================================\n");
    printf("Total Players: %d\n", teamPtr->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", teamPtr->averageBattingStrikeRate);
    
}

void displayTeamsByAvgSR(Team *teamListHead)
{
    Team *teamArr[teamCount];
    Team *tempPtr = teamListHead;
    int index = 0;

    while (tempPtr != NULL)
    {
        teamArr[index] = tempPtr;
        index++;
        tempPtr = tempPtr->next;
    }

    for (int i = 0; i < teamCount - 1; i++)
    {
        for (int j = i + 1; j < teamCount; j++)
        {
            if (teamArr[i]->averageBattingStrikeRate < teamArr[j]->averageBattingStrikeRate)
            {
                Team *swap = teamArr[i];
                teamArr[i] = teamArr[j];
                teamArr[j] = swap;
            }
        }
    }

    printf("Teams Sorted by Average Batting Strike Rate\n");
    printf("=============================================================\n");
    printf("ID\tTeam Name\t\tAvg Bat SR\tTotal Players\n");
    printf("=============================================================\n");

    for (int i = 0; i < teamCount; i++)
    {
        printf("%d\t%s\t%.2f\t\t%d\n", teamArr[i]->teamID, teamArr[i]->teamName, teamArr[i]->averageBattingStrikeRate, teamArr[i]->totalPlayers);
    }
    printf("=============================================================\n");
}

void addPlayerToTeam(PlayerNode **playerListHead, Team *teamListHead)
{
    int teamID;
    printf("Enter Team ID to add player: ");
    if (scanf("%d", &teamID) != 1)
    {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    Team *teamPtr = teamListHead;
    while (teamPtr != NULL)
    {
        if(teamPtr->teamID == teamID) break;
        teamPtr = teamPtr->next;
    } 

    if (teamPtr == NULL)
    {
        printf("Team not found!\n");
        return;
    }

    PlayerNode *newNode = (PlayerNode*)malloc(sizeof(PlayerNode));

    newNode->playerID = (rand() % 1000) + 1;
    while (!checkPlayerIdExist(*playerListHead, newNode->playerID))
    {
        newNode->playerID = (rand() % 1000) + 1;
    }

    newNode->playerName = malloc(50 * sizeof(char));
    newNode->teamName = malloc(50 * sizeof(char));
    newNode->role = malloc(50 * sizeof(char));

    printf("Enter Player Name: ");
    scanf("%49s", newNode->playerName);  

    strcpy(newNode->teamName, teamPtr->teamName);

    int roleChoice;
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleChoice);

    if (roleChoice == 1) strcpy(newNode->role, "Batsman");
    else if (roleChoice == 2) strcpy(newNode->role, "Bowler");
    else strcpy(newNode->role, "All-rounder");

    printf("Total Runs: ");
    scanf("%d", &newNode->totalRuns);

    printf("Batting Average: ");
    scanf("%f", &newNode->battingAverage);

    printf("Strike Rate: ");
    scanf("%f", &newNode->strikeRate);

    printf("Wickets: ");
    scanf("%d", &newNode->wickets);

    printf("Economy Rate: ");
    scanf("%f", &newNode->economyRate);

    newNode->perfoemanceIndex = calculatePerformanceIndex(newNode);

    newNode->next = NULL;

    if (*playerListHead == NULL)
    {
        *playerListHead = newNode;
    }
    else
    {
        PlayerNode *ptr = *playerListHead;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }   
        ptr->next = newNode;
    }

    teamPtr->totalPlayers++;
    
    if(strcmp(newNode->role, "Batsman") == 0 || strcmp(newNode->role, "All-rounder") == 0)
    {
        float sumOfStrikeRate = teamPtr->averageBattingStrikeRate * (teamPtr->totalPlayers - 1);
        sumOfStrikeRate += newNode->strikeRate;

        int countOfBatsmanPlusAllRouder = 0;
        PlayerNode *player = *playerListHead;
        while (player != NULL)
        {
            if (strcmp(player->teamName, teamPtr->teamName) == 0)
            {
                if (strcmp(player->role, "Batsman") == 0 || strcmp(player->role, "All-rounder") == 0)
                {
                    countOfBatsmanPlusAllRouder++;
                }   
            }
            player = player->next;
        }
        teamPtr->averageBattingStrikeRate = sumOfStrikeRate / countOfBatsmanPlusAllRouder;

    }
    printf("Player added successfully to Team %s!\n", teamPtr->teamName);
}


int main()
{
    srand(time(NULL));
    PlayerNode *playerListHead = createPlayersList();
    Team *teamListHead = createTeamList(playerListHead);
    
    int choice;

    do
    {
        printf("\n==============================================================================\n");
        printf("                   ICC ODI Player Performance Analyzer\n");
        printf("==============================================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display All Players of Specific Role Across All Teams\n");
        printf("6. Exit\n");
        printf("==============================================================================\n");

        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Enter a number between 1–6.\n");
            while (getchar() != '\n');  // clear buffer
            continue;
        }

        switch (choice)
        {
            case 1:
                addPlayerToTeam(&playerListHead, teamListHead);
                break;

            case 2:
                displayPlayersOfTeam(playerListHead, teamListHead);
                break;

            case 3:
                displayTeamsByAvgSR(teamListHead);
                break;

            case 4:
                break;

            case 5:
                break;

            case 6:
                printf("Exiting\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 6);
    
    return 0;
}