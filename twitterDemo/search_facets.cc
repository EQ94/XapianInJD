#include <xapian.h>

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>

#include "support.h"

using namespace std;

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
    
    queryparser.add_prefix("_unit_id","XU");
    queryparser.add_prefix("unit_state", "S");
    queryparser.add_prefix("last_judgement", "ON");
    queryparser.add_prefix("gender","TW");
    queryparser.add_prefix("gender_confidence","FO");
    queryparser.add_prefix("created", "SI");
    queryparser.add_prefix("description", "EV");
    queryparser.add_prefix("fav_number", "GH");
    queryparser.add_prefix("name", "NI");
    queryparser.add_prefix("profilementimage", "WO");
    queryparser.add_prefix("text" , "CT");
    queryparser.add_prefix("tweet_count", "ZF");
    queryparser.add_prefix("user_timezone", "WV"); 

    // And parse the query.
    Xapian::Query query = queryparser.parse_query(querystring);

    // Use an Enquire object on the database to run the query.
    Xapian::Enquire enquire(db);
    enquire.set_query(query);

    // Start of example code.
    // Set up a spy to inspect the MAKER value at slot 1
    Xapian::ValueCountMatchSpy spy0(0);
    enquire.add_matchspy(&spy0);
    Xapian::ValueCountMatchSpy spy1(1);
    enquire.add_matchspy(&spy1);
    Xapian::ValueCountMatchSpy spy2(2);
    enquire.add_matchspy(&spy2);
    Xapian::ValueCountMatchSpy spy3(3);
    enquire.add_matchspy(&spy3);
    Xapian::ValueCountMatchSpy spy4(4);
    enquire.add_matchspy(&spy4);


    // And print out something about each match.
    Xapian::MSet mset = enquire.get_mset(offset, pagesize, 100);

    clog << "'" << querystring << "'[" << offset << ":" << offset + pagesize  << "] =" <<endl;
    cout<< " mset.size() = " << mset.size() <<endl;

    for (Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m) {
	    Xapian::docid did = *m;
	    cout << m.get_rank() + 1 << ": #" << setfill('0') << setw(3) << did  << ' ';

        const size_t DOC_FIELD_DESCRIPTION = 10;
	    const string & data = m.get_document().get_data();
	    cout << get_field(data, DOC_FIELD_DESCRIPTION) << endl;
    }
    clog << endl;

    // Fetch and display the spy values
    //for (Xapian::TermIterator facet = spy.values_begin(); facet != spy.values_end(); ++facet) {
    //    cout << "Facet: " << *facet << "; count: " << facet.get_termfreq() << endl;
    //}
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
