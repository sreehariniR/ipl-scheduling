#define MAX_TEAMS 10
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

// Structure for team details
typedef struct {
    char name[50];
    char homeground[50];
} Team;

// Structure for match details
// Structure for match details
typedef struct {
    char team1[50];
    char team2[50];
    char venue[50];
    int day;
    int month;
    char time[10];
    int team1_runs;   // Runs scored by team1
    int team2_runs;   // Runs scored by team2
    float team1_overs;  // Overs played by team1
    float team2_overs;  // Overs played by team2
} Match;


//Structure for points table
typedef struct {
    char team[50];
    int wins;
    int losses;
    int points;
    float netRunRate;
} PointsTable;
#define MAX_UNAVAILABLE_DATES 20

typedef struct {
    char venue[50];
    int unavailableDates[MAX_UNAVAILABLE_DATES][2];
    int unavailableCount;
} VenueAvailability;


Team teams[MAX_TEAMS] = {
    {"CSK", "M. A. Chidambaram Stadium"},
    {"MI", "Wankhede Stadium"},
    {"RCB", "M. Chinnaswamy Stadium"},
    {"KKR", "Eden Gardens"},
    {"SRH", "Rajiv Gandhi International "},
    {"DC", "Arun Jaitley Stadium"},
    {"RR", "Sawai Mansingh Stadium"},
    {"PBKS", "PCA IS Bindra Stadium"},
    {"LSG", "Atal Bihari Vajpayee Ekana"},
    {"GT", "Narendra Modi Stadium"}
};


int start_day, start_month, end_day, end_month,start_year;
int matchIndex = 0;
int lastPlayed[MAX_TEAMS];
Match matchDetails[100];
Team teams[MAX_TEAMS];
int teamCount = 0;
PointsTable pointsTable[MAX_TEAMS];
VenueAvailability venueAvailability[MAX_TEAMS];



// Function to get the number of days in a given month
int getDaysInMonth(int month, int year) {
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            return 29;
        }
    }

    return daysInMonth[month - 1];
}

// Function to take IPL start and end dates
void inputDates() {
    printf("Enter the IPL start date (day month year): ");
    scanf("%d %d %d", &start_day, &start_month,&start_year);

    printf("Enter the IPL end date (day month year): ");
    scanf("%d %d %d", &end_day, &end_month,&start_year);
}

void inputVenueAvailability() {
    for (int i = 0; i < MAX_TEAMS; i++) {
        printf("Enter the venue name: %s\n", teams[i].homeground);
        strcpy(venueAvailability[i].venue, teams[i].homeground);

        printf("Enter the number of unavailable dates for %s: ", teams[i].homeground);
        scanf("%d", &venueAvailability[i].unavailableCount);

        for (int j = 0; j < venueAvailability[i].unavailableCount; j++) {
            printf("Enter unavailable date %d (day month): ", j + 1);
            scanf("%d %d", &venueAvailability[i].unavailableDates[j][0], &venueAvailability[i].unavailableDates[j][1]);
        }
    }
}
int isDateUnavailable(char *venue, int day, int month) {
    for (int i = 0; i < MAX_TEAMS; i++) {
        if (strcmp(venueAvailability[i].venue, venue) == 0) {
            for (int j = 0; j < venueAvailability[i].unavailableCount; j++) {
                if (venueAvailability[i].unavailableDates[j][0] == day &&
                    venueAvailability[i].unavailableDates[j][1] == month) {
                    return 1;
                }
            }
        }
    }
    return 0;
}


// Function to check if a given date falls on a weekend
int isWeekend(int day, int month) {
    struct tm date = {0};
    date.tm_year = 2024 - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;

    mktime(&date);

    int weekDay = date.tm_wday;

    return (weekDay == 0 || weekDay == 6);
}




void incrementDate(int *day, int *month, int *matchCountToday) {

    if (*matchCountToday >= 2 || !isWeekend(*day, *month)) {
        *matchCountToday = 0;
        (*day)++;


        if (*day > getDaysInMonth(*month,start_year)) {
            *day = 1;
            (*month)++;
        }
    }
}
void scheduleMatch(int homeIndex, int awayIndex, int day, int month,int matchCount) {
    while (isDateUnavailable(teams[homeIndex].homeground, day, month)) {
        printf("Match at %s on %02d/%02d is unavailable. Rescheduling...\n", teams[homeIndex].homeground, day, month);
        incrementDate(&day, &month, &(int){0});
    }
    strcpy(matchDetails[matchIndex].team1, teams[homeIndex].name);
    strcpy(matchDetails[matchIndex].team2, teams[awayIndex].name);
    strcpy(matchDetails[matchIndex].venue, teams[homeIndex].homeground);
    matchDetails[matchIndex].day = day;
    matchDetails[matchIndex].month = month;
    strcpy(matchDetails[matchIndex].time, isWeekend(day, month) ? (matchCount % 2 == 0 ? "03:30 PM" : "07:30 PM") : "07:30 PM");
    matchIndex++;

}


void matchScheduling() {
    int current_day = start_day, current_month = start_month,current_year= start_year;
    int matchCountToday = 0;

    for (int i = 0; i < MAX_TEAMS; i++) {
        lastPlayed[i] = -2;
    }
    matchIndex = 0;

    // Phase 1: Single Round-Robin (45 matches)
    for (int round = 0; round < MAX_TEAMS - 1; round++) {
        for (int i = 0; i < MAX_TEAMS / 2; i++) {
            int team1, team2;

            if (i == 0) {
                team1 = round % (MAX_TEAMS - 1);
                team2 = MAX_TEAMS - 1;
            } else {
                team1 = (round + i) % (MAX_TEAMS - 1);
                team2 = (round + MAX_TEAMS - 1 - i) % (MAX_TEAMS - 1);
            }

            scheduleMatch(team1, team2, current_day, current_month,matchCountToday);
            matchCountToday++;

            if (isWeekend(current_day, current_month) && matchCountToday == 1) {
                continue;
            }

            incrementDate(&current_day, &current_month, &matchCountToday);
        }
    }

    // Phase 2: Extra 25 Matches
    int extraRounds = 5;
    int extraPairings[5][5][2] = {
        { {0,5}, {1,6}, {2,7}, {3,8}, {4,9} },
        { {0,6}, {1,7}, {2,8}, {3,9}, {4,5} },
        { {0,7}, {1,8}, {2,9}, {3,5}, {4,6} },
        { {0,8}, {1,9}, {2,5}, {3,6}, {4,7} },
        { {0,9}, {1,5}, {2,6}, {3,7}, {4,8} }
    };

    for (int round = 0; round < extraRounds; round++) {
        for (int i = 0; i < MAX_TEAMS / 2; i++) {
            int teamA = extraPairings[round][i][0];
            int teamB = extraPairings[round][i][1];

            if (round % 2 == 0) {
                scheduleMatch(teamA, teamB, current_day, current_month,matchCountToday);
            } else {
                scheduleMatch(teamB, teamA, current_day, current_month,matchCountToday);
            }

            matchCountToday++;

            if (isWeekend(current_day, current_month) && matchCountToday == 1) {
                continue;
            }

            incrementDate(&current_day, &current_month, &matchCountToday);
        }
    }
}


// Function to display the match schedule
void displaySchedule() {
    printf("\nIPL Match Schedule:\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("%-3s %-6s %-3s %-6s %-40s %-10s %-15s\n", "No", "Team1", "vs", "Team2", "Venue", "Date", "Time");
    printf("-------------------------------------------------------------------------------\n");
    for (int i = 0; i < matchIndex; i++) {
        printf("%-3d %-6s %-3s %-6s %-40s %02d/%02d %-15s\n",
               i + 1, matchDetails[i].team1, "vs", matchDetails[i].team2,
               matchDetails[i].venue, matchDetails[i].day,
               matchDetails[i].month, matchDetails[i].time);
    }
    printf("-------------------------------------------------------------------------------\n");
}


// Function to find or add a team in the points table
int findOrAddTeam(char *teamName) {
    for (int i = 0; i < teamCount; i++) {
        if (strcmp(pointsTable[i].team, teamName) == 0) {
            return i;
        }
    }

    strcpy(pointsTable[teamCount].team, teamName);
    pointsTable[teamCount].wins = 0;
    pointsTable[teamCount].losses = 0;
    pointsTable[teamCount].points = 0;
    pointsTable[teamCount].netRunRate = 0.0;
    return teamCount++;
}

// Function to update Net Run Rate in the points table
void updateNetRunRate() {
    // Loop through each match to update NRR
    for (int i = 0; i < matchIndex; i++) {
        int team1Runs = 0, team2Runs = 0;
        float team1Overs = 0.0, team2Overs = 0.0;

        // Get the runs and overs for both teams in the current match
        for (int j = 0; j < matchIndex; j++) {
            if (strcmp(matchDetails[i].team1, pointsTable[j].team) == 0) {
                team1Runs = matchDetails[i].team1_runs; // Runs scored by team1
                team1Overs = matchDetails[i].team1_overs; // Overs played by team1
            } else if (strcmp(matchDetails[i].team2, pointsTable[j].team) == 0) {
                team2Runs = matchDetails[i].team2_runs; // Runs scored by team2
                team2Overs = matchDetails[i].team2_overs; // Overs played by team2
            }
        }

        // Calculate the NRR for both teams
        if (team1Overs > 0) {
            float team1NRR = ((float)team1Runs / team1Overs) - ((float)team2Runs / team2Overs);
            int index1 = findOrAddTeam(matchDetails[i].team1);
            pointsTable[index1].netRunRate += team1NRR; // Update team1's NRR
        }

        if (team2Overs > 0) {
            float team2NRR = ((float)team2Runs / team2Overs) - ((float)team1Runs / team1Overs);
            int index2 = findOrAddTeam(matchDetails[i].team2);
            pointsTable[index2].netRunRate += team2NRR; // Update team2's NRR
        }
    }
}


// Function to update points table based on match results, including runs and overs
void getMatchResults() {
    int winningTeam = 0;
    int team1Runs, team2Runs;
    float team1Overs, team2Overs;

    printf("\n\nEnter match results:-\n");
    for (int i = 0; i < matchIndex; i++) {
        printf("Match %d: %s(1) vs %s(2) at %s on %02d/%02d\n",
               i + 1,
               matchDetails[i].team1,
               matchDetails[i].team2,
               matchDetails[i].venue,
               matchDetails[i].day,
               matchDetails[i].month);

        // Get the runs and overs for team 1
        printf("Enter runs scored by %s: ", matchDetails[i].team1);
        scanf("%d", &team1Runs);
        printf("Enter overs played by %s: ", matchDetails[i].team1);
        scanf("%f", &team1Overs);

        // Get the runs and overs for team 2
        printf("Enter runs scored by %s: ", matchDetails[i].team2);
        scanf("%d", &team2Runs);
        printf("Enter overs played by %s: ", matchDetails[i].team2);
        scanf("%f", &team2Overs);

        // Store the runs and overs for the teams in the matchDetails array
        matchDetails[i].team1_runs = team1Runs;
        matchDetails[i].team1_overs = team1Overs;
        matchDetails[i].team2_runs = team2Runs;
        matchDetails[i].team2_overs = team2Overs;

        // Get the winning team
        printf("Enter the winning team (1 or 2): ");
        scanf("%d", &winningTeam);

        if (winningTeam != 1 && winningTeam != 2) {
            printf("Invalid input! Skipping this match.\n");
            continue;
        }

        // Update the points table based on the winner
        int index1 = findOrAddTeam(matchDetails[i].team1);
        int index2 = findOrAddTeam(matchDetails[i].team2);

        if (winningTeam == 1) {
            pointsTable[index1].wins++;
            pointsTable[index1].points += 2; // 2 points for a win
            pointsTable[index2].losses++;
        } else {
            pointsTable[index2].wins++;
            pointsTable[index2].points += 2; // 2 points for a win
            pointsTable[index1].losses++;
        }
    }

    // After all match results have been entered, update the Net Run Rate
    updateNetRunRate();
}


//function to sort the points table
// Function to sort the points table
void sortPointsTable() {
    for (int i = 0; i < teamCount - 1; i++) {
        for (int j = i + 1; j < teamCount; j++) {
            // Sorting first by points, then by NRR if points are the same
            if (pointsTable[i].points < pointsTable[j].points ||
                (pointsTable[i].points == pointsTable[j].points && pointsTable[i].netRunRate < pointsTable[j].netRunRate)) {
                PointsTable temp = pointsTable[i];
                pointsTable[i] = pointsTable[j];
                pointsTable[j] = temp;
            }
        }
    }
}

// Function to display the points table
// Function to display the points table
void displayPointsTable() {
    sortPointsTable();  // Sort the points table first

    printf("\nPoints Table:\n");
    printf("-------------------------------------------------------------\n");
    printf("%-20s %-5s %-5s %-7s %-10s\n", "Team", "Wins", "Losses", "Points", "Net Run Rate");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < teamCount; i++) {
        printf("%-20s %-5d %-5d %-7d %-10.2f\n",
               pointsTable[i].team,
               pointsTable[i].wins,
               pointsTable[i].losses,
               pointsTable[i].points,
               pointsTable[i].netRunRate);
    }
}




// Function to display the playoffs
void displayPlayoffs() {
    printf("\nTop 4 Teams Qualifying for Playoffs:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, pointsTable[i].team);
    }
    char top4[4][50];
    strcpy(top4[0], pointsTable[0].team);
    strcpy(top4[1], pointsTable[1].team);
    strcpy(top4[2], pointsTable[2].team);
    strcpy(top4[3], pointsTable[3].team);

    char inputWinner[50];

    char q1_winner[50], q1_loser[50];
    printf("\nQualifier 1: %s vs %s\n", top4[0], top4[1]);
    printf("Enter winner of Qualifier 1: ");
    scanf("%s", inputWinner);
    strcpy(q1_winner, inputWinner);
    if (strcmp(q1_winner, top4[0]) == 0)
        strcpy(q1_loser, top4[1]);
    else
        strcpy(q1_loser, top4[0]);
    printf("   Winner: %s (directly qualifies for the Final)\n", q1_winner);
    printf("   Loser: %s (gets a second chance in Qualifier 2)\n\n", q1_loser);

    char elim_winner[50], elim_loser[50];
    printf("Eliminator: %s vs %s\n", top4[2], top4[3]);
    printf("Enter winner of the Eliminator: ");
    scanf("%s", inputWinner);
    strcpy(elim_winner, inputWinner);
    if (strcmp(elim_winner, top4[2]) == 0)
        strcpy(elim_loser, top4[3]);
    else
    strcpy(elim_loser, top4[2]);
    printf("   Winner: %s (advances to Qualifier 2)\n", elim_winner);
    printf("   Loser: %s (eliminated from the tournament)\n\n", elim_loser);

    char q2_winner[50], q2_loser[50];
    printf("Qualifier 2: %s vs %s\n", q1_loser, elim_winner);
    printf("Enter winner of Qualifier 2: ");
    scanf("%s", inputWinner);
    strcpy(q2_winner, inputWinner);
    if (strcmp(q2_winner, q1_loser) == 0)
        strcpy(q2_loser, elim_winner);
    else
        strcpy(q2_loser, q1_loser);
    printf("   Winner: %s (qualifies for the Final)\n", q2_winner);
    printf("   Loser: %s (eliminated from the tournament)\n\n", q2_loser);

    char final_winner[50];
    printf("Final: %s vs %s\n", q1_winner, q2_winner);
    printf("Enter champion of the Final: ");
    scanf("%s", inputWinner);
    strcpy(final_winner, inputWinner);
    printf("   Champion: %s\n", final_winner);
}



int main() {

    inputDates();
    inputVenueAvailability();
    matchScheduling();
    displaySchedule();
    getMatchResults();
    displayPointsTable();
    displayPlayoffs();

    return 0;
}