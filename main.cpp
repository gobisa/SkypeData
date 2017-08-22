/*
Analyzes a csv file of skype data found in main.db,
extracted using SQLite.
Calculates edit frequency, word count, unique words,
punctuation frequency, link frequency, emoji frequency, 
negative and positive emotional word use, bad/dirty word use.
Outputs data as csv.



References:
-negative_words.txt and positive_words.txt:
	Minqing Hu and Bing Liu. "Mining and Summarizing Customer Reviews."
		Proceedings of the ACM SIGKDD International Conference on Knowledge
		Discovery and Data Mining (KDD-2004), Aug 22-25, 2004, Seattle,
		Washington, USA.
-bad_words.txt: http://fffff.at/googles-official-list-of-bad-words/
-csvstream.h by Andrew DeOrio <awdeorio@umich.edu>, https://github.com/awdeorio/csvstream
*/


#include "csvstream.h"
#include "SkypeData.h"
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <regex>
#include <cassert>
#include <iomanip>

using std::string;
using std::cout;
using std::map;
using std::set;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::regex;
using std::regex_replace;
using std::stringstream;
using std::getline;

//http://www.sqlapi.com/ use this to get sql into this program so that only the .db file has to be used
//https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm THIS ONE LOOKS BETTER


void test_XMLToStringConverter();
void testFunctions();

int main(int argc, char** argv) {

	string entry;

	//get negative word set from file
	set<string> negative_words;
	ifstream negative_words_file("negative_words.txt");
	if (negative_words_file) {
		while (getline(negative_words_file, entry)) {
			//if statement needed to ignore header of file
			if (entry[0] != ';') {
				negative_words.insert(entry);
			}
		}
	}
	negative_words_file.close();

	
	//get positive word set from file
	set<string> positive_words;
	ifstream positive_words_file("positive_words.txt");
	if (positive_words_file) {
		while (getline(positive_words_file, entry)) {
			//if statement needed to ignore header of file
			if (entry[0] != ';') {
				positive_words.insert(entry);
			}
		}
	}
	positive_words_file.close();



	set<string> bad_words;
	ifstream bad_words_file("bad_words_edited.txt");
	if (bad_words_file) {
		while (bad_words_file >> entry) {
			bad_words.insert(entry);
		}
	}
	bad_words_file.close();

	
	try {
		//column names are: author,edited_by,body_xml
		//"replace(body_xml, CHAR(10), '')"
		//author,edited_by,"replace(body_xml, CHAR(10), '')"
		csvstream skype_data("main.csv"); 
		csvstream::row_type row;


		set<string> authors;
		vector<SkypeUser*> skype_users;


		//create all users and fill vector rows
		int user_index = -1;
		while (skype_data >> row) {
		
			if (authors.insert(row["author"]).second) {//new author
				skype_users.push_back(new SkypeUser(row["author"]));
				++user_index;
			}

			skype_users[user_index]->addRow(row);
		}

		ofstream output_file("SkypeDataResults.csv");
		if (output_file) {

			output_file << std::setprecision(3);

			output_file << "Name,Post Count,Edits Made,Average Edits per Message,Word Count,Unique Words,"
				<< "Punctuation Count,Average Punctuation per Message, Link Count,"
				<< "Average Links per Message,Emoji Count,Average Emojis per Message,"
				<< "Negative Word Count,Positive Word Count,Bad Word Count,"
				<< "Negative Messages, Positive Messages, Messages with Bad Word,"
				<< "Negative Message Frequency, Positive Message Frequency, Message with Bad Word Frequency,"
				<< "Top 3 Emojis (count), Top 3 Words (count)"
				<< "\n";

			//analyze data
			for (SkypeUser* user : skype_users) {
				user->sortData();
				user->analyzeData(negative_words, positive_words, bad_words);
				user->outputData(output_file);
			}
		}


		output_file.close();

		//delete dynamic memory
		for (SkypeUser* user : skype_users) {
			delete user;
		}
	}
	catch (csvstream_exception e) {
		cout << "Error with file... quitting\n";
		exit(EXIT_FAILURE);
	}



	cout << "Finished\n";
	return 0;
}






void testFunctions() {
	//test SkypeUser::removePunctuation
	SkypeUser test("test");
	std::cout << test.removePunctuation(",hello,");
	assert(test.removePunctuation(",hello,") == "hello");


	assert(test.XMLSpecialCharToString("don&apos;t") == "don't");
	assert(test.XMLSpecialCharToString("What&apos;s") == "What's");
	assert(test.XMLSpecialCharToString("&lt;") == "<");
	assert(test.XMLSpecialCharToString("a&amp;p") == "a&p");
	assert(test.XMLSpecialCharToString("&gt;") == ">");
	assert(test.XMLSpecialCharToString("&quot;oh&quot;") == "\"oh\"");
	//above all passed
}






void test_XMLToStringConverter() {

	SkypeUser s("test");

	string test1 = "<partlist type=""started"" alt=""""><part identity=""andrius.gobis""><name>andrius.gobis</name><duration>3414</duration></part><part identity=""erikas.anuzis""><name>erikas.anuzis</name><duration>3414</duration></part><part identity=""matas.lelis""><name>matas.lelis</name><duration>3414</duration></part></partlist>";
	string test1correct = "";
	assert(test1correct == s.XMLToStringConverter(test1));

	string test2 = "<URIObject type=""Picture.1"" uri=""https://api.asm.skype.com/v1/objects/0-cus-d1-868439622f7ea62b724aab8f84dcba72"" url_thumbnail=""https://api.asm.skype.com/v1/objects/0-cus-d1-868439622f7ea62b724aab8f84dcba72/views/imgt1"">You&apos;ve received a new picture. View it at: <a href=""https://login.skype.com/login/sso?go=xmmfallback?pic=0-cus-d1-868439622f7ea62b724aab8f84dcba72"">https://login.skype.com/login/sso?go=xmmfallback?pic=0-cus-d1-868439622f7ea62b724aab8f84dcba72</a><FileSize v=""74616""/><OriginalName v=""""/><meta type=""photo"" originalName=""""/></URIObject>";
	string test2correct = "";
	assert(test2correct == s.XMLToStringConverter(test2));

	string test3 = "<a href=""https://scontent-ort2-1.xx.fbcdn.net/v/t1.0-9/3338_1144447407178_4380501_n.jpg?oh=8047854a7e343508a3e2e6418ba34695&amp;oe=5917F777"">https://scontent-ort2-1.xx.fbcdn.net/v/t1.0-9/3338_1144447407178_4380501_n.jpg?oh=8047854a7e343508a3e2e6418ba34695&amp;oe=5917F777</a>";
	string test3correct = "";
	assert(test3correct == s.XMLToStringConverter(test3));

	string test4 = "<ss type=""cry"">;(</ss>";
	string test4correct = ";(";
	string test4debug = s.XMLToStringConverter(test4);
	assert(test4correct == s.XMLToStringConverter(test4));

	return;
}