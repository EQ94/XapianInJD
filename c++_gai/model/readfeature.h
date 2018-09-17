/*  
解决结构体偏特化，并在类中分别调用问题（方案：通过私有函数，进行重载）
完成并完善LR模块实现
*/
#include "xapian.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <typeinfo>
#include <sstream>

using namespace std;

template<class T1>
struct DataType
{
    vector<T1> value;
    double weight;
    bool classlabel;
};
template<>
struct DataType<string>
{
    vector<string> value;
    double weight;
    bool classlabel;
};

// 读取特征类
class ReadFeature
{
public:
    
    void ReadFeatureData(vector<vector<double> > &data, Xapian::MSet mset);    // 重载

    void ReadFeatureData(vector<vector<int> > &data, Xapian::MSet mset);
    
    void ReadFeatureData(vector<vector<string> > &data, Xapian::MSet mset);    
    
    void GetWeight(vector<double> &weight, Xapian::MSet mset);                // 获取权重
    
    void GetNumDoc(vector<int> &number, Xapian::MSet mset);                   // 获取行号
     
    template<class T1>
    void ReadFeatureDataType(vector<DataType<T1> > &data, Xapian::MSet mset)  // 自定义类型读取
    {
        // 委托私有成员重载该函数
    	ReadFeatureDataType(data, mset, DataType<T1>());
    }
private:
    template<class T1>
    void ReadFeatureDataType(vector<DataType<T1> > &data, Xapian::MSet mset, DataType<T1>);
    void ReadFeatureDataType(vector<DataType<string> > &data, Xapian::MSet mset, DataType<string>);
};

// 机器学习接口，调用FeatureInput类提供的数据，留出空间，以供用户调用使用（继承），并添加自己需要的模块，完成组织数据等部分。
template<class T2>
class GenerateFeature 
{
public:
    GenerateFeature(){  cout<<"Initialion The MLmodel_Base Class !"<<endl; };
    
    int GetStrNum(vector<string> vec);                       // 获取一维数组中字符串的个数
    
    vector<int> GetStrPos(vector<string> vec);               // 获取一维数组中字符串的位置
    
    vector<int> GetNumPos(vector<string> vec);               // 获取数组中数字的位置
    
    vector<T2> GetNumRange(vector<T2> vec,int n1,int n2);    // 获取一维数据中某一截断
    
    vector<vector<T2> > GetNumRange(vector<vector<T2> > vec, int n1, int n2);   // 获取二维数据某一组截断
    
    vector<T2> GetNumRange(vector<T2> vec, vector<int> pos); // 获取一维数据某几组位置的数据
    
    vector<vector<T2> > GetNumRange(vector<vector<T2> > vec, vector<int> pos);  // 获取二维数组某几组位置的数据
    
    vector<vector<T2> > DeleteRow(vector<vector<T2> > vec, int row); // 删除某一行
    
    vector<vector<T2> > DeleteCol(vector<vector<T2> > vec, int col); // 删除某一列
    
    vector<T2> GetRow(vector<vector<T2> > vec, int row) ;    // 获取某一行
    
    vector<T2> GetCol(vector<vector<T2> > vec, int col) ;    // 获取某一列

//    void Transpose(vectir<vector> vec);                      // 转置
private:
   
};

// 字符转换成double 类型
double StringToDouble(char * src)
{
    double a;
    stringstream str;
    str << src;
    str >> a;
    str.clear();
    return a;
}
// 判断字符串是否全是数字
bool AllIsNum(string str)
{
    stringstream sin(str);
    double d;
    char c;
    if(!(sin>>d)) 
	return false; 
    if(sin>>c)
	return false; 
    return true;
}

void ReadFeature::ReadFeatureData(vector<vector<double> > &Tdata, Xapian::MSet mset)
{
    if(mset.size() == 0)
    {  
        cout<<"the data read from xapian is empty !"<<endl;
        return ;
    }
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m)
    {
        const string &data = m.get_document().get_data();
        int len = 0, start = 0, end = 0;
        while( len < data.length() )
        {
            if(data[len]!='\n')
            {   end++;  }
            else
            {
                Tdata[count].push_back(atof(data.substr(start,end).c_str())); 
                start = len + 1;
                end = 0;
            }
            len++;
        }
        Tdata[count].push_back(atof(data.substr(start,end).c_str())); 
        count++;
   }
}

void ReadFeature::ReadFeatureData(vector<vector<int> > &Tdata, Xapian::MSet mset)
{
    if(mset.size() == 0)
    {  
        cout<<"the data read from xapian is empty !"<<endl;
        return ;
    }
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m)
    {
        const string &data = m.get_document().get_data();
        int len = 0, start = 0, end = 0;
        while( len < data.length() )
        {
            if(data[len]!='\n')
            {   end++;  }
            else
            {
                Tdata[count].push_back(atoi(data.substr(start,end).c_str())); 
                start = len + 1;
                end = 0;
            }
            len++;
        }
        Tdata[count].push_back(atoi(data.substr(start,end).c_str())); 
        count++;
   }
}


void ReadFeature::ReadFeatureData(vector<vector<string> > &Tdata, Xapian::MSet mset)
{
    if(mset.size() == 0)
    {  
        cout<<"the data read from xapian is empty !"<<endl;
        return ;
    }
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m!=mset.end(); ++m)
    {
        const string & data = m.get_document().get_data();
        int len = 0, start = 0, end = 0;
        while( len < data.length() )
        {
            if(data[len] != '\n' )
                end++;
            else
            {
                Tdata[count].push_back(data.substr(start,end));
                start = len+1;
                end = 0;
            }
            len++;
        }
        Tdata[count].push_back(data.substr(start,end));
        count++;
    }

}

void ReadFeature::GetWeight(vector<double> &weight, Xapian::MSet mset)
{
    if(mset.size() == 0)
    {   return ;    }
    for(Xapian::MSetIterator m = mset.begin(); m!=mset.end(); ++m)
    {
    	weight.push_back(m.get_weight());
    }
}

void ReadFeature::GetNumDoc(vector<int> &number,Xapian::MSet mset)
{
    if(mset.size() == 0)
    {   return;    }
    for(Xapian::MSetIterator m= mset.begin(); m != mset.end(); ++m)
    {
    	Xapian::docid did = *m;
    	number.push_back(did);
    }
}

template<class T1>
void ReadFeature::ReadFeatureDataType(vector<DataType<T1> > &Tdata,Xapian::MSet mset, DataType<T1>)
{
    if(mset.size() == 0)
    {  
        cout<<"the data read from xapian is empty !"<<endl;
        return ;
    }
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m)
    {
        const string &data = m.get_document().get_data();
        int len = 0, start = 0, end = 0;
        while( len < data.length() )
        {
            if(data[len]!='\n')
            {   end++;  }
            else
            {
                if(typeid(T1) == typeid(double) )
                     Tdata[count].value.push_back(atof(data.substr(start,end).c_str()));
                else if(typeid(T1) == typeid(int) )
                     Tdata[count].value.push_back(atoi(data.substr(start,end).c_str()));
                else
                {
                    cout<<" There is no type of "<< typeid(T1).name()<<" ."<<endl;
                    return ;
                }    
                start = len + 1;
                end = 0;
            }
            len++;
        }
        if( typeid(T1) == typeid(double) )
            Tdata[count].value.push_back(atof(data.substr(start,end).c_str())); 
        else if(typeid(T1) == typeid(int) )
            Tdata[count].value.push_back(atoi(data.substr(start,end).c_str()));
        Tdata[count].weight = m.get_weight();
        count++;
   }
}

void ReadFeature::ReadFeatureDataType(vector<DataType<string> > &Tdata,Xapian::MSet mset, DataType<string>)
{
    if(mset.size() == 0)
    {  
        cout<<"the data read from xapian is empty !"<<endl;
        return ;
    }
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m)
    {
        const string &data = m.get_document().get_data();
        int len = 0, start = 0, end = 0;
        while( len < data.length() )
        {
            if(data[len]!='\n')
            {   end++;  }
            else
            {
                Tdata[count].value.push_back(data.substr(start,end));  
                start = len + 1;
                end = 0;
            }
            len++;
        }
        Tdata[count].value.push_back(data.substr(start,end));
        Tdata[count].weight = m.get_weight();
        count++;
   }
}

// ML接口类 获取字符串数量
template<class T2>
int GenerateFeature<T2>::GetStrNum(vector<string> vec)
{
    int count = 0;
    for(int i=0;i<vec.size();i++)
    {
        if(!AllIsNum(vec[i]))
            count++;
    }
    return count;
}
// ML接口类 获取字符串位置
template<class T2>
vector<int> GenerateFeature<T2>::GetStrPos(vector<string> vec)
{
    vector<int> ans;
    for(int i=0;i<vec.size();i++)
    {
        if(!AllIsNum(vec[i]))
            ans.push_back(i);
    }
    return ans;
}
// ML接口类 获取数字位置
template<class T2>
vector<int> GenerateFeature<T2>::GetNumPos(vector<string> vec)
{
    vector<int> ans;
    for(int i=0;i<vec.size();i++)
    {
        if(AllIsNum(vec[i]))
            ans.push_back(i);
    }
    return ans;
}
// ML接口类 获取范围内数据
template<class T2>
vector<T2> GenerateFeature<T2>::GetNumRange(vector<T2> vec, int n1, int n2)
{
    vector<T2> ans;
    if( n1 <= 0 || n2 <= 0 || n2 > vec.size()-1)
    {
        cout<<" Wrong Input : make sure the number is bigger than zero"<<endl;
        return ans;   
    }
    if(n1 > n2)
    {
        for(int i = n1;i <= n2 ; ++i)
            ans.push_back(vec[i]);
    }
    else
    {
        for(int i=n2; i>= n1; --i)
            ans.push_back(vec[i]);
    }
    return ans;
}
// ML接口类 获取范围内矩阵
template<class T2>
vector<vector<T2> > GenerateFeature<T2>::GetNumRange(vector<vector<T2> > vec, int n1, int n2)
{
    vector<vector<T2> > ans(vec.size(),vector<T2>());
    if(n1 <= 0 || n2 <= 0 || n2 > vec.size() - 1)
    {
        cout<<" Wrong Input : make sure the number is bigger than zero" << endl;
        return ans;
    }
    for(int i=0;i<vec.size();++i)
    {
        //ans[i].push_back(GetNumRange(vec[i],n1,n2));
        if(n1>n2)
        {
            for(int j=n1;j<=n2;++j)
                ans[i].push_back(vec[i][j]);
        }
        else 
        {
            for(int j=n2;j>=n1;--j)
                ans[i].push_bacl(vec[i][j]);
        }
    }
    return ans;
}
// ML接口类 获取相关位置的数据
template<class T2>
vector<T2> GenerateFeature<T2>::GetNumRange(vector<T2> vec, vector<int> pos)
{
    if(pos.size() == 0 || vec.size() == 0 || pos.size() > vec.size() )
    {
        cout<<" Invalid input Position"<<endl;
        return vec;
    }
    vector<T2> ans(pos.size());
    for(int i=0;i<pos.size();i++)
        ans[i] = vec[pos[i]];

    return ans;
}
// ML接口类 获取相关位置矩阵
template<class T2>
vector<vector<T2> > GenerateFeature<T2>::GetNumRange(vector<vector<T2> > vec, vector<int> pos)
{
    if(pos.size() == 0 || vec.size() == 0 || pos.size() > vec.size() )
    {
        cout<<"Invalid Input Data"<<endl;
        return vec;
    }
    vector<vector<T2> > ans(vec.size(), vector<T2>());
    for(int i=0;i<vec.size();i++)
    {
       // ans[i].push_back(GetNumRange(vec[i],pos));
       for(int j=0;j<pos.size();j++)
       {
           ans[i].push_back(vec[i][pos[j]]);
       }
    }
    return ans;
}
// ML接口类 删除某一行
template<class T2>
vector<vector<T2> > GenerateFeature<T2>::DeleteRow(vector<vector<T2> > vec, int row)
{
    if(row <= 0 || row > vec.size())
    {
        cout<<"Make Sure the number of row."<<endl;
        return vec;
    }
    vector<vector<T2> > ans;
    for(int i=0;i<vec.size();++i)
    {
        if(i == (row-1))
            continue;
        else
            ans.push_back(vec[i]);
    }
    return ans;
}
// ML接口类 删除某一列
template<class T2>
vector<vector<T2> > GenerateFeature<T2>::DeleteCol(vector<vector<T2> > vec, int col)
{
    if(col<=0 || col > vec[0].size())
    {
        cout<<"Make Sure the number of col"<<endl;
        return vec;
    }
    vector<vector<T2> > ans(vec.size(),vector<T2>());
    for(int i=0;i<vec.size();i++)
    {
        int count =0;
        for(int j=0;j<vec[0].size();j++)
        {
            if(j == (col-1))
                continue;
            else
                ans[i].push_back(vec[i][j]);
        }
    }
    return ans;
}
// ML接口类 获取某一行
template<class T2>
vector<T2> GenerateFeature<T2>::GetRow(vector<vector<T2> > vec, int row)
{
    vector<T2> ans;
    if(vec.size() == 0 || vec[0].size() == 0 || row<=0 || row > vec.size() )
    {
        cout<<"Make Sure the matriix or the row number is right !"<<endl;
        return ans;
    }
    ans.push_back(vec[row-1]);
    return ans;
}
// ML接口类 获取某一列
template<class T2>
vector<T2> GenerateFeature<T2>::GetCol(vector<vector<T2> > vec, int col)
{ 
    vector<T2> ans;
    if(vec.size() == 0 || vec[0].size() == 0 || col <=0 || col > vec[0].size() )
    {
        cout<<"Make Sure the matrix or the col number is right !"<<endl;
        return ans;
    }
    for(int i=0;i<vec.size();i++)
        ans.push_back(vec[i][col-1]);
    return ans;
}
// ML接口类 转置
//template<class T2>
//void GenerateFeature<T2>::Transpose(vector<T2> vec, int col)
//{  
//}   
