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
    const size_t FIELD_LAST_JUDGMENT = 0;
    const size_t FIELD_GENDER = 1;
    const size_t FIELD_GENDER_CONFI = 2;

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
    if (fields.at(FIELD_LAST_JUDGMENT) != "_last_judgment_at" ||
	fields.at(FIELD_GENDER) != "gender" ||
	fields.at(FIELD_GENDER_CONFI) != "gender:confidence"){
	// The CSV format doesn't match what we expect.
	cerr << "CSV format has changed!" << endl;
	exit(1);
    }

    int count = 0;
    while (csv_parse_line(csv, fields)) {
	// 'fields' is a vector mapping from field number to value.
	// We look up fields with the 'at' method so we get an exception
	// if that field isn't set.
    
    if(fields.size()!=TOTAL)
        continue;
    ++count;
	const string & state = fields.at(FIELD_LAST_JUDGMENT);
	const string & gender = fields.at(FIELD_GENDER);
	const string & gender_con = fields.at(FIELD_GENDER_CONFI);

	// We make a document and tell the term generator to use this.
	Xapian::Document doc;
	termgenerator.set_document(doc);

	// Index each field with a suitable prefix.
	termgenerator.index_text(state, 1, "S");
	termgenerator.index_text(gender, 1, "XD");
    termgenerator.index_text(gender_con,1,"EF");

	// Index fields without prefixes for general search.
	termgenerator.index_text(state);
	termgenerator.increase_termpos();
	termgenerator.index_text(gender);
    termgenerator.increase_termpos();
    termgenerator.index_text(gender_con);
    termgenerator.increase_termpos();

	// Add the collection as a value in slot 0.
	//doc.add_value(1, maker);

	// Store all the fields for display purposes.
	doc.set_data(state + "\n" + gender + "\n" + gender_con);

	// We use the identifier to ensure each object ends up in the
	// database only once no matter how many times we run the
	// indexer.
	string idterm = "Q" + state;
	doc.add_boolean_term(idterm);
	db.replace_document(idterm, doc);
    
    }
}
// End of example code.

int main(int argc, char** argv) {
    if (argc != 3) {
	cerr << "Usage: " << argv[0] << " DATAPATH DBPATH" << endl;
	return 1;
    }
    index(argv[1], argv[2]);
}
