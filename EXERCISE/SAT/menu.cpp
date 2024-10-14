#include "global.h"

extern int vn;
extern int cn;
extern int blockbound;
extern int upperbound;
extern int blank;
extern bool canNotChanged[10][10];

void menu()
{
    cout << "=========================================================" << endl;
    cout << "  HUST Programing Design: SAT Solver by DPLL & X-Sudoku  " << endl;
    cout << "         1. SAT Solver   2. X-Sudoku   0. exit           " << endl;
    cout << "=========================================================" << endl;
}

void difficultyChoose()
{
    cout << "=======================================================" << endl;
    cout << "               XSudoku Difficulty Option               " << endl;
    cout << "      1: Easy(default)     2: Medium     3: Hard       " << endl;
    cout << "=======================================================" << endl;
}

void setDifficulty(int difficulty)
{
    switch (difficulty)
    {
    case 1:
    {
        blockbound = upperbound = 5;
        blank = 30;
        break;
    }
    case 2:
    {
        blockbound = upperbound = 7;
        blank = 40;
        break;
    }
    case 3:
    {
        blockbound = 9;
        upperbound = 7;
        blank = 50;
        break;
    }
    case 100: // test
    {
        blockbound = upperbound = 1;
        blank = 1;
        break;
    }
    default: // 默认为简单
    {
        blockbound = upperbound = 5;
        blank = 30;
        break;
    }
    }
}

void SudokuMenu()
{
    cout << "==========================================================" << endl;
    cout << "                        X-Sudoku                          " << endl;
    cout << "       1. X-Sudoku to .cnf file     2. Get Answer         " << endl;
    cout << "       3. Play                      0. Exit               " << endl;
    cout << "==========================================================" << endl;
}

void solve(int** Sudoku) // 2
{   
    string fn = "Sudoku.cnf";
    SudokuTocnf(Sudoku, fn);
    ClsNode* head = read_cnf(fn);
    short* ans = new short[vn + 1];
    bool res = newDPLL(head, ans);
    for (int i = 111; i <= vn; ++i)
    {
        if (ans[i] == 1)
        {
            int temp = i;
            int num = temp % 10;
            temp /= 10;
            int col = temp % 10;
            temp /= 10;
            int row = temp % 10;
            Sudoku[row][col] = num;
        }
    }
    cout << "Here is the answer : " << endl;
}
void input(int** board)
{
    cout << "Which row do you want to fill in : ";
    int row;
    cin >> row;
    while (row < 1 || row > 9)
    {
        cout << "Position Error!" << endl;
        cout << "Enter again : ";
        cin >> row;
    }

    cout << "Which column do you want to fill in : ";
    int col;
    cin >> col;
    while (col < 1 || col > 9)
    {
        cout << "Position Error!" << endl;
        cout << "Enter again : ";
        cin >> col;
    }

    while (canNotChanged[row][col])
    {
        cout << "Position Error!" << " ";
        cout << "This position is already filled!" << endl;
        cout << "Enter again!" << endl;

        cout << "Which row do you want to fill in : ";
        cin >> row;
        while (row < 1 || row > 9)
        {
            cout << "Position Error!" << endl;
            cout << "Enter again : ";
            cin >> row;
        }

        cout << "Which column do you want to fill in : ";
        cin >> col;
        while (col < 1 || col > 9)
        {
            cout << "Position Error!" << endl;
            cout << "Enter again : ";
            cin >> col;
        }
    }

    cout << "Which number do you want to fill in this position : ";
    int num;
    cin >> num;
    board[row][col] = num;
}

bool check_ans(int** board, int** Sudoku)
{
    for (int i = 1; i <= 9; ++i)
    {
        for (int j = 1; j <= 9; ++j)
        {
            if (board[i][j] != Sudoku[i][j])
                return false;
        }
    }
    return true;
}

void play(int** Sudoku)
{
    //创建副本，用于修改

    int** board = new int* [10];
    for (int i = 1; i <= 9; ++i)
    {
        board[i] = new int[10];
        memcpy(board[i], Sudoku[i], sizeof(int) * 10);
    }

    //防止修改到原来就填充的数字
    for (int i = 1; i <= 9; ++i)
    {
        memset(canNotChanged[i], 0, sizeof(bool) * 10);
    }

    for (int i = 1; i <= 9; ++i)
    {
        for (int j = 1; j <= 9; ++j)
        {
            if (Sudoku[i][j])
            {
                canNotChanged[i][j] = true;
            }
        }
    }

    solve(Sudoku); // 得到求解的数独，便于检查

    bool isCorrect = false;
    bool showAnswer = false;
    int choice = 1;

    while (!isCorrect)
    {
        system("cls");
        show(board);
        printf("Enter 0 to show answer, others to continue playing : ");
        cin >> choice;

        if (choice == 0)
        {
            showAnswer = true;
            show(Sudoku);
            cout << "Press any key to continue" << endl;
            getchar();
            getchar();
            system("cls");
        }

        if (!showAnswer)
        {
            input(board);
            isCorrect = check_ans(board, Sudoku);
        }
        else
        {
            system("cls");
            show(Sudoku);
            break;
        }
    }

    if (isCorrect)
    {
        cout << "Correct!" << endl;
        show(Sudoku);
        cout << "Press any key to continue" << endl;
        getchar();
        getchar();
        system("cls");
    }

    for (int i = 1; i <= 9; ++i)
    {
        delete board[i];
    }
    delete[] board;
}