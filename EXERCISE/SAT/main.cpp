#include "global.h"

int vn; // variable num
int cn; // clause num

int difficulty = 1;
int blockbound = 0;
int upperbound = 0;
int blank = 0;
bool canNotChanged[10][10];

int main()
{
	menu();
	int choice;
	cin >> choice;
	while (choice)
	{
		if (choice == 2)
		{
			srand((unsigned)time(nullptr));
			system("cls");
			string fn = "Sudoku.cnf";
			int *Sudoku[R + 1];
			for (int i = 0; i <= 9; i++)
			{
				Sudoku[i] = new int[C + 1];
				memset(Sudoku[i], 0, sizeof(int) * (C + 1));
			}
			reset();
			generateSudoku(Sudoku, 1, 1);
			int diff;
			difficultyChoose();
			cin >> diff;
			setDifficulty(diff);
			initSudoku(Sudoku, 1, 1, 0);
			SudokuMenu();
			int choice_Sudoku;
			cin >> choice_Sudoku;
			while (choice_Sudoku)
			{
				if (choice_Sudoku == 1)
				{
					SudokuTocnf(Sudoku, fn);
					cout << "Successful!" << endl;
					cout << "Press any key to continue" << endl;
					getchar();
					getchar();
					system("cls");
				}
				else if (choice_Sudoku == 2)
				{
					int **temp = copy(Sudoku);
					solve(temp);
					show(temp);
					temp = nullptr;
					cout << "Press any key to continue" << endl;
					getchar();
					getchar();
					system("cls");
				}
				else if (choice_Sudoku == 3)
				{
					play(Sudoku);
				}
				system("cls");
				SudokuMenu();
				cin >> choice_Sudoku;
			}
			system("cls");
			for (int i = 0; i <= 9; i++)
			{
				delete Sudoku[i];
				Sudoku[i] = nullptr;
			}
		}
		else if (choice == 1)
		{
			system("cls");
			string prefix_read = "C:\\CodeField\\CODE_C\\CPP_Multiple\\EXERCISE\\SAT\\testfile\\";
			string prefix_output = "C:\\CodeField\\CODE_C\\CPP_Multiple\\EXERCISE\\SAT\\resultfile\\";
			string suffix_read = ".cnf";
			string suffix_output = ".res";
			string fn1;
			cout << "Please enter the filename (Enter \"quit\" to quit) : ";
			cin >> fn1;
			while (fn1 != "quit")
			{
				string read = prefix_read + fn1 + suffix_read;
				ClsNode *cnf = read_cnf(read);
				short *ans = new short[vn + 1];
				memset(ans, 0, sizeof(short) * (vn + 1));
				/*
				for (int i = 0; i < vn + 1; i++)
				{
					cout << ans[i] << endl;
				}
				*/
				clock_t start = clock();
				bool status = DPLL(cnf, ans);
				clock_t end = clock();
				string output = prefix_output + fn1 + suffix_output;
				output_res(ans, output, status, start, end);
				if (status)
				{
					show(ans);
				}
				else
				{
					cout << "Unsatisfied!" << endl;
				}
				cout << "Duration : " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
				int improve = 0;
				cout << "Using improved algorithm? (input 1 for yes or 0 for no) : ";
				cin >> improve;
				if (improve == 1)
				{
					cnf = read_cnf(read);
					memset(ans, 0, sizeof(short) * (vn + 1));
					double temp = (double)(end - start);
					start = clock();
					bool status = newDPLL(cnf, ans);
					end = clock();
					string output = prefix_output + fn1 + suffix_output;
					output_res(ans, output, status, start, end);
					if (status)
					{
						show(ans);
					}
					else
					{
						cout << "Unsatisfied!" << endl;
					}
					cout << "Duration : " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
					cout << ((temp - (double)(end - start)) / temp) * 100 << "% " << "faster after optimization!" << endl;
					cout << "Press any key to continue" << endl;
				}
				getchar();
				getchar();
				system("cls");
				cout << "Please enter the filename (enter quit to quit) : ";
				cin >> fn1;
			}
			system("cls");
		}
		menu();
		cin >> choice;
	}
	cout << "GoodBye!";
	/**/
	return 0;
}

