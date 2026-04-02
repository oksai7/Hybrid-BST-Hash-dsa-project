#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <ctime>


using namespace std;


double CountingSort(vector<int>& A, vector<int>& B, int n, double& space)
{
   auto start = chrono::steady_clock::now(); //start timer before the sort


   int min = *min_element(A.begin(), A.end()); //find the minium value of the array
   int max = *max_element(A.begin(), A.end()); //find the maximum value of the array


   int k; //range of count sort
   k = max - min; // range = max - min


   vector<int> C(k + 1); //intial count array of size k + 1 to store the count of each unique element of the array
   for (int i = 0; i <= k; i++)
       C[i] = 0;           //intialize the count array with 0


   for (int j = 0; j < n; j++) //array to count the occurrence of each unique elemnent in the array
       C[A[j] - min] = C[A[j] - min] + 1; //index's are based on the value of the elment in the array relative to the minimum value of the array


   for (int i = 1; i <= k; i++) //cummulative count array to store the cumulative count of each unique elemnt in the count array
       C[i] = C[i] + C[i - 1];  //add previous to current to get the cumulative count


   for (int j = n - 1; j >= 0; j--) //output array to store the sorted array
   {
       B[C[A[j] - min] - 1] = A[j]; //place the element in the output array based on its count starting from the end of the input array to mainting its stability
       C[A[j] - min] = C[A[j] - min] - 1; //decrease the count of the elemnt that you put into the output array by 1
   }
   space = (k + n) * 32; //estimation of space based on the size of the count array and the output array, where each element is 32 bits
   auto end = chrono::steady_clock::now(); //sort is finiished, so we stop the timer, and output the time taken for the sort to complete
   chrono::duration<double> time = end - start;
   return time.count();
}


void PopulateVector(int size, vector <int>& a, int type) //functions to build the different types of input arrays for benchmarking
{
   if (type == 0)  // The program generates array, ascending
   {
       for (int i = 0; i < size; i++) //sorted array
       {
           a.push_back(i + 1);
       }
   }


   if (type == 1)  // The program generates array, descending
   {
       for (int i = size; i > 0; i--) //inverse sorted array
       {
           a.push_back(i);
       }
   }


   if (type == 2)  // The program generates array, ascending with skew
   {
       for (int i = 0; i < size - 1; i++) //sorted with skew
       {
           a.push_back(i + 1);
       }
       a.push_back(40000000); //drastically increases the range of the input array
   }


   if (type == 3)  // The program generates array, descending with skew
   {
       for (int i = size; i > 1; i--) //inverse sorted with skew
       {
           a.push_back(i);
       }
       a.push_back(-40000000);//drastically increases the range of the input array
   }


   if (type == 4)  // The program generates array, random
   {
       for (int i = 0; i < size; i++) //random array
       {
           a.push_back(rand());
       }
   }


   if (type == 5)  // The program generates array, random with some duplicates
   {
       for (int i = 0; i < size; i++)
       {
           a.push_back(rand() % 100);
       }
   }


   if (type == 6)  // The program generates array, random with many duplicates
   {
       for (int i = 0; i < size; i++)
       {
           a.push_back(rand() % 20);
       }
   }
   if (type == 7) // The program generates array, random with many duplicates and a skew
   {
       for (int i = 0; i < size - 1; i++)
       {
           a.push_back(rand() % 20); //reduce the range of random numbers so that the skew has a larger impact on range
       }
       a.push_back(40000000); //drastically increases the range of the input array
   }
}


int main()
{
   srand(static_cast<unsigned int>(time(nullptr)));
   vector<int> a; //intialize vector to store the input array for sorting
   cout << "Set Type \t\tSize   \t\tTime (s)  \tSpace(bits)" << endl;
   for (int test = 10; test < 1500000; test = test * 2) //testing the counting sort with all different types of input arrays and different sizes,and outputing the average time taken and space used for each type and size of input array
   {                                                   // 10, 20, 40, .... 1500000 for n tested
       for (int type = 0; type < 8; type++)
       {
           double space = 0, totalSpace = 0;
           double seconds = 0;
           if (type == 0)
               cout << "\nSorted           \t";
           if (type == 1)
               cout << "\nInverse          \t";
           if (type == 2)
               cout << "\nSrt w/ Skw       \t";
           if (type == 3)
               cout << "\nInv w/ Skw       \t";
           if (type == 4)
               cout << "\nRandom           \t";
           if (type == 5)
               cout << "\nRandom w/ Dupes  \t";
           if (type == 6)
               cout << "\nRandom w/ ++Dupes\t";
           if (type == 7)
               cout << "\nRand w/++Dup &Skw\t";
           for (int i = 0; i < 10; i++) //run each type 10 times and take the average after
           {
               PopulateVector(test, a, type); //populate the input array based on the type and size being tested
               vector<int> srtA(a.size()); //intialize vector to store the output of the counting sort, which is the sorted array
               seconds = seconds + CountingSort(a, srtA, test, space); //acculates the time of the 10 runs
               totalSpace = space + totalSpace; //accumulates the space of the 10 runs
               a.clear(); //clear input array
               srtA.clear(); //clear output array
           }
           cout << test;
           cout << "\t\t" << seconds / 10; //average time for the input type
           cout << "   \t" << totalSpace / 10; //average space for the input type
       }
       cout << endl;
   }
   return 0;
}



