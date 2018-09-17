#include "tmp.h"

int main()
{
    SOL Q1;
    vector<vector<int> > a1(10,vector<int>());
    Q1.testnum1(a1);

    vector<DataType<double> >a2(10);
    Q1.testnum2(a2);

    vector<DataType<string> >a3(10);
    Q1.testnum2(a3);

    return 0;
}
