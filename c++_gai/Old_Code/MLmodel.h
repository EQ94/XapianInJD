/*
设计接口类，注入机器学习模型
*/
#ifndef _MLMODEL_H_
#define _MLMODEL_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <xapian.h>

// #include "LR.h"

using namespace std;

template<class Type1,class Type2>
class MLmodel
{
public:
    void LRmodel(vector<vector<Type1> > dataIn, vector<Type2>dataOut);

private:
    void LRNormalize(Type1);

};

template<class Type1,class Type2>
void LRNormalize(vector<vector<Type1> > &dataIn)
{
    int attriNum = dataIn[0].size();
    int sampleNum = dataIn.size();
    vector<Type1> attriMinValue[attriNum];
    vector<Type1> attriMaxValue[attriNum];

    for(int i=0; i<attriNum; ++i)
    {
        attriMinValue[i] = dataIn[0][i];
        attriMaxValue[i] = dataIn[0][i];
    }
    for(int row=1;row<sampleNum;++row)
    {
        for(int col = 0;col<attriNum;++col)
        {
            if(dataIn[row][col] > attriMaxValue[col] && (dataIn[row][col] + 1000 )>0.0001)
                attriMaxValue[col] = dataIn[row][col];
            if(dataIn[row][col] < attriMinValue[col] && (dataIn[row][col] + 1000 )>0.0001)
                attriMinValue[col] = dataIn[row][col];
        }
    }

    for(int row = 1; row < sampleNum; ++row)
    {
        for(int col = 0; col < attriNum; ++col)
        {
            if( (dataIn[row][col] + 1000)>0.0001)
                data[row][col] = (data[row][col] - attriMinValue[col])/(attriMaxValue[col] - attriMinValue[col]);
            else
                data[row][col] = 0;
        }
    }
}

template(class Type1, class Type2)
void MLmode<Type1,Type2>::LRPredict(vector<Type1> data)
{
    double ll[34] = {-5.900101,0.294425,-3.332734,-29.605830,-298.944291,5.05830    5,4.753524,4.487502,5.192096,4.491832,3.347162,3.862049,3.202196,2.952448,      -18.184351,2.329524,3.040505,3.139998,3.806897,3.548766, 3.645437,-0.905558,    -6.286791,-36.197617,-426.355291,5.479833,4.087029, 4.575023,6.994482,4.2440    44,3.270083,2.640522,0.955509,-0.000000};
    
    double h = 0.0;
    bool label = 0;
     
    for(int col = 0; col < data.size(); col++)
    {
        h += data[col]*logisW[col];
    }
    if(h > 0.5)
        label = 1;
    else
    label = 0;

    if(label ==  
}

template<class Type1,class Type2>
void MLmode<Type1,Type2>::LRmodel(vector<vector<Type1> > dataIn,vector<Type2> dataOut)
{
    LRNormalize(dataIn);
}





