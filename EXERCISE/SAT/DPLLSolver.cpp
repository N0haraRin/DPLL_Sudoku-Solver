#include "global.h"

extern int vn;

int findSC(ClsNode* cnf)
{
	while (cnf)
	{
		if (cnf->varnum == 1)
		{
			//cout << "Find : " << cnf->ClsHead->var << endl; // test
			return cnf->ClsHead->var;
		}
		cnf = cnf->down;
	}
	//cout << "No single clause!" << endl;
	return 0;
}

void removeCls(ClsNode*& cnf, ClsNode* target)
{
	if (!cnf)
	{
		return;
	}
	if (cnf == target)
	{
		VarNode* cur = target->ClsHead;
		while (cur)
		{
			int var = cur->var;
			cur = cur->next;
		}
		//cout << "Remove : " << target->ClsHead->var << endl; // test
		cnf = cnf->down;
	}
	else
	{
		ClsNode* temp = cnf;
		ClsNode* current = cnf->down;
		while (current)
		{
			if (current == target)
			{
				VarNode* cur = target->ClsHead;
				while (cur)
				{
					cur = cur->next;
				}
				//cout << "Remove : " << target->ClsHead->var << endl; // test
				cnf->down = current->down;
				cnf = temp;
				return;
			}
			cnf = current;
			current = current->down;
		}
		cnf = temp;
	}
	return;
}

void removeSC(ClsNode*& cnf, int var)
{
	if (!cnf)
	{
		return;
	}
	while (cnf && cnf->varnum == 1 && cnf->ClsHead->var == var)
	{
		removeCls(cnf, cnf);
		return;
	}
	if (!cnf)
	{
		return;
	}
	ClsNode* head = cnf;
	ClsNode* current = cnf->down;
	while (current)
	{
		if (current->varnum == 1 && current->ClsHead->var == var)
		{
			removeCls(cnf, current);
			cnf = head;
			break;
		}
		cnf = current;
		current = current->down;
	}
	// test
	 /*
	current = head;
	while (current)
	{
		cout << current->varnum << " : ";
		VarNode* cur = current->ClsHead;
		while (cur)
		{
			cout << cur->var;
			cout << " ";
			cur = cur->next;
		}
		cout << endl;
		current = current->down;
	}
	*/
}

void removeVar(ClsNode*& cnf, int var)
{
	if (!cnf)
	{
		return;
	}
	ClsNode* current = cnf;
	while (current)
	{
		ClsNode* temp = current->down;
		VarNode* prev, * cur;
		prev = cur = nullptr;
		if (current->ClsHead->var == var)
		{
			removeCls(cnf, current);
			current = temp;
			continue;
		}
		else if (current->ClsHead->var == -var)
		{
			int t_var = current->ClsHead->var;
			current->ClsHead = current->ClsHead->next;
			current->varnum--;
		}
		if (current->varnum)
		{
			prev = current->ClsHead;
			cur = current->ClsHead->next;
			while (cur)
			{
				if (cur->var == var)
				{
					removeCls(cnf, current);
					break;
				}
				else if (cur->var == -var)
				{
					prev->next = cur->next;
					current->varnum--;
				}
				prev = cur;
				if (cur)
				{
					cur = prev->next;
				}
			}
		}
		current = temp;
	}
	// test
	/*
	current = cnf;
	cout << "After remove variable " << var << " : " << endl;
	while (current)
	{
		cout << current->varnum << " : ";
		VarNode* cur = current->ClsHead;
		while (cur)
		{
			cout << cur->var;
			cout << " ";
			cur = cur->next;
		}
		cout << endl;
		current = current->down;
	}
	*/
}

bool hasEmpty(ClsNode* cnf)
{
	while (cnf)
	{
		if (cnf->varnum == 0 || cnf->ClsHead == nullptr)
		{
			return true;
		}
		cnf = cnf->down;
	}
	return false;
}

ClsNode* addVar(ClsNode* cnf, int var)
{
	//cout << "Add variable " << var << endl;
	ClsNode* newhead = new ClsNode;
	newhead->varnum = 1;
	newhead->ClsHead = new VarNode;
	newhead->ClsHead->var = var;
	newhead->ClsHead->next = NULL;
	newhead->down = cnf;
	cnf = newhead;
	return cnf;
}

ClsNode* copy(ClsNode* cnf)
{
	ClsNode* current = cnf;
	ClsNode* newhead, * newtail;
	newhead = newtail = nullptr;
	while (current)
	{
		VarNode* cur = current->ClsHead;
		VarNode* head, * tail;
		head = tail = nullptr;
		while (cur)
		{
			if (!head)
			{
				head = new VarNode;
				head->next = nullptr;
				head->var = cur->var;
				tail = head;
			}
			else
			{
				tail->next = new VarNode;
				tail->next->next = nullptr;
				tail->next->var = cur->var;
				tail = tail->next;
			}
			cur = cur->next;
		}
		if (!newhead)
		{
			newhead = new ClsNode;
			newhead->down = nullptr;
			newhead->ClsHead = head;
			newhead->varnum = current->varnum;
			newtail = newhead;
		}
		else
		{
			newtail->down = new ClsNode;
			newtail->down->down = nullptr;
			newtail->down->ClsHead = head;
			newtail->down->varnum = current->varnum;
			newtail = newtail->down;
		}
		current = current->down;
	}
	return newhead;
}

int findMinlength(ClsNode* cnf)
{
	ClsNode* current = cnf;
	ClsNode* target = cnf;
	int min = INT_MAX;
	while (current)
	{
		if (current->varnum < min)
		{
			target = current;
			min = current->varnum;
		}
		current = current->down;
	}
	return -target->ClsHead->var;
}

bool DPLL(ClsNode* cnf, short ans[])
{
	int target_var = findSC(cnf); // find single clause
	while (target_var)
	{
		removeSC(cnf, target_var);
		removeVar(cnf, target_var);
		ans[target_var > 0 ? target_var : -target_var] = target_var > 0 ? 1 : -1;
		if (!cnf)
		{
			cout << "Solved!" << endl;
			return true;
		}
		else if (hasEmpty(cnf))
		{
			return false;
		}
		target_var = findSC(cnf);
	}
	// test
	/**/
	int newnode = cnf->ClsHead->var;
	ClsNode* cnf_copy = copy(cnf);
	ClsNode* cnf_true = addVar(cnf, newnode);
	if (DPLL(cnf_true, ans))
	{
		return true;
	}
	else
	{
		ClsNode* cnf_false = addVar(cnf_copy, -newnode);
		return DPLL(cnf_false, ans);
	}
	/**/
}

bool newDPLL(ClsNode* cnf, short ans[])
{
	int target_var = findSC(cnf); // find single clause
	while (target_var)
	{
		removeSC(cnf, target_var);
		removeVar(cnf, target_var);
		ans[target_var > 0 ? target_var : -target_var] = target_var > 0 ? 1 : -1;
		if (!cnf)
		{
			cout << "Solved!" << endl;
			return true;
		}
		else if (hasEmpty(cnf))
		{
			return false;
		}
		target_var = findSC(cnf);
	}
	// test
	/**/
	int newnode = findMinlength(cnf);
	ClsNode* cnf_copy = copy(cnf);
	ClsNode* cnf_true = addVar(cnf, newnode);
	if (newDPLL(cnf_true, ans))
	{
		return true;
	}
	else
	{
		ClsNode* cnf_false = addVar(cnf_copy, -newnode);
		return newDPLL(cnf_false, ans);
	}
	/**/
}

void output_res(short ans[], string& filename, bool status, clock_t start, clock_t end)
{
	if (status)
	{
		ofstream fp(filename);
		fp << "s" << " " << "1" << endl;
		fp << "v" << " ";
		for (int i = 1; i < vn + 1; i++)
		{
			if (ans[i] > 0)
			{
				fp << i << " ";
			}
			else
			{
				fp << -i << " ";
			}
		}
		fp << endl;
		fp << "t" << " " << (end - start) << "ms";
	}
	else
	{
		ofstream fp(filename);
		fp << "s" << " " << "-1" << endl;
		fp << "v" << endl;
		fp << "t" << " " << (end - start) << "ms";
	}
}

void show(short ans[])
{
	for (int i = 1; i < vn + 1; i++)
	{
		cout << i << " : ";
		if (ans[i] > 0)
		{
			cout << "true";
		}
		else
		{
			cout << "false";
		}
		cout << endl;
	}
}
