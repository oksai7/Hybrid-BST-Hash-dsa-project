#include <iostream>   // input/output
#include <cstdlib>    // rand, srand
#include <vector>     // dynamic arrays
#include <chrono>     // timing
#include <algorithm>  // min_element, max_element
#include <random>     // rand
#include <ctime>      // time()
#include <stack>      // iterative traversal stack

using namespace std;

// NODE STRUCT
struct Node
{
	int value;            // stored key
	int frequency;        // handles duplicates
	bool isRed;           // true = red, false = black

	Node* left;           // left child
	Node* right;          // right child
	Node* parent;         // parent pointer

	Node(int data)
	{
		value = data;        // initialize value
		isRed = true;        // new nodes start RED
		frequency = 1;       // first occurrence

		left = nullptr;      // initialize pointers
		right = nullptr;
		parent = nullptr;
	}
};

// RBT CLASS
class RedBlackTree
{
private:
	Node* root;   // root node
	Node* NIL;    // sentinel node (represents leaves)
	int curSize;  // number of unique nodes

	// LEFT ROTATION
	void LeftRotate(Node* x)
	{
		Node* y = x->right;     // set y

		x->right = y->left;     // turn y's left subtree into x's right

		if (y->left != NIL)      // update parent if needed
			y->left->parent = x;

		y->parent = x->parent;   // link y to x's parent

		if (x->parent == nullptr) // x was root
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;

		y->left = x;            // put x on left of y
		x->parent = y;          // update parent
	}

	// RIGHT ROTATION
	void RightRotate(Node* x)
	{
		Node* y = x->left;       // symmetric to left rotate

		x->left = y->right;      // move subtree

		if (y->right != NIL)
			y->right->parent = x;

		y->parent = x->parent;

		if (x->parent == nullptr)
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
		Node* u; // uncle node

		// while parent exists AND parent is red (violation)
		while (k->parent != nullptr && k->parent->isRed)
		{
			// parent is left child of grandparent
			if (k->parent == k->parent->parent->left)
			{
				u = k->parent->parent->right; // uncle

				// uncle is red = recolor
				if (u != NIL && u->isRed)
				{
					k->parent->isRed = false;              // parent black
					u->isRed = false;                      // uncle black
					k->parent->parent->isRed = true;       // grandparent red
					k = k->parent->parent;                 // move up
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
					k->parent->isRed = false;
					k->parent->parent->isRed = true;
					RightRotate(k->parent->parent);
				}
			}
			else
			{
				// parent is right child
				u = k->parent->parent->left;

				if (u != NIL && u->isRed)
				{
					// recolor case
					k->parent->isRed = false;
					u->isRed = false;
					k->parent->parent->isRed = true;
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

					k->parent->isRed = false;
					k->parent->parent->isRed = true;
					LeftRotate(k->parent->parent);
				}
			}

			if (k == root) break; // stop at root
		}

		root->isRed = false; // root always black
	}

public:
	// CONSTRUCTOR
	RedBlackTree()
	{
		NIL = new Node(0);   // create sentinel
		NIL->isRed = false;  // NIL is always black

		NIL->left = NIL;     // point to itself
		NIL->right = NIL;

		root = NIL;          // empty tree
		curSize = 0;
	}

	// INSERT NODE
	void Insert(int val)
	{
		Node* cur = root;        // start at root
		Node* parent = nullptr;  // track parent

		// BST search
		while (cur != NIL)
		{
			if (val == cur->value)
			{
				cur->frequency++; // duplicate handling
				return;
			}

			parent = cur;

			if (val < cur->value)
				cur = cur->left;
			else
				cur = cur->right;
		}

		Node* newNode = new Node(val); // create node

		newNode->parent = parent;
		newNode->left = NIL;
		newNode->right = NIL;

		// attach to tree
		if (parent == nullptr)
			root = newNode;
		else if (val < parent->value)
			parent->left = newNode;
		else
			parent->right = newNode;

		curSize++; // increase node count

		// if root
		if (newNode->parent == nullptr)
		{
			newNode->isRed = false;
			return;
		}

		// if no grandparent
		if (newNode->parent->parent == nullptr)
			return;

		FixInsert(newNode); // restore properties
	}

	// ITERATIVE INORDER TRAVERSAL
	void InOrderTraversal(Node* root, vector<int>& S, int n)
	{
		S.reserve(n); // optimize memory

		stack<Node*> st; // explicit stack
		Node* curr = root;

		while (curr != NIL || !st.empty())
		{
			while (curr != NIL)
			{
				st.push(curr);   // go left
				curr = curr->left;
			}

			curr = st.top();     // visit node
			st.pop();

			// push duplicates
			for (int i = 0; i < curr->frequency; i++)
				S.push_back(curr->value);

			curr = curr->right;  // go right
		}
	}

	Node* GetRoot() { return root; }  // return root
	int GetSize() { return curSize; } // return size
};

// HYBRID SORT FUNCTION
double CountingSortHybrid(vector<int>& A, vector<int>& B, vector<int>& S, int n, double& space)
{
	auto start = chrono::steady_clock::now(); // start timer

	bool useTree; // decision flag

	int minVal = *min_element(A.begin(), A.end()); // min value
	int maxVal = *max_element(A.begin(), A.end()); // max value

	// decision rule
	if ((maxVal - minVal) > n * log(n))
		useTree = true;
	else
		useTree = false;

	// COUNTING SORT
	if (!useTree)
	{
		int k = maxVal - minVal; // range size

		vector<int> C(k + 1, 0); // counting array

		for (int j = 0; j < n; j++)
			C[A[j] - minVal]++; // count frequency

		for (int i = 1; i <= k; i++)
			C[i] += C[i - 1]; // prefix sum

		for (int j = n - 1; j >= 0; j--)
		{
			B[C[A[j] - minVal] - 1] = A[j]; // output
			C[A[j] - minVal]--;             // decrement
		}

		space = ((k + 1) + n) * 32; // space estimate
	}

	// TREE SORT
	if (useTree)
	{
		RedBlackTree rbt; // create tree

		for (int i = 0; i < n; i++)
			rbt.Insert(A[i]); // insert all values

		rbt.InOrderTraversal(rbt.GetRoot(), S, n); // extract sorted

		space = rbt.GetSize(); // node count

		space = (space * ((3 * 64.0) + 8 + (2 * 32.0))) + (2 * 64) + 32;
	}

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


// MAIN
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
			double totalTime = 0;
			double totalSpace = 0;

			// label output

			if (type == 0) cout << "\nSorted           \t";
			if (type == 1) cout << "\nInverse          \t";
			if (type == 2) cout << "\nSrt w/ Skw       \t";
			if (type == 3) cout << "\nInv w/ Skw       \t";
			if (type == 4) cout << "\nRandom           \t";
			if (type == 5) cout << "\nRandom w/ Dupes  \t";
			if (type == 6) cout << "\nRandom w/++Dupes \t";
			if (type == 7) cout << "\nRand w/++Dup &Skw\t";

			// average over 10 runs
			for (int i = 0; i < 10; i++)
			{
				PopulateVector(test, a, type); // generate input

				vector<int> sortedA(a.size()); // output array
				vector<int> treeOutput;        // tree result

				double space = 0;

				totalTime += CountingSortHybrid(a, sortedA, treeOutput, test, space);
				totalSpace += space;

				a.clear();
				sortedA.clear();
				treeOutput.clear();
			}
			// outputs
			cout << test;
			cout << "\t\t" << totalTime / 10;
			cout << "   \t" << totalSpace / 10;
		}

		cout << endl;
	}

	return 0;
}

