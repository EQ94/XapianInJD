#ifndef MAIN_H
#define MAIN_H
 
#include "stdio.h"
#include "stdlib.h"
#include "iostream"
#include "string"
#include "string.h"
#include <sstream>
#include <memory.h>
 
#include "math.h"
 
using namespace std;
 
#define maxClassLabelNum 10;
int curLabelNum = 0;

extern int attriNum;
extern int trainNum;
extern int sampleNum; 

double *logisW = new double[attriNum+1];

struct DataSample
{
	vector<double> attriValue;
	bool classLabel;
};
double ll[11] = {-5.900101,5.294425,-3.332734,-29.605830,-9.944291,5.058305,4.753524,4.487502,5.192096,4.491832,3.347162};

//use newton gradient descent algorithm to get the w
//logistic model: 1/(1+exp(-z))
//class label
void Logistic( DataSample* data, double *logisW )
{
 
	//memset( logisW,1.0,(attriNum+1)*sizeof(double) );//initial
 
	
	for( int i=0;i<(attriNum+1);++i )
	{
		logisW[i] = ll[i]; //1.0;
	}
 
}


void Normalize( DataSample* data )
{
	double atrriMinValue[attriNum];
	double atrriMaxValue[attriNum];//for normalization (x-xmin)/(xmax-xmin)
 
	//think about the first sample is none-loss
	//get the min and max value of each attribute without thinking about the loss atrribute
	for( int i=0;i<attriNum;++i )
	{
		atrriMinValue[i] = data[0].attriValue[i];
		atrriMaxValue[i] = data[0].attriValue[i];
	}
	
	for( int row = 1; row < sampleNum; ++row )
		for( int column = 0; column < attriNum; ++column )
		{
			if( data[row].attriValue[column] > atrriMaxValue[column] && (data[row].attriValue[column]+1000)>0.0001 )
				atrriMaxValue[column] = data[row].attriValue[column];
 
			if( data[row].attriValue[column] < atrriMinValue[column] && (data[row].attriValue[column]+1000)>0.0001 )
				atrriMinValue[column] = data[row].attriValue[column];
		}
 
	for( int row = 1; row < sampleNum; ++row )
		for( int column = 0; column < attriNum; ++column )
		{
			if( (data[row].attriValue[column]+1000)>0.0001)
				data[row].attriValue[column] = (data[row].attriValue[column]-atrriMinValue[column])/(atrriMaxValue[column]-atrriMinValue[column]);
			else
				data[row].attriValue[column] = 0;//set loss value 0;
		}

        Logistic(data,logisW);
}
 
 
bool Predict( DataSample sample)
{
	double h = 0.0;
	bool label = 0;
	for( int column=0; column<attriNum; ++column )
	{
		h += sample.attriValue[column]*logisW[column];
	}
	h += logisW[attriNum];
 
	if( h>0.333 )
		label = 1;
	else
		label = 0;
 
	if( label==sample.classLabel )
		return 1;
	else
		return 0;

	cout<<"Predict Done"<<endl;
}

#endif
