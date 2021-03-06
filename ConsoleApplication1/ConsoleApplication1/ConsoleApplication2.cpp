// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include <math.h>
#include<stdlib.h>
#include<ctime>
using namespace std;

int bf = 3, depth = 5,horizon=4,approx=100,interestingness=20;
int horizonthreshold=interestingness;
struct node
{
	int eval;
	bool fl ;
	node **child;
};
node *root = NULL;
node *init()
{
	node *np = new node;
	np->child = new node*[bf+2];
	for (int i = 0; i < bf; i++)
	{
	np->child[i] = NULL;
	np->fl = true;		//nodes are considered interesting by default unless proven otherwise
	}
	return np;
};
int count2 = 0;
bool haschild(node *n)
{
	for (int i = 0; i < bf + 2; i++)
	{
		if(n->child[i])
		{
			return 1;
		}

	}
	return 0;
}
void tree(node* x,int limit)
{
	//checking interestingness of the given node.the way code is written depth-limit is the no. of links from root to node.
	int intthreshold = (interestingness + (horizon - (depth - limit)) * 30);
	if ((depth - limit) < horizon)
	{
		if ((rand() % 100) > intthreshold)
		{
			x->fl = false;
			return; //since not interesting
		}
	}
	//branching factor choosing part
	int bfselector = (rand() % 100) + 1,tbf;
	if (x == root){
			tbf = bf;
		}
	else if (bfselector < 4){
			tbf = bf + 2;
		}
	else if(bfselector>3 && bfselector<8){
			tbf = bf - 2;
		}
	else if (bfselector > 7 && bfselector < 14){
			tbf = bf + 1;
		}
	else if (bfselector > 13 && bfselector < 21){
			tbf = bf - 1;
		}
	else{
			tbf = bf;
		}
	if (tbf < 1)
	{
		return;
	}
	int r = (rand() % tbf) + 1;

	//reached horizon different interestingness factor used.
	if (limit == 0 || (depth - limit) == horizon)
	{
		for (int i = 0; i < tbf; i++)
		{
			if (rand() % 100 > horizonthreshold)
			{
				x->fl = false;
				return;
			}
			else
			{
				x->fl = true;
				x->child[i] = init();
				x->child[i]->eval= (rand() % (10000 + (x->eval))) - x->eval;
				if (i == r)
				{
					x->child[i]->eval = -(x->eval);
				}

				horizonthreshold = horizonthreshold - 2; //new interesting threshold lowered more slowly
				tree(x->child[i], limit);
			}
		}
	}
	
	//tree recursively building part.
	
	/*if (limit == 4)
		{
			cout << ("goin in parent getting new children! ") << count2++ << "depth is at " << limit << " Parent value " << x->eval << "\n";
		}
		if (limit == 3)
		{
			cout << ("goin in parent getting new children! ") << count2++ << "depth is at " << limit << " Parent value " << x->eval<<"\n";
		}*/
	for (int i = 0; i < tbf; i++)
	{
		x->child[i] = init();
		if (i == r)
		{
			x->child[i]->eval = -(x->eval);
		}
		else {
			//srand((int)time(0));
			x->child[i]->eval = (rand()%(10000+(x->eval))) - x->eval;
			if (x->child[i]->eval > 10000)
			{
				x->child[i]->eval = 10000;
				return;
			}
			if (x->child[i]->eval < -(x->eval))
			{
				x->child[i]->eval = -(x->eval) + 1;
			}
		}
		tree(x->child[i], limit - 1);
	}
	limit--;
}

void approxifyinterior(node *x,int limit)
{
	//adds impurity to data.
	
	x->eval = x->eval + (rand() % (2 * approx)) - approx;
	if (x->eval >=10000)
	{
		x->eval = 10000;
		x->fl = false;//checkmate!
		return;
	}
	int flag = 0;
	if (x->fl && haschild(x))
	{
		for (int i = 0; i < bf + 2; i++)
		{
			//srand((int)time(0));
			if (x->child[i] != NULL) {
				approxifyinterior(x->child[i], limit - 1);
			}

		}
	}
	
}


void display(node* n,int d) 
{
	//cout << d << "\n";
	if (depth - d == 1 || depth-d==0)
	{

		cout << boolalpha << "distance of node from root =" << depth - d << " currently value is =" << n->eval << " node interesting ? " << n->fl << " count " << count2++ << "\n";
	}
	if (haschild(n))
	{
		for (int i = 0; i < bf + 2; i++)
		{
			if (n->child[i] != NULL && d >-10)
			{
				display(n->child[i], d - 1);
			}
		}
	}
	
}

int main()
{
	int j = 0;
	node *root = init();
	srand((int)time(0));
	while (j<20)
	{
		//initialize root
		root->eval = root->eval = (rand() % 5000) - 2499;
		root->fl = true;
		//builds tree with all nodes having their true search value.
		tree(root, depth);
		cout << root->eval << "\n";
		cout << root->child[0]->eval;

		//adds impurity to interior values to challenge the search
		//approxifyinterior(root, depth);		

		//display the tree
		display(root, depth);
		j++;
	}
	//
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