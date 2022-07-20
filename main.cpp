/*
	Name: Solutions for the graded semantics of AAFs
	Author: Lixing Tan
	Date: 2021/4/30
	Description: cf, na, ad, co, gr, pr and stb
*/
#include "set.h"
#include "semanticsUtils.h"

/*Global variable*/
const int MAXN = 1000 + 5;
int iArgs = 0;                    //The number of arguments
int AAF[MAXN][MAXN];   			  //The adjacency matrix of AAF

/*AAF diagram: conversion between characters and numbers, e.g., a <--> 1*/
map<string, int> vecStr2Num;
map<int, string> vecNum2Str;

/** \brief Initialization of AAF
 *
 * \param void
 * \return void
 *
 */
void init()
{
    memset(AAF, 0, sizeof AAF);
    string str;
    while(cin >> str)
    {
        int l_bracket = str.find('('), r_bracket = str.find(')'), comma = str.find(',');
        if(str.substr(0,4) == "arg(")
        {
            int r_bracket = str.find(')');
            string point = str.substr(l_bracket + 1, r_bracket - l_bracket  - 1);
            ++iArgs;
            vecStr2Num[point] = iArgs;
            vecNum2Str[iArgs] = point;
        }
        if(str.substr(0,4) == "att(")
        {
            //Find the attacker and victims
            string attacker = str.substr(l_bracket + 1, comma - l_bracket  - 1);
            string victim = str.substr(comma + 1, r_bracket - (comma + 1));
            if (!vecStr2Num[attacker] || !vecStr2Num[victim]) continue;
            AAF[vecStr2Num[attacker]][vecStr2Num[victim]]++;
        }
    }
}

/** \brief print a given semantics
 *
 * \param a semantics
 * \return void
 *
 */
void print_solution(vector<vector<int> > semantics)
{
    int i;
    for(i = 0; i < (int)semantics.size(); ++i)
    {
        cout << "-";
        for(int j = 0; j < (int)semantics[i].size(); ++j)
        {
            cout << vecNum2Str[semantics[i][j]] << " ";
        }
        cout << endl;
    }
}

void neutrality_set(int t, vector<int> vecE, vector<int> &vecN)
{
    int i,j,iCount;
    vecN.clear();
    for(i = 1; i <= iArgs; i++)
    {
        iCount = 0;
        for(j = 0; j < (int)vecE.size(); j++)
        {
            if(AAF[vecE[j]][i])
            {
                ++iCount;
            }
        }
        if(iCount >= t)
        {
            continue;
        }
        vecN.push_back(i);
    }
}

void defense_set(int m, int n, vector<int> vecE, vector<int> &vecD)
{
    vector<int> vecNE;
    neutrality_set(n, vecE, vecNE);
    neutrality_set(m, vecNE, vecD);
}

vector<int> argument_set()
{
    int i;
    vector<int> vecSrc = {};
    for(i = 1; i <= iArgs; i++)
    {
        vecSrc.push_back(i);
    }
    return vecSrc;
}

void iteration_of_defense(int m, int n, vector<int> &vecDD)
{
    vector<int> vecD;
    do
    {
        vecD.assign(vecDD.begin(),vecDD.end());
        defense_set(m,n,vecD,vecDD);
    }
    while(!compare_extension(vecD,vecDD));
}

bool isConflictFree(int l, vector<int> vecE)
{
    vector<int> vecNE;
    neutrality_set(l, vecE, vecNE);
    return isSubset(vecNE,vecE);
}

bool isSelfDefense(int m, int n, vector<int> vecE)
{
    vector<int> vecDE;
    defense_set(m,n,vecE,vecDE);
    return isSubset(vecDE,vecE);
}

bool isFixpoint(int l, int m, int n, vector<int> vecDD)
{
    vector<int> vecD;
    vecD.assign(vecDD.begin(),vecDD.end());
    iteration_of_defense(m, n, vecDD);
    if(!compare_extension(vecD,vecDD)) return false;

    return isConflictFree(l,vecD);
}

void cf_solution(int l, vector<vector<int> > &cf)
{
    int i;
    vector<vector<int>> vecPowerSet;
    vector<int> vecSrc = argument_set();
    get_subsets(vecSrc, vecPowerSet);
    for(i = 0; i < (int)vecPowerSet.size(); i++)
    {
        if(isConflictFree(l,vecPowerSet[i]))
        {
            sort(vecPowerSet[i].begin(),vecPowerSet[i].end());
            cf.push_back(vecPowerSet[i]);
        }
    }
}

void def_solution(int m, int n, vector<vector<int>> &def)
{
    int i;
    vector<vector<int>> vecPowerSet;
    vector<int> vecSrc = argument_set();
    get_subsets(vecSrc, vecPowerSet);
    for(i = 0; i < (int)vecPowerSet.size(); i++)
    {
        if(isSelfDefense(m,n,vecPowerSet[i]))
        {
            def.push_back(vecPowerSet[i]);
        }
    }
}

void fixpoint_solution(int m, int n, vector<vector<int>> &fp)
{
    int i;
    vector<vector<int>> vecPowerSet;
    vector<int> vecSrc = argument_set();
    get_subsets(vecSrc, vecPowerSet);
    for(i = 0; i < (int)vecPowerSet.size(); i++)
    {
        iteration_of_defense(m, n, vecPowerSet[i]);
        fp.push_back(vecPowerSet[i]);
    }
    sort(fp.begin(), fp.end());
    fp.erase(unique(fp.begin(), fp.end()), fp.end());
}

void ad_solution(int m, int n, vector<vector<int>> cf, vector<vector<int>> &ad)
{
    int i, j;
    vector<int> vecD;
    for(i = 0; i < (int)cf.size(); i++)
    {
        vecD.clear();
        defense_set(m, n, cf[i], vecD);
        for(j = 0; j < (int)cf[i].size(); j++)
        {
            if(!find_element(cf[i][j], vecD)) break;
        }
        if(j == (int)cf[i].size()) ad.push_back(cf[i]);
    }
}

void co_solution(int l, int m, int n, vector<vector<int>> gr, vector<vector<int>> &co)
{
    int i;
    vector<vector<int>> vecPowerSet;
    vector<int> vecComplement,vecE;
    vector<int> vecAAF = argument_set();
    vecComplement.resize(vecAAF.size());
    vector<int>::iterator iter = set_difference(vecAAF.begin(),vecAAF.end(),
                                                gr[0].begin(),gr[0].end(),
                                                vecComplement.begin());
    vecComplement.resize(iter-vecComplement.begin());
    get_subsets(vecComplement, vecPowerSet);
    for(i = 0; i < (int)vecPowerSet.size(); i++)
    {
        vecE.clear();
        set_union(vecPowerSet[i].begin(),vecPowerSet[i].end(),
                              gr[0].begin(),gr[0].end(),
                              inserter(vecE, vecE.begin()));
        if(isFixpoint(l,m,n,vecE))
        {
            co.push_back(vecE);
        }
    }
}

bool gr_solution(int l, int m, int n, vector<vector<int>> &gr)
{
    vector<int> vecD = {};
    iteration_of_defense(m, n, vecD);
    if(isConflictFree(l,vecD))
    {
        gr.push_back(vecD);
        return true;
    }
    return false;
}

void max_solution(vector<vector<int>> src, vector<vector<int>> &vecMax)
{
    int i, j;
    vector<int> vis(src.size());
    sort(src.begin(),src.end(),[](const vector<int>& a,const vector<int>& b){
         return a.size()< b.size();});

    for(i=0;i < (int)src.size();++i)
    {
        for(j = i + 1;j < (int)src.size();++j)
        {
            if(isSubset(src[j],src[i]))
            {
                vis[i]=1;
            }

        }
    }
    for(i = 0;i < (int)src.size(); ++i)
    {

        if(!vis[i])
        {
            sort(src[i].begin(),src[i].end());
            vecMax.push_back(src[i]);
        }
    }
}

void stb_solution(int n, vector<vector<int>> co, vector<vector<int>> &stb)
{
    int i;
    vector<int> vecNE;
    for(i = 0; i < (int)co.size(); i++)
    {
        vecNE.clear();
        neutrality_set(n, co[i], vecNE);
        if(compare_extension(vecNE,co[i]))
        {
            stb.push_back(co[i]);
        }
    }
}

int main()
{
    //1、Enter the dl file name and l, m, n
    string strDlName;
    int l, m, n;
    cout<<"Please enter the dl file name:"<<endl;
    cin >> strDlName;
    cout<<"Please enter l, m and n:"<<endl;
    cin>>l>>m>>n;
    if(NULL == freopen(strDlName.c_str(), "r", stdin))
    {
        cout<<"Error in redirecting stdout!"<<endl;
        return -1;
    }

    //2、Init the AAF diagram
    bool isCfPreserved = false;
    vector<vector<int> > fp;
    vector<vector<int> > cf;
    vector<vector<int> > na;
    vector<vector<int> > def;
    vector<vector<int> > ad;
    vector<vector<int> > co;
    vector<vector<int> > gr;
    vector<vector<int> > pr;
    vector<vector<int> > stb;
    init();

    //test
    vector<int> vecE = {1,3,5},vecD,vecNE;
    neutrality_set(n, vecE, vecNE);
    //defense_set(m,n,vecE,vecD);
    for(int i = 0; i < (int)vecNE.size(); i++)
    {
        cout<<vecNE[i]<<" ";
    }
    cout<<endl;
    //3、Caculate the graded semantics of AAF
    isCfPreserved = gr_solution(l, m, n, gr);
    cout << "The lmn-grounded extensions:" << endl;
    print_solution(gr);
    cout << endl;

    if(isCfPreserved)
    {
        co_solution(l,m,n,gr,co);
        cout << "The lmn-complete extensions:" << endl;
        print_solution(co);
        cout << endl;

        max_solution(co, pr);
        cout << "The lmn-preferred extensions:" << endl;
        print_solution(pr);

        stb_solution(n, co, stb);
        cout << endl << "The lmn-stable extensions:" << endl;
        print_solution(stb);
        cout << endl;
    }
    else
    {
        cout << "The lmn-complete, preferred and stable semantics are empty.\n" << endl;
    }

    cf_solution(l, cf);
    cout << "The l-conflict-free sets:" << endl;
    print_solution(cf);
    cout << endl;

    max_solution(cf, na);
    cout << "The l-naive extensions:" << endl;
    print_solution(na);

    ad_solution(m, n, cf, ad);
    cout << endl << "The lmn-admissible sets:" << endl;
    print_solution(ad);
    cout << endl;

    def_solution(m, n, def);
    cout << "The mn-self-defended sets:" << endl;
    print_solution(def);
    cout << endl;

    fixpoint_solution(m, n, fp);
    cout << "All the fixed points:" << endl;
    print_solution(fp);

    return 0;
}
