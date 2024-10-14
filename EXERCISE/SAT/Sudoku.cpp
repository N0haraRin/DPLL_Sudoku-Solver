#include "global.h"

int check_bit[10] = { 0, 1, 2, 4, 8, 16, 32, 64, 128, 256 };
int check_row[10] = { 0 };
int check_col[10] = { 0 };
int check_block[4][4] = { 0 };
int check_diag[2] = { 0 };

void reset()
{
    memset(check_row, 0, sizeof(int) * 10);
    memset(check_col, 0, sizeof(int) * 10);
    memset(check_diag, 0, sizeof(int) * 2);
    for (int i = 0; i < 4; i++)
    {
        memset(check_block[i], 0, sizeof(int) * 4);
    }
}

void randRow(int S0[], int row)
{
    for (int i = 1; i <= 9; i++)
    {
        int num = rand() % 9 + 1;
        while (check(num, row, i))
        {
            num = rand() % 9 + 1;
        }
        S0[i] = num;
        addNum(num, row, i);
    }
}

void randValue(int** Sudoku, int n)
{
    int row, col, val;
    for (int i = 0; i < n; i++)
    {
        do
        {
            row = rand() % 9 + 1;
            col = rand() % 9 + 1;
        } while (Sudoku[row][col]);
        do
        {    
            bool c[10] = { false };
            val = rand() % 9 + 1;
            c[val] = true;
            if (c[1] && c[2] && c[3] && c[4] && c[5] && c[6] && c[7] && c[8] && c[9])
            {
                i--;
                break;
            }
        } while (check(val, row, col));
        Sudoku[row][col] = val;
        addNum(val, row, col);
    }
}

void addNum(int n, int row, int col)
{
    int num = check_bit[n > 0 ? n : -n];
    if (n < 0)
    {
        num = -num;
    }
    if (row == col)
    {
        check_diag[0] += num;
    }
    if (row + col == 10)
    {
        check_diag[1] += num;
    }
    check_col[col] += num;
    check_row[row] += num;
    check_block[(row + 2) / 3][(col + 2) / 3] += num;
}

bool check(int n, int row, int col)
{
    bool res1 = false;
    bool res2 = false;
    if (row == col)
    {
        res1 = res1 || (check_diag[0] & check_bit[n]);
    }
    if (row + col == 10)
    {
        res2 = res2 || (check_diag[1] & check_bit[n]);
    }
    return res1 || res2 || (check_row[row] & check_bit[n]) || (check_col[col] & check_bit[n]) || (check_block[(row + 2) / 3][(col + 2) / 3] & check_bit[n]);
}

bool generateSudoku(int** Sudoku, int row, int col)
{
    if (row <= R && col <= C)
    {
        if (!Sudoku[0][0])
        {
            randValue(Sudoku, 11);
            //show(Sudoku);
            //cout << endl;
            Sudoku[0][0] = 1;
        }
        if (Sudoku[row][col])
        {
            if (row != R && col == C)
            {
                return (generateSudoku(Sudoku, row + 1, 1));
            }
            else if (col != C)
            {
                return (generateSudoku(Sudoku, row, col + 1));
            }
        }
        /**/
        int flag = 0;
        int i;
        for (i = 1; i <= 9 && !flag; i++)
        {
            if (!check(i, row, col))
            {
                flag = 1;
                if (Sudoku[row][col])
                {
                    addNum(-Sudoku[row][col], row, col);
                }
                Sudoku[row][col] = i;
                addNum(i, row, col);
                //show(Sudoku);
                //cout << endl;
                if (row != R && col == C)
                {
                    if (generateSudoku(Sudoku, row + 1, 1)) return true;
                    else flag = 0;
                }
                else if (col != C)
                {
                    if (generateSudoku(Sudoku, row, col + 1)) return true;
                    else flag = 0;
                }
            }
        }
        if (flag == 0)
        {
            if (Sudoku[row][col])
            {
                addNum(-Sudoku[row][col], row, col);
            }
            Sudoku[row][col] = 0;
            return false;
        }
    }
    return true;
}

bool initSudoku(int** Sudoku, int row, int col, int count)
{
    extern int blockbound;
    extern int upperbound;
    extern int blank;
    static int blank_inRow[10] = { 0 };
    static int blank_inCol[10] = { 0 };
    static int blank_inBlock[4][4] = { 0 };
    bool flag = true;

    if (count == blank)
    {
        memset(blank_inRow, 0, sizeof(int) * 10);
        memset(blank_inCol, 0, sizeof(int) * 10);
        for (int i = 0; i < 4; i++)
        {
            memset(blank_inBlock[i], 0, sizeof(int) * 4);
        }
        return Sudoku;
    }
    
    if (!Sudoku[row][col])
    {
        row = rand() % 9 + 1;
        col = rand() % 9 + 1;
        return initSudoku(Sudoku, row, col, count); 
    }

    /*
    if (row == 9 && col == 9)
    {
        memset(blank_inRow, 0, sizeof(int) * 10);
        memset(blank_inCol, 0, sizeof(int) * 10);
        memset(blank_inBlock, 0, sizeof(int) * 10);
        return Sudoku;
    }
    */
    
    if (blank_inRow[row] == upperbound) 
    {
        /*
        if (row == 9)
        {
            memset(blank_inRow, 0, sizeof(int) * 10);
            memset(blank_inCol, 0, sizeof(int) * 10);
            memset(blank_inBlock, 0, sizeof(int) * 10);
            return Sudoku;
        }*/
        row = rand() % 9 + 1;
        col = rand() % 9 + 1;
        return initSudoku(Sudoku, row, col, count);
        //return initSudoku(Sudoku, row + 1, 1, count);
    }

    if (blank_inCol[col] == upperbound)
    {
        /*
        if (row != 9 && col == 9)
        {
            return initSudoku(Sudoku, row + 1, 1, count);
        }
        else
        {
            return initSudoku(Sudoku, row, col + 1, count);
        }*/
        row = rand() % 9 + 1;
        col = rand() % 9 + 1;
        return initSudoku(Sudoku, row, col, count);
    }

    if (blank_inBlock[(row + 2) / 3][(col + 2) / 3] == blockbound)
    {
        row = rand() % 9 + 1;
        col = rand() % 9 + 1;
        return initSudoku(Sudoku, row, col, count);
        /*
        if ((row + 2) / 3 == 3 && (col + 2) / 3 == 3)
        {
            memset(blank_inRow, 0, sizeof(int) * 10);
            memset(blank_inCol, 0, sizeof(int) * 10);
            memset(blank_inBlock, 0, sizeof(int) * 10);
            return Sudoku;
        }
        else
        {
            if (row != 9 && col == 9)
            {
                return initSudoku(Sudoku, row + 1, 1, count);
            }
            else
            {
                return initSudoku(Sudoku, row, col + 1, count);
            }       
        }*/
    }

    int num = Sudoku[row][col];
    for (int i = 1; i <= 9 && flag; i++)
    {
        if (i != num && !(check(num, row, col)))
        {
            Sudoku[row][col] = i;
            if (generateSudoku(Sudoku, 1, 1))
            {
                flag = false;
            }
        }
    }
    if (flag)
    {
        addNum(-num, row, col);
        Sudoku[row][col] = 0;
        blank_inRow[row]++;
        blank_inCol[col]++;
        blank_inBlock[(row + 2) / 3][(col + 2) / 3]++;
        count++;
    }

    else
    {
        Sudoku[row][col] = num;
    }

    row = rand() % 9 + 1;
    col = rand() % 9 + 1;
    return initSudoku(Sudoku, row, col, count);

    //show(Sudoku);
    /*
    if (row != 9 && col == 9)
    {
        return initSudoku(Sudoku, row + 1, 1, count);
    }
    else
    {
        return initSudoku(Sudoku, row, col + 1, count);
    }*/
}

void SudokuTocnf(int** Sudoku, string& filename)
{
    ofstream fp(filename);
    if (!fp)
    {
        cout << "Can't open file" << " " << filename << "!";
        exit(0);
    }

    fp << "c X-Sudoku's CNF file" << endl;

    int num[82];
    int row[82];
    int col[82];
    int count = 0;
    /*开始检查有几个非 0 元素，并保存其位置*/
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            if (Sudoku[i][j] != 0)
            {
                count++;
                row[count] = i;
                col[count] = j;
                num[count] = Sudoku[i][j];
            }
        }
    }
    int varnum = 999;
    int clsnum = 9737 + count;
    fp << "p " << "cnf " << varnum << " " << clsnum << endl;

    /*先输出非0元素*/
    for (int i = 1; i <= count; ++i)
    {
        fp << row[i] * 100 + col[i] * 10 + num[i] << " " << 0 << endl;
    }

    /*Restriction 1:每个格子至少有一个数字*/
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            for (int k = 1; k <= 9; k++)
            {
                fp << i * 100 + j * 10 + k << " ";
            }
            fp << "0" << endl;
        }
    }
    /*Restriction 2:每个格子最多只有一个数字*/
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            for (int k = 1; k <= 8; k++)
            {
                for (int x = k + 1; x <= 9; x++)
                {
                    fp << (i * 100 + j * 10 + k) * (-1) << " " << (i * 100 + j * 10 + x) * (-1) << " " << "0";
                }
            }
        }
    }

    /*Restriction 3:每行有全部的数字*/
    for (int i = 1; i <= 9; i++)
    {
        for (int k = 1; k <= 9; k++)
        {
            for (int j = 1; j <= 9; j++)
            {
                fp << i * 100 + j * 10 + k << " ";
            }
            fp << "0" << endl;
        }
    }
    /*Restriction 4:每行的数字需要互斥*/
    for (int i = 1; i <= 9; i++)
    {
        for (int k = 1; k <= 9; k++)
        {
            for (int j = 1; j <= 8; j++)
            {
                for (int x = j + 1; x <= 9; ++x)
                {
                    fp << (i * 100 + j * 10 + k) * (-1) << " " << (i * 100 + x * 10 + k) * (-1) << " " << "0" << endl;
                }
            }
        }
    }
    /*Restriction 5:每列有全部的数字*/
    for (int j = 1; j <= 9; j++)
    {
        for (int k = 1; k <= 9; k++)
        {
            for (int i = 1; i <= 9; i++)
            {
                fp << i * 100 + j * 10 + k << " ";
            }
            fp << "0" << endl;
        }
    }
    /*Restriction 6:每列的数字需要互斥*/
    for (int j = 1; j <= 9; j++)
    {
        for (int k = 1; k <= 9; k++)
        {
            for (int i = 1; i <= 8; i++)
            {
                for (int x = i + 1; x <= 9; x++)
                {
                    fp << (i * 100 + j * 10 + k) * (-1) << " " << (x * 100 + j * 10 + k) * (-1) << " " << "0" << endl;
                }
            }
        }
    }
    /*Restriction 7:每个块有全部的数字*/

    for (int p = 0; p <= 2; p++)
    {
        for (int q = 0; q <= 2; q++)
        {
            for (int k = 1; k <= 9; k++)
            {
                for (int i = 1; i <= 3; i++)
                {
                    for (int j = 1; j <= 3; j++)
                    {
                        int x = 3 * p + i;
                        int y = 3 * q + j;
                        fp << x * 100 + y * 10 + k << " ";
                    }
                }
                fp << "0" << endl;
            }
        }
    }

    /*Restriction 8:每条对角线上有全部数字*/
    for (int k = 1; k <= 9; k++)
    {
        for (int i = 1; i <= 9; i++)
        {
            fp << i * 110 + k << " ";
        }
        fp << "0" << endl;
    }
    for (int k = 1; k <= 9; k++)
    {
        for (int i = 1; i <= 9; i++)
        {
            fp << i * 100 + (10 - i) * 10 + k << " ";
        }
        fp << "0" << endl;
    }
    /*Restriction 9:每条对角线上的数字互斥*/
    for (int k = 1; k <= 9; k++)
    {
        for (int i = 1; i <= 8; i++)
        {
            for (int x = i + 1; x <= 9; x++)
            {
                fp << (i * 110 + k) * (-1) << " " << (x * 110 + k) * (-1) << " " << "0" << endl;
            }
        }
    }

    for (int k = 1; k <= 9; k++)
    {
        for (int i = 1; i <= 8; i++)
        {
            for (int x = i + 1; x <= 9; x++)
            {
                fp << (i * 100 + (10 - i) * 10 + k) * (-1) << " " << (x * 100 + (10 - x) * 10 + k) * (-1) << " " << "0" << endl;
            }
        }
    }
    fp.close();
}

void show(int** Sudoku)
{
    for (int j = 1; j <= 39; ++j)
    {
        cout << "-";
    }
    cout << endl;
    for (int i = 1; i <= 9; ++i)
    {
        cout << " | ";
        for (int j = 1; j <= 9; ++j)
        {
            cout << " " << Sudoku[i][j] << " ";
            if (j % 3 == 0)
            {
                cout << " | ";
            }
        }
        if (i % 3 == 0)
        {
            cout << endl;
            for (int j = 1; j <= 39; ++j)
            {
                cout << "-";
            }
        }
        cout << endl;
    }
}

int** copy(int** Sudoku)
{
    int** ans = new int* [R + 1];
    for (int i = 0; i <= 9; i++)
    {
        ans[i] = new int[C + 1];
        memset(ans[i], 0, sizeof(int) * (C + 1));
    }
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            ans[i][j] = Sudoku[i][j];
        }
    }
    ans[0][0] = Sudoku[0][0];
    return ans;
}
