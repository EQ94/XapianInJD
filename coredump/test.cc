#include<iostream>
using namespace std;
int main()
{
    cout<<"Hello world! dump core for set value to NULL pointer"<<endl;

    *(char*)0=0;
    return 0;
}
