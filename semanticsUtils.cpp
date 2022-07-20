#include "semanticsUtils.h"

bool compare_extension(vector<int> vecExtSrc, vector<int> vecExtDes)
{
    int i;
    if(vecExtSrc.size() != vecExtDes.size()) return false;
    for(i = 0; i < (int)vecExtSrc.size(); i++)
    {
        if(vecExtSrc[i] != vecExtDes[i])
        {
            return false;
        }
    }
    return true;
}
