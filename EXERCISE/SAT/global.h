#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <Windows.h>
#include <random>

using namespace std;

#define C 9
#define R 9

typedef struct vnode
{
	int var;
	struct vnode* next;
} VarNode; // 文字（布尔变元）

typedef struct cnode
{
	int varnum;
	struct vnode* ClsHead;
	struct cnode* down;
} ClsNode; // 子句

/* SAT_CNF */

// read .cnf file
ClsNode* read_cnf(string& filename);

// clauses
int findSC(ClsNode* cnf);
void removeCls(ClsNode*& cnf, ClsNode* target);
void removeSC(ClsNode*& cnf, int var);
void removeVar(ClsNode*& cnf, int var);
bool hasEmpty(ClsNode* cnf);
ClsNode* addVar(ClsNode* cnf, int var);
ClsNode* copy(ClsNode* cnf);

// select var
int findMinlength(ClsNode* cnf); // min length

// DPLL
bool DPLL(ClsNode* cnf, short ans[]);
bool newDPLL(ClsNode* cnf, short ans[]);

// output to .res file
void output_res(short ans[], string& filename, bool status, clock_t start, clock_t end);

// show
void show(short ans[]);

/* Sudoku */

// generate Sudoku
void reset();
void randRow(int S0[], int row);
void randValue(int** Sudoku, int n);
void addNum(int n, int row, int col);
bool check(int n, int row, int col);
bool generateSudoku(int** Sudoku, int row, int col);
bool initSudoku(int** Sudoku, int row, int col, int count);

// solve Sudoku
void SudokuTocnf(int** Sudoku, string& filename);

// show Sudoku
void show(int** Sudoku);

// copy Sudoku
int** copy(int** Sudoku);

/* menu */

void menu();

// Sudoku menu
void difficultyChoose();
void setDifficulty(int difficulty);
void SudokuMenu();
void solve(int** Sudoku);
void input(int** board);
bool check_ans(int** board, int** Sudoku);
void play(int** Sudoku);

#endif