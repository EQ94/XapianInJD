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
#include "xapian/matchspy.h"
using namespace std;

class Demo:public MLmodel  
{
public:
    Demo(){}
    void normalization(){
        for(int i=0;i<12;i++)
            value.push_back(10 * rand() / double(RAND_MAX));
    }
    void readFeature(Xapian::MSet mset){
    // TODO for the user
         normalization();
         int count = 0;
         for(Xapian::MSetIterator m = mset.begin();m!=mset.end();++m){
              const string & data = m.get_document().get_data();
              double ans = 0.0;
              for(int i=2;i<10;i++){
                  string tmp = get_field(data, i);
                  ans+=atof(tmp.c_str());
              }
              arrNum.push_back(ans*value[count]);
              count++;
          }
     }
     void getResult(){
     // TODO for the user
          for(int i = 0; i< arrNum.size(); ++i)
              cout<<arrNum[i]<<" ";
              cout<<endl;
      }
      void readSpyData (const vector<Xapian::Internal::opt_intrusive_ptr<Xapian::MatchSpy> > &matchspies){
     // void readSpyData(const vector<Xapian::Internal::opt_intrusive_ptr<Xapian::ValueCountMatchSpy> >& matchspies){
     // TODO for the user
         
         cout<<"matchspies.size() = " << matchspies.size()<<endl;
         Xapian::MatchSpy *m1=matchspies[0].get();
         cout<<m1<<endl;
         
         double wt;
         Xapian::Document doc;
         m1->operator()(doc,wt);
         cout<<"wt : "<<wt<<endl;
 
    /*     cout<<"doc.get_data() = "<<doc.get_data()<<endl;
         cout<<"doc.get_description() = "<<doc.get_description()<<endl;
         
         //cout<<"doc.get_value() = "<<doc.get_value()<<endl;
         Xapian::TermIterator face1 = doc.termlist_begin();
         Xapian::ValueIterator face2 = doc.values_begin();

         //cout<<"Face : "<<*face<<" ; count: "<<face.get_termfreq()<<endl;

         for(Xapian::TermIterator facet = doc.termlist_begin();facet!=doc.termlist_end();++facet){
            cout<<"Facet : "<<*facet<<" ; count: "<<facet.get_termfreq()<<endl;

         }
    */

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
            
    
    // Get the index store in the database
    const size_t FIELD_UNIT_ID = 0;
    const size_t FIELD_UNIT_STATE = 1;
    const size_t FIELD_LAST_JUDGEMENT = 2;
    const size_t FIELD_GENDER = 3;
    const size_t FIELD_GENDER_CONFIDENCE = 4;
    const size_t FIELD_CREATED = 5;
    const size_t FIELD_DESCRIPTION = 6;
    const size_t FIELD_FAV_NUMBER = 7;
    const size_t FIELD_NAME = 8;
    const size_t FIELD_PROFILEMENTIMAGE = 9;
    const size_t FIELD_TEXT = 10;
    const size_t FIELD_TWEET_COUNT = 11;
    const size_t FIELD_USER_TIMEZONE = 12;
    
    // Start of prefix configuration.
    queryparser.add_prefix("_unit_id", "XU");
    queryparser.add_prefix("unit_state", "S");
    queryparser.add_prefix("last_judgement","ON");
    queryparser.add_prefix("gender","TW");
    queryparser.add_prefix("gender_confidence","FO");
    queryparser.add_prefix("created","SI");
    queryparser.add_prefix("description","EV");
    queryparser.add_prefix("fav_number","GH");
    queryparser.add_prefix("name","NI");
    queryparser.add_prefix("profilementimage","WO");
    queryparser.add_prefix("text","CT");
    queryparser.add_prefix("tweet_count","ZF");
    queryparser.add_prefix("user_timezone","WV");
    // End of prefix configuration.

    
    // And parse the query.
    Xapian::Query query = queryparser.parse_query(querystring);
            
    
    // Use an Enquire object on the database to run the query.
    Xapian::Enquire enquire(db);
    enquire.set_query(query);

    
    // 添加spy
    Xapian::ValueCountMatchSpy spy0(0);
    enquire.add_matchspy(&spy0);          // last_judgment
    
    Xapian::ValueCountMatchSpy spy1(1);
    enquire.add_matchspy(&spy1);          // gender_confidence
    
    Xapian::ValueCountMatchSpy spy2(2);
    enquire.add_matchspy(&spy2);          // created

    Xapian::ValueCountMatchSpy spy3(3);
    enquire.add_matchspy(&spy3);          // fav_number
    
    Xapian::ValueCountMatchSpy spy4(4);
    enquire.add_matchspy(&spy4);          // tweet_count

    Xapian::ValueCountMatchSpy spy5(5);   // gender text test
    enquire.add_matchspy(&spy5);

    // Get the number of documents in the database
    Xapian::doccount doc = db.get_doccount();
    
    // And print out something about each match.
    Demo Q;
    mset = enquire.get_mset(offset, pagesize, Q);
    
    cout<<"'"<< querystring << " '[ "<< offset <<":"<<offset + pagesize << "] ="<<endl;
    cout<<"The search results are mset.size() = "<<mset.size() <<" items. "<<endl;
    
    for(Xapian::MSetIterator m=mset.begin(); m!=mset.end(); ++m){
    Xapian::docid did = *m;
    cout<< m.get_rank()+1<<" :# "<<setfill('0') <<setw(3) <<did<<' ';
    const string & data = m.get_document().get_data();
    cout << get_field(data,FIELD_GENDER) <<"; " <<get_field(data,FIELD_GENDER_CONFIDENCE) <<"; " << get_field(data,FIELD_DESCRIPTION)<< endl;
    
    }
    cout<<endl;
   
    /*
    cout<<"The Data of Spy5 :"<<endl;
    for(Xapian::TermIterator facet = spy5.values_begin();facet!=spy5.values_end();++facet){
    cout<<"Facet : "<<*facet<<" ; count: "<<facet.get_termfreq()<<endl;
    }
    */
    cout<<"The Data of Spy0 :"<<endl;
    for(Xapian::TermIterator facet = spy0.values_begin();facet!=spy0.values_end();++facet){
    cout<<"Facet : "<<*facet<<" ; count: "<<facet.get_termfreq()<<endl;
    }

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
