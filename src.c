// Code Glub 2023 - All rights reserved

#include <stdio.h>
#include <stdbool.h>

#define EMPTY -1
#define BLACK 0
#define RED 1
#define INF 99999
#define MAX_DEPTH 4
#define S_WIN 100

void minimizerTurn(int board[6][7], int);
void maximizerTurn(int board[6][7], int);
int findBestMove(int board[6][7], int);
int miniMax(int tempBoard[6][7], int, bool, int, int, int, int);
int scoreOfBoard(int board[6][7], int);
int winningSide(int board[6][7]);
int checkForDirectionStreak(int board[6][7], int, int *, int, int);
void setAdditionForDirectionWin(int, int *, int *);
int findHeight(int board[6][7], int);
void clearBoard(int board[6][7]);
void copyBoard(int board[6][7], int board2[6][7]);
void printBoard(int board[6][7]);
int max(int, int);
int min(int, int);

int main()
{
    int mySide;
    char s;
    scanf("%c", &s);
    if (s == 'b')
        mySide = BLACK;
    else
        mySide = RED;

    int board[6][7];
    clearBoard(board);
    while (1)
    {
        minimizerTurn(board, !mySide);
        printBoard(board);
        int winner = winningSide(board);
        if (winner == BLACK)
        {
            printf("BLACK WINS");
            break;
        }
        else if (winner == RED)
        {
            printf("RED WINS");
            break;
        }
        maximizerTurn(board, mySide);
        printf("BOT:\n");
        printBoard(board);
        winner = winningSide(board);
        if (winner == BLACK)
        {
            printf("BLACK WINS");
            break;
        }
        else if (winner == RED)
        {
            printf("RED WINS");
            break;
        }
    }

    return 0;
}

void minimizerTurn(int board[6][7], int side)
{
    int moveInput;
    scanf("%d", &moveInput);
    board[findHeight(board, moveInput - 1)][moveInput - 1] = side;
}

void maximizerTurn(int board[6][7], int side)
{
    int moveIndex = findBestMove(board, side);
    board[findHeight(board, moveIndex)][moveIndex] = side;
    // int moveInput;
    // scanf("%d", &moveInput);
    // board[findHeight(board, moveInput - 1)][moveInput - 1] = side;
}

int findBestMove(int board[6][7], int side)
{
    int bestMoveIndex = 0;
    int bestMoveScore = -1 * INF;
    for (int j = 0; j < 7; j++)
    {
        int tempBoard[6][7];
        copyBoard(board, tempBoard);
        int score = miniMax(tempBoard, side, true, 0, j, -1 * INF, INF);
        if (bestMoveScore < score)
        {
            bestMoveScore = score;
            bestMoveIndex = j;
        }
    }
    return bestMoveIndex;
}

int miniMax(int tempBoard[6][7], int side, bool isMaximizer, int depth, int column, int alpha, int beta)
{
    if (depth == MAX_DEPTH)
        return scoreOfBoard(tempBoard, side);
    if (winningSide(tempBoard) == side)
        return S_WIN;
    else if (winningSide(tempBoard) == !side)
        return -1 * S_WIN;
    int bestValue;
    if (isMaximizer)
    {
        bestValue = -1 * INF;
        for (int j = 0; j < 7; j++)
        {
            int changedCellHeight = findHeight(tempBoard, j);
            tempBoard[changedCellHeight][j] = side;
            int score = miniMax(tempBoard, side, false, depth + 1, j, alpha, beta);
            tempBoard[changedCellHeight][j] = EMPTY;
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
            int changedCellHeight = findHeight(tempBoard, j);
            tempBoard[changedCellHeight][j] = side;
            int score = miniMax(tempBoard, side, true, depth + 1, j, alpha, beta);
            tempBoard[changedCellHeight][j] = EMPTY;
            bestValue = min(bestValue, score);
            beta = min(bestValue, beta);
            if (alpha >= beta)
                break;
        }
    }
    return bestValue;
}

int scoreOfBoard(int board[6][7], int side)
{
    int score = 0;
    int result = winningSide(board);
    if (result == side)
        return S_WIN;
    else if (result == !side)
        return -1 * S_WIN;
    // Check for double cells
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (board[i][j] != EMPTY)
            {
                for (int dir = 1; dir <= 4; dir++)
                {
                    int additionToScore = 0;
                    int blockedState = 0;
                    int streak = checkForDirectionStreak(board, dir, &blockedState, i, j);
                    if (streak >= 2)
                    {
                        if (blockedState == 0)
                            additionToScore = 4;
                        else if (blockedState == 1)
                            additionToScore = 2;
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

int winningSide(int board[6][7])
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
                    int result = checkForDirectionStreak(board, dir, &x, i, j);
                    if (result == 4)
                    {
                        return board[i][j];
                    }
                }
            }
        }
    }
    return EMPTY;
}

int checkForDirectionStreak(int board[6][7], int direction, int *blockedState, int i, int j)
{
    int side = board[i][j];
    int iAddition, jAddition;
    setAdditionForDirectionWin(direction, &iAddition, &jAddition);
    int iLast = i - iAddition;
    int jLast = j - jAddition;
    int iLast2 = i - 2 * iAddition;
    int jLast2 = j - 2 * jAddition;
    bool last2IsBlocked = 0;
    if (iLast < 0 || jLast > 6 || jLast < 0 || board[iLast][jLast] == !side)
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
            //Check if the second front addition is blocked on double streaks with one blocked side
            if(*blockedState == 1 && streak == 2)
            {
                i += iAddition;
                j += jAddition;
                if((i < 0 || j > 6 || j < 0 || board[i][j] == !side))
                    *blockedState = 2;
            }
            break;
        }
    }
    //Check if the second back addition is blocked on double streaks with one blocked side
    if(*blockedState == 1 && streak == 2 & last2IsBlocked)
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

int findHeight(int board[6][7], int moveIndex)
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

void clearBoard(int board[6][7])
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            board[i][j] = EMPTY;
        }
    }
}

void copyBoard(int board[6][7], int board2[6][7])
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            board2[i][j] = board[i][j];
        }
    }
}

void printBoard(int board[6][7])
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