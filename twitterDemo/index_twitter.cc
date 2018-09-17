#include <xapian.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "support.h"

using namespace std;

// Start of example code.
void index(const string & datapath, const string & dbpath)
{
    // Hardcode field offsets for simplicity.
    const size_t FIELD_UNIT_ID = 0;
    const size_t FIELD_UNIT_STATE = 2;
    const size_t FIELD_LAST_JUDGEMENT = 4;
    const size_t FIELD_GENDER = 5;
    const size_t FIELD_GENDER_CONFIDENCE = 6;
    const size_t FIELD_CREATE = 9;
    const size_t FIELD_DESCRIPTION = 10;
    const size_t FIELD_FAV_NUMBER = 11;
    const size_t FIELD_NAME = 14;
    const size_t FIELD_PROFILEIMAGE = 16;
    const size_t FIELD_TEXT = 19;
    const size_t FIELD_TWEET_COUNT = 21;
    const size_t FIELD_USER_TIMEZONE = 25;

    // Create or open the database we're going to be writing to.
    Xapian::WritableDatabase db(dbpath, Xapian::DB_CREATE_OR_OPEN);

    // Set up a TermGenerator that we'll use in indexing.
    Xapian::TermGenerator termgenerator;
    termgenerator.set_stemmer(Xapian::Stem("en"));

    ifstream csv(datapath.c_str());
    vector<string> fields;
    csv_parse_line(csv, fields);

    const size_t TOTAL = fields.size();

    // Check the CSV header line matches our hard-code offsets.
    if(fields.at(FIELD_UNIT_ID) != "_unit_id" ||
    fields.at(FIELD_UNIT_STATE) != "unit_state"  ||
	fields.at(FIELD_LAST_JUDGEMENT) != "_last_judgment" ||
	fields.at(FIELD_GENDER) != "gender" ||
	fields.at(FIELD_GENDER_CONFIDENCE) != "gender_confidence" ||
	fields.at(FIELD_CREATE) != "created"  ||
    fields.at(FIELD_DESCRIPTION) != "description" ) {
	// The CSV format doesn't match what we expect.
    	cerr << "CSV format has changed!" << endl;
	    exit(1);
    }
    cout<<" TOTAL.size() = "<<TOTAL<<endl;
    int count = 0;
    while (csv_parse_line(csv, fields)) {
    
    if(fields.size()!=TOTAL)
        continue;
    ++count;
	// 'fields' is a vector mapping from field number to value.
	// We look up fields with the 'at' method so we get an exception
	// if that field isn't set.
    const string & unit_id = fields.at(FIELD_UNIT_ID);
	const string & unit_state = fields.at(FIELD_UNIT_STATE);
	const string & last_judgement = fields.at(FIELD_LAST_JUDGEMENT);
	const string & gender = fields.at(FIELD_GENDER);
	const string & gender_confidence = fields.at(FIELD_GENDER_CONFIDENCE);
	const string & created = fields.at(FIELD_CREATE);
    const string & description = fields.at(FIELD_DESCRIPTION);
    const string & fav_number = fields.at(FIELD_FAV_NUMBER);
    const string & name = fields.at(FIELD_NAME);
    const string & profilementimage = fields.at(FIELD_PROFILEIMAGE);
    const string & text = fields.at(FIELD_TEXT);
    const string & tweet_count = fields.at(FIELD_TWEET_COUNT);
    const string & user_timezone = fields.at(FIELD_USER_TIMEZONE);

    // We make a document and tell the term generator to use this.
	Xapian::Document doc;
	termgenerator.set_document(doc);

	// Index each field with a suitable prefix.
    termgenerator.index_text(unit_id,1,"XU");
	termgenerator.index_text(unit_state, 1, "S");
    termgenerator.index_text(last_judgement, 1, "ON");
	termgenerator.index_text(gender , 1, "TW");
	termgenerator.index_text(gender_confidence , 1, "FO");
	termgenerator.index_text(created , 1, "SI");
	termgenerator.index_text(description , 1, "EV");
	termgenerator.index_text(fav_number , 1, "GH");
	termgenerator.index_text(name , 1, "NI");
	termgenerator.index_text(profilementimage , 1, "WO");
	termgenerator.index_text(text , 1, "CT");
	termgenerator.index_text(tweet_count, 1, "ZF"); 
	termgenerator.index_text(user_timezone, 1, "WV"); 

	// Index fields without prefixes for general search.
    termgenerator.index_text(unit_id);
    termgenerator.increase_termpos();
	termgenerator.index_text(unit_state);
	termgenerator.increase_termpos();
	termgenerator.index_text(last_judgement); 
	termgenerator.increase_termpos();
	termgenerator.index_text(gender);
	termgenerator.increase_termpos();
	termgenerator.index_text(gender_confidence); 
	termgenerator.increase_termpos();
	termgenerator.index_text(created);
	termgenerator.increase_termpos();
	termgenerator.index_text(description); 
	termgenerator.increase_termpos();
	termgenerator.index_text(fav_number);
	termgenerator.increase_termpos();
	termgenerator.index_text(name); 
	termgenerator.increase_termpos();
	termgenerator.index_text(profilementimage);
	termgenerator.increase_termpos();
	termgenerator.index_text(text); 
	termgenerator.increase_termpos();
	termgenerator.index_text(tweet_count);
	termgenerator.increase_termpos();
	termgenerator.index_text(user_timezone);  

	// Add ... as a value in slot 0.
	doc.add_value(0, last_judgement); 
	doc.add_value(1, gender_confidence);
	doc.add_value(2, created);
	doc.add_value(3, fav_number);
	doc.add_value(4, tweet_count);
    doc.add_value(5, gender);

	// Store all the fields for display purposes.
	doc.set_data(unit_id + "\n" + unit_state + "\n" + last_judgement + "\n" + gender + "\n" + gender_confidence + "\n" + created + "\n" + description + "\n" + fav_number + "\n" + name + "\n" + profilementimage + "\n" + text + "\n" + tweet_count + "\n" + user_timezone );


	// We use the identifier to ensure each object ends up in the
	// database only once no matter how many times we run the
	// indexer.
	string idterm = "Q" + unit_id;
	doc.add_boolean_term(idterm);
	db.replace_document(idterm, doc);
    }

    cout<<" COUNT = "<<count<<endl;
}
// End of example code.

int main(int argc, char** argv) {
    if (argc != 3) {
	cerr << "Usage: " << argv[0] << " DATAPATH DBPATH" << endl;
	return 1;
    }
    index(argv[1], argv[2]);
}
