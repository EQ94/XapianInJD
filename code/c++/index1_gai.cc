/*
name: index1_gai.cc
time: 20180719
author: zhangliang
*/

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
    const size_t FIELD_ID_RAND = 0;
    const size_t FIELD_JUDGE = 1;
    const size_t FIELD_NU1 = 2;
    const size_t FIELD_NU2 = 3;
    const size_t FIELD_NU3 = 4;
    // const size_t FIELD_DESCRIPTION = 8;

    // Create or open the database we're going to be writing to.
    Xapian::WritableDatabase db(dbpath, Xapian::DB_CREATE_OR_OPEN);

//    Xapian::WritableDatabase db = Xapian::InMemory::open();
//    Xapian::WritableDatabase db =  Xapian.inmemory_open();
    
    // Set up a TermGenerator that we'll use in indexing.
    Xapian::TermGenerator termgenerator;
    termgenerator.set_stemmer(Xapian::Stem("english")); // en means English ; ge means German

    ifstream csv(datapath.c_str());
    vector<string> fields;
    csv_parse_line(csv, fields);

    // Check the CSV header line matches our hard-code offsets.
    if (fields.at(FIELD_ID_RAND) != "ID_RAND" || fields.at(FIELD_JUDGE) != "JUDGE") {
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
	const string & JUDGE = fields.at(FIELD_JUDGE);
	const string & identifier = fields.at(FIELD_ID_RAND);
    const string & NU1 = fields = fields.at(FIELD_NU1);
    const string & NU2 = fields = fields.at(FIELD_NU2);
    const string & NU3 = fields = fields.at(FIELD_NU3);

	// We make a document and tell the term generator to use this.
	Xapian::Document doc;
	termgenerator.set_document(doc);

	// Index each field with a suitable prefix.
	termgenerator.index_text(identifier, 1, "S");
	termgenerator.index_text(JUDGE, 1, "XD");
    termgenerator.index_text(NU1,1,"ON");
    termgenerator.index_text(NU2,1,"TW");
    termgenerator.index_text(NU3,1,"TH");

	// Index fields without prefixes for general search.
	termgenerator.index_text(identifier);
	termgenerator.increase_termpos();

	termgenerator.index_text(JUDGE);
    termgenerator.increase_termpos();
    
    termgenerator.index_text(NU1);
    termgenerator.increase_termpos();
    
    termgenerator.index_text(NU2);
    termgenerator.increase_termpos();
    
    termgenerator.index_text(NU3);

	// Store all the fields for display purposes.
	doc.set_data(identifier + "\n" + JUDGE + "\n" + NU1 + "\n" + NU2 + "\n" + NU3);

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
