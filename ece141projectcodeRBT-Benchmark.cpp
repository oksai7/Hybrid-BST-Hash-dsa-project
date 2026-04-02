#include <iostream>		// input/output
#include <cstdlib>		// rand, srand
#include <vector>		// dynamic arrays
#include <chrono>		// timing
#include <random>		// rand
#include <ctime>		// time ()

using namespace std;

// NODE STRUCT
struct Node
{
	int value;              // stored key
	string color;           // RED or BLACK
	Node* left;             // left child
	Node* right;            // right child
	Node* parent;           // parent pointer

	Node(int data)
	{
		value = data;		// initialize value
		color = "RED";		// new nodes start RED

		left = nullptr;		// initialize pointers
		right = nullptr;
		parent = nullptr;
	}
};

// RED BLACK TREE CLASS
class RedBlackTree
{
private:
	Node* root;		// root node
	Node* NIL;		// sentinel node (leaf)
	int curSize;	// number of nodes

	// LEFT ROTATION
	void LeftRotate(Node* x)
	{
		Node* y = x->right;		// set y

		x->right = y->left;		// turn y's left subtree into x's right

		if (y->left != NIL)		// update parent if needed
			y->left->parent = x;

		y->parent = x->parent;	// link y to x's parent

		if (x->parent == nullptr || x->parent == NIL)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;

		y->left = x;			// put x on left of y
		x->parent = y;			// update parent
	}

	// RIGHT ROTATION
	void RightRotate(Node* x)
	{
		Node* y = x->left;		// symmetrical to left rotate

		x->left = y->right;

		if (y->right != NIL)
			y->right->parent = x;

		y->parent = x->parent;

		if (x->parent == nullptr || x->parent == NIL)
			root = y;
		else if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;

		y->right = x;
		x->parent = y;
	}

	// FIX INSERT (restores RB properties after insertion)
	void FixInsert(Node* k)
	{
		Node* u;	// uncle node

		while (k->parent != nullptr && k->parent->parent != nullptr && k->parent->color == "RED") // while parent exists AND parent is red (violation)
		{
			// parent is left child of grandparent
			if (k->parent == k->parent->parent->left)
			{
				u = k->parent->parent->right;	// uncle
				// uncle is red = recolor
				if (u != NIL && u->color == "RED")
				{
					k->parent->color = "BLACK";		// parent black
					u->color = "BLACK";				// uncle black
					k->parent->parent->color = "RED";	// grandparent red
					k = k->parent->parent;			// move up
				}
				else
				{
					// triangle = rotate
					if (k == k->parent->right)
					{
						k = k->parent;
						LeftRotate(k);
					}
					// line = rotate + recolor
					k->parent->color = "BLACK";
					k->parent->parent->color = "RED";
					RightRotate(k->parent->parent);
				}
			}
			else
			{
				// parent is right child
				u = k->parent->parent->left;

				if (u != NIL && u->color == "RED")
				{
					// recolor case
					k->parent->color = "BLACK";
					u->color = "BLACK";
					k->parent->parent->color = "RED";
					k = k->parent->parent;
				}
				else
				{
					// rotation case
					if (k == k->parent->left)
					{
						k = k->parent;
						RightRotate(k);
					}

					k->parent->color = "BLACK";
					k->parent->parent->color = "RED";
					LeftRotate(k->parent->parent);
				}
			}

			if (k == root) break;	// stop at root
		}

		root->color = "BLACK";	// root always black
	}

public:
	// CONSTRUCTOR
	RedBlackTree()
	{
		NIL = new Node(0);		// create sentinel
		NIL->color = "BLACK";	// NIL always black

		NIL->left = NIL;		// point to self
		NIL->right = NIL;

		root = NIL;				 // empty tree
		curSize = 0;
	}

	// INSERT NODE
	void Insert(int val)
	{
		Node* newNode = new Node(val); // create node

		newNode->left = NIL;
		newNode->right = NIL;

		Node* parent = nullptr;
		Node* cur = root;

		// BST search
		while (cur != NIL)
		{
			parent = cur;

			if (newNode->value < cur->value)
				cur = cur->left;
			else
				cur = cur->right;
		}

		newNode->parent = parent;

		// attach node
		if (parent == nullptr)
			root = newNode;
		else if (newNode->value < parent->value)
			parent->left = newNode;
		else
			parent->right = newNode;

		curSize++; // increment size

		// if root
		if (newNode->parent == nullptr)
		{
			newNode->color = "BLACK";
			return;
		}

		// if no grandparent
		if (newNode->parent->parent == nullptr)
			return;

		FixInsert(newNode); // restore properties
	}

	// INORDER TRAVERSAL
	void InOrderTraversal(Node* root, vector<int>& S, int n)
	{
		if (root != NIL)
		{
			InOrderTraversal(root->left, S, n);	// call on left

			S.reserve(n);						 // optimize allocation
			S.push_back(root->value);		     // visit node
			InOrderTraversal(root->right, S, n); // call on right
		}
	}

	Node* GetRoot()
	{
		return root; // return root
	}

	int GetSize()
	{
		return curSize; // return size
	}
};

// RBT SORT FUNCTION
double RBTSort(vector<int>& A, vector<int>& S, int n, double& space)
{
	auto start = chrono::steady_clock::now(); // start timer

	RedBlackTree rbt; // create tree

	for (int i = 0; i < n; i++)
		rbt.Insert(A[i]); // insert all values

	space = rbt.GetSize(); // node count

	space = (space * ((3 * 64.0) + 36 + 32.0)) + (2 * 64) + 32;

	rbt.InOrderTraversal(rbt.GetRoot(), S, n); // sorted output

	auto end = chrono::steady_clock::now(); // end timer

	chrono::duration<double> time = end - start;

	return time.count(); // return runtime
}

// VECTOR GENERATOR
void PopulateVector(int size, vector<int>& a, int type)
{
	if (type == 0) // ascending
	{
		for (int i = 0; i < size; i++)
			a.push_back(i + 1);
	}

	if (type == 1) // descending
	{
		for (int i = size; i > 0; i--)
			a.push_back(i);
	}

	if (type == 2) // ascending with skew
	{
		for (int i = 0; i < size - 1; i++)
			a.push_back(i + 1);

		a.push_back(40000000);
	}

	if (type == 3) // descending with skew
	{
		for (int i = size; i > 1; i--)
			a.push_back(i);

		a.push_back(-40000000);
	}

	if (type == 4) // random
	{
		for (int i = 0; i < size; i++)
			a.push_back(rand());
	}

	if (type == 5) // random with duplicates
	{
		for (int i = 0; i < size; i++)
			a.push_back(rand() % 100);
	}

	if (type == 6) // random with many duplicates
	{
		for (int i = 0; i < size; i++)
			a.push_back(rand() % 20);
	}

	if (type == 7) // random with many duplicates + skew
	{
		for (int i = 0; i < size - 1; i++)
			a.push_back(rand() % 20);

		a.push_back(40000000);
	}
}

// MAIN FUNCTION
int main()
{
	srand(static_cast<unsigned int>(time(nullptr))); // seed RNG

	vector<int> a; // input array

	cout << "Set Type \t\tSize   \t\tTime (s)  \tSpace(bits)" << endl;

	// test increasing sizes
	for (int test = 10; test < 1500000; test = test * 2)
	{
		// test all distributions
		for (int type = 0; type < 8; type++)
		{
			double totalSpace = 0, space = 0;
			double seconds = 0;

			// label output
			if (type == 0) cout << "\nSorted           \t";
			if (type == 1) cout << "\nInverse          \t";
			if (type == 2) cout << "\nSrt w/ Skw       \t";
			if (type == 3) cout << "\nInv w/ Skw       \t";
			if (type == 4) cout << "\nRandom           \t";
			if (type == 5) cout << "\nRandom w/ Dupes  \t";
			if (type == 6) cout << "\nRandom w/ ++Dupes\t";
			if (type == 7) cout << "\nRand w/++Dup &Skw\t";

			// average over 10 runs
			for (int i = 0; i < 10; i++)
			{
				vector<int> srtT;

				a.clear();
				srtT.clear(); // tree result

				PopulateVector(test, a, type); // generate input

				seconds = RBTSort(a, srtT, test, space);

				totalSpace += space;
			}
			// outputs
			cout << test;
			cout << "\t\t" << seconds / 10;
			cout << "   \t" << totalSpace;
		}

		cout << endl;
	}

	return 0;
}
