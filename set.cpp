#include "set.h"

void print_ext(vector<int> vec)
{
    int i;
    cout<<"-";
    for(i = 0; i < (int)vec.size(); i++)
    {
        cout<<vec[i]<<" ";
    }
    cout<<endl;
}

bool find_element(int iNum, vector<int> vec)
{
    int i;
    for(i = 0; i < (int)vec.size(); i++)
    {
        if(iNum == vec[i])
        {
            return true;
        }
    }
    return false;
}

bool isSubset(vector<int> vecSup, vector<int> vecSrc)
{
	int i = 0,j = 0,m = vecSup.size(),n = vecSrc.size();
	if(m < n)
    {
		return false;
	}
	sort(vecSup.begin(),vecSup.end());
	sort(vecSrc.begin(),vecSrc.end());
	while(i < n && j < m)
	{
		if(vecSup[j] < vecSrc[i])
		{
			j++;
		}
		else if(vecSup[j] > vecSrc[i])
		{
			return false;
		}
		else
        {
            j++;
			i++;
        }
	}
	return n == i;
}

void get_subsets(vector<int> vecSrc, vector<vector<int>> &vecPowerSet)
{
    int i = 0, j = 0;
	bool flag = false;
    vector<int> vecSubset = {};
    if(0 == vecSrc.size()) return;
    vecPowerSet.clear();
	vecPowerSet.push_back(vecSubset);
	for(i = 1; !flag ; i++)
	{
		flag = true;
		vecSubset.clear();
		for(j = 0; j < (int)vecSrc.size(); j++)
		{
			if( (i >> j) & 1)
            {

                vecSubset.push_back(vecSrc[j]);
            }
			else
            {
                flag = false;
            }

		}
        vecPowerSet.push_back(vecSubset);
	}
}
