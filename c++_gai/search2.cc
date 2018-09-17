/*
1 设置 get_mset() 默认对象功能
2 学习Matcher文件 multimatcher函数，在该函数中进行二次搜索
*/
#include <xapian.h>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include "support.h"
#include <string>
#include <algorithm>

using namespace std;

class Demo:public MLmodel  
{
public:
    Demo(){}
    void normalization(){
        for(int i=0;i<12;i++)
            value.push_back(10 * rand() / double(RAND_MAX));
    }
    // 读取特征
    void readFeature(Xapian::MSet mset){
        normalization();
        int count = 0;
        for(Xapian::MSetIterator m = mset.begin();m!=mset.end();++m){
            const string & data = m.get_document().get_data();
            double ans = 0.0;
            for(int i=2;i<10;i++)
            {
                string tmp = get_field(data, i);
                ans+=atof(tmp.c_str());
            }
            arrNum.push_back(ans*value[count]);
            count++;
        }
    }
    // 输出结果
    void getResult(){
        for(int i = 0; i< arrNum.size(); ++i)
            cout<<arrNum[i]<<" ";
        cout<<endl;
    }
    // 读取SPY数据（value格式）
    void readSPY()
    {
        
    }
private:
    vector<double> value;
    vector<double> arrNum;

};


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
    Xapian::ValueCountMatchSpy spy1(1);
    enquire.add_matchspy(&spy1);

    Xapian::ValueCountMatchSpy spy0(0);
    enquire.add_matchspy(&spy0);

    Xapian::ValueCountMatchSpy spy2(2);
    enquire.add_matchspy(&spy2);

    Xapian::ValueCountMatchSpy spy3(3);
    enquire.add_matchspy(&spy3);


    // Get the number of documents in the database
    Xapian::doccount doc = db.get_doccount();

    // And print out something about each match.

    Demo Q;
    mset = enquire.get_mset(offset, pagesize, Q);
    
    cout<<"The search results are mset.size() = "<<mset.size() <<" items. "<<endl;

    cout<<"The SPY0 = :"<<endl;
    for(Xapian::TermIterator facet = spy0.values_begin();facet!=spy0.values_end();++facet){
        cout<<"Facet : "<<*facet<<" ; count: "<<facet.get_termfreq()<<endl;
    }
/*
    cout<<"The SPY1 = :"<<endl;
    for(Xapian::TermIterator facet = spy1.values_begin();facet!=spy1.values_end();++facet){
        cout<<"Facet : "<<*facet<<" ; count: "<<facet.get_termfreq()<<endl;
    }
*/
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

}
