#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
#include<typeinfo>
#include<algorithm>

using namespace std;
 
template<class T1>
struct DataType
{
    vector<T1> attriNum;
    double weight;
    bool label;
};

template<>
struct DataType<string>
{
    vector<string> attriNum;
    double weight;
    bool label;
};

class SOL
{
public:
    void testnum1(vector<vector<int> > & arr);
    /*
    template<class T1>
    void testnum2(vector<DataType<T1> > &arr);
    template<>
    void testnum2(vector<DataType<string> >& arr);
    */
    template<class T1>
    void testnum2(vector<DataType<T1> > &arr)
    {
        testnum2(arr,DataType<T1>());
    }
private:
    template<class T1>
    void testnum2(vector<DataType<T1> > & arr,DataType<T1>);
/*    {
    	for(int i=0;i<arr.size();i++)
            {
                //arr.push_back(DataType<T1>() );
                for(int j=10;j<20;j++)
                {
                    if(typeid(T1) == typeid(double))
                        arr[i].attriNum.push_back(j+2.23);
                    else if(typeid(T1) == typeid(int))
                        arr[i].attriNum.push_back(j);
                    arr[i].weight = rand();
                    arr[i].label = 0;
                }
            }
    }
*/
    void testnum2(vector<DataType<string> > &arr,DataType<string>);
/*    {
        for(int i=0;i<arr.size();i++)
        {
            //arr.push_bacl(DataType<string>() );
            for(int j=10;j<20;j++)
            {
                arr[i].attriNum.push_back("s");
                arr[i].weight = rand();
                arr[i].label = 0;
            }
        }
    }*/
};


void SOL::testnum1(vector<vector<int> > & arr)
{
    for(int i=0;i<arr.size();i++)
    {
        for(int j = 0;j<10;j++)
        {
            arr[i].push_back(j);
        }
    }
}

template<class T1>
void SOL::testnum2(vector<DataType<T1> > & arr,DataType<T1>)
{
    for(int i=0;i<arr.size();i++)
    {
        //arr.push_back(DataType<T1>() );
        for(int j=10;j<20;j++)
        {
            if(typeid(T1) == typeid(double))
                arr[i].attriNum.push_back(j+2.23);
            else if(typeid(T1) == typeid(int))
                arr[i].attriNum.push_back(j);
            arr[i].weight = rand();
            arr[i].label = 0;
        }
    }
}


void SOL::testnum2(vector<DataType<string> >& arr,DataType<string>)
{
    for(int i=0;i<arr.size();i++)
    {
        //arr.push_bacl(DataType<string>() );
        for(int j=10;j<20;j++)
        {
            arr[i].attriNum.push_back("s");
            arr[i].weight = rand();
            arr[i].label = 0;
        }
    }
}
