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
typedef struct {
    char team1[50];
    char team2[50];
    char venue[50];
    int day;
    int month;
    char time[10];
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
int lastMatchDay[MAX_TEAMS]={0};
int lastMatchMonth[MAX_TEAMS]={0};
int lastMatchYear[MAX_TEAMS]={0};
int lastplayedmonth;

// Function to calculate date difference manually
int dateDifference(int d1, int m1, int y1, int d2, int m2, int y2) {
    // Convert both dates to days
    int days1 = y1 * 365 + m1 * getDaysInMonth(m1,y1) + d1;
    int days2 = y2 * 365 + m2 * getDaysInMonth(m2,y2) + d2;
    return days2 - days1;
}

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

int needsRest(int team, int day, int month, int year) {
    if (lastMatchYear[team] == 0) return 0; 
    int daysSinceLastMatch = dateDifference(lastMatchDay[team], lastMatchMonth[team], lastMatchYear[team], day, month, year);
    if(daysSinceLastMatch < 2) return 1; 
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

void scheduleMatch(int homeIndex, int awayIndex, int day, int month, int matchCount) {
    // Ensure venue is available
    while (isDateUnavailable(teams[homeIndex].homeground, day, month)) {
        printf("Match at %s on %02d/%02d is unavailable. Rescheduling...\n", teams[homeIndex].homeground, day, month);
        incrementDate(&day, &month, &(int){0});
    }

    // Ensure the away team gets rest
    while (needsRest(awayIndex, day, month)==1) {
        printf("Team %s needs rest. Rescheduling match...\n", teams[awayIndex].name);
        incrementDate(&day, &month, &(int){0});
    }

    // Store match details
    strcpy(matchDetails[matchIndex].team1, teams[homeIndex].name);
    strcpy(matchDetails[matchIndex].team2, teams[awayIndex].name);
    strcpy(matchDetails[matchIndex].venue, teams[homeIndex].homeground);
    matchDetails[matchIndex].day = day;
    matchDetails[matchIndex].month = month;
    strcpy(matchDetails[matchIndex].time, isWeekend(day, month) ? (matchCount % 2 == 0 ? "03:30 PM" : "07:30 PM") : "07:30 PM");
    matchIndex++;

    // Update last played details
    lastPlayed[homeIndex] = matchIndex;
    lastPlayed[awayIndex] = matchIndex;
    lastPlayedDay[homeIndex] = day;
    lastPlayedDay[awayIndex] = day;
    lastPlayedMonth[homeIndex] = month;
    lastPlayedMonth[awayIndex] = month;
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

// Function to update points table based on match results
// Function to update points table based on match results and calculate NRR
void getMatchResults() {
    int winningTeam = 0;
    int runDifference = 0;

    printf("\n\nEnter match results:-\n");
    for (int i = 0; i < matchIndex; i++) {
        printf("Match %d: %s(1) vs %s(2) at %s on %02d/%02d\n",
               i + 1,
               matchDetails[i].team1,
               matchDetails[i].team2,
               matchDetails[i].venue,
               matchDetails[i].day,
               matchDetails[i].month);

        printf("Enter the winning team (1 or 2): ");
        scanf("%d", &winningTeam);

        if (winningTeam != 1 && winningTeam != 2) {
            printf("Invalid input! Skipping this match.\n");
            continue;
        }

        printf("Enter the difference in runs between winner and loser: ");
        scanf("%d", &runDifference);

        int index1 = findOrAddTeam(matchDetails[i].team1);
        int index2 = findOrAddTeam(matchDetails[i].team2);

        if (winningTeam == 1) {
            pointsTable[index1].wins++;
            pointsTable[index1].points += 2;
            pointsTable[index1].netRunRate += (float)runDifference/20.0;
            pointsTable[index2].losses++;
            pointsTable[index2].netRunRate -= (float)runDifference/20.0;
        } else {
            pointsTable[index2].wins++;
            pointsTable[index2].points += 2;
            pointsTable[index2].netRunRate += (float)runDifference/20.0;
            pointsTable[index1].losses++;
            pointsTable[index1].netRunRate -= (float)runDifference/20.0;
        }
    }
}



//function to sort the points table
void sortPointsTable() {
    for (int i = 0; i < teamCount - 1; i++) {
        for (int j = i + 1; j < teamCount; j++) {
            // Sort by points first
            if (pointsTable[i].points < pointsTable[j].points ||
                (pointsTable[i].points == pointsTable[j].points &&
                 pointsTable[i].netRunRate < pointsTable[j].netRunRate)) {
                // Swap teams
                PointsTable temp = pointsTable[i];
                pointsTable[i] = pointsTable[j];
                pointsTable[j] = temp;
            }
        }
    }
}

// Function to display the points table with Net Run Rate
void displayPointsTable() {
    sortPointsTable();  // Automatically sort by Points + NRR
    printf("\n🏆 Points Table:\n");
    printf("----------------------------------------------------------------------------\n");
    printf("%-20s %-5s %-5s %-7s %-10s\n", "Team", "Wins", "Losses", "Points", "NRR");
    printf("----------------------------------------------------------------------------\n");

    for (int i = 0; i < teamCount; i++) {
        printf("%-20s %-5d %-5d %-7d %-10.2f\n",
               pointsTable[i].team,
               pointsTable[i].wins,
               pointsTable[i].losses,
               pointsTable[i].points,
               pointsTable[i].netRunRate);  // Printing Net Run Rate
    }
    printf("----------------------------------------------------------------------------\n");
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