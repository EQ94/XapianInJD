/*
name: search_gai
time: 20180719
author: zhangliang
function:
1) 查找关键字，并注入LR模型
2) 根据搜索到的数据，首先直接调用存放于model文件夹内的各种机器学习模型
*/

#include <xapian.h>

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "support.h"

#include "model/inputfeature.h"
//#include "model/MLmodel.h"

using namespace std;

class LR : public MLmodel<double>
{
public:
    LR(){cout<<"LR construct "<<endl;};
};


// Start of example code.
static void
search(const string & dbpath, const string & querystring,
       Xapian::doccount offset = 0, Xapian::doccount pagesize = 100)
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
    queryparser.add_prefix("NU1","ON");
    queryparser.add_prefix("NU2","TW");
    queryparser.add_prefix("NU3","TH");
    queryparser.add_prefix("NU4","FO");
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
    Xapian::MSet mset = enquire.get_mset(offset, pagesize,100);
    /*
    cout << "mset.size() = " << mset.size() << endl;
    clog << "'" << querystring << "'[" << offset << ":" << offset + pagesize  << "] ="<<endl;
    // 输出搜索到数据项
    for (Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m) 
    {
	    Xapian::docid did = *m;
        // Get the length of a document
        Xapian::termcount ll2 = db.get_doclength(did);
        // Get the number of unique terms in document
        Xapian::termcount ll3 = db.get_unique_terms(did);


        //Xapian::TermIterator it = enquire.get_matching_terms_begin(m);

        cout << m.get_rank() + 1 << ": #" << setfill('0') << setw(3) << did << ' ';
        cout<< m.get_weight() << " ";
	    
        const size_t DOC_FIELD_TITLE = 1;
	    const string & data = m.get_document().get_data();

        //Get the termfreq of a term
        Xapian::doccount ll4 = mset.get_termfreq(data);

        const Xapian::termcount len = m.get_document().termlist_count();
        
        cout <<"data.size() = "<<data.size() <<" ;  data.length() = "<<data.length() <<" ; data = "<<  endl<< data<<endl;
        //cout << get_field(data, DOC_FIELD_TITLE) <<"    "<<get_field(data,2)<<"     "<<get_field(data,3)
        //     <<"     "<<get_field(data,4)<<"    "<<get_field(data,5)<< endl;
    }
    clog << endl;
   */ 
    
    // 统计并显示值槽中的值以及出现的频率
    // int count = 0;
    // for(Xapian::TermIterator facet = spy.values_begin();facet!=spy.values_end();++facet)
    // {
    //    count++;
    //    cout<<" Facet: "<< *facet <<"; count: "<< facet.get_termfreq() <<endl;
    // }
    // cout<< "spy's size = "<<    count   << endl;
    
    //vector<string> vec;
    //for(Xapian::MSetIterator m = mset.begin(); m!= mset.end(); ++m)
    //{
    //    const string & data = m.get_document().get_data();
    //    vec.push_back(get_field(data,5));
    //}
    
    // 传入 model 文件内LR模型
    //ReadData(vec);

    // 传入 FeatureInput 接口函数---vector容器
    FeatureInput Qcheck;
    vector<DataType> vec2 = Qcheck.FeatureDataInput(mset);
    cout<<"vec2.size() = " << vec2.size() <<endl;
    cout<<"vec2[0].str.size() = "<< vec2[0].str.size() <<endl;
    cout<<"vec2[0].numvalue.size() = "<< vec2[0].numValue.size() <<endl;
    cout<<"vec2[0].str[0] = "<< vec2[0].str[0] <<"  "<<vec2[0].numValue[0] <<endl;
    cout<<"vec2[0].weight = "<< vec2[0].weight << endl;
    cout<<"vec2[0].classLabel =  "<< vec2[0].classLabel << endl;
    
    // 传入 FeatureInput 接口函数 --- 内置变量类型
    vector<vector<string> > data(mset.size(),vector<string>());
    Qcheck.FeatureDataInput(data,mset);  // 通过预设定数组大小，读取mset类型
    cout<<"data.size() = "<<data.size()<<endl;
    cout<<"data[0].size() = "<<data[0].size()<<endl;

    // TO DO For The User
    LR mllr;
    int num = mllr.GetStrNum(data[0]);
    cout<<"GetStrNum = "<<num<<endl;
    vector<int> pos = mllr.GetNumPos(data[2]);
    for(int i=0;i<pos.size();i++)
        cout<<pos[i]<<" ';
    cout<<endl;
    

    // 导入模型
    
    
}
// End of example code.

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

    search(dbpath, querystring);
}
