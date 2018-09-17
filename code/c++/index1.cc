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
    cout<<"GetIn_Now"<<endl;
    // Hardcode field offsets for simplicity.
    const size_t FIELD_ID_NUMBER = 0;
    const size_t FIELD_TITLE = 2;
    const size_t FIELD_DESCRIPTION = 8;

    // Create or open the database we're going to be writing to.
//    Xapian::WritableDatabase db(dbpath, Xapian::DB_CREATE_OR_OPEN);

    Xapian::WritableDatabase db = Xapian::InMemory::open();
//    Xapian::WritableDatabase db =  Xapian.inmemory_open();
    
    // Set up a TermGenerator that we'll use in indexing.
    Xapian::TermGenerator termgenerator;
    termgenerator.set_stemmer(Xapian::Stem("english")); // en means English ; ge means German

    ifstream csv(datapath.c_str());
    vector<string> fields;
    csv_parse_line(csv, fields);

    // Check the CSV header line matches our hard-code offsets.
    if (fields.at(FIELD_ID_NUMBER) != "id_NUMBER" ||
	fields.at(FIELD_TITLE) != "TITLE" ||
	fields.at(FIELD_DESCRIPTION) != "DESCRIPTION") {
	// The CSV format doesn't match what we expect.
	cerr << "CSV format has changed!" << endl;
	exit(1);
    }
    
    cout<<"ready to write"<<endl;

    while (csv_parse_line(csv, fields)) {
	// 'fields' is a vector mapping from field number to value.
	// We look up fields with the 'at' method so we get an exception
	// if that field isn't set.
	//
	// We're just going to use DESCRIPTION, TITLE and id_NUMBER.
	const string & description = fields.at(FIELD_DESCRIPTION);
	const string & title = fields.at(FIELD_TITLE);
	const string & identifier = fields.at(FIELD_ID_NUMBER);

	// We make a document and tell the term generator to use this.
	Xapian::Document doc;
	termgenerator.set_document(doc);

	// Index each field with a suitable prefix.
	termgenerator.index_text(title, 1, "S");
	termgenerator.index_text(description, 1, "XD");

	// Index fields without prefixes for general search.
	termgenerator.index_text(title);
	termgenerator.increase_termpos();
	termgenerator.index_text(description);

	// Store all the fields for display purposes.
	doc.set_data(identifier + "\n" + title + "\n" + description);

	// We use the identifier to ensure each object ends up in the
	// database only once no matter how many times we run the
	// indexer.
	string idterm = "Q" + identifier;
	doc.add_boolean_term(idterm);
	db.replace_document(idterm, doc);
    }
}
// End of example code.

int main(int argc, char** argv) {
    if (argc != 3) {
        cout<<"bad-run-here-20180705"<<endl;
	cerr << "Usage: " << argv[0] << " DATAPATH DBPATH" << endl;
	return 1;
    }
    cout<<"InStriaght"<<endl;  
    index(argv[1], argv[2]);
}