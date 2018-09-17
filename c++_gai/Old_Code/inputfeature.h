/*
20180723/24:设计接口类，将Mset数据转换成适合机器学习模型输入数据类型的数据格式
20180725: 完成重载，留出类模板接口 
20170726: 完成调用ML的基类，扩展相应功能模块
20170727: 编写LR类木块，调用MLmodel类，完成相关功能测试
*/
#ifndef _INPUTFEATURE_H_
#define _INPUTFEATURE_H_

#include <xapian.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <string.h>
#include <typeinfo>

using namespace std;

template<class T0>
struct DataType
{
    vector<T0> value;         // 存储数据内容
    double weight;            // 每一组的权重
    bool classLabel;          // 标签备份
};

template<>
struct DataType<string>       // 偏特化
{
    vector<string> value;
    double weight;
    bool classLabel;
};

// FeatureUnput 类：从Xapian的一轮排序搜索中读取数据
class FeatureInput
{
public:
       FeatureInput(){   cout<<"Get in the inputfeature.h!"<<endl;  }
      
       // double  重载
       void FeatureDataInput(vector<vector<double> > &data,Xapian::MSet mset);
       // int 重载
       void FeatureDataInput(vector<vector<int> > &data,Xapian::MSet mset);
       // string 重载
       void FeatureDataInput(vector<vector<string> > &data,Xapian::MSet mset);
       // 获取权重
       void GetWeight(vector<double> &weight, Xapian::MSet mset);
       // 获取行号
       void GetNumDoc(vector<int> &number, Xapian::MSet mset);
        
       // 自定义类型
       template<class T0>
       void FeatureReadData(vector<DataType<T0> > &Tdata,     Xapian::MSet mset);
       //template<class >
       void FeatureReadDataStr(vector<DataType<string> > &Tdata, Xapian::MSet mset);

private:
       // double *data = new double(); 

};

// 判断字符串是否全是数字
bool AllIsNum(string str);
// 字符转换成double 类型
double StringToDouble(char *src);


template<class T0>
void FeatureInput::FeatureReadData(vector<DataType<T0> > &Tdata, Xapian::MSet mset)
{
    if(mset.size() == 0)
    {  
        cout<<"the data read from xapian is empty !"<<endl;
        return ;
    }
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m)
    {
       // Tdata.push_back(DataType<T0>() );
        const string &data = m.get_document().get_data();
        int len = 0, start = 0, end = 0;
        while( len < data.length() )
        {
            if(data[len]!='\n')
            {   end++;  }
            else
            {
                if(typeid(T0) == typeid(double) )
                {
                    Tdata[count].value.push_back(atof(data.substr(start,end).c_str()));
                }
                else if(typeid(T0) == typeid(int) )
                {
                    Tdata[count].value.push_back(atoi(data.substr(start,end).c_str()));
                }
                else
                {
                    cout<<" There is no type of "<< typeid(T0).name()<<" ."<<endl;
                    return ;
                }   
                start = len + 1;
                end = 0;
            }
            len++;
        }
        if( typeid(T0) == typeid(double) )
            Tdata[count].value.push_back(atof(data.substr(start,end).c_str())); 
        else if(typeid(T0) == typeid(int) )
            Tdata[count].value.push_back(atoi(data.substr(start,end).c_str()));
        Tdata[count].weight = m.get_weight();
        count++;
   }
    //return Tdata;
}


//template<class >
void FeatureInput::FeatureReadDataStr(vector<DataType<string> > &Tdata, Xapian::MSet mset)
{
    if(mset.size()==0)
    {
        cout<<"The data read from xapian is empty !"<<endl;
        return ;
    }
    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m!=mset.end();++m)
    {
        //Tdata.push_back(DataType<string>() );
        const string &data = m.get_document().get_data();
        int len = 0, start = 0, end = 0;
        while( len < data.length() )
        {
            if(data[len] != '\n' )
                end++;
            else
            {
                Tdata[count].value.push_back(data.substr(start,end));
                start = len+1;
                end = 0;
            }
            len++;
        } 
        Tdata[count].value.push_back(data.substr(start,end));
        Tdata[count].weight = m.get_weight();
        count++;
     }
}


// 机器学习接口，调用FeatureInput类提供的数据，留出空间，以供用户调用使用（继承），并添加自己需要的模块，完成组织数据等部分。
template<class T2>
class MLmodel
{
public:
    MLmodel(){  cout<<"Initialion The MLmodel_Base Class !"<<endl; };
    int GetStrNum(vector<string> vec);  // 获取一维数组中字符串的个数
    vector<int> GetStrPos(vector<string> vec); // 获取一维数组中字符串的位置
    vector<int> GetNumPos(vector<string> vec); // 获取数组中数字的位置
    vector<T2> GetNumRange(vector<T2> vec,int n1,int n2);  // 获取一维数据中某一截断
    vector<vector<T2> > GetNumRange(vector<vector<T2> > vec, int n1, int n2); // 获取二维数据某一组截断
    vector<T2> GetNumRange(vector<T2> vec, vector<int> pos); // 获取一维数据某几组位置的数据
    vector<vector<T2> > GetNumRange(vector<vector<T2> > vec, vector<int> pos);  // 获取二维数组某几组位置的数据

private:
   
};

template<class T2>
int MLmodel<T2>::GetStrNum(vector<string> vec)
{
    int count = 0;
    for(int i=0;i<vec.size();i++)
    {
        if(!AllIsNum(vec[i]))
            count++;
    }
    return count;
}

template<class T2>
vector<int> MLmodel<T2>::GetStrPos(vector<string> vec)
{
    vector<int> ans;
    for(int i=0;i<vec.size();i++)
    {
        if(!AllIsNum(vec[i]))
            ans.push_back(i);
    }
    return ans;
}

template<class T2>
vector<int> MLmodel<T2>::GetNumPos(vector<string> vec)
{
    vector<int> ans;
    for(int i=0;i<vec.size();i++)
    {
        if(AllIsNum(vec[i]))
            ans.push_back(i);
    }
    return ans;
}

template<class T2>
vector<T2> MLmodel<T2>::GetNumRange(vector<T2> vec, int n1, int n2)
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

template<class T2>
vector<vector<T2> > MLmodel<T2>::GetNumRange(vector<vector<T2> > vec, int n1, int n2)
{
    vector<vector<T2> > ans(vec.size(),vector<T2>());
    if(n1 <= 0 || n2 <= 0 || n2 > vec.size() - 1)
    {
        cout<<" Wrong Input : make sure the number is bigger than zero" << endl;
        return ans;
    }
    for(int i=0;i<vec.size();++i)
    {
        ans[i].push_back(GetNumRange(vec[i],n1,n2));
    }
    return ans;
}

template<class T2>
vector<T2> MLmodel<T2>::GetNumRange(vector<T2> vec, vector<int> pos)
{
    if(pos.size() == 0 || vec.size() == 0 || pos.size() > vec.size() )
    {
        cout<<" Invalid input Position"<<endl;
        return ;
    }
    vector<T2> ans(pos.size());
    for(int i=0;i<pos.size();i++)
        ans[i] = vec[pos[i]];

    return ans;
}

template<class T2>
vector<vector<T2> > MLmodel<T2>::GetNumRange(vector<vector<T2> > vec, vector<int> pos)
{
    if(pos.size() == 0 || vec.size() == 0 || pos.size() > vec.size() )
    {
        cout<<"Invalid Input Data"<<endl;
        return ;
    }
    vector<vector<T2> > ans(vec.size(), vector<T2>());
    for(int i=0;i<vec.size();i++)
    {
        ans[i].push_back(GetNumRange(vec[i],pos));
    }
    return ans;
}







// 备份——类模板：从 Xapian数据库读取数据过程
template<class Type>
class ReadData
{
public:
    void FeatureReadData(vector<vector<Type> > &data, Xapian::MSet mset);
};

template<class Type>
void ReadData<Type>::FeatureReadData(vector<vector<Type> > &data,Xapian::MSet mset)
{
    if(mset.size()==0)
    {   return;  }

    int count = 0;
    for(Xapian::MSetIterator m = mset.begin(); m!=mset.end(); ++m)
    {
        Xapian::docid did = *m;
        const string &Tdata = m.get_document().get_data();
        int len = 0, start = 0, end = 0;
        while(len<Tdata.length())
        {
            if(Tdata[len]!='\n')
            {   end++;  }
            else
            {
                // 根据 Type 内容选择相应的处理需求
                if(typeid(Type)==typeid(string))
                {    data[count].push_back(Tdata.substr(start,end));   }
                else if(typeid(Type) == typeid(double))
                {    data[count].push_back(atof(Tdata.substr(start,end).c_str()));  }
                else if(typeid(Type) == typeid(int))
                {    data[count].push_back(atoi(Tdata.substr(start,end).c_str()));  }
                else
                {
                    cout<<"There is no type of "<<typeid(Type).name()<<" !" <<endl;
                }
                start = len + 1;
                end = 0;
            }
            len++;
        }
        if(typeid(Type) == typeid(string))
            data[count].push_back(Tdata.substr(start,end));
        else if(typeid(Type) == typeid(double))
            data[count].push_back(atof(Tdata.substr(start,end).c_str()));
        else if(typeid(Type) == typeid(int))
            data[count].push_back(atoi(Tdata.substr(start,end).c_str()));
        count++; 
    }
}

#endif
