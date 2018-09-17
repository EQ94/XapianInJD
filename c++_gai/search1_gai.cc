/*
function:
1) 查找关键字，并注入LR模型
2) 根据搜索到的数据，首先直接调用存放于model文件夹内的各种机器学习模型
*/

#include <xapian.h>
#include <cstdio>
#include <iomanip>

#include "support.h"
#include "model/readfeature.h"
#include "model/LR.h"


int attriNum = 10;
int trainNum = 47;
int sampleNum = 47;

Xapian::MSet mset;
// Start of example code.
static void search(const string & dbpath, const string & querystring,Xapian::doccount offset = 0, Xapian::doccount pagesize = 100)
{
    // offset - defines starting point within result set.
    // pagesize - defines number of records to retrieve.

    // Open the database we're going to search.
    Xapian::Database db(dbpath);

    // Set up a QueryParser with a stemmer and suitable prefixes.
    Xapian::QueryParser queryparser;
    queryparser.set_stemmer(Xapian::Stem("en"));
    queryparser.set_stemming_strategy(queryparser.STEM_SOME);
    
    // Start of prefix configuration.
    queryparser.add_prefix("ID_RAND", "S");
    queryparser.add_prefix("JUDGE", "XD");
    queryparser.add_prefix("NU1","ONE");    queryparser.add_prefix("NU2","TWO");    queryparser.add_prefix("NU3","THR");
    queryparser.add_prefix("NU4","FOU");    queryparser.add_prefix("NU5","FIV");    queryparser.add_prefix("NU6","SIX");
    queryparser.add_prefix("NU7","SEV");    queryparser.add_prefix("NU8","EIG");    queryparser.add_prefix("NU9","NIN");
    queryparser.add_prefix("NU10","TEN");
    // End of prefix configuration.

    // And parse the query.
    Xapian::Query query = queryparser.parse_query(querystring);
    
    // Use an Enquire object on the database to run the query.
    Xapian::Enquire enquire(db);
    enquire.set_query(query);
    
    // 添加spy
    Xapian::ValueCountMatchSpy spy(1);
    enquire.add_matchspy(&spy);

    // Get the number of documents in the database
    Xapian::doccount doc = db.get_doccount();

    // And print out something about each match.
    mset = enquire.get_mset(offset, pagesize);
    
    cout<<"The search results are "<<mset.size() <<" items. "<<endl;
}

// TODO for the users
template<class T0>
class LR : public GenerateFeature<T0>    //<double>
{
public:
    LR(){cout<<"Default LR Construct"<<endl;}
    
    LR(double al,int Num,int traincount):alph(al),attriNum(Num),trainNum(traincount)
        {cout<<"Init LR Construct"<<endl;}
    
    int ReadData(DataSample* data,vector<vector<double> >vec, vector<string> str);
    
    //friend struct DataSample;
    
private:
    double alph;    
    int attriNum;   
    int trainNum;   
};
// 自定义结构体,在模型中声明
/*struct DataSample
{   
    vector<double> attriValue;
    bool classLabel;
    
   // template<class T0>
   // DataSample(LR<T0> lr)
   // {   // 设计构造函数初始化
   //     cout<<"lr.attriNum = "<<lr.attriNum<<endl;
   //     for(int i=0;i<lr.attriNum;i++)
   //         attriValue.push_back(0);
   // }
    
};*/
// 依据LR数据模型，整理数据
template<class T0>
int LR<T0>::ReadData(DataSample* data, vector<vector<double> > vec, vector<string> str)
{
    if(vec.size() == 0 || vec[0].size() == 0|| str.size() == 0)
        return -1;
    for(int i=0;i<vec.size();i++)
    {
        for(int j = 0; j<10;j++)
        {
            data[i].attriValue.push_back(vec[i][j]);
        }
        if(str[i] == "R")
            data[i].classLabel = 1;
        else
            data[i].classLabel = 0;
    }
    return 1;
}
int main(int argc, char** argv) {
    if (argc < 3) {
	cerr << "Usage: " << argv[0] << " DBPATH QUERYTERM..." << endl;
	return 1;
    }
    const char * dbpath = argv[1];

    // Join the rest of the arguments with spaces to make the query string.
    string querystring;
    for (argv += 2; *argv; ++argv) {
	if (!querystring.empty()) querystring += ' ';
	querystring += *argv;
    }

    search(dbpath, querystring);  // 开始查询

//  readfeature.h 文件  ReadFeature类：从Xapian中获取数据信息 
    ReadFeature Qdouble;
    vector<vector<double> > vec1(mset.size());    
    Qdouble.ReadFeatureData(vec1,mset);       //  读取 double 类型

    ReadFeature Qstring;
    vector<vector<string> > vec2(mset.size());
    Qstring.ReadFeatureData(vec2,mset);      //  读取 string 类型

    ReadFeature Qint;
    vector<vector<int> > vec3(mset.size());
    Qint.ReadFeatureData(vec3,mset);         //  读取 int 类型

    ReadFeature Qdoubletype;
    vector<DataType<double> > vec4(mset.size());
    Qdoubletype.ReadFeatureDataType(vec4,mset); 	//  读取自定义 double 类型
  
    ReadFeature Qstrtype;
    vector<DataType<string> > vec5(mset.size());
    Qstrtype.ReadFeatureDataType(vec5,mset);		//  读取自定义 string 类型  
    
//  readfeature.h文件 MLmodel类：继承基类之后，根据需要自定义功能实现，并调用ML模型
    LR<double> mllr(0.3,10,mset.size());        // 初始化参数
    vector<DataSample > vv(5);
    //cout<<"Vector<DataSample> vv[0].attriValue.size() = "<< vv[0].attriValue.size()<<endl;

    int num = mllr.GetStrNum(vec2[0]); 		    //  获取字符串个数
    //cout<<"GetStrNum = "<<num<<endl;

    vector<int> pos = mllr.GetNumPos(vec2[2]);	//  获取数字的位置
    //for(int i=0;i<pos.size();i++)
    //    cout<<pos[i]<<" ";
    //cout<<endl;

    vector<vector<double> > matrix_num = mllr.GetNumRange(vec1,pos);        //  获取有效范围内矩阵
    //cout<<"matrix_num.size() = "<<matrix_num.size() <<"; matrix_num[0].size() = "<<matrix_num[0].size()<<endl;

    vector<vector<double> > matrix_valid = mllr.DeleteCol(matrix_num,1);    //  删除第一列
    //cout<<"matrix_valid.size() = "<< matrix_valid.size() <<" ;  matrix.valid[0].size() = "<<matrix_valid[0].size()<<endl;

    LR<string> mllr_str;
    vector<string> str_valid = mllr_str.GetCol(vec2,2);		//  获取某一列
    //cout<<"str_valid.size() = "<<str_valid.size() <<" ; str_valid[0] = " << str_valid[0]<<endl;

//  调用LR.h文件，导入模型
    DataSample* data = new DataSample[matrix_valid.size()];
    // 如果读取数据成功
    if( -1 != mllr.ReadData(data,matrix_valid,str_valid)  )
    {   
        Normalize(data);                     // 调用LR中函数，初始化数据
        int correct = 0;
        for(int i=0;i<matrix_valid.size();++i)
        {
            bool res = Predict(data[i]);     // 调用LR中函数，做LR模型预测
            if(res)
                ++correct;
        }
        double res = (double)correct/matrix_valid.size();
        cout<<"The right rate is : "<<res<<" ."<<endl;
    }

}
