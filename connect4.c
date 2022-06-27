#include <stdio.h>
#include <stdlib.h>

// #define's

#define PLAYER 1
#define AI 2
#define EMPTY 0
#define DRAW 3
#define INPROGRESS 4
#define WINNUM 4

#define NROWS 6
#define NCOLS 7

#define SCALE "\n1 2 3 4 5 6 7\n"


// Typedefs
typedef int board[NCOLS][NROWS];

// Function Prototypes


int gameLoop();
void printBoard(board);
void initialiseBoard(board);
int scoreBoard(board, int, int);
int getPlayerTurn(board);
int getAiTurn(board);
void updateBoard(board, int, int );





int main()
{
    int outcome = gameLoop();
    if (outcome == PLAYER) printf("Congratulations, you win!");
    if (outcome == AI) printf("Sorry, you lost this time :(");
    if (outcome == DRAW) printf("Well done, it's hard to draw (really)!");
    return 0;
}



int gameLoop()
{
    int status = INPROGRESS;
    board b;
    initialiseBoard(b);
    printBoard(b);
    while (status == INPROGRESS)
    {
        // Get player turn
        int playerTurn;
        playerTurn = getPlayerTurn(b);
        // Player move
        updateBoard(b, playerTurn, PLAYER);
        printBoard(b);
        // Score board
        if (scoreBoard(b, playerTurn, PLAYER) == PLAYER) return PLAYER;

        int aiTurn;
        aiTurn = getAiTurn(b);

        updateBoard(b, aiTurn, AI);
        printBoard(b);
        if (scoreBoard(b, aiTurn, AI) == AI) return AI;

        

        // AI turn
        //int aiTurn;
        //updateBoard(b, aiTurn, AI);
        //Score board


    }
    return status;
}


void printBoard(board b)
{
    printf("\n");
    for (int i = 0; i < NROWS; i++)
    {
        for (int j = 0; j < NCOLS; j++)
        {
            printf("%d ", b[j][NROWS - i - 1]);
        }
        printf("\n");
    }
    printf(SCALE);
}

void initialiseBoard(board b)
{
    for (int i = 0; i < NROWS; i++)
        for (int j = 0; j < NCOLS; j++)
            b[j][i] = EMPTY;
}

// Gets the players turn and validates it
int getPlayerTurn(board b)
{
    int turn;
    printf("Enter move: ");
    scanf("%d", &turn);

    // must subtract 1 from the turn because the computer board representation 
    // is zero-indexed whereas the player representation is one-indexed
    turn--;
    // If column number is greater than board size,
    // move is invalid
    if (turn > NCOLS - 1)
    {
        printf("invalid move, column out of board range\n");
        turn = getPlayerTurn(b);
    }

    // If column is full move is invalid
    else if (b[turn][NROWS - 1] != EMPTY)
    {
        printf("invalid move, column full\n");
        turn = getPlayerTurn(b);
    }

    return turn;
}

// Only check if the last Player has won, because if the previous player didn't
// win on their turn, they won't have won after the other player has moved
int scoreBoard(board b, int lastTurn, int lastPlayer)
{
    // Check column

    // if column is full, the highest chip is the number of rows (-1 since it's
    // indexed at 0)
    int lastChip = NROWS - 1;
    int inaRow = 0;
    for (int i = 0; i < NROWS; i++)
    {
        if (b[lastTurn][i] == lastPlayer)
        {
            inaRow ++;
            if (inaRow == WINNUM)
            {
                // The player that just went won
                return lastPlayer;
            }
        }
        else if (b[lastTurn][i] == EMPTY)
        {
            // Noone won via 4 in a column
            // last chip placed is the square under the empty square
            lastChip = i - 1;
            break;
        }
        else
        {
            // Opponents token is here
            inaRow = 0;
        }

    }
    
    
    // Check row


    // the win can only be surrounding the token last played,
    // therefore the only row that can win is that of the last token played
    inaRow = 0;
    for (int i = 0; i < NCOLS; i++)
    {
        // TODO check row at level lastChip - 1
        if (b[i][lastChip] == lastPlayer)
        {
            inaRow++;
            if (inaRow == WINNUM)
            {
                // last player who went just won
                return lastPlayer;
            }
        }
        else 
        {
            inaRow = 0;
        }
    }



    // Check diagonal (NW -> SE)
    
    inaRow = 0;
    // First check going in NW direction

    int col = lastTurn;
    int row = lastChip;

    while (col >= 0 && row < NROWS)
    {
        if (b[col][row] == lastPlayer)
        {
            inaRow++;
            col--; row++;
            if (inaRow >= WINNUM)
            {
                return lastPlayer;
            }
        }
        else 
        {
            break;
        }
    }

    // then check going in SE direction and add the two together 

    // values 1 tile in the SE diagonal direction as to not duplicate the 
    // placed tile in the row amount
    col = lastTurn + 1;
    row = lastChip - 1;


    while (col < NCOLS && row >= 0)
    {
        if (b[col][row] == lastPlayer)
        {
            inaRow++;
            col++; row--;
            if (inaRow == WINNUM)
            {
                return lastPlayer;
            }
        }
        else 
        {
            break;
        }
    }

    // Check other diagonal (SW -> NE)




    inaRow = 0;
    // First check going in NE direction

    col = lastTurn;
    row = lastChip;

    while (col < NCOLS && row < NROWS)
    {
        if (b[col][row] == lastPlayer)
        {
            inaRow++;
            col++; row++;

            if (inaRow >= WINNUM) return lastPlayer;
        }
        else break;
        
    }

    // then check going in SW direction and add the two together 

    // values 1 tile in the SW diagonal direction as to not duplicate the 
    // placed tile in the row amount
    col = lastTurn - 1;
    row = lastChip - 1;

    while (col >= 0 && row >= 0)
    {
        if (b[col][row] == lastPlayer)
        {
            inaRow++;
            col--; row--;

            if (inaRow == WINNUM) return lastPlayer;
        } 
        else break;
        
    }    
    
    return INPROGRESS;
}

// Move validity is checked in the getPlayerTurn function
// therefore no move validation is required here
void updateBoard(board b, int lastTurn, int player)
{
    for (int i = 0; i < NROWS; i++)
    {
        if (b[lastTurn][i] == EMPTY)
        {
            b[lastTurn][i] = player;
            return;
        }
    }
}


int getAiTurn(board b)
{
    int turn;
    turn = rand() % NCOLS; // Generate random integer between 0 and 6


    // If column number is greater than board size,
    // move is invalid, this should never happen
    if (turn > NCOLS - 1)
    {
        turn = getAiTurn(b);
    }

    // If column is full move is invalid
    else if (b[turn][NROWS - 1] != EMPTY)
    {
        turn = getAiTurn(b);
    }

    return turn;
}
