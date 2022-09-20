#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define's

#define PLAYER 1
#define AI 2
#define EMPTY 0
#define DRAW 3
#define INPROGRESS 4
#define WINNUM 3
#define VALID 1
#define INVALID 0

#define INFINITY 9999

#define NROWS 4
#define NCOLS 5

#define SCALE "\n1 2 3 4 5 6 7\n"


// Typedefs
typedef int board[NCOLS][NROWS];

// Function Prototypes

int gameLoop(board);
void printBoard(board);
void initialiseBoard(board);
int scoreBoard(board, int, int);
int getPlayerTurn(board);
int getAiTurn(board);
void updateBoard(board, int, int );
int* convertStringToIntegerArray(char*, int *);
void initialiseSpecificBoard(board, int *, int);
void reverseMoveBoard(board, int);
int moveMinimax(board);
int minimax(board, int, int, int, int, int);
int isValidMove(board, int);
int isPlayerValidMove(board, int);
int checkResult(int);
int evalTerminalState(int, int);
int max(int a, int b);
int min(int a, int b);
int maxVal(board, int, int , int, int);
int minVal(board, int, int , int, int);




int main(int argc, char **argv)
{
    board b;
    printf("%d\n", argc);
    initialiseBoard(b);
    if (argc == 2) 
    {
        char *c = argv[1];
        int len = strlen(c);
        int seed[50];
        for (int i = 0; i < len; i++)
        {
            if (c[i] == '1')
            {
                seed[i] = 0;
            }
            if (c[i] == '2')
            {
                seed[i] = 1;
            }
            if (c[i] == '3')
            {
                seed[i] = 2;
            }
            if (c[i] == '4')
            {
                seed[i] = 3;
            }
            if (c[i] == '5')
            {
                seed[i] = 4;
            }
            if (c[i] == '6')
            {
                seed[i] = 5;
            }
            if (c[i] == '7')
            {
                seed[i] = 6;
            }
            
        }
        
        printBoard(b);
        initialiseSpecificBoard(b, seed, len);
    }

    int outcome = gameLoop(b);
    if (outcome == PLAYER) printf("Congratulations, you win!");
    if (outcome == AI) printf("Sorry, you lost this time :(");
    if (outcome == DRAW) printf("Well done, it's hard to draw (really)!");
    return 0;
}

int* convertStringToIntegerArray(char* c, int *n)
{
    *n = strlen(c);
    int *arr = (int*)malloc((*n) * sizeof(int));
    printf("YOUR\n");
    printf("%c\n", c[0]);
    for(int i = 0; i < *n; i++)
    {
        sscanf(&c[i], "%d", &arr[i]);
        //arr[i] = atoi(&c[i]) - 1;
        printf("%d", arr[i]);
    }
    return arr;
}

int gameLoop(board b)
{
    int status = INPROGRESS;
    int result;

    printBoard(b);
    while (status == INPROGRESS)
    {
        // Get player turn
        int playerTurn = getPlayerTurn(b);

        // Player move
        updateBoard(b, playerTurn, PLAYER);
        printBoard(b);

        // Score board
        result = scoreBoard(b, playerTurn, PLAYER);
        if (result != INPROGRESS) return result;
       
        // Get AI turn
        int aiTurn = getAiTurn(b);

        updateBoard(b, aiTurn, AI); 
        printBoard(b);
        result = scoreBoard(b, aiTurn, AI);
        if (result != INPROGRESS) return result;
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

// Initialise empty board
void initialiseBoard(board b)
{
    for (int i = 0; i < NROWS; i++)
        for (int j = 0; j < NCOLS; j++)
            b[j][i] = EMPTY;
}

// Generate the board given in the 'game' array
void initialiseSpecificBoard(board b, int *game, int n)
{
    int player = PLAYER;
    for (int i = 0; i < n; i++)
    {
        printf("%d", game[i]);
        updateBoard(b, game[i], player);

        if (player == PLAYER) player = AI;
        else if (player == AI) player = PLAYER;
    }
}

// Gets the players turn and validates it
int getPlayerTurn(board b)
{
    int move;
    printf("Enter move: ");
    scanf("%d", &move);

    /*
      must subtract 1 from the turn because the computer board representation 
      is zero-indexed whereas the player representation is one-indexed
    */
    move--;

    if (!isPlayerValidMove(b, move))
        return getPlayerTurn(b);

    return move;
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
                // The player that just went won
                return lastPlayer;
        }
        else if (b[lastTurn][i] == EMPTY)
        {
            // Noone won via 4 in a column
            // last chip placed is the square under the empty square
            lastChip = i - 1;
            break;
        }
        else // Opponents token here
            inaRow = 0;
        
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
                // last player who went just won
                return lastPlayer;
        }
        else 
            inaRow = 0;
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
                return lastPlayer;
        }
        else 
            break;
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
                return lastPlayer;
        }
        else 
            break;
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

    // Check if board is full -> DRAW
    for (int i = 0; i < NCOLS; i++)
        if (b[i][NROWS - 1] == EMPTY)
            return INPROGRESS;
            
    return DRAW;
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

// Removes the top piece of the column
// Essentially reverses a move
void reverseMoveBoard(board b, int column)
{
    for (int i = 0; i < NROWS; i++)
    {
        if (b[column][i] == EMPTY)
        {
            b[column][i - 1] = EMPTY;
            return;
        } 
    }
    b[column][NROWS - 1] = EMPTY;
}

int getAiTurn(board b)
{
    int turn;
    //turn = rand() % NCOLS; // Generate random integer between 0 and 6
    turn = moveMinimax(b);
    
    printf("turn: %d, valid: %d", turn, isValidMove(b, turn));
    

    return turn;
}

// identical to minimax except it tracks the best move rather than just the 
// best score
// doesnt need to check for root case because that is already checked after the
// player makes their move
int moveMinimax(board b)
{
    // Every score is garuanteed to be better than this, hence bestMove doesnt 
    // need to be defined at this point.
    int bestScore = -INFINITY;
    int bestMove;
    printf("\n\n --------Calculating -------\n\n");
    for (int i = 0; i < NCOLS; i++)
    {
        if (isValidMove(b, i))
        {
            updateBoard(b, i, AI);
            
            printBoard(b);
            int score = minVal(b, i, 1, -INFINITY, INFINITY); // depth = 1
            printf("move: %d, score: %d\n", i, score);
            reverseMoveBoard(b, i);

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return bestMove;
}

// The 'max' part of minimax
int maxVal(board b, int move, int depth, int alpha, int beta)
{
    // Check terminal state
    int result = scoreBoard(b, move, PLAYER);
    if (result != INPROGRESS)
    {
        //if (depth <= 3)
        {
        printf("--------------\nTERMINAL STATE\n-----------\n");
        printBoard(b);
        printf("result: %d\n", result);
        }
        return evalTerminalState(result, depth); //------ 0 means minimising
    }
    
    int bestScore = -INFINITY;

    for (int i = 0; i < NCOLS; i++)
    {
        if (isValidMove(b, i))
        {
            updateBoard(b, i, AI);
            if (depth <= 2)
                printBoard(b);

            bestScore = max(bestScore, minVal(b, i, depth + 1, alpha, beta));
            reverseMoveBoard(b, i);

            //Alpha-Beta pruning
            if (bestScore >= beta)
                break;
            
            alpha = max(alpha, bestScore);
        }
    }
    return bestScore;
}

// The 'mini' part of minimax
int minVal(board b, int move, int depth, int alpha, int beta)
{
    // Check terminal state
    int result = scoreBoard(b, move, AI);

    if (result != INPROGRESS)
    {
        //if (depth <= 3)
        {
            printf("##############\nTERMINAL STATE\n##############\n");

            printBoard(b);
            printf("result: %d\n", result);
        }

        return evalTerminalState(result, depth); //---- 1 means maximising
    }
    
    int bestScore = INFINITY;

    for (int i = 0; i < NCOLS; i++)
    {
        if (isValidMove(b, i))
        {
            updateBoard(b, i, PLAYER);
            if (depth <= 2)
                printBoard(b);
            bestScore = min(bestScore, maxVal(b, i, depth + 1, alpha, beta));
            reverseMoveBoard(b, i);

            //Alpha-Beta pruning
            if (bestScore <= alpha)
                break;
            
            beta = min(alpha, bestScore);
        }
    }
    return bestScore;
}

int evalTerminalState(int result, int depth)
{
    if (result == AI)
        return (NCOLS * NROWS) - depth;
    else if (result == PLAYER)
        return (-NCOLS * NROWS) + depth;
    else // result = DRAW
        return DRAW;
}

int isValidMove(board b, int move)
{
    // row is out of range
    if (move < 0 || move >= NCOLS)
        return INVALID;
    
    // column is full
    if (b[move][NROWS - 1] != EMPTY)
        return INVALID;
    
    return VALID;
}

int isPlayerValidMove(board b, int move)
{
    // row is out of range
    if (move < 0 || move >= NCOLS)
    {
        printf("invalid move, column out of board range\n");
        return INVALID;
    }
    
    // column is full
    if (b[move][NROWS - 1] != EMPTY)
    {
        printf("invalid move, column full\n");
        return INVALID;
    }
    
    return VALID;
}

int checkResult(int result)
{
    // a vast majority of positions will be in progress rather than in an end 
    // state, for these cases, only one comparison will be made rather than 
    // all three end states
    if (result != INPROGRESS)
    {
        if (result == DRAW)
            return 0;
        
        if (result == AI)
            return 1;
        
        if (result == PLAYER)
            return -1;
    } 
    return INPROGRESS;
}

// return the higher of the two values
int max(int a, int b)
{
    if (a >= b) return a;
    return b;
}

// return the lower of the two values
int min(int a, int b)
{
    if (b >= a) return a;
    return b;
}