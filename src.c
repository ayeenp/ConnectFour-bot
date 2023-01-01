// Code Glub 2023 - All rights reserved

#include <stdio.h>
#include <stdbool.h>

#define EMPTY -1
#define BLACK 0
#define RED 1
#define INF 99999
#define S_WIN 1000
#define DRAW -2

int runGame(int);
int runBlackGame();
int runRedGame();
void minimizerTurn(int);
void maximizerTurn(int);
int findBestMove(int);
int miniMax(int, bool, int, int, int);
int scoreOfBoard(int);
int winningSide();
int checkForDirectionStreak(int, int *, int, int);
void setAdditionForDirectionWin(int, int *, int *);
int findHeight(int);
void clearBoard();
void copyBoard(int board2[6][7]);
void printBoard();
int max(int, int);
int min(int, int);

int board[6][7];
int MAX_DEPTH;

int main()
{

    int mySide;
    char s;
    scanf("%c", &s);
    if (s == 'b')
    {
        mySide = BLACK;
        MAX_DEPTH = 7;
    }
    else
    {
        mySide = RED;
        MAX_DEPTH = 8;
    }
    clearBoard();
    int a = runGame(mySide);
    printf("WIN: %d", a);

    return 0;
}

int runGame(int mySide)
{
    if (mySide == BLACK)
        return runBlackGame();
    else
        return runRedGame();
}

int runBlackGame()
{
    while (1)
    {
        maximizerTurn(BLACK);
        //printBoard();
        int winner = winningSide();
        if (winner == BLACK)
        {
            return BLACK;
        }
        else if (winner == RED)
        {
            return RED;
        }
        else if (winner == DRAW)
        {
            return DRAW;
        }
        minimizerTurn(RED);
        winner = winningSide();
        if (winner == BLACK)
        {
            return BLACK;
        }
        else if (winner == RED)
        {
            return RED;
        }
        else if (winner == DRAW)
        {
            return DRAW;
        }
    }
}

int runRedGame()
{
    while (1)
    {
        minimizerTurn(BLACK);
        int winner = winningSide();
        if (winner == BLACK)
        {
            return BLACK;
        }
        else if (winner == RED)
        {
            return RED;
        }
        else if (winner == DRAW)
        {
            return DRAW;
        }
        maximizerTurn(RED);
        //printBoard();
        winner = winningSide();
        if (winner == BLACK)
        {
            return BLACK;
        }
        else if (winner == RED)
        {
            return RED;
        }
        else if (winner == DRAW)
        {
            return DRAW;
        }
    }
}

void minimizerTurn(int side)
{
    int moveInput;
    scanf("%d", &moveInput);
    board[findHeight(moveInput - 1)][moveInput - 1] = side;
}

void maximizerTurn(int side)
{
    int moveIndex = findBestMove(side);
    printf("%d", moveIndex + 1);
    board[findHeight(moveIndex)][moveIndex] = side;
}

int findBestMove(int side)
{
    int bestMoveIndex = 0;
    int bestMoveScore = -1 * INF;
    int alpha = bestMoveScore;
    for (int j = 0; j < 7; j++)
    {
        int changedCellHeight = findHeight(j);
        if (changedCellHeight > 5)
            continue;
        board[changedCellHeight][j] = side;
        int score = miniMax(side, false, 0, alpha, INF);
        board[changedCellHeight][j] = EMPTY;
        // printf("BMS: %4d\tBMI: %d\tMAX %d: %d\n", bestMoveScore, bestMoveIndex, j, score);
        if (bestMoveScore < score)
        {
            bestMoveScore = score;
            bestMoveIndex = j;
            alpha = score;
        }
    }
    return bestMoveIndex;
}

int miniMax(int side, bool isMaximizer, int depth, int alpha, int beta)
{
    // Terminating conditions
    if (depth == MAX_DEPTH)
        return scoreOfBoard(side);
    int winner = winningSide();
    if (winner == side)
        return S_WIN - depth;
    else if (winner == !side)
        return -1 * S_WIN + depth;
    else if (winner == DRAW)
        return 0;
    int bestValue;
    if (isMaximizer)
    {
        bestValue = -1 * INF;
        for (int j = 0; j < 7; j++)
        {
            int changedCellHeight = findHeight(j);
            if (changedCellHeight > 5)
                continue;
            board[changedCellHeight][j] = side;
            int score = miniMax(side, false, depth + 1, alpha, beta);
            board[changedCellHeight][j] = EMPTY;
            bestValue = max(bestValue, score);
            alpha = max(bestValue, alpha);
            if (alpha >= beta)
                break;
        }
    }
    if (!isMaximizer)
    {
        bestValue = INF;
        for (int j = 0; j < 7; j++)
        {
            int changedCellHeight = findHeight(j);
            if (changedCellHeight > 5)
                continue;
            board[changedCellHeight][j] = !side;
            int score = miniMax(side, true, depth + 1, alpha, beta);
            board[changedCellHeight][j] = EMPTY;
            bestValue = min(bestValue, score);
            beta = min(bestValue, beta);
            if (alpha >= beta)
                break;
        }
    }
    return bestValue;
}

int scoreOfBoard(int side)
{
    int score = 0;
    int result = winningSide();
    if (result == side)
        return S_WIN - MAX_DEPTH;
    else if (result == !side)
        return -1 * S_WIN + MAX_DEPTH;
    // Check for double cells
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (board[i][j] != EMPTY)
            {
                // centerness
                int addition = min(6 - j, j);
                if (board[i][j] == !side)
                    addition *= -1;
                score += addition;
                // check for double
                for (int dir = 1; dir <= 4; dir++)
                {
                    int additionToScore = 0;
                    int blockedState = 0;
                    int streak = checkForDirectionStreak(dir, &blockedState, i, j);
                    if (streak >= 2)
                    {
                        if (blockedState == 0)
                            additionToScore = 10;
                        else if (blockedState == 1)
                            additionToScore = 5;
                    }
                    if (board[i][j] == !side)
                        additionToScore *= -1;
                    score += additionToScore;
                }
            }
        }
    }
    return score;
}

int winningSide()
{
    int x;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (board[i][j] != EMPTY)
            {
                for (int dir = 1; dir <= 4; dir++)
                {
                    int result = checkForDirectionStreak(dir, &x, i, j);
                    if (result == 4)
                    {
                        return board[i][j];
                    }
                }
            }
        }
    }
    int isDraw = 1;
    for (int i = 0; i < 7; i++)
    {
        if (board[5][i] == EMPTY)
        {
            isDraw = 0;
            break;
        }
    }
    if (isDraw)
        return DRAW;
    return EMPTY;
}

int checkForDirectionStreak(int direction, int *blockedState, int i, int j)
{
    int side = board[i][j];
    int iAddition, jAddition;
    setAdditionForDirectionWin(direction, &iAddition, &jAddition);
    int iLast = i - iAddition;
    int jLast = j - jAddition;
    int iLast2 = i - 2 * iAddition;
    int jLast2 = j - 2 * jAddition;
    bool last2IsBlocked = 0;
    if (iLast < 0 || jLast > 6 || jLast < 0 || board[iLast][jLast] != EMPTY)
        *blockedState = *blockedState + 1;
    else if (iLast2 < 0 || jLast2 > 6 || jLast2 < 0 || board[iLast2][jLast2] == !side)
        last2IsBlocked = 1;
    int streak = 1;
    for (; streak < 5; streak++)
    {
        i += iAddition;
        j += jAddition;
        if (i > 5 || j > 6 || j < 0 || board[i][j] == !side)
        {
            *blockedState = *blockedState + 1;
            break;
        }
        if (board[i][j] == EMPTY)
        {
            // Check if the second front addition is blocked on double streaks with one blocked side
            if (*blockedState == 1 && streak == 2)
            {
                i += iAddition;
                j += jAddition;
                if ((i < 0 || j > 6 || j < 0 || board[i][j] == !side))
                    *blockedState = 2;
            }
            break;
        }
    }
    // Check if the second back addition is blocked on double streaks with one blocked side
    if (*blockedState == 1 && streak == 2 & last2IsBlocked)
    {
        *blockedState = 2;
    }
    return streak;
}

void setAdditionForDirectionWin(int direction, int *iAddition, int *jAddition)
{
    switch (direction)
    {
    case 1:
        *iAddition = 0;
        *jAddition = 1;
        break;
    case 2:
        *iAddition = 1;
        *jAddition = 0;
        break;
    case 3:
        *iAddition = 1;
        *jAddition = 1;
        break;
    case 4:
        *iAddition = 1;
        *jAddition = -1;
        break;
    }
}

int findHeight(int moveIndex)
{
    int height = 5;
    while (height >= 0)
    {
        if (board[height][moveIndex] != EMPTY)
            break;
        height--;
    }
    return height + 1;
}

void clearBoard()
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            board[i][j] = EMPTY;
        }
    }
}

void copyBoard(int board2[6][7])
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            board2[i][j] = board[i][j];
        }
    }
}

void printBoard()
{
    for (int i = 5; i >= 0; i--)
    {
        for (int j = 0; j < 7; j++)
        {
            printf("%2d ", board[i][j]);
        }
        printf("\n");
    }
}

int max(int a, int b)
{
    if (a >= b)
        return a;
    return b;
}

int min(int a, int b)
{
    if (a <= b)
        return a;
    return b;
}