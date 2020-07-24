#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool DFS(int m, int n, bool visited[]);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(candidates[i], name))
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int index = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[index].winner = i;
                pairs[index].loser = j;
                index++;
            }
        }
    }
    pair_count = index;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int strength[pair_count];
    int temp = 0;
    pair temp_pairs;

    for (int i = 0; i < pair_count; i++)
    {
        // printf("%i > %i\n", pairs[i].winner, pairs[i].loser);
        strength[i] = (preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]);
        // printf("%i\n", strength[i]);

    }

    // Insertion Sort

    for (int i = 1; i < pair_count; i++)
    {
        int j = i - 1;
        temp = strength[i];
        temp_pairs = pairs[i];
        while (temp > strength[j])
        {
            if (j == -1)
            {
                break;
            }
            strength[j + 1] = strength[j];
            pairs[j + 1] = pairs[j];
            j--;
        }
        strength[j + 1] = temp;
        pairs[j + 1] = temp_pairs;
    }

    printf("\n");
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("%i > %i\n", pairs[i].winner, pairs[i].loser);
    //     printf("%i\n", strength[i]);
    // }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int m = pairs[i].winner;
        int n = pairs[i].loser;
        bool visited[candidate_count];
        for (int j = 0; j < candidate_count; j++)
        {
            visited[j] = 0;
        }
        int edge = DFS(m, n, visited);
        if (edge)
        {
            locked[m][n] = 1;
        }
    }
    return;
}

// Determine if adding an edge from m to n will create a cycle via DFS
bool DFS(int m, int n, bool visited[])
{
    visited[n] = 1;
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[n][i])
        {
            DFS(m, i, visited);
        }
    }
    return !visited[m];
}



// Print the winner of the election
void print_winner(void)
{
    int winner[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        winner[i] = 1;
    }

    for (int i = 0; i < candidate_count; i++)
    {

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                winner[j] = 0;
            }
        }
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (winner[i])
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}