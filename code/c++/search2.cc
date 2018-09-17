#include <xapian.h>

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "support.h"

using namespace std;

// Start of example code.
static void
search(const string & dbpath, const string & querystring,
       Xapian::doccount offset = 0, Xapian::doccount pagesize = 10)
{
    // build the database
    const size_t FIELD_ID_NUMBER=0;
    const size_t FIELD_ID_TITLE=2;
    const size_t FIELD_DESCRIPTION=8;
   

    // offset - defines starting point within result set.
    // pagesize - defines number of records to retrieve.

    // Open the database we're going to search.
    string ddpath = "db3";
    Xapian::WritableDatabase db(ddpath,Xapian::DB_CREATE_OR_OPEN);
//    Xapian::WritableDatabase db = Xapian::InMemory::open();
    
    Xapian::TermGenerator termgenerator;
    termgenerator.set_stemmer(Xapian::Stem("en"));

    ifstream csv(dbpath.c_str());
    vector<string> fields;
    csv_parse_line(csv,fields);
    
    if(fields.at(FIELD_ID_NUMBER)!="id_NUMBER"||
    fields.at(FIELD_ID_TITLE)!="TITLE"||
    fields.at(FIELD_DESCRIPTION)!="DESCRIPTION"){
    cerr<<"CSV format has changed!"<<endl;
   // exit(1);
     cout<<"ERROR HERE!!!!"<<endl;
    };
    cout<<"ready to write"<<endl;

    int count = 0;
    while(csv_parse_line(csv,fields))
    {
    count++;
    const string & description = fields.at(FIELD_DESCRIPTION);
    const string & title = fields.at(FIELD_ID_TITLE);
    const string & identifier = fields.at(FIELD_ID_NUMBER);

    Xapian::Document doc;
    termgenerator.set_document(doc);

    termgenerator.index_text(title,1,"S");
    termgenerator.index_text(description,1,"XD");

    doc.set_data(identifier + "\n" + title + "\n" + description);

    string idterm = "Q" + identifier;
    doc.add_boolean_term(idterm);
    db.replace_document(idterm,doc);
    }
    cout<<"count = "<<count<<endl;
    cout<<"build the database in memory, done"<<endl;



    // Set up a QueryParser with a stemmer and suitable prefixes.
    Xapian::QueryParser queryparser;
    queryparser.set_stemmer(Xapian::Stem("en"));
    queryparser.set_stemming_strategy(queryparser.STEM_SOME);
    
    
    
    // Start of prefix configuration.
    queryparser.add_prefix("title", "S");
    queryparser.add_prefix("description", "XD");
    // End of prefix configuration.

    // And parse the query.
    Xapian::Query query = queryparser.parse_query(querystring);

    // Use an Enquire object on the database to run the query.
    Xapian::Enquire enquire(db);
    enquire.set_query(query);

    // And print out something about each match.
    Xapian::MSet mset = enquire.get_mset(offset, pagesize);

    cout<<"mset.size = "<<mset.size()<<endl;

    clog << "'" << querystring << "'[" << offset << ":" << offset + pagesize << "] =";
    for (Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m) {
	Xapian::docid did = *m;
	cout << m.get_rank() + 1 << ": #" << setfill('0') << setw(3) << did << ' ';

	const size_t DOC_FIELD_TITLE = 1;
	const string & data = m.get_document().get_data();
	cout << get_field(data, DOC_FIELD_TITLE) << endl;
	// Log the document id.
	clog << ' ' << did;
    }
    clog << endl;
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
     string watch = "watch";
//    search(dbpath, querystring);
     search(dbpath,watch);
}
