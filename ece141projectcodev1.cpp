#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

int VECTYPE = 0;

struct Node
{
	int value;
	int frequency;
	string color;
	Node* left, * right, * parent;

	Node(int data)
	{
		value = data;
		color = "RED";
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}
};

double countingSort(vector<int>& A, vector<int>& B)
{
	auto start = chrono::steady_clock::now();
	int k, min, max;
	int n = A.size();
	min = *min_element(A.begin(), A.end());
	max = *max_element(A.begin(), A.end());
	k = max - min;

	vector<int> C(k + 1);
	for (int i = 0; i <= k; i++)
		C[i] = 0;

	for (int j = 0; j < n; j++)
		C[A[j] - min] = C[A[j] - min] + 1;

	for (int i = 1; i <= k; i++)
		C[i] = C[i] + C[i - 1];

	for (int j = n - 1; j >= 0; j--)
	{
		B[C[A[j] - min] - 1] = A[j];
		C[A[j] - min] = C[A[j] - min] - 1;
	}
	auto end = chrono::steady_clock::now();
	chrono::duration<double> time = end - start;
	return time.count();	// returns the time to sort
}

void PopulateVectors(int size, vector <int>& a)
// Populates the vectors based on user input and the Run
{
	if (VECTYPE == 0)	// The program generates array, ascending
	{
		for (int i = 0; i < size; i++)
		{
			a.push_back(i + 1);
		}
	}

	if (VECTYPE == 1)	// The program generates array, descending
	{
		for (int i = size; i > 0; i--)
		{
			a.push_back(i);
		}
	}

	if (VECTYPE == 2)	// The program generates array, ascending with skew
	{
		for (int i = 0; i < size - 1; i++)
		{
			a.push_back(i + 1);
		}
		a.push_back(400000000);
	}

	if (VECTYPE == 3)	// The program generates array, descending with skew
	{
		for (int i = size; i > 1; i--)
		{
			a.push_back(i);
		}
		a.push_back(-400000000);
	}

	if (VECTYPE == 4)	// The program generates array, random
	{
		for (int i = 0; i < size; i++)
		{
			a.push_back(rand());
		}
	}
}


int main()
{
	for (int test = 10; test <= 1000000; test = test * 10)
	{

	}
	return 0;
}
