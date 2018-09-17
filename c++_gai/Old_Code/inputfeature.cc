#include "inputfeature.h"
//#include <string.h>
//#include <sstream>
//#include <string>

double StringToDouble(char * src)
{
    double a;
    stringstream str;
    str << src;
    str >> a;
    str.clear();
    return a;
}

bool AllIsNum(string str)
{
// 判断字符串是否全是数字
    stringstream sin(str);
    double d;
    char c;
    if(!(sin>>d))
    {
        return false;
    }
    if(sin>>c)
    {
        return false;
    }
    return true;
}

/*
vector<DataType > FeatureInput::FeatureDataInput(Xapian::MSet mset)
{
    cout<<"Get In The inputfeatur.cc file !!"<<endl;
    
    //char *tmpdata = NULL;
    //char delim[]="\n";
    vector<DataType >  A(mset.size());
    if(mset.size()==0)
    {
        cout<<"the data read in is empty !"<<endl;
        return A;
    }
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m!=mset.end();++m)
    {
        Xapian::docid did = *m;  // did 获取行号
        const string & data = m.get_document().get_data();
        int len = 0,start=0,end=0;
        while(len<data.length())
        {
            if(data[len]!='\n')
            {   end++;  }
            else
            {
                A[count].str.push_back(data.substr(start,end));
                // string2double: stod(string s) or atof(string s.c_str())
                if(AllIsNum(data.substr(start,end)))
                {
                    A[count].numValue.push_back(atof(data.substr(start,end).c_str()));
                }
                else
                {
                    A[count].numValue.push_back(000);
                }
                start = len+1;
                end = 0;    
            }
            len++;
        }
        A[count].str.push_back(data.substr(start,end));
        if(AllIsNum(data.substr(start,end)))
        {
            A[count].numValue.push_back(atof(data.substr(start,end).c_str()));
        }
        else
        {
            A[count].numValue.push_back(000);
        }
        A[count].weight = m.get_weight();
       
//        char *sdata = new char[data.length()+1];
//        strcpy(sdata,data.c_str());
//        tmpdata = strtok(sdata,delim);
//        if(tmpdata!=NULL)
//        {
//            A[count].push_back(tmpdata);
//        }
//        while(tmpdata!=NULL)
//        {
//            tmpdata = strtok(NULL,delim);
//            if(tmpdata==NULL)
//            {    break;     }
//            A[count].push_back(tmpdata);
//      w  }
        
        count++;
   }
   if(A[0].str.size() == 0 || A.size()==0)
   {
        cout<<"The Read Process is wrong! "<<endl;
        return A;
   }
   else
   {    return A;   }
}
*/

void FeatureInput::FeatureDataInput(vector<vector<double> > &data,Xapian::MSet mset)
{
    if(mset.size() == 0)
    {   return ;    }

    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m!=mset.end(); ++m)
    {
    	Xapian::docid did = *m;
    	const string & ddata = m.get_document().get_data();
    	int len = 0, start = 0, end = 0;
    	while(len<ddata.length())
	    {
	        if(ddata[len]!='\n')
	        {    end++;    }
	        else
	        {
		        data[count].push_back(atof(ddata.substr(start,end).c_str())); 
		        start = len+1;
		        end = 0;
	         }
	        len++;
	    }
	    data[count].push_back(atof(ddata.substr(start,end).c_str()));
        count++;
    }
}

void FeatureInput::FeatureDataInput(vector<vector<string> > &data,Xapian::MSet mset)
{   
    if(mset.size() == 0 )
    {   return ;    }
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m!=mset.end(); ++m)
    {
        Xapian::docid did = *m;  // did 获取行号
        const string & sdata = m.get_document().get_data();
        int len = 0, start = 0, end = 0;
        while(len<sdata.length())
        {
            if(sdata[len]!='\n')
            {   end++;  }
            else
            {
                data[count].push_back(sdata.substr(start,end));
                start = len+1;
                end = 0;    
            }
            len++;
        }
	    data[count].push_back(sdata.substr(start,end));
	    count++;
    }
}

void FeatureInput::FeatureDataInput(vector<vector<int> > &data,Xapian::MSet mset)
{
    if(mset.size() == 0 )
    {   return ;    }
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m!=mset.end(); ++m)
    {
	    Xapian::docid did = *m;
	    const string & idata = m.get_document().get_data();
	    int len = 0, start = 0, end = 0;
	    while(len<idata.length())
    	{
	        if(idata[len]!='\n')
    	    {    end++;    }
	        else
	        {
    	 	    data[count].push_back(atoi(idata.substr(start,end).c_str()));
	    	    start = len + 1;
	        	end = 0;
	        }
	        len++;
	    }
	    data[count].push_back(atoi(idata.substr(start,end).c_str()));
	    count++;
    }
}

/*
void FeatureInput::FeatureDataInput(vector<vector<const char* > > &data, Xapian::MSet mset)
{
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m!=mset.end(); ++m)
    {
        Xapian::docid did = *m;
        const string & cdata = m.get_document().get_data();
        int len = 0, start = 0, end = 0;
        while(len<cdata.length())
        {
            if(cdata[len]!='\n')
            {   end++;  }
            else
            {
                data[count].push_back(cdata.substr(start,end).c_str());
                start = len + 1;
                end = 0;
            }
            len++;
        }
	    data[count].push_back(cdata.substr(start,end).c_str());
        count++;
    }
}
*/

void FeatureInput::GetWeight(vector<double> &weight, Xapian::MSet mset)
{
    if(mset.size() == 0)
    {   return ;    }
    for(Xapian::MSetIterator m = mset.begin(); m!=mset.end(); ++m)
    {
    	weight.push_back(m.get_weight());
    }
}

void FeatureInput::GetNumDoc(vector<int> &number,Xapian::MSet mset)
{
    if(mset.size() == 0)
    {   return;    }
    for(Xapian::MSetIterator m= mset.begin(); m != mset.end(); ++m)
    {
    	Xapian::docid did = *m;
    	number.push_back(did);
    }
}
