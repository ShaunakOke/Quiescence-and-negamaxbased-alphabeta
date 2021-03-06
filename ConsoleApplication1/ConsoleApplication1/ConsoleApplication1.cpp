#include "pch.h"
#include <iostream>
#include <math.h>
#include<stdlib.h>
#include<ctime>
#include<algorithm>
using namespace std;
int bf = 5, horizon = 5, approx = 100, interestingness = 20;
int counterquiesc = 0, counternmax = 0;
int counternofnodes = 0, horizonthreshold = interestingness;

//node structure
struct node
{
  int eval;
  bool fl;
  node **child;
};

//initiliaze default node
node *init()
{
  node *np = new node;
  np->child = new node*[bf + 2];
  np->fl = true;
  //nodes are considered interesting by default unless proven otherwise
  for (int i = 0; i < bf + 2; i++)
  {
    np->child[i] = NULL;
  }
  return np;
};

//check if a give node has a child or not
bool haschild(node *n)
{
  //checks if leaf node
  for (int i = 0; i < bf + 2; i++)
  {
    if (n->child[i] != NULL)
    {
      return 1;
    }

  }
  return 0;
}

//generate tree 
void tree(node* x, int limit)
{
  //branching factor choosing part
  int bfselector = (rand() % 100) + 1, tbf;
  if (limit == horizon) {
    tbf = bf;
  }
  else if (bfselector < 4) {
    tbf = bf + 2;
  }
  else if (bfselector > 3 && bfselector < 8) {
    tbf = bf - 2;
  }
  else if (bfselector > 7 && bfselector < 14) {
    tbf = bf + 1;
  }
  else if (bfselector > 13 && bfselector < 21) {
    tbf = bf - 1;
  }
  else {
    tbf = bf;
  }
  if (tbf < 1)
  {
    return;
  }
  int r = (rand() % tbf) + 1;

  //checking interestingness of the given node.the way code is written limit =horizon-distance from root
  int intthreshold = (interestingness + (limit * 30));
  if (limit>0)
  {
    if ((1 + rand() % 100) > intthreshold)
    {
      x->fl = false;
      return; //since not interesting
    }
    else {

      //tree recursively building part.
      for (int i = 0; i < tbf; i++)
      {
        x->child[i] = init();
        if (i == r)
        {
          x->child[i]->eval = -(x->eval);
        }
        else 
        {
          //srand((int)time(0));
          x->child[i]->eval = (rand() % (10000 + (x->eval))) - x->eval;
          if (x->child[i]->eval >= 10000)
          {
            x->child[i]->eval = 10000;
            return;
          }
        }
        tree(x->child[i], limit - 1);
      }
      limit--;
    }
  }

 
  //reached horizon different interestingness factor used.
  if (limit<=0)
  {
    horizonthreshold = interestingness+ (limit * 2);
    //horizon threshold gets reduced by 2 for every height below horizon 
    //limit goes from 0,-1,-2,-3 and so on ,after horizon is reached.Also horizon threshold gets reduced by 2 instead of 30.
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
        x->child[i]->eval = (rand() % (10001 + (x->eval))) - x->eval ;
        if (i == r)
        {
          x->child[i]->eval = -(x->eval);
        }
        tree(x->child[i], limit - 1);
      }
    }
 
  }

}

//muddy the values to challenge the search
void approxifyinterior(node *x, int limit)
{
  if (haschild(x))
  {
    x->eval = x->eval + (rand() % (2 * approx)) - approx;
    if (x->eval >= 10000)
    {
      x->eval = 10000;
      x->fl = false;//checkmate!
      return;
    }
    for (int i = 0; i < bf + 2; i++)
    {
      //srand((int)time(0));
      if (x->child[i] != NULL) 
      {
        approxifyinterior(x->child[i], limit - 1);
      }

    }
  }
}

//display nodes
void display(node* n, int d)
{
  cout << boolalpha << "distance of node from root =" << horizon - d << " currently value is =" << n->eval;
  cout<<" node interesting ? " << n->fl << " count " << counternofnodes++ << "\n";

  if (haschild(n))
  {
    for (int i = 0; i < bf + 2; i++)
    {
      if (n->child[i] != NULL)
      {
        display(n->child[i], d - 1);
      }
    }
    d--;
  }
}

//negamax alpha beta without quiescence
int negamax(node *n, int limit, int achievable, int hope)
{
  if (limit==0 || haschild(n) == 0)
  {
    counternmax++; //global variable to count no. of static evaluations.
    return n->eval;
  }
  else
  {
    int temp = 0;
    for (int i = 0; i < bf + 2; i++)
    {
      if (n->child[i] != NULL)
      {
        temp = -negamax(n->child[i], limit - 1, -hope, -achievable);
        if (temp >= hope)
        {
          return temp;
        }

        achievable = max(temp, achievable);
      }

    }
    return achievable;
  }

}

//search nodes indicated as "interesting" at the horizon
int quiescence(node *n, int achievable, int hope)
{
  int temp, score;
  score = n->eval;
  counterquiesc++;
  if (score >= hope)
  {
    return score;
  }
  else if (haschild(n))
  {
    for (int i = 0; (i < bf + 2); i++)  //testing if children interesting
    {
      if (n->child[i] != NULL && (n->child[i]->fl = true))
      {
        counterquiesc++;
        temp = -quiescence(n->child[i], -hope, -achievable);
        if (temp > score) { score = temp; }
        if (score >= achievable) { achievable = score; }
        if (score >= hope) { break; }
      }
    }
  }
  return score;
}

//negamax alpha beta that checks for interesting nodes instead of static evaluation
int negaquiescence(node *n, int limit, int achievable, int hope)
{
  if (limit==0 || haschild(n)) 
  {
     return quiescence(n, achievable,hope);
  }
  else
  {
    int temp = 0;
    for (int i = 0; i < bf + 2; i++)
    {
      if (n->child[i] != NULL)
      {
        temp = -negaquiescence(n->child[i], limit - 1, -hope, -achievable);
        if (temp >= hope)
        {
          return temp;
        }

        achievable = max(temp, achievable);
      }
    }
    return achievable;
  }
}

//call everything!
 int main()
{
   horizon = 7, bf =7, approx = 100;
  node *root = init();
  srand((int)time(0));
  //initialize root
  int trueval;
  root->eval = root->eval = (rand() % 5000) - 2499;
  root->fl = true;

  tree(root, horizon-2);
  cout << root->eval << "\n";
  trueval = root->eval;
 
  approxifyinterior(root, horizon-2);

  //display(root, horizon); remove comments to display all nodes of the tree.

  //negamax first result without quiescence and second with
  cout << "\n";
  int nmax = negamax(root, horizon-2, -10000, 10000);
  int nqmax = negaquiescence(root, horizon-2, -10000, 10000);
  cout << nmax << "evaluations:" << counternmax << "difference" << abs(nmax - trueval) << "\n";
  cout << nqmax << "evaluations" << counterquiesc << "difference" << abs(nqmax - trueval);

  return 0;
}

