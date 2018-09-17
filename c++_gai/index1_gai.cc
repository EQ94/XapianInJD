/*
function:
1)  建立wpbc.data.csv数据库，建立值槽，统计feautre
2)  根据数据特点，将一整行一起存入
3) 【注意！！】当出现很多feature时，为了效率，将很多feature组合起来，存储为一组，这样会改变后续搜索时一轮搜索排序后的顺序
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
    const size_t FIELD_NU4 = 5;
    const size_t FIELD_NU5 = 6;
    const size_t FIELD_NU6 = 7;
    const size_t FIELD_NU7 = 8;
    const size_t FIELD_NU8 = 9;
    const size_t FIELD_NU9 = 10;
    const size_t FIELD_NU10 = 11;
    // const size_t FIELD_DESCRIPTION = 8;

    // Create or open the database we're going to be writing to.
    Xapian::WritableDatabase db(dbpath, Xapian::DB_CREATE_OR_OPEN);
    
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
	const string & ID_RAND = fields.at(FIELD_ID_RAND);
    const string & NU1 = fields.at(FIELD_NU1);
    const string & NU2 = fields.at(FIELD_NU2);
    const string & NU3 = fields.at(FIELD_NU3);
    const string & NU4 = fields.at(FIELD_NU4);
    const string & NU5 = fields.at(FIELD_NU5);
    const string & NU6 = fields.at(FIELD_NU6);
    const string & NU7 = fields.at(FIELD_NU7);
    const string & NU8 = fields.at(FIELD_NU8);
    const string & NU9 = fields.at(FIELD_NU9);
    const string & NU10 = fields.at(FIELD_NU10);

	// We make a document and tell the term generator to use this.
	Xapian::Document doc;
	termgenerator.set_document(doc);

	// Index each field with a suitable prefix.
	termgenerator.index_text(ID_RAND, 1, "S");
	termgenerator.index_text(JUDGE, 1, "XD");
    termgenerator.index_text(NU1,1,"ONE");
    termgenerator.index_text(NU2,1,"TWO");
    termgenerator.index_text(NU3,1,"THR");
    termgenerator.index_text(NU4,1,"FOU");
    termgenerator.index_text(NU5,1,"FIV");
    termgenerator.index_text(NU6,1,"SIX");
    termgenerator.index_text(NU7,1,"SEV");
    termgenerator.index_text(NU8,1,"EIG");
    termgenerator.index_text(NU9,1,"NIN");
    termgenerator.index_text(NU10,1,"TEN");

	// Index fields without prefixes for general search.
	termgenerator.index_text(ID_RAND);
	termgenerator.increase_termpos();

	termgenerator.index_text(JUDGE);
    termgenerator.increase_termpos();
    
    termgenerator.index_text(NU1);
    termgenerator.increase_termpos();
    
    termgenerator.index_text(NU2);
    termgenerator.increase_termpos();
    
    termgenerator.index_text(NU3);
    termgenerator.increase_termpos();

    termgenerator.index_text(NU4);
    termgenerator.increase_termpos();

    termgenerator.index_text(NU5);
    termgenerator.increase_termpos();

    termgenerator.index_text(NU6);
    termgenerator.increase_termpos();

    termgenerator.index_text(NU7);
    termgenerator.increase_termpos();

    termgenerator.index_text(NU8);
    termgenerator.increase_termpos();

    termgenerator.index_text(NU9);
    termgenerator.increase_termpos();

    termgenerator.index_text(NU10);


    /*  建立值槽  */
    doc.add_value(0,NU1);
    doc.add_value(1,NU2);
    doc.add_value(2,NU3);
    doc.add_value(3,NU4);
    doc.add_value(4,NU5);
    doc.add_value(5,NU6);
    doc.add_value(6,NU7);
    doc.add_value(7,NU8);
    doc.add_value(8,NU9);
    doc.add_value(9,NU10);

	// Store all the fields for display purposes.
	doc.set_data(ID_RAND + "\n" + JUDGE + "\n" + NU1 + "\n" + NU2 + "\n" + NU3  + "\n" + NU4 + "\n" + NU5 + "\n" + NU6 + "\n" + NU7 + "\n" + NU8 + "\n" + NU9 + "\n" + NU10);

	// We use the identifier to ensure each object ends up in the
	// database only once no matter how many times we run the
	// indexer.
	string idterm = "Q" + ID_RAND;
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
