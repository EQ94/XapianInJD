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
    const size_t FIELD_GENDER = 5;
    const size_t FIELD_DESCRIPTION = 10;


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
    if (fields.at(FIELD_GENDER) != "gender" ||
	fields.at(FIELD_DESCRIPTION) != "description") {
	// The CSV format doesn't match what we expect.
	cerr << "CSV format has changed!" << endl;
	exit(1);
    }

    while (csv_parse_line(csv, fields)) {
	
    if(fields.size()!=TOTAL)
        continue;
    // 'fields' is a vector mapping from field number to value.
	// We look up fields with the 'at' method so we get an exception
	// if that field isn't set.
	//
	// We're just going to use DESCRIPTION, TITLE and id_NUMBER.
	const string & description = fields.at(FIELD_DESCRIPTION);
	const string & gender = fields.at(FIELD_GENDER);

	// We make a document and tell the term generator to use this.
	Xapian::Document doc;
	termgenerator.set_document(doc);

	// Index each field with a suitable prefix.
	termgenerator.index_text(gender, 1, "S");
	termgenerator.index_text(description, 1, "XD");

	// Index fields without prefixes for general search.
	termgenerator.index_text(gender);
	termgenerator.increase_termpos();
	termgenerator.index_text(description);

	// Store all the fields for display purposes.
	doc.set_data(gender + "\n" + description);

	// We use the identifier to ensure each object ends up in the
	// database only once no matter how many times we run the
	// indexer.
	string idterm = "Q" + gender;
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
