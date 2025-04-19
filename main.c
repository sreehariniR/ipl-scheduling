#include <stdio.h>
#include <string.h>

/*


void ipl_teams(){
  printf("Enter the number of IPL teams: ");
  scanf("%d",&n);
  getchar();
  for (int i=0;i<n;i++){
    printf("Enter the name of team %d : ",i+1);
    fgets(teams[i], sizeof(teams[i]), stdin);
    teams[i][strcspn(teams[i], "\n")] = '\0';
    printf("Enter the homeground of the team %d : ",i+1);
    fgets(homeground[i], sizeof(homeground[i]), stdin);
    homeground[i][strcspn(homeground[i], "\n")] = '\0';
  }
}

}*/

int no_of_teams = 10;
char team_names[10][10] = {"CSK","MI","RCB","KKR","SRH","DC","RR","PBKS","GT","LSG"};
void chooseRandomTeams(team_names)
{

    srand(time(NULL));

    int team1Index = rand() % no_of_teams;
    int team2Index;

    do
    {
        team2Index = rand() % no_of_teams;
    } while (team2Index == team1Index);

    printf("Chosen teams: %s vs %s\n", teams[team1Index], teams[team2Index]);
}
struct Team
{
    char name[40];
    char homeground[50];
};
struct Stadium
{
    char venue_name[40];
    char unavailable_dates[11][30];
};

int main()
{
    struct Team teams[] = {
        {"CSK", "M. A. Chidambaram Stadium"},
        {"MI", "Wankhede Stadium"},
        {"RCB", "M. Chinnaswamy Stadium"},
        {"KKR", "Eden Gardens"},
        {"SRH", "Rajiv Gandhi International Stadium"},
        {"DC", "Arun Jaitley Stadium"},
        {"RR", "Sawai Mansingh Stadium"},
        {"PBKS", "Punjab Cricket Association Stadium"},
        {"GT", "Narendra Modi Stadium"},
        {"LSG", "Ekana Cricket Stadium"}};
    struct Stadium venue = {

        {"M. A. Chidambaram Stadium", {"25/02/2024", "02/03/2024"}},
        {"Wankhede Stadium", {"10/03/2024", "15/03/2024"}},
        {"M. Chinnaswamy Stadium", {"05/04/2024", "20/04/2024"}},
        {"Eden Gardens", {"25/02/2024", "10/03/2024"}},
        {"Rajiv Gandhi International Stadium", {"12/03/2024", "18/04/2024"}},
        {"Arun Jaitley Stadium", {"11/03/2024", "22/03/2024"}},
        {"Sawai Mansingh Stadium", {"14/03/2024", "30/03/2024"}},
        {"Punjab Cricket Association Stadium", {"07/03/2024", "17/03/2024"}},
        {"Narendra Modi Stadium", {"28/03/2024", "03/04/2024"}},
        {"Ekana Cricket Stadium", {"09/04/2024", "15/04/2024"}}};
}
