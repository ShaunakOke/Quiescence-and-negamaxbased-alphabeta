// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <math.h>
#include<stdlib.h>
using namespace std;
int bf = 7, depth = 7,horizon,approx,interestingness;
int i = 0;
struct node
{
	int eval;
	bool fl = 0;
	node **child;
};
node *root = NULL;
node *init()
{
	node *np = new node;
	//np->eval = int(rand()%20)-10;
		//int daught = int(rand()*bf);
	np->child = new node*[bf];
	for (i = 0; i < bf; i++)
	{
	np->child[i] = NULL;
	}
	return np;
};
int count2 = 0;
void tree(node* n,int limit)
{
	int temp,data=int(rand()*20);
	node *x = init();
	x = root;
	if (x == NULL)
	{
		root = init();
		root->eval = (rand() % 5000) - 2499;
		x = root;
	}
	if (limit == 0) { return; }
	else {
		int r = (rand() % bf) + 1;
		for (int i = 0; i < bf; i++)
		{
			if (limit == 0)
			{
				return;
			}
			x->child[i] = init();
			if (i == r) {
				x->child[i]->eval = -(x->eval);
			}
			else {
				x->child[i]->eval = (rand() % (10000+(x->eval))) - (x->eval) - 1;
				if(x->child[i]->eval==10000)
				{
					return;
				}
			}
					cout << ("goin in ! ") << count2++<<"depth is at "<<limit<<"\n";
					tree(x->child[i],limit-1);
				}
			limit--;
			}
		
		}
void display(node* n,int d) 
{
	if (n == root) {
		count2 = 0;
		cout << "at root with eval"<<n->eval<<"\n";
	}
	if (d == 0)
	{
		return;
	}
	cout << "at depth =" << d << " currently ";
	for (int i = 0; i < bf; i++)
	{
		 if (n->child[i] != NULL) {
			 
			display(n->child[i], d-1);
		}
	}
	d--;
	
}
int main()
{
	tree(root, depth);
	
	//display(root,depth);
    cout << "Hello World!\n"; 
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file