#ifndef SET_H_INCLUDED
#define SET_H_INCLUDED

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

extern void print_ext(vector<int> vec);
extern bool find_element(int iNum, vector<int> vec);
extern bool isSubset(vector<int> vecSup, vector<int> vecSrc);
extern void get_subsets(vector<int> vecSrc, vector<vector<int>> &vecPowerSet);

#endif // SET_H_INCLUDED
